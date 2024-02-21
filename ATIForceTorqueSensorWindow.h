/**
  * @file ATIForceTorqueSensorWindow.h
  * @brief Declaration of the ATIForceTorqueSensorWindow class
  */

#ifndef _ATI_FORCE_TORQUE_SENSOR_WINDOW_H_
#define _ATI_FORCE_TORQUE_SENSOR_WINDOW_H_

#include <QCloseEvent>
#include <QDebug>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QInputDialog>
#include <QMutex>
#include <QReadWriteLock>
#include <QString>
#include <QTimer>
#include <QWidget>

#include "ATIForceTorqueSensorWidget/ATIForceTorqueSensorWidget.h"
#include "ATIForceTorqueSensor/ATIForceTorqueSensor.h"
#include "FileHandler/FileHandler.h"
#include "FileHandlerDialog/FileHandlerDialog.h"
#include "Timer/Timer.h"

class ATIForceTorqueSensorWindow : public ATIForceTorqueSensorWidget, public ATIForceTorqueSensor
{
    Q_OBJECT

private:
    QReadWriteLock * bufferLock;
    QString filename;
    QString comediString;
    QString calibrationString;
    QTimer * displayBufferTimer;
    QTimer * measureTimer;

    bool writing;

    FileHandler fh;

    float buffer[6];
    float cumulativeSum[6];
    float runningAverage[6];

    Timer nanoTimer;

    unsigned int numSamples;

    bool connect_nano();
    bool connect_nano(float * tool_transform);
    void connect_signals();
    void display_bias();
    void display_running_average();
    void initialize_members();
    void initialize_window();
    void reset_buffers();
    void running_average();
    void set_bias();
    void set_bias(int counts);
    void set_calibration(QString qString);
    void set_comedi(QString qString);
    void set_writing(bool flg);
    bool threaded_function(double elapsed_ms);
    void update_buffer();

private slots:
    void display_buffer();

public:
    ATIForceTorqueSensorWindow(QWidget * parent = 0);
    ATIForceTorqueSensorWindow(QString comediDevice, QString calibrationFile, QWidget * parent = 0);
    ATIForceTorqueSensorWindow(QString comediDevice, QString calibrationFile, float * tool_transform, QWidget * parent = 0);
    ~ATIForceTorqueSensorWindow();

    void closeEvent(QCloseEvent *event);
    float * get_buffer_pointer();
    float get_ft(unsigned int i);
    bool is_writing();
    bool start_thread();
    bool start_thread(double ts);
    bool stop_thread();

public slots:
    void measure();
    void toggle_thread(bool flg);
    void toggle_write(bool flg);
    void update_bias();
    void update_sensor();
};

#endif // _ATI_FORCE_TORQUE_SENSOR_WINDOW_H_
