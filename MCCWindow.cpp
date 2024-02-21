#include "MCCWindow/MCCWindow.h"

MCCWindow::MCCWindow(QWidget *parent) : MCCWidget(parent)
{
    qDebug() << "MCCWindow::MCCWindow(QWidget *parent)";
    this->initialize_members();
    this->initialize_window();
}

MCCWindow::~MCCWindow()
{
    qDebug() << "MCCWindow::~MCCWindow()";
}


void MCCWindow::connect_signals()
{
    connect(this, SIGNAL(signal_blink()), this, SLOT(device_blink()));
    connect(this, SIGNAL(signal_burnout()), this, SLOT(device_burnout()));
    connect(this, SIGNAL(signal_calibration()), this, SLOT(device_calibration()));
    connect(this, SIGNAL(signal_cjc()), this, SLOT(device_cjc()));
    connect(this, SIGNAL(signal_reset()), this, SLOT(device_reset()));
    connect(this, SIGNAL(signal_serial()), this, SLOT(device_serial()));
    connect(this, SIGNAL(signal_status()), this, SLOT(device_status()));
    connect(this, SIGNAL(signal_update()), this, SLOT(update_inputs()));
    connect(this->autoUpdateCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_thread(bool)));
    connect(this->writeOutputCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_write(bool)));
    connect(this->fahrCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_fahr(bool)));
}


void MCCWindow::device_blink()
{
    this->MCC_USB_TC::blink();
}

void MCCWindow::device_burnout()
{
    this->MCC_USB_TC::BurnoutStatus();
}

void MCCWindow::device_calibration()
{
    this->MCC_USB_TC::Calibrate();
}

void MCCWindow::device_cjc() /* read the CJC */
{
    this->MCC_USB_TC::read_cjc();
}

void MCCWindow::device_reset()
{
    this->MCC_USB_TC::Reset();
}

void MCCWindow::device_serial()
{
    this->MCC_USB_TC::Serialnumber();
}

void MCCWindow::device_status()
{
    this->MCC_USB_TC::Status();
}

void MCCWindow::display_buffer()
{
    if (this->is_fahr())
    {
        this->thermo1Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(0)));
        this->thermo2Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(1)));
        this->thermo3Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(2)));
        this->thermo4Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(3)));
        this->thermo5Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(4)));
        this->thermo6Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(5)));
        this->thermo7Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(6)));
        this->thermo8Display->display(this->MCC_USB_TC::celsius2fahr(this->get_temperature_values(7)));
    }
    else
    {
        this->thermo1Display->display(this->get_temperature_values(0));
        this->thermo2Display->display(this->get_temperature_values(1));
        this->thermo3Display->display(this->get_temperature_values(2));
        this->thermo4Display->display(this->get_temperature_values(3));
        this->thermo5Display->display(this->get_temperature_values(4));
        this->thermo6Display->display(this->get_temperature_values(5));
        this->thermo7Display->display(this->get_temperature_values(6));
        this->thermo8Display->display(this->get_temperature_values(7));
    }
}

float MCCWindow::get_temperature_values(unsigned int i)
{
    QReadLocker readLocker(this->bufferLock);
    return this->MCC_USB_TC::get_temps(i);
}

void MCCWindow::initialize_members()
{
    this->bufferLock = new QReadWriteLock(); // initialize the read write locker
    this->set_writing(false);
    this->set_fahr(false);
}

void MCCWindow::initialize_window()
{
    this->connect_signals();

    // update the LCD displays on a regular interval
    this->displayBufferTimer = new QTimer(this);
    connect(this->displayBufferTimer, SIGNAL(timeout()), this, SLOT(display_buffer()));
    this->displayBufferTimer->start(1000); // do not need to be faster than 1 sec
}

bool MCCWindow::is_fahr()
{
    return this->fahr;
}

bool MCCWindow::is_writing()
{
    return this->writing;
}

void MCCWindow::set_fahr(bool flg)
{
    this->fahr = flg;
}

void MCCWindow::set_writing(bool flg)
{
    this->writing = flg;
}

bool MCCWindow::start_thread()
{
    // select sample time
    bool ok;
    /*double ts = QInputDialog::getDouble(this, tr("enter sample time"),
                                  tr("ms"), 10.0, 10.0, 100.0, 1, &ok);*/

    double ts = QInputDialog::getDouble(this, tr("enter sample time"),
                                  tr("ms"), 500.0, 500.0, 1000.0, 1, &ok);

    return this->start_thread(ts);
}

bool MCCWindow::start_thread(double ts)
{
    QString tmpString;
    tmpString.setNum(ts);
    if (this->mccTimer.startTicking<MCCWindow>(*this, &MCCWindow::threaded_function, ts))
    {
        this->autoUpdateCheckBox->setText("Ts = " + tmpString + " ms");
        this->autoUpdateCheckBox->setChecked(true);
        this->updateButton->setEnabled(false); // there is no need to manually update anymore
        this->blinkButton->setEnabled(false); // the blink interferes with the thread
        return true;
    }
    else
    {
        return false;
    }
}

bool MCCWindow::stop_thread()
{
    if (this->mccTimer.stopTicking())
    {
        this->autoUpdateCheckBox->setText("Auto Update");
        this->autoUpdateCheckBox->setChecked(false);
        this->updateButton->setEnabled(true);
        this->blinkButton->setEnabled(true);
        return true;
    }
    else
    {
        return false;
    }
}


bool MCCWindow::threaded_function(double elapsed_ms)
{
    this->update_inputs();

    if (this->is_writing())
    {
        // save buffers
        float tmpTime = (float) elapsed_ms;

        float tmpBuffer[8];
        for (unsigned int ii = 0; ii < 8; ii++)
        {
            tmpBuffer[ii] = this->get_temperature_values(ii);
        }

        // write buffers
        this->fh.write(& tmpTime);

        if (! this->fh.get_binary()) // include delimiters for ascii mode
        {
            char comma[1] = {','};
            this->fh.write(comma);
            //this.fh.write(","); // this will also work
        }

        for (unsigned int ii = 0; ii < 8; ii++)
        {
            this->fh.write(tmpBuffer + ii); // send the appropriate array sequentially

            if (! this->fh.get_binary()) // include delimiters for ascii mode
            {
                if (ii == 7)
                {
                    char endline[2] = {'\n'};
                    this->fh.write(endline);
                    //this->fh.write("\n"); // this will also work
                }
                else
                {
                    char comma[1] = {','};
                    this->fh.write(comma);
                    //this->fh.write(","); // this will also work
                }
            }
        }
    }
    return true;
}

void MCCWindow::toggle_fahr(bool flg)
{
    this->set_fahr(flg);
}

void MCCWindow::toggle_thread(bool flg)
{
    if (flg) // starting thread
    {
        if (!this->start_thread()) // not successful
        {
            this->autoUpdateCheckBox->setChecked(false);
        }
    }
    else // stopping thread
    {
        if (!this->stop_thread()) // not successful
        {
            this->autoUpdateCheckBox->setChecked(true);
        }
    }
}

void MCCWindow::toggle_write(bool flg)
{
    if (flg) // attempt to write buffer to file
    {
        FileHandlerDialog fhDialog(& this->fh);
        if (fhDialog.exec()) // file selected
        {
            this->fh.create_file();

            // stop the timer if it is already ticking
            if (this->mccTimer.isTicking()) // timer is indeed ticking
            {
                if (!this->stop_thread()) // stop thread first
                {   // if not successful
                    qDebug() << "SensorayWindow::toggle_write(bool flg)\t ERROR resetting thread";
                    this->set_writing(false);
                    this->writeOutputCheckBox->setChecked(false);
                }
                else
                {
                    this->set_writing(true);
                    this->start_thread();
                }
            }
            else
            {
                this->set_writing(true);
                this->start_thread();
            }
        }
        else // no file selected, cancel write operation
        {
            qDebug() << "Sorry, no file selected ... try again";
            this->set_writing(false);
            this->writeOutputCheckBox->setChecked(false);
        }
    }
    else // stop writing
    {
        this->stop_thread();
        this->fh.close();
        qDebug() << "closing the file";

        if (this->fh.get_convert())
        {
            float tmpBuffer[9];
            this->fh.to_ascii(this->fh.get_filename(), this->fh.get_asciiFilename(),tmpBuffer,9);
        }
        this->set_writing(false);
    }
}

void MCCWindow::update_inputs()
{
    // need to write protect this as a different thread can call this
    QWriteLocker writeLocker(this->bufferLock);

    // reads the card
    this->MCC_USB_TC::update_temps();
}
