#include "MCCWidget/MCCWidget.h"

MCCWidget::MCCWidget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "MCCWidget::MCCWidget(QWidget *parent)";
    this->create_buttons();
    this->create_checkboxes();
    this->create_displays();
    this->create_layouts();
    this->connect_slots();
}

MCCWidget::~MCCWidget()
{
    qDebug() << "MCCWidget::~MCCWidget()";
}

void MCCWidget::connect_slots()
{
    connect(this->blinkButton, SIGNAL(clicked()), this, SLOT(emit_blink()));
    connect(this->burnoutButton, SIGNAL(clicked()), this, SLOT(emit_burnout()));
    connect(this->cjcButton, SIGNAL(clicked()), this, SLOT(emit_cjc()));
    connect(this->calibrationButton, SIGNAL(clicked()), this, SLOT(emit_calibration()));
    connect(this->resetButton, SIGNAL(clicked()), this, SLOT(emit_reset()));
    connect(this->serialButton, SIGNAL(clicked()), this, SLOT(emit_serial()));
    connect(this->statusButton, SIGNAL(clicked()), this, SLOT(emit_status()));
    connect(this->updateButton, SIGNAL(clicked()), this, SLOT(emit_update()));
}

void MCCWidget::create_buttons()
{
    this->blinkButton = new QPushButton();
    this->blinkButton->setText("Blink");

    this->burnoutButton = new QPushButton();
    this->burnoutButton->setText("Burnout Status");

    this->calibrationButton = new QPushButton();
    this->calibrationButton->setText("Calibration");

    this->cjcButton = new QPushButton();
    this->cjcButton->setText("Read CJC");

    this->resetButton = new QPushButton();
    this->resetButton->setText("Reset");

    this->serialButton = new QPushButton();
    this->serialButton->setText("Serial Number");

    this->statusButton = new QPushButton();
    this->statusButton->setText("Status");

    this->updateButton = new QPushButton();
    this->updateButton->setText("Update");
}

void MCCWidget::create_checkboxes()
{
    this->autoUpdateCheckBox = new QCheckBox(tr("Auto Update"));
    this->autoUpdateCheckBox->setChecked(false);

    this->writeOutputCheckBox = new QCheckBox(tr("Write Output"));
    this->writeOutputCheckBox->setChecked(false);

    this->fahrCheckBox = new QCheckBox(tr("Change to F"));
    this->fahrCheckBox->setChecked(false);
}

void MCCWidget::create_displays()
{
    this->thermo1Display = new QLCDNumber;
    this->thermo2Display = new QLCDNumber;
    this->thermo3Display = new QLCDNumber;
    this->thermo4Display = new QLCDNumber;
    this->thermo5Display = new QLCDNumber;
    this->thermo6Display = new QLCDNumber;
    this->thermo7Display = new QLCDNumber;
    this->thermo8Display = new QLCDNumber;

    this->thermo1Label = new QLabel("TC 1");
    this->thermo2Label = new QLabel("TC 2");
    this->thermo3Label = new QLabel("TC 3");
    this->thermo4Label = new QLabel("TC 4");
    this->thermo5Label = new QLabel("TC 5");
    this->thermo6Label = new QLabel("TC 6");
    this->thermo7Label = new QLabel("TC 7");
    this->thermo8Label = new QLabel("TC 8");

}

void MCCWidget::create_layouts()
{
    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->thermo1Label);
    this->hlayout1->addWidget(this->thermo1Display);

    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->thermo2Label);
    this->hlayout2->addWidget(this->thermo2Display);

    this->hlayout3 = new QHBoxLayout();
    this->hlayout3->addWidget(this->thermo3Label);
    this->hlayout3->addWidget(this->thermo3Display);

    this->hlayout4 = new QHBoxLayout();
    this->hlayout4->addWidget(this->thermo4Label);
    this->hlayout4->addWidget(this->thermo4Display);

    this->hlayout5 = new QHBoxLayout();
    this->hlayout5->addWidget(this->thermo5Label);
    this->hlayout5->addWidget(this->thermo5Display);

    this->hlayout6 = new QHBoxLayout();
    this->hlayout6->addWidget(this->thermo6Label);
    this->hlayout6->addWidget(this->thermo6Display);

    this->hlayout7 = new QHBoxLayout();
    this->hlayout7->addWidget(this->thermo7Label);
    this->hlayout7->addWidget(this->thermo7Display);

    this->hlayout8 = new QHBoxLayout();
    this->hlayout8->addWidget(this->thermo8Label);
    this->hlayout8->addWidget(this->thermo8Display);

    this->hlayout9 = new QHBoxLayout();
    this->hlayout9->addWidget(this->autoUpdateCheckBox);
    this->hlayout9->addWidget(this->writeOutputCheckBox);


    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addLayout(this->hlayout1);
    this->vlayout1->addLayout(this->hlayout2);
    this->vlayout1->addLayout(this->hlayout3);
    this->vlayout1->addLayout(this->hlayout4);
    this->vlayout1->addLayout(this->hlayout5);
    this->vlayout1->addLayout(this->hlayout6);
    this->vlayout1->addLayout(this->hlayout7);
    this->vlayout1->addLayout(this->hlayout8);

    this->vlayout2 = new QVBoxLayout();
    this->vlayout2->addWidget(this->fahrCheckBox);
    this->vlayout2->addWidget(this->blinkButton);
    this->vlayout2->addWidget(this->burnoutButton);
    this->vlayout2->addWidget(this->calibrationButton);
    this->vlayout2->addWidget(this->cjcButton);
    //this->vlayout2->addWidget(this->resetButton);
    this->vlayout2->addWidget(this->serialButton);
    this->vlayout2->addWidget(this->statusButton);
    this->vlayout2->addWidget(this->updateButton);

    this->layout0 = new QHBoxLayout();
    this->layout0->addLayout(this->vlayout1);
    this->layout0->addLayout(this->vlayout2);

    this->layout = new QVBoxLayout();
    this->layout->addLayout(this->layout0);
    this->layout->addLayout(this->hlayout9);

    this->setLayout(this->layout);
    this->show();
}

void MCCWidget::create_menus()
{

}

void MCCWidget::initialize_members()
{

}

void MCCWidget::emit_blink()
{
    emit this->signal_blink();
}

void MCCWidget::emit_burnout()
{
    emit this->signal_burnout();
}

void MCCWidget::emit_calibration()
{
    emit this->signal_calibration();
}

void MCCWidget::emit_cjc()
{
    emit this->signal_cjc();
}

void MCCWidget::emit_reset()
{
    emit this->signal_reset();
}

void MCCWidget::emit_serial()
{
    emit this->signal_serial();
}

void MCCWidget::emit_status()
{
    emit this->signal_status();
}

void MCCWidget::emit_update()
{
    emit this->signal_update();
}
