/**
  * @file SensorayWidget.cpp
  * @brief Code for the SensorayWidget class methods
  * @author Lisandro Leon
  */

#include "SensorayWidget/SensorayWidget.h"

SensorayWidget::SensorayWidget(QWidget *parent) : QWidget(parent)
{
    qDebug() << "SensorayWidget::SensorayWidget(QWidget *parent)";

    this->create_buttons();
    this->create_checkboxes();
    this->create_displays();
    this->create_groupbox();
    this->create_labels();
    this->create_line_inputs();
    this->create_layout();
    this->connect_slots();
}

SensorayWidget::~SensorayWidget()
{
    qDebug() << "SensorayWidget::~SensorayWidget()";
}

void SensorayWidget::connect_slots()
{
    connect(this->sendButton, SIGNAL(clicked()), this, SLOT(emit_send()));
    connect(this->updateButton, SIGNAL(clicked()), this, SLOT(emit_update()));
    connect(this->conversionButton, SIGNAL(clicked()), this, SLOT(emit_conversions()));
}

void SensorayWidget::create_buttons()
{
    this->sendButton = new QPushButton();
    this->sendButton->setText("Send Outputs");

    this->updateButton = new QPushButton();
    this->updateButton->setText("Update Inputs");

    this->conversionButton = new QPushButton();
    this->conversionButton->setText("Set Conversions");
}

void SensorayWidget::create_checkboxes()
{
    this->autoUpdateCheckBox = new QCheckBox(tr("Auto Update"));
    this->autoUpdateCheckBox->setChecked(false);

    this->writeOutputCheckBox = new QCheckBox(tr("Write Output"));
    this->writeOutputCheckBox->setChecked(false);
}

void SensorayWidget::create_displays()
{
    this->displayIn1 = new QLCDNumber();
    this->displayIn1->setDecMode();
    this->displayIn1->setDigitCount(10);
    this->displayIn1->display(0.0);
    this->displayIn1->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn2 = new QLCDNumber();
    this->displayIn2->setDecMode();
    this->displayIn2->setDigitCount(10);
    this->displayIn2->display(0.0);
    this->displayIn2->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn3 = new QLCDNumber();
    this->displayIn3->setDecMode();
    this->displayIn3->setDigitCount(10);
    this->displayIn3->display(0.0);
    this->displayIn3->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn4 = new QLCDNumber();
    this->displayIn4->setDecMode();
    this->displayIn4->setDigitCount(10);
    this->displayIn4->display(0.0);
    this->displayIn4->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn5 = new QLCDNumber();
    this->displayIn5->setDecMode();
    this->displayIn5->setDigitCount(10);
    this->displayIn5->display(0.0);
    this->displayIn5->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn6 = new QLCDNumber();
    this->displayIn6->setDecMode();
    this->displayIn6->setDigitCount(10);
    this->displayIn6->display(0.0);
    this->displayIn6->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn7 = new QLCDNumber();
    this->displayIn7->setDecMode();
    this->displayIn7->setDigitCount(10);
    this->displayIn7->display(0.0);
    this->displayIn7->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn8 = new QLCDNumber();
    this->displayIn8->setDecMode();
    this->displayIn8->setDigitCount(10);
    this->displayIn8->display(0.0);
    this->displayIn8->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn9 = new QLCDNumber();
    this->displayIn9->setDecMode();
    this->displayIn9->setDigitCount(10);
    this->displayIn9->display(0.0);
    this->displayIn9->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn10 = new QLCDNumber();
    this->displayIn10->setDecMode();
    this->displayIn10->setDigitCount(10);
    this->displayIn10->display(0.0);
    this->displayIn10->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn11 = new QLCDNumber();
    this->displayIn11->setDecMode();
    this->displayIn11->setDigitCount(10);
    this->displayIn11->display(0.0);
    this->displayIn11->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn12 = new QLCDNumber();
    this->displayIn12->setDecMode();
    this->displayIn12->setDigitCount(10);
    this->displayIn12->display(0.0);
    this->displayIn12->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn13 = new QLCDNumber();
    this->displayIn13->setDecMode();
    this->displayIn13->setDigitCount(10);
    this->displayIn13->display(0.0);
    this->displayIn13->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn14 = new QLCDNumber();
    this->displayIn14->setDecMode();
    this->displayIn14->setDigitCount(10);
    this->displayIn14->display(0.0);
    this->displayIn14->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn15 = new QLCDNumber();
    this->displayIn15->setDecMode();
    this->displayIn15->setDigitCount(10);
    this->displayIn15->display(0.0);
    this->displayIn15->setSegmentStyle(QLCDNumber::Flat);

    this->displayIn16 = new QLCDNumber();
    this->displayIn16->setDecMode();
    this->displayIn16->setDigitCount(10);
    this->displayIn16->display(0.0);
    this->displayIn16->setSegmentStyle(QLCDNumber::Flat);
}

void SensorayWidget::create_groupbox()
{
    // values to channel output
    this->outGroupBox = new QGroupBox(tr("Output (V)"));
    this->outGroupBox->setAlignment(Qt::AlignHCenter);
    this->outLabelGroupBox = new QGroupBox(tr("Output Pin ID"));
    this->outLabelGroupBox->setAlignment(Qt::AlignHCenter);

    // values from channel input
    this->inGroupBox1 = new QGroupBox(tr("Input (V)"));
    this->inGroupBox1->setAlignment(Qt::AlignHCenter);
    this->inLabelGroupBox1 = new QGroupBox(tr("Input Pin ID"));
    this->inLabelGroupBox1->setAlignment(Qt::AlignHCenter);

    this->inGroupBox2 = new QGroupBox(tr("Input (V)"));
    this->inGroupBox2->setAlignment(Qt::AlignHCenter);
    this->inLabelGroupBox2 = new QGroupBox(tr("Input Pin ID"));
    this->inLabelGroupBox2->setAlignment(Qt::AlignHCenter);

    // button group
    this->buttonGroupBox = new QGroupBox(tr("Actions"));
    this->buttonGroupBox->setAlignment(Qt::AlignHCenter);
}

void SensorayWidget::create_labels()
{
    /* widget for output channels */
    this->labelOut1 = new QLabel("DAC0 (42)");
    this->labelOut2 = new QLabel("DAC1 (44)");
    this->labelOut3 = new QLabel("DAC2 (46)");
    this->labelOut4 = new QLabel("DAC3 (48)");

    /* widget for input channels */
    this->labelIn1 = new QLabel("ADC0 (3-4)");
    this->labelIn2 = new QLabel("ADC1 (5-6)");
    this->labelIn3 = new QLabel("ADC2 (7-8)");
    this->labelIn4 = new QLabel("ADC3 (9-10)");
    this->labelIn5 = new QLabel("ADC4 (11-12)");
    this->labelIn6 = new QLabel("ADC5 (13-14)");
    this->labelIn7 = new QLabel("ADC6 (15-16)");
    this->labelIn8 = new QLabel("ADC7 (17-18)");
    this->labelIn9 = new QLabel("ADC8 (21-22)");
    this->labelIn10 = new QLabel("ADC9 (23-24)");
    this->labelIn11 = new QLabel("ADC10 (25-26)");
    this->labelIn12 = new QLabel("ADC11 (27-28)");
    this->labelIn13 = new QLabel("ADC12 (29-30)");
    this->labelIn14 = new QLabel("ADC13 (31-32)");
    this->labelIn15 = new QLabel("ADC14 (33-34)");
    this->labelIn16 = new QLabel("ADC15 (35-36)");
}

void SensorayWidget::create_layout()
{
    this->create_layout_control();
    this->create_layout_inputs();
    this->create_layout_outputs();

    this->layout0 = new QHBoxLayout();
    this->layout0->addLayout(this->hlayout1);
    this->layout0->addLayout(this->hlayout2);

    this->layout1 = new QVBoxLayout();
    this->layout1->addLayout(this->hlayout3);
    this->layout1->addWidget(this->buttonGroupBox);

    this->mainLayout = new QHBoxLayout();
    this->mainLayout->addLayout(this->layout0);
    this->mainLayout->addLayout(this->layout1);

    this->setLayout(this->mainLayout);
}

void SensorayWidget::create_layout_control()
{
    this->vlayout0 = new QVBoxLayout();
    this->vlayout0->addWidget(this->sendButton);
    this->vlayout0->addWidget(this->updateButton);
    this->vlayout0->addWidget(this->conversionButton);
    this->vlayout0->addWidget(this->autoUpdateCheckBox);
    this->vlayout0->addWidget(this->writeOutputCheckBox);
    this->buttonGroupBox->setLayout(this->vlayout0);
}

void SensorayWidget::create_layout_inputs()
{
    /* channels 0-7 */
    // input lcd displays
    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addWidget(this->displayIn1);
    this->vlayout1->addWidget(this->displayIn2);
    this->vlayout1->addWidget(this->displayIn3);
    this->vlayout1->addWidget(this->displayIn4);
    this->vlayout1->addWidget(this->displayIn5);
    this->vlayout1->addWidget(this->displayIn6);
    this->vlayout1->addWidget(this->displayIn7);
    this->vlayout1->addWidget(this->displayIn8);
    this->inGroupBox1->setLayout(this->vlayout1);

    // input pin out labels
    this->vlayout2 = new QVBoxLayout();
    this->vlayout2->addWidget(this->labelIn1);
    this->vlayout2->addWidget(this->labelIn2);
    this->vlayout2->addWidget(this->labelIn3);
    this->vlayout2->addWidget(this->labelIn4);
    this->vlayout2->addWidget(this->labelIn5);
    this->vlayout2->addWidget(this->labelIn6);
    this->vlayout2->addWidget(this->labelIn7);
    this->vlayout2->addWidget(this->labelIn8);
    this->inLabelGroupBox1->setLayout(this->vlayout2);

    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->inLabelGroupBox1);
    this->hlayout1->addWidget(this->inGroupBox1);

    /* channels 8-15 */
    // input lcd displays
    this->vlayout3 = new QVBoxLayout();
    this->vlayout3->addWidget(this->displayIn9);
    this->vlayout3->addWidget(this->displayIn10);
    this->vlayout3->addWidget(this->displayIn11);
    this->vlayout3->addWidget(this->displayIn12);
    this->vlayout3->addWidget(this->displayIn13);
    this->vlayout3->addWidget(this->displayIn14);
    this->vlayout3->addWidget(this->displayIn15);
    this->vlayout3->addWidget(this->displayIn16);
    this->inGroupBox2->setLayout(this->vlayout3);

    // input pin out labels
    this->vlayout4 = new QVBoxLayout();
    this->vlayout4->addWidget(this->labelIn9);
    this->vlayout4->addWidget(this->labelIn10);
    this->vlayout4->addWidget(this->labelIn11);
    this->vlayout4->addWidget(this->labelIn12);
    this->vlayout4->addWidget(this->labelIn13);
    this->vlayout4->addWidget(this->labelIn14);
    this->vlayout4->addWidget(this->labelIn15);
    this->vlayout4->addWidget(this->labelIn16);
    this->inLabelGroupBox2->setLayout(this->vlayout4);

    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->inGroupBox2);
    this->hlayout2->addWidget(this->inLabelGroupBox2);
}

void SensorayWidget::create_layout_outputs()
{
    // output channels displays
    this->vlayout5 = new QVBoxLayout();
    this->vlayout5->addWidget(this->labelOut1);
    this->vlayout5->addWidget(this->labelOut2);
    this->vlayout5->addWidget(this->labelOut3);
    this->vlayout5->addWidget(this->labelOut4);
    this->outGroupBox->setLayout(this->vlayout5);

    // output channel labels
    this->vlayout6 = new QVBoxLayout();
    this->vlayout6->addWidget(this->lineOut1);
    this->vlayout6->addWidget(this->lineOut2);
    this->vlayout6->addWidget(this->lineOut3);
    this->vlayout6->addWidget(this->lineOut4);
    this->outLabelGroupBox->setLayout(this->vlayout6);

    this->hlayout3 = new QHBoxLayout();
    this->hlayout3->addWidget(this->outLabelGroupBox);
    this->hlayout3->addWidget(this->outGroupBox);
}

void SensorayWidget::create_line_inputs()
{
    QDoubleValidator *myDblVal = new QDoubleValidator(this);
    myDblVal->setBottom(-10.00);
    myDblVal->setTop(10.00);
    myDblVal->setDecimals(2);
    myDblVal->setNotation(QDoubleValidator::StandardNotation);

    this->lineOut1 = new QLineEdit();
    this->lineOut1->setText("0");
    this->lineOut1->setValidator(myDblVal);

    this->lineOut2 = new QLineEdit();
    this->lineOut2->setText("0");
    this->lineOut2->setValidator(myDblVal);

    this->lineOut3 = new QLineEdit();
    this->lineOut3->setText("0");
    this->lineOut3->setValidator(myDblVal);

    this->lineOut4 = new QLineEdit();
    this->lineOut4->setText("0");
    this->lineOut4->setValidator(myDblVal);
}

void SensorayWidget::emit_conversions()
{
    emit this->signal_conversions(); // sends signal to be connected by a different object
}

void SensorayWidget::emit_send()
{
    emit this->signal_send(); // sends signal to be connected by a different object
}

void SensorayWidget::emit_update()
{
    emit this->signal_update(); // sends signal to be connected by a different object
}

bool SensorayWidget::is_verbose()
{
    return this->verbose;
}

void SensorayWidget::set_verbose(bool flg)
{
    this->verbose = flg;
}
