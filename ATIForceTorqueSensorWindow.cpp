/**
  * @file ATIForceTorqueSensorWindow.cpp
  * @brief Code for the ATIForceTorqueSensorWindow class methods
  */

#include "ATIForceTorqueSensorWindow/ATIForceTorqueSensorWindow.h"

ATIForceTorqueSensorWindow::ATIForceTorqueSensorWindow(QWidget *parent)
    : ATIForceTorqueSensorWidget(parent)
{
    this->initialize_members();
    this->initialize_window();
}

ATIForceTorqueSensorWindow::ATIForceTorqueSensorWindow(QString comediDevice, QString calibrationFile, QWidget * parent)
    : ATIForceTorqueSensorWidget(parent)
{
    this->set_comedi(comediDevice);
    this->set_calibration(calibrationFile);

    this->initialize_members();
    this->initialize_window();
    if (this->connect_nano())
    {
        this->update_buffer();
    }
}

ATIForceTorqueSensorWindow::ATIForceTorqueSensorWindow(QString comediDevice, QString calibrationFile, float * tool_transform, QWidget * parent)
    : ATIForceTorqueSensorWidget(parent)
{
    this->set_comedi(comediDevice);
    this->set_calibration(calibrationFile);

    this->initialize_members();
    this->initialize_window();
    if (this->connect_nano(tool_transform))
    {
        this->update_buffer();
    }
}

ATIForceTorqueSensorWindow::~ATIForceTorqueSensorWindow()
{
    qDebug() << "ATIForceTorqueSensorWindow::~ATIForceTorqueSensorWindow()";
}

void ATIForceTorqueSensorWindow::closeEvent(QCloseEvent *event)
{
    if (this->is_writing()) // close the file
    {
        this->fh.close();
    }
}

bool ATIForceTorqueSensorWindow::connect_nano()
{
    if (this->comediString.isEmpty() || this->calibrationString.isEmpty())
    {
        qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
        qDebug() << "\t not enough data to connect with Nano17";
        return false;
    }
    else
    {
        this->ATIForceTorqueSensor::initialize(this->comediString.toStdString().c_str(),
                         this->calibrationString.toStdString().c_str());
        if (this->ATIForceTorqueSensor::is_initialized())
        {
            qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
            qDebug() << "\t Nano17 connected successfully";
            return true;
        }
        else
        {
            qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
            qDebug() << "\t failed to connect with Nano17";
            return false;
        }
    }
}

bool ATIForceTorqueSensorWindow::connect_nano(float * tool_transform)
{
    if (this->comediString.isEmpty() || this->calibrationString.isEmpty())
    {
        qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
        qDebug() << "\t not enough data to connect with Nano17";
        return false;
    }
    else
    {
        this->ATIForceTorqueSensor::initialize(this->comediString.toStdString().c_str(),
                         this->calibrationString.toStdString().c_str(), tool_transform);
        if (this->ATIForceTorqueSensor::is_initialized())
        {
            qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
            qDebug() << "\t Nano17 connected successfully";
            return true;
        }
        else
        {
            qDebug() << "ATIForceTorqueSensorWindow::connect_nano()";
            qDebug() << "\t failed to connect with Nano17";
            return false;
        }
    }
}

void ATIForceTorqueSensorWindow::connect_signals()
{
    connect(this, SIGNAL(signal_bias()), this, SLOT(update_bias()));
    connect(this, SIGNAL(signal_measure()), this, SLOT(measure()));
    connect(this, SIGNAL(signal_update()), this, SLOT(update_sensor()));
    connect(this->autoUpdateCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_thread(bool)));
    connect(this->writeOutputCheckBox, SIGNAL(clicked(bool)), this, SLOT(toggle_write(bool)));
}

void ATIForceTorqueSensorWindow::display_bias()
{
    float * biasValues = this->getBias();

    if (!this->is_biased())
    {
        qDebug() << "\t sensor has not been biased yet. Values should be zero";
    }

    if (this->is_verbose())
    {
        qDebug() << "ATIForceTorqueSensorWindow::display_bias()";
        qDebug() << "\t Bias Fx (N) = \t" << biasValues[0];
        qDebug() << "\t Bias Fy (N) = \t" << biasValues[1];
        qDebug() << "\t Bias Fz (N) = \t" << biasValues[2];
        qDebug() << "\t Bias Tx (N) = \t" << biasValues[3];
        qDebug() << "\t Bias Ty (N) = \t" << biasValues[4];
        qDebug() << "\t Bias Tz (N) = \t" << biasValues[5];
        qDebug();
    }

    this->displayFxBias->display(biasValues[0]);
    this->displayFyBias->display(biasValues[1]);
    this->displayFzBias->display(biasValues[2]);
    this->displayTxBias->display(biasValues[3]);
    this->displayTyBias->display(biasValues[4]);
    this->displayTzBias->display(biasValues[5]);
}

void ATIForceTorqueSensorWindow::display_buffer()
{
    if (this->is_verbose())
    {
        qDebug() << "ATIForceTorqueSensorWindow::display_buffer()";
        qDebug() << "\t Fx (N) = \t" << this->get_ft(0);
        qDebug() << "\t Fy (N) = \t" << this->get_ft(1);
        qDebug() << "\t Fz (N) = \t" << this->get_ft(2);
        qDebug() << "\t Tx (N) = \t" << this->get_ft(3);
        qDebug() << "\t Ty (N) = \t" << this->get_ft(4);
        qDebug() << "\t Tz (N) = \t" << this->get_ft(5);
        qDebug();
    }
    this->displayFx->display(this->get_ft(0));
    this->displayFy->display(this->get_ft(1));
    this->displayFz->display(this->get_ft(2));
    this->displayTx->display(this->get_ft(3));
    this->displayTy->display(this->get_ft(4));
    this->displayTz->display(this->get_ft(5));
}

void ATIForceTorqueSensorWindow::display_running_average()
{
    if (this->is_verbose())
    {
        qDebug() << "ATIForceTorqueSensorWindow::display_running_average()";
        qDebug() << "\t Fx (N) = \t" << this->runningAverage[0];
        qDebug() << "\t Fy (N) = \t" << this->runningAverage[1];
        qDebug() << "\t Fz (N) = \t" << this->runningAverage[2];
        qDebug() << "\t Tx (N) = \t" << this->runningAverage[3];
        qDebug() << "\t Ty (N) = \t" << this->runningAverage[4];
        qDebug() << "\t Tz (N) = \t" << this->runningAverage[5];
        qDebug() << "\t\t computed using " << this->numSamples << " samples";
        qDebug();
    }

    this->displayFxAvg->display(this->runningAverage[0]);
    this->displayFyAvg->display(this->runningAverage[1]);
    this->displayFzAvg->display(this->runningAverage[2]);
    this->displayTxAvg->display(this->runningAverage[3]);
    this->displayTyAvg->display(this->runningAverage[4]);
    this->displayTzAvg->display(this->runningAverage[5]);
}

float * ATIForceTorqueSensorWindow::get_buffer_pointer()
{
    return this->buffer;
}

float ATIForceTorqueSensorWindow::get_ft(unsigned int i)
{
    QReadLocker readLocker(this->bufferLock);
    if (i < 0) i = 0;
    if (i > 5) i = 5;
    return this->buffer[i];
}

void ATIForceTorqueSensorWindow::initialize_members()
{
    this->bufferLock = new QReadWriteLock(); // initialize the read write locker
    this->reset_buffers();
    this->set_verbose(false);
    this->set_writing(false);
}

void ATIForceTorqueSensorWindow::initialize_window()
{
    this->connect_signals();

    this->displayBufferTimer = new QTimer(this);
    connect(this->displayBufferTimer, SIGNAL(timeout()), this, SLOT(display_buffer()));
    this->displayBufferTimer->start(1000); // do not need to be faster than 1 sec
}

bool ATIForceTorqueSensorWindow::is_writing()
{
    return this->writing;
}

void ATIForceTorqueSensorWindow::measure()
{
    bool ok;
    int mt = QInputDialog::getInt(this, tr("Min = 1000, Max = 10,000, 1000 ms increments"),
                                 tr("Measurement time in ms"), 1000, 1000, 10000, 1000, &ok);
    QString tmpStr;
    tmpStr.setNum(mt/1000.0);

    /* On success, this function returns the integer which has been entered by the user;
     * on failure, it returns the initial value (which is 1000).
     */

    qDebug() << "\t\t taking measurements for " << (mt/1000.0) << " seconds";

    this->reset_buffers();

    QElapsedTimer stopWatch;
    stopWatch.start();

    while (!stopWatch.hasExpired(mt))
    {
        this->running_average();
    }

    qDebug() << "\t\t " << stopWatch.elapsed()/1000.0 << " sec. elapsed";
    qDebug();

    this->display_running_average();
    this->measurementGroupBox->setTitle("Running Avg (" + tmpStr + " sec)");
}

void ATIForceTorqueSensorWindow::reset_buffers()
{
    QWriteLocker writeLocker(this->bufferLock);
    for (unsigned int ii = 0; ii < 6; ii++)
    {
        this->cumulativeSum[ii] = 0.0;
        this->runningAverage[ii] = 0.0;
        this->buffer[ii] = 0.0;
    }
    this->numSamples = 0;
}

void ATIForceTorqueSensorWindow::running_average()
{
    /*
     * do not use this function as part of a timer on a separate thread
     */
    this->update_buffer();

    this->numSamples++;

    for (unsigned int i = 0; i < 6; i++)
    {
        this->cumulativeSum[i] += this->get_ft(i);
        this->runningAverage[i] = this->cumulativeSum[i] / this->numSamples;
    }
}

void ATIForceTorqueSensorWindow::set_bias()
{
    this->performBias();
}

void ATIForceTorqueSensorWindow::set_bias(int counts)
{
    this->performBias(counts);
}

void ATIForceTorqueSensorWindow::set_calibration(QString qString)
{
    this->calibrationString = qString;
}

void ATIForceTorqueSensorWindow::set_comedi(QString qString)
{
    this->comediString = qString;
}

void ATIForceTorqueSensorWindow::set_writing(bool flg)
{
    this->writing = flg;
}

bool ATIForceTorqueSensorWindow::start_thread()
{
    // select sample time
    bool ok;
    double ts = QInputDialog::getDouble(this, tr("enter sample time"),
                                  tr("ms"), 10.0, 10.0, 100.0, 1, &ok);

    return this->start_thread(ts);
}

bool ATIForceTorqueSensorWindow::start_thread(double ts)
{
    QString tmpString;
    tmpString.setNum(ts);

    if (this->nanoTimer.startTicking<ATIForceTorqueSensorWindow>(*this, &ATIForceTorqueSensorWindow::threaded_function, ts))
    {
        //this->autoUpdateCheckBox->setText("Auto Updating (" + tmpString + " ms)");
        //this->autoUpdateCheckBox->setText("Updating");
        this->autoUpdateCheckBox->setText("Ts = " + tmpString + " ms");
        this->autoUpdateCheckBox->setChecked(true);
        this->measureButton->setEnabled(false);
        this->biasButton->setEnabled(false);
        this->updateButton->setEnabled(false); // there is no need to manually update anymore
        return true;
    }
    else
    {
        return false;
    }
}

bool ATIForceTorqueSensorWindow::stop_thread()
{
    if (this->nanoTimer.stopTicking())
    {
        this->autoUpdateCheckBox->setText("Auto Update");
        this->autoUpdateCheckBox->setChecked(false);
        this->measureButton->setEnabled(true);
        this->biasButton->setEnabled(true);
        this->updateButton->setEnabled(true);
        return true;
    }
    else
    {
        return false;
    }
}

bool ATIForceTorqueSensorWindow::threaded_function(double elapsed_ms)
{
    this->update_buffer();

    if (this->is_writing())
    {
        // save buffers
        float tmpTime = (float) elapsed_ms;
        float tmpBuffer[6];
        for (unsigned int ii = 0; ii < 6; ii++)
        {
            tmpBuffer[ii] = this->get_ft(ii);
        }

        // write buffers
        this->fh.write(& tmpTime);

        if (! this->fh.get_binary()) // include delimiters for ascii mode
        {
            char comma[1] = {','};
            this->fh.write(comma);
            //this.fh.write(","); // this will also work
        }

        for (unsigned int ii = 0; ii < 6; ii++)
        {
            this->fh.write(tmpBuffer + ii);

            if (! this->fh.get_binary()) // include delimiters for ascii mode
            {
                if (ii == 5)
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

void ATIForceTorqueSensorWindow::toggle_thread(bool flg)
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

void ATIForceTorqueSensorWindow::toggle_write(bool flg)
{
    if (flg) // attempt to write buffer to file
    {
        FileHandlerDialog fhDialog(& this->fh);
        if (fhDialog.exec()) // file selected
        {
            this->fh.create_file();

            // stop the timer if it is already ticking
            if (this->nanoTimer.isTicking()) // timer is indeed ticking
            {
                if (!this->stop_thread()) // stop thread first
                {   // if not successful
                    qDebug() << "ATIForceTorqueSensorWindow::toggle_write(bool flg)\t ERROR resetting thread";
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
            float tmpBuffer[7];
            this->fh.to_ascii(this->fh.get_filename(), this->fh.get_asciiFilename(),tmpBuffer,7);
        }
        this->set_writing(false);
    }
}

void ATIForceTorqueSensorWindow::update_bias()
{
    // choose the number of counts needed to measure the bias
    bool ok;
    int counts = QInputDialog::getInt(this, tr("Select Bias Counts"),
                                 tr("Number of counts used to bias"), 5000, 0, 10000, 100, &ok);

    QString tmpStr;
    tmpStr.setNum(counts);

    if (ok)
    {
        this->set_bias(counts);
    }
    else
    {
        this->set_bias(); // use default
    }

    this->display_bias();
    this->biasGroupBox->setTitle("Bias (" + tmpStr + " counts)");
}

void ATIForceTorqueSensorWindow::update_buffer()
{
    // need to write protect this as a different thread can call this
    QWriteLocker writeLocker(this->bufferLock);
    this->ATIForceTorqueSensor::getForceTorque(this->buffer);
}

void ATIForceTorqueSensorWindow::update_sensor()
{
    this->update_buffer();
    this->display_buffer();
}
