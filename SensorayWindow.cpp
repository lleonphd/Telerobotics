/**
  * @file SensorayWindow.cpp
  * @brief Code for the SensorayWindow class methods
  * @author Lisandro Leon
  */

#include "SensorayWindow/SensorayWindow.h"

SensorayWindow::SensorayWindow(QWidget *parent)
    : SensorayWidget(parent)
{
    this->initialize_members();
    this->initialize_window();
}

SensorayWindow::~SensorayWindow()
{
    qDebug() << "SensorayWindow::~SensorayWindow()";
}

void SensorayWindow::closeEvent(QCloseEvent *event)
{
    if (this->is_writing()) // close the file
    {
        this->fh.close();
    }
    qDebug() << "SensorayWindow closeEvent detected";
    //this->~SensorayWindow();
}

void SensorayWindow::connect_signals()
{
    connect(this, SIGNAL(signal_conversions()), this, SLOT(set_conversions()));
    connect(this, SIGNAL(signal_send()), this, SLOT(update_send()));
    connect(this, SIGNAL(signal_update()), this, SLOT(update_inputs()));
    connect(this->autoUpdateCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_thread(bool)));
    connect(this->writeOutputCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_write(bool)));
}

void SensorayWindow::default_conversions_to_buffer()
{
    for (unsigned int i = 0; i < 16; i++)
    {
          //this->conversions[i] = 7.7;
          this->conversions[i] = 1.0;
    }
    qDebug() << "Set conversions to default values";
}

void SensorayWindow::display_buffer()
{
    this->displayIn1->display(this->get_analog_value(0));
    this->displayIn2->display(this->get_analog_value(1));
    this->displayIn3->display(this->get_analog_value(2));
    this->displayIn4->display(this->get_analog_value(3));
    this->displayIn5->display(this->get_analog_value(4));
    this->displayIn6->display(this->get_analog_value(5));
    this->displayIn7->display(this->get_analog_value(6));
    this->displayIn8->display(this->get_analog_value(7));
    this->displayIn9->display(this->get_analog_value(8));
    this->displayIn10->display(this->get_analog_value(9));
    this->displayIn11->display(this->get_analog_value(10));
    this->displayIn12->display(this->get_analog_value(11));
    this->displayIn13->display(this->get_analog_value(12));
    this->displayIn14->display(this->get_analog_value(13));
    this->displayIn15->display(this->get_analog_value(14));
    this->displayIn16->display(this->get_analog_value(15));
}

float SensorayWindow::get_analog_value(unsigned int ch)
{
    QReadLocker readLocker(this->bufferLock);
    //return ( (float) this->buffer[ch] ) * this->scaleADC[ch] / 32767.0;
    return ( (float) this->buffer[ch] ) * (this->scaleADC[ch] / 32767.0) * (conversions[ch]);
}

void SensorayWindow::initialize_members()
{
    this->bufferLock = new QReadWriteLock(); // initialize the read write locker
    this->set_verbose(false);
    this->set_writing(false);
    this->buffer = this->Sensoray::returnBufferPointer();
    this->scaleADC = this->Sensoray::returnAnalogInputScale();
    this->conversions = new float[16];
    this->default_conversions_to_buffer();
    this->update_buffer();
}

void SensorayWindow::initialize_window()
{
    this->connect_signals();

    // update the LCD displays on a regular interval
    this->displayBufferTimer = new QTimer(this);
    connect(this->displayBufferTimer, SIGNAL(timeout()), this, SLOT(display_buffer()));
    this->displayBufferTimer->start(1000); // do not need to be faster than 1 sec
}

bool SensorayWindow::is_writing()
{
    return this->writing;
}

float * SensorayWindow::returnConversionsPointer()
{
    return this->conversions;
}

void SensorayWindow::set_conversions()
{
    SensorayDialog sd(this->returnConversionsPointer());
    sd.exec();
    /*if (sd.exec())
    {
        qDebug() << "conversion values updated";
    }
    else
    {
        qDebug() << "user defined conversions cancelled";
    }*/
}

void SensorayWindow::set_writing(bool flg)
{
    this->writing = flg;
}

bool SensorayWindow::start_thread()
{
    // select sample time
    bool ok;
    double ts = QInputDialog::getDouble(this, tr("enter sample time"),
                                  tr("ms"), 10.0, 10.0, 100.0, 1, &ok);

    return this->start_thread(ts);
}

bool SensorayWindow::start_thread(double ts)
{
    QString tmpString;
    tmpString.setNum(ts);

    if (this->sensorayTimer.startTicking<SensorayWindow>(*this, &SensorayWindow::threaded_function, ts))
    {
        this->autoUpdateCheckBox->setText("Ts = " + tmpString + " ms");
        this->autoUpdateCheckBox->setChecked(true);
        this->updateButton->setEnabled(false); // there is no need to manually update anymore
        return true;
    }
    else
    {
        return false;
    }
}

bool SensorayWindow::stop_thread()
{
    if (this->sensorayTimer.stopTicking())
    {
        this->autoUpdateCheckBox->setText("Auto Update");
        this->autoUpdateCheckBox->setChecked(false);
        this->updateButton->setEnabled(true);
        return true;
    }
    else
    {
        return false;
    }
}

bool SensorayWindow::threaded_function(double elapsed_ms)
{
    this->update_buffer();

    if (this->is_writing())
    {
        // save buffers
        float tmpTime = (float) elapsed_ms;
        float tmpBuffer[16];
        for (unsigned int ii = 0; ii < 16; ii++)
        {
            tmpBuffer[ii] = this->get_analog_value(ii);
        }

        // write buffers
        this->fh.write(& tmpTime);

        if (! this->fh.get_binary()) // include delimiters for ascii mode
        {
            char comma[1] = {','};
            this->fh.write(comma);
            //this.fh.write(","); // this will also work
        }

        for (unsigned int ii = 0; ii < 16; ii++)
        {
            this->fh.write(tmpBuffer + ii); // send the appropriate array sequentially

            if (! this->fh.get_binary()) // include delimiters for ascii mode
            {
                if (ii == 15)
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

void SensorayWindow::toggle_thread(bool flg)
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

void SensorayWindow::toggle_write(bool flg)
{
    if (flg) // attempt to write buffer to file
    {
        FileHandlerDialog fhDialog(& this->fh);
        if (fhDialog.exec()) // file selected
        {
            this->fh.create_file();

            // stop the timer if it is already ticking
            if (this->sensorayTimer.isTicking()) // timer is indeed ticking
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
            float tmpBuffer[17];
            this->fh.to_ascii(this->fh.get_filename(), this->fh.get_asciiFilename(),tmpBuffer,17);
        }
        this->set_writing(false);
    }
}

void SensorayWindow::update_buffer()
{
    // need to write protect this as a different thread can call this
    QWriteLocker writeLocker(this->bufferLock);

    // reads the card
    this->Sensoray::readIn();
}

void SensorayWindow::update_inputs()
{
    this->update_buffer();
    this->display_buffer();
}

void SensorayWindow::update_send()
{
    qDebug() << "SensorayWindow::update_send()";

    double val1 = this->lineOut1->text().toDouble(); // units in V
    qDebug()  << "Send " << val1 << " to Channel 1";
    this->sendOut(0,val1);

    double val2 = this->lineOut2->text().toDouble(); // units in V
    qDebug()  << "Send " << val2 << " to Channel 2";
    this->sendOut(1,val2);

    double val3 = this->lineOut3->text().toDouble(); // units in V
    qDebug()  << "Send " << val3 << " to Channel 3";
    this->sendOut(2,val3);

    double val4 = this->lineOut4->text().toDouble(); // units in V
    qDebug()  << "Send " << val4 << " to Channel 4";
    this->sendOut(3,val4);
}
