/**
  * @file SensorayWindow.h
  * @brief Declaration of the SensorayWindow class
  * @author Lisandro Leon
  */

#ifndef _SENSORAY_WINDOW_H_
#define _SENSORAY_WINDOW_H_

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

#include "Sensoray/Sensoray.h"
#include "SensorayWidget/SensorayWidget.h"
#include "SensorayDialog/SensorayDialog.h"
#include "FileHandler/FileHandler.h"
#include "FileHandlerDialog/FileHandlerDialog.h"
#include "Timer/Timer.h"

// Sensoray has defined their own data types
#include "DAQ/s626types.h"

/// This class integrates other classes together into one main application to use the Sensoray S626 card.
class SensorayWindow : public SensorayWidget, public Sensoray
{
    Q_OBJECT

private:
    /// Qt mutex class needed to prevent error with threaded functions acessing common buffers
    QReadWriteLock * bufferLock;
    /// Qt timer class used to regularly update the display
    QTimer * displayBufferTimer;
    /// boolean flag indicating if the program is writing to a csv file
    bool writing;
    /// Filehandler object used to write output
    FileHandler fh;
    /// buffer to store values read from the ADC channels
    SHORT * buffer;
    /// conversion to scale the values into volts
    float * scaleADC;
    /// conversion to scale volts into some other unit that user defines
    float * conversions;
    /// Timer object designed by Art Mahoney to execute threaded functions
    Timer sensorayTimer;
    /// connect Qt signals to Qt slots
    void connect_signals();
    /// intializes values for member variables of this class
    void initialize_members();
    /// initializes the window including starting the Qt timers
    void initialize_window();
    /// allows changing of the member boolean "writing"
    void set_writing(bool flg);

    /// initiates the thread
    /**
      * @param double elapsed_ms: sets the sample time of this thread
      * @return bool: returns true if successful; returns false if not
      */
    bool threaded_function(double elapsed_ms);

    /// polls the S626 card and updates the buffer memory
    void update_buffer();

private slots:
    /// displays the buffer values to the window
    void display_buffer();

public:
    /// default constructor
    SensorayWindow(QWidget * parent = 0); // default initalization
    /// default destructor
    ~SensorayWindow();

    /// clean up stuff if the widget is closed
    void closeEvent(QCloseEvent *event);
    /// boolean flag indicating if widget is writing output
    bool is_writing();
    /// return pointer of conversions buffer
    float * returnConversionsPointer();
    /// start the thread associated with Timer object sensorayTimer
    bool start_thread();

    /// overload function with the thread associated with Timer object sensorayTimer
    /**
      * @param double ts is the sample time for the thread
      * @return boo: returns true if successful; returns false if not
      */
    bool start_thread(double ts);

    /// stop the threaded function and do some clean up
    bool stop_thread();
    /// queries the buffer for the analog value stored
    float get_analog_value(unsigned int ch);
    /// reset the conversions buffer to its default values
    void default_conversions_to_buffer();

public slots:
    /// receives the signal emitted by the set conversion push button
    void set_conversions();
    /// receives the signal emitted by the thread toggled
    void toggle_thread(bool flg);
    /// receives the signal emitted by write output checkbox
    void toggle_write(bool flg);
    /// receives the signal emitted by the send output push button
    void update_send(); // send to output channels
    /// receives the signal emitted by the update input push button
    void update_inputs(); // update input buffer
};

#endif // _SENSORAY_WINDOW_H_
