/**
  * @file SensorayDialog.cpp
  * @brief Code for the SensorayDialog class methods
  * @author Lisandro Leon
  */

#include "SensorayDialog/SensorayDialog.h"

SensorayDialog::SensorayDialog(float * ptrArray, QWidget * parent) : QDialog(parent)
{
    this->dialogBuffer = ptrArray;
    this->create_buttons();
    this->create_labels();
    this->create_line_entries();
    this->create_layouts();
    this->connect_slots();
    this->update_entries_from_buffer();
}

SensorayDialog::~SensorayDialog()
{
    qDebug() << "SensorayDialog::~SensorayDialog()";
}

void SensorayDialog::cancel_conversions()
{
    this->close(); // close this widget
}

void SensorayDialog::closeEvent(QCloseEvent *event)
{
    qDebug() << "SensorayDialog::closeEvent(QCloseEvent *event)";
}

void SensorayDialog::connect_slots()
{
    connect(this->cancelButton, SIGNAL(clicked()), this, SLOT(cancel_conversions()));
    connect(this->defaultButton, SIGNAL(clicked()), this, SLOT(default_conversions()));
    connect(this->resetButton, SIGNAL(clicked()), this, SLOT(reset_conversions()));
    connect(this->saveButton, SIGNAL(clicked()), this, SLOT(save_conversions()));
    //connect(this, SIGNAL(accepted()), this, SLOT(save_properties()));
}

void SensorayDialog::create_buttons()
{
    this->cancelButton = new QPushButton();
    this->cancelButton->setText("Cancel");

    this->defaultButton = new QPushButton();
    this->defaultButton->setText("Default");

    this->resetButton = new QPushButton();
    this->resetButton->setText("Reset");

    this->saveButton = new QPushButton();
    this->saveButton->setText("Save");
}

void SensorayDialog::create_labels()
{
    this->channel1 = new QLabel("Channel 0");
    this->channel2 = new QLabel("Channel 1");
    this->channel3 = new QLabel("Channel 2");
    this->channel4 = new QLabel("Channel 3");
    this->channel5 = new QLabel("Channel 4");
    this->channel6 = new QLabel("Channel 5");
    this->channel7 = new QLabel("Channel 6");
    this->channel8 = new QLabel("Channel 7");
    this->channel9 = new QLabel("Channel 8");
    this->channel10 = new QLabel("Channel 9");
    this->channel11 = new QLabel("Channel 10");
    this->channel12 = new QLabel("Channel 11");
    this->channel13 = new QLabel("Channel 12");
    this->channel14 = new QLabel("Channel 13");
    this->channel15 = new QLabel("Channel 14");
    this->channel16 = new QLabel("Channel 15");
}

void SensorayDialog::create_layouts()
{
    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->channel1);
    this->hlayout1->addWidget(this->lineOut1);

    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->channel2);
    this->hlayout2->addWidget(this->lineOut2);

    this->hlayout3 = new QHBoxLayout();
    this->hlayout3->addWidget(this->channel3);
    this->hlayout3->addWidget(this->lineOut3);

    this->hlayout4 = new QHBoxLayout();
    this->hlayout4->addWidget(this->channel4);
    this->hlayout4->addWidget(this->lineOut4);

    this->hlayout5 = new QHBoxLayout();
    this->hlayout5->addWidget(this->channel5);
    this->hlayout5->addWidget(this->lineOut5);

    this->hlayout6 = new QHBoxLayout();
    this->hlayout6->addWidget(this->channel6);
    this->hlayout6->addWidget(this->lineOut6);

    this->hlayout7 = new QHBoxLayout();
    this->hlayout7->addWidget(this->channel7);
    this->hlayout7->addWidget(this->lineOut7);

    this->hlayout8 = new QHBoxLayout();
    this->hlayout8->addWidget(this->channel8);
    this->hlayout8->addWidget(this->lineOut8);

    this->hlayout9 = new QHBoxLayout();
    this->hlayout9->addWidget(this->channel9);
    this->hlayout9->addWidget(this->lineOut9);

    this->hlayout10 = new QHBoxLayout();
    this->hlayout10->addWidget(this->channel10);
    this->hlayout10->addWidget(this->lineOut10);

    this->hlayout11 = new QHBoxLayout();
    this->hlayout11->addWidget(this->channel11);
    this->hlayout11->addWidget(this->lineOut11);

    this->hlayout12 = new QHBoxLayout();
    this->hlayout12->addWidget(this->channel12);
    this->hlayout12->addWidget(this->lineOut12);

    this->hlayout13 = new QHBoxLayout();
    this->hlayout13->addWidget(this->channel13);
    this->hlayout13->addWidget(this->lineOut13);

    this->hlayout14 = new QHBoxLayout();
    this->hlayout14->addWidget(this->channel14);
    this->hlayout14->addWidget(this->lineOut14);

    this->hlayout15 = new QHBoxLayout();
    this->hlayout15->addWidget(this->channel15);
    this->hlayout15->addWidget(this->lineOut15);

    this->hlayout16 = new QHBoxLayout();
    this->hlayout16->addWidget(this->channel16);
    this->hlayout16->addWidget(this->lineOut16);

    this->hlayout17 = new QHBoxLayout();
    this->hlayout17->addWidget(this->saveButton);
    this->hlayout17->addWidget(this->cancelButton);

    this->hlayout18 = new QHBoxLayout();
    this->hlayout18->addWidget(this->resetButton);
    this->hlayout18->addWidget(this->defaultButton);


    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addLayout(this->hlayout1);
    this->vlayout1->addLayout(this->hlayout2);
    this->vlayout1->addLayout(this->hlayout3);
    this->vlayout1->addLayout(this->hlayout4);
    this->vlayout1->addLayout(this->hlayout5);
    this->vlayout1->addLayout(this->hlayout6);
    this->vlayout1->addLayout(this->hlayout7);
    this->vlayout1->addLayout(this->hlayout8);
    this->vlayout1->addLayout(this->hlayout17);

    this->vlayout2 = new QVBoxLayout();
    this->vlayout2->addLayout(this->hlayout9);
    this->vlayout2->addLayout(this->hlayout10);
    this->vlayout2->addLayout(this->hlayout11);
    this->vlayout2->addLayout(this->hlayout12);
    this->vlayout2->addLayout(this->hlayout13);
    this->vlayout2->addLayout(this->hlayout14);
    this->vlayout2->addLayout(this->hlayout15);
    this->vlayout2->addLayout(this->hlayout16);
    this->vlayout2->addLayout(this->hlayout18);

    this->hlayout0 = new QHBoxLayout();
    this->hlayout0->addLayout(this->vlayout1);
    this->hlayout0->addLayout(this->vlayout2);
    this->setLayout(this->hlayout0);
}

void SensorayDialog::create_line_entries()
{
    QDoubleValidator *myDblVal = new QDoubleValidator(this);
    //myDblVal->setBottom(-10.00);
    //myDblVal->setTop(10.00);
    myDblVal->setDecimals(3);
    myDblVal->setNotation(QDoubleValidator::StandardNotation);

    this->lineOut1 = new QLineEdit();
    this->lineOut1->setText("1.0");
    this->lineOut1->setValidator(myDblVal);

    this->lineOut2 = new QLineEdit();
    this->lineOut2->setText("1.0");
    this->lineOut2->setValidator(myDblVal);

    this->lineOut3 = new QLineEdit();
    this->lineOut3->setText("1.0");
    this->lineOut3->setValidator(myDblVal);

    this->lineOut4 = new QLineEdit();
    this->lineOut4->setText("1.0");
    this->lineOut4->setValidator(myDblVal);

    this->lineOut5 = new QLineEdit();
    this->lineOut5->setText("1.0");
    this->lineOut5->setValidator(myDblVal);

    this->lineOut6 = new QLineEdit();
    this->lineOut6->setText("1.0");
    this->lineOut6->setValidator(myDblVal);

    this->lineOut7 = new QLineEdit();
    this->lineOut7->setText("1.0");
    this->lineOut7->setValidator(myDblVal);

    this->lineOut8 = new QLineEdit();
    this->lineOut8->setText("1.0");
    this->lineOut8->setValidator(myDblVal);

    this->lineOut9 = new QLineEdit();
    this->lineOut9->setText("1.0");
    this->lineOut9->setValidator(myDblVal);

    this->lineOut10 = new QLineEdit();
    this->lineOut10->setText("1.0");
    this->lineOut10->setValidator(myDblVal);

    this->lineOut11 = new QLineEdit();
    this->lineOut11->setText("1.0");
    this->lineOut11->setValidator(myDblVal);

    this->lineOut12 = new QLineEdit();
    this->lineOut12->setText("1.0");
    this->lineOut12->setValidator(myDblVal);

    this->lineOut13 = new QLineEdit();
    this->lineOut13->setText("1.0");
    this->lineOut13->setValidator(myDblVal);

    this->lineOut14 = new QLineEdit();
    this->lineOut14->setText("1.0");
    this->lineOut14->setValidator(myDblVal);

    this->lineOut15 = new QLineEdit();
    this->lineOut15->setText("1.0");
    this->lineOut15->setValidator(myDblVal);

    this->lineOut16 = new QLineEdit();
    this->lineOut16->setText("1.0");
    this->lineOut16->setValidator(myDblVal);
}

void SensorayDialog::default_conversions()
{
    this->lineOut1->setText("1");
    this->lineOut2->setText("1");
    this->lineOut3->setText("1");
    this->lineOut4->setText("1");
    this->lineOut5->setText("1");
    this->lineOut6->setText("1");
    this->lineOut7->setText("1");
    this->lineOut8->setText("1");
    this->lineOut9->setText("1");
    this->lineOut10->setText("1");
    this->lineOut11->setText("1");
    this->lineOut12->setText("1");
    this->lineOut13->setText("1");
    this->lineOut14->setText("1");
    this->lineOut15->setText("1");
    this->lineOut16->setText("1");
}

void SensorayDialog::reset_conversions()
{
    this->update_entries_from_buffer();
}

void SensorayDialog::save_conversions()
{
    this->dialogBuffer[0] = this->lineOut1->text().toFloat();
    this->dialogBuffer[1] = this->lineOut2->text().toFloat();
    this->dialogBuffer[2] = this->lineOut3->text().toFloat();
    this->dialogBuffer[3] = this->lineOut4->text().toFloat();
    this->dialogBuffer[4] = this->lineOut5->text().toFloat();
    this->dialogBuffer[5] = this->lineOut6->text().toFloat();
    this->dialogBuffer[6] = this->lineOut7->text().toFloat();
    this->dialogBuffer[7] = this->lineOut8->text().toFloat();
    this->dialogBuffer[8] = this->lineOut9->text().toFloat();
    this->dialogBuffer[9] = this->lineOut10->text().toFloat();
    this->dialogBuffer[10] = this->lineOut11->text().toFloat();
    this->dialogBuffer[11] = this->lineOut12->text().toFloat();
    this->dialogBuffer[12] = this->lineOut13->text().toFloat();
    this->dialogBuffer[13] = this->lineOut14->text().toFloat();
    this->dialogBuffer[14] = this->lineOut15->text().toFloat();
    this->dialogBuffer[15] = this->lineOut16->text().toFloat();
    this->close();
}

void SensorayDialog::update_entries_from_buffer()
{
    this->lineOut1->setText(QString::number(dialogBuffer[0]));
    this->lineOut2->setText(QString::number(dialogBuffer[1]));
    this->lineOut3->setText(QString::number(dialogBuffer[2]));
    this->lineOut4->setText(QString::number(dialogBuffer[3]));
    this->lineOut5->setText(QString::number(dialogBuffer[4]));
    this->lineOut6->setText(QString::number(dialogBuffer[5]));
    this->lineOut7->setText(QString::number(dialogBuffer[6]));
    this->lineOut8->setText(QString::number(dialogBuffer[7]));
    this->lineOut9->setText(QString::number(dialogBuffer[8]));
    this->lineOut10->setText(QString::number(dialogBuffer[9]));
    this->lineOut11->setText(QString::number(dialogBuffer[10]));
    this->lineOut12->setText(QString::number(dialogBuffer[11]));
    this->lineOut13->setText(QString::number(dialogBuffer[12]));
    this->lineOut14->setText(QString::number(dialogBuffer[13]));
    this->lineOut15->setText(QString::number(dialogBuffer[14]));
    this->lineOut16->setText(QString::number(dialogBuffer[15]));
}
