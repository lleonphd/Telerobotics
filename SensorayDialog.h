/**
  * @file SensorayDialog.h
  * @brief Declaration of the SensorayDialog class
  * @author Lisandro Leon
  */

#ifndef _SENSORAY_DIALOG_H_
#define _SENSORAY_DIALOG_H_

#include <QCheckBox>
#include <QDebug>
#include <QDialog>
#include <QDoubleSpinBox>
#include <QDoubleValidator>
#include <QFileDialog>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

class SensorayDialog : public QDialog
{
    Q_OBJECT

private:
    /// pointer to horizontal layout (ADC channel 0 group)
    QHBoxLayout * hlayout1;
    /// pointer to horizontal layout (ADC channel 1 group)
    QHBoxLayout * hlayout2;
    /// pointer to horizontal layout (ADC channel 2 group)
    QHBoxLayout * hlayout3;
    /// pointer to horizontal layout (ADC channel 3 group)
    QHBoxLayout * hlayout4;
    /// pointer to horizontal layout (ADC channel 4 group)
    QHBoxLayout * hlayout5;
    /// pointer to horizontal layout (ADC channel 5 group)
    QHBoxLayout * hlayout6;
    /// pointer to horizontal layout (ADC channel 6 group)
    QHBoxLayout * hlayout7;
    /// pointer to horizontal layout (ADC channel 7 group)
    QHBoxLayout * hlayout8;
    /// pointer to horizontal layout (ADC channel 8 group)
    QHBoxLayout * hlayout9;
    /// pointer to horizontal layout (ADC channel 9 group)
    QHBoxLayout * hlayout10;
    /// pointer to horizontal layout (ADC channel 10 group)
    QHBoxLayout * hlayout11;
    /// pointer to horizontal layout (ADC channel 11 group)
    QHBoxLayout * hlayout12;
    /// pointer to horizontal layout (ADC channel 12 group)
    QHBoxLayout * hlayout13;
    /// pointer to horizontal layout (ADC channel 13 group)
    QHBoxLayout * hlayout14;
    /// pointer to horizontal layout (ADC channel 14 group)
    QHBoxLayout * hlayout15;
    /// pointer to horizontal layout (ADC channel 15 group)
    QHBoxLayout * hlayout16;
    /// pointer to horizontal layout (accept and cancel buttons)
    QHBoxLayout * hlayout17;
    /// pointer to horizontal layout (reset and default buttons)
    QHBoxLayout * hlayout18;
    /// pointer to main layout (horizontal layout)
    QHBoxLayout * hlayout0;

    /// pointer to label (ADC channel 0 group)
    QLabel * channel1;
    /// pointer to label (ADC channel 1 group)
    QLabel * channel2;
    /// pointer to label (ADC channel 2 group)
    QLabel * channel3;
    /// pointer to label (ADC channel 3 group)
    QLabel * channel4;
    /// pointer to label (ADC channel 4 group)
    QLabel * channel5;
    /// pointer to label (ADC channel 5 group)
    QLabel * channel6;
    /// pointer to label (ADC channel 6 group)
    QLabel * channel7;
    /// pointer to label (ADC channel 7 group)
    QLabel * channel8;
    /// pointer to label (ADC channel 8 group)
    QLabel * channel9;
    /// pointer to label (ADC channel 9 group)
    QLabel * channel10;
    /// pointer to label (ADC channel 10 group)
    QLabel * channel11;
    /// pointer to label (ADC channel 11 group)
    QLabel * channel12;
    /// pointer to label (ADC channel 12 group)
    QLabel * channel13;
    /// pointer to label (ADC channel 13 group)
    QLabel * channel14;
    /// pointer to label (ADC channel 14 group)
    QLabel * channel15;
    /// pointer to label (ADC channel 15 group)
    QLabel * channel16;

    /// pointer to edit line of conversion value (ADC channel 0 conversion)
    QLineEdit * lineOut1;
    /// pointer to edit line of conversion value (ADC channel 1 conversion)
    QLineEdit * lineOut2;
    /// pointer to edit line of conversion value (ADC channel 2 conversion)
    QLineEdit * lineOut3;
    /// pointer to edit line of conversion value (ADC channel 3 conversion)
    QLineEdit * lineOut4;
    /// pointer to edit line of conversion value (ADC channel 4 conversion)
    QLineEdit * lineOut5;
    /// pointer to edit line of conversion value (ADC channel 5 conversion)
    QLineEdit * lineOut6;
    /// pointer to edit line of conversion value (ADC channel 6 conversion)
    QLineEdit * lineOut7;
    /// pointer to edit line of conversion value (ADC channel 7 conversion)
    QLineEdit * lineOut8;
    /// pointer to edit line of conversion value (ADC channel 8 conversion)
    QLineEdit * lineOut9;
    /// pointer to edit line of conversion value (ADC channel 9 conversion)
    QLineEdit * lineOut10;
    /// pointer to edit line of conversion value (ADC channel 10 conversion)
    QLineEdit * lineOut11;
    /// pointer to edit line of conversion value (ADC channel 11 conversion)
    QLineEdit * lineOut12;
    /// pointer to edit line of conversion value (ADC channel 12 conversion)
    QLineEdit * lineOut13;
    /// pointer to edit line of conversion value (ADC channel 13 conversion)
    QLineEdit * lineOut14;
    /// pointer to edit line of conversion value (ADC channel 14 conversion)
    QLineEdit * lineOut15;
    /// pointer to edit line of conversion value (ADC channel 15 conversion)
    QLineEdit * lineOut16;

    /// pointer to cancel button (cancel this process)
    QPushButton * cancelButton;
    /// pointer to default button (use the default values as the conversion factors)
    QPushButton * defaultButton;
    /// pointer to reset button (reset the entries to the current conversion factors)
    QPushButton * resetButton;
    /// pointer to save button (save the entries as the new conversion factors)
    QPushButton * saveButton;

    /// pointer to vertical layout 1 (ADC channels 0-7 group)
    QVBoxLayout * vlayout1;
    /// pointer to vertical layout 1 (ADC channels 8-15 group)
    QVBoxLayout * vlayout2;

    /// this buffer is the conversion factors applied to the input channels
    float * dialogBuffer;

    /// connect the Qt signals to user defined Qt slots
    void connect_slots();
    /// create Qt buttons used in this widget
    void create_buttons();
    /// create labels identifying the channel numbers
    void create_labels();
    /// organizes the gui elements into an organized layout
    void create_layouts();
    /// create the line entries that the user can modify to convert the voltages
    void create_line_entries();

public:
    /// Default constructor for SensorayDialog
    /**
      * @param float * ptrArray: this is the pointer to the conversion values from SensorayWindow
      * @param QWidget * parent = 0: if left empty, zero will be used. You should leave it empty
      */
    SensorayDialog(float * ptrArray, QWidget * parent = 0);

    /// Default destructor for SensorayDialog
    ~SensorayDialog();

    /// replaces the line entries with the values from dialogBuffer
    void update_entries_from_buffer();

    /// used to do final stuff before this widget is finally closed. The close() method will trigger this.
    void closeEvent(QCloseEvent *event);

public slots:
    /// connect cancel button to signal_cancel
    void cancel_conversions();
    /// connect default button to signal_default
    void default_conversions();
    /// connect reset button to signal_reset
    void reset_conversions();
    /// connect save button to signal_save
    void save_conversions();
};

#endif // _SENSORAY_DIALOG_H_
