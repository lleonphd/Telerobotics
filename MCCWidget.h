#ifndef _MCC_WIDGET_H_
#define _MCC_WIDGET_H_

/*
 * include Qt header files
 */
#include <QtGui/QWidget>
#include <QCheckBox>
#include <QDebug>
#include <QLCDNumber>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>


/*
 * include my own header files
 */


class MCCWidget : public QWidget
{
    Q_OBJECT

private:
    void connect_slots();
    void create_buttons();
    void create_checkboxes();
    void create_displays();
    void create_layouts();
    void create_menus();
    void initialize_members();

public:
    MCCWidget(QWidget *parent = 0);
    ~MCCWidget();

    /* define lcd displays for thermocouples */
    QLCDNumber * thermo1Display;
    QLCDNumber * thermo2Display;
    QLCDNumber * thermo3Display;
    QLCDNumber * thermo4Display;
    QLCDNumber * thermo5Display;
    QLCDNumber * thermo6Display;
    QLCDNumber * thermo7Display;
    QLCDNumber * thermo8Display;

    /* label each thermocouple lcd display */
    QLabel * thermo1Label;
    QLabel * thermo2Label;
    QLabel * thermo3Label;
    QLabel * thermo4Label;
    QLabel * thermo5Label;
    QLabel * thermo6Label;
    QLabel * thermo7Label;
    QLabel * thermo8Label;

    /* main layout of the GUI */
    QHBoxLayout * layout0;
    QVBoxLayout * layout;

    /* vertical layout that LCD numbers are arranged within */
    QVBoxLayout * vlayout1;
    QVBoxLayout * vlayout2;

    /* each thermocouple lcd and lable are arranged horizontally */
    QHBoxLayout * hlayout1;
    QHBoxLayout * hlayout2;
    QHBoxLayout * hlayout3;
    QHBoxLayout * hlayout4;
    QHBoxLayout * hlayout5;
    QHBoxLayout * hlayout6;
    QHBoxLayout * hlayout7;
    QHBoxLayout * hlayout8;
    QHBoxLayout * hlayout9;

    /* pushbuttons */
    QPushButton * blinkButton;
    QPushButton * burnoutButton;
    QPushButton * calibrationButton;
    QPushButton * cjcButton;
    QPushButton * resetButton;
    QPushButton * serialButton;
    QPushButton * statusButton;
    QPushButton * updateButton;

    QCheckBox * autoUpdateCheckBox;
    QCheckBox * writeOutputCheckBox;
    QCheckBox * fahrCheckBox;

public slots:
    void emit_blink();
    void emit_burnout();
    void emit_calibration();
    void emit_cjc();
    void emit_reset();
    void emit_serial();
    void emit_status();
    void emit_update();

signals:
    void signal_blink();    // blink the DAQ
    void signal_burnout();
    void signal_calibration();
    void signal_cjc();
    void signal_reset();
    void signal_serial();
    void signal_status();
    void signal_update();   // update input channel display

};

#endif // _MCC_WIDGET_H_
