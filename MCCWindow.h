#ifndef _MCC_WINDOW_H_
#define _MCC_WINDOW_H_

/*
 * include Qt header files
 */
#include <QtGui/QWidget>
#include <QDebug>
#include <QInputDialog>
#include <QReadWriteLock>
#include <QTimer>

/*
 * include my own header files
 */
#include "MCCWidget/MCCWidget.h"
#include "MCC_USB_TC/MCC_USB_TC.h"
#include "FileHandler/FileHandler.h"
#include "FileHandlerDialog/FileHandlerDialog.h"
#include "Timer/Timer.h"

/*
 * include measurement computing header files
 */
#include "DAQMC/usb-tc.h"
#include "DAQMC/pmd.h"

class MCCWindow : public MCCWidget, public MCC_USB_TC
{
    Q_OBJECT

private:
    QTimer * displayBufferTimer;
    QReadWriteLock * bufferLock;

    bool writing;
    bool fahr;

    FileHandler fh;

    Timer mccTimer;

    void connect_signals();
    void initialize_members();
    void initialize_window();

    void set_fahr(bool flg);
    void set_writing(bool flg);
    bool threaded_function(double elapsed_ms);

public:
    MCCWindow(QWidget *parent = 0);
    ~MCCWindow();

    float get_temperature_values(unsigned int i);
    bool is_writing();
    bool is_fahr();

    bool start_thread();
    bool start_thread(double ts);
    bool stop_thread();

public slots:
    void device_blink();
    void device_burnout();
    void device_calibration();
    void device_cjc();
    void device_reset();
    void device_serial();
    void device_status();
    void display_buffer();
    void update_inputs();
    void toggle_fahr(bool flg);
    void toggle_thread(bool flg);
    void toggle_write(bool flg);

signals:

};

#endif // _MCC_WIDGET_H_
