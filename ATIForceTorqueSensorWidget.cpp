/**
  * @file ATIForceTorqueSensorWidget.cpp
  * @brief Code for the ATIForceTorqueSensorWidget class methods
  * @author Lisandro Leon
  */

#include "ATIForceTorqueSensorWidget/ATIForceTorqueSensorWidget.h"

ATIForceTorqueSensorWidget::ATIForceTorqueSensorWidget(QWidget *parent) : QWidget(parent)
{
    qDebug() << "ATIForceTorqueSensorWidget Constructor";

    this->create_buttons();
    this->create_checkboxes();
    this->create_displays();
    this->create_layouts();
    this->connect_slots();
}

ATIForceTorqueSensorWidget::~ATIForceTorqueSensorWidget()
{
    qDebug() << "ATIForceTorqueSensorWidget Destructor";
}

void ATIForceTorqueSensorWidget::connect_slots()
{
    connect(this->biasButton, SIGNAL(clicked()), this, SLOT(emit_bias()));
    connect(this->measureButton, SIGNAL(clicked()), this, SLOT(emit_measure()));
    connect(this->updateButton, SIGNAL(clicked()), this, SLOT(emit_update()));
}

void ATIForceTorqueSensorWidget::create_buttons()
{
    this->biasButton = new QPushButton();
    this->biasButton->setText("Update Bias");

    this->measureButton = new QPushButton();
    this->measureButton->setText("Take Measurement");

    this->updateButton = new QPushButton();
    this->updateButton->setText("Update Buffer");
}

void ATIForceTorqueSensorWidget::create_checkboxes()
{
    this->autoUpdateCheckBox = new QCheckBox(tr("Auto Update Buffer"));
    this->autoUpdateCheckBox->setChecked(false);

    this->writeOutputCheckBox = new QCheckBox(tr("Write Output"));
    this->writeOutputCheckBox->setChecked(false);
}

void ATIForceTorqueSensorWidget::create_displays()
{
    this->fxLabel = new QLabel("Fx (N)");
    this->fyLabel = new QLabel("Fy (N)");
    this->fzLabel = new QLabel("Fz (N)");
    this->txLabel = new QLabel("Tx (N)");
    this->tyLabel = new QLabel("Ty (N)");
    this->tzLabel = new QLabel("Tz (N)");
    this->bufferLabel = new QLabel("Buffer");
    this->runningAverageLabel = new QLabel("Running Average");

    /* display buffers */
    this->displayFx = new QLCDNumber();
    this->displayFx->setDecMode();
    this->displayFx->setDigitCount(10);
    this->displayFx->display(0.0);
    this->displayFx->setSegmentStyle(QLCDNumber::Flat);

    this->displayFy = new QLCDNumber();
    this->displayFy->setDecMode();
    this->displayFy->setDigitCount(10);
    this->displayFy->display(0.0);
    this->displayFy->setSegmentStyle(QLCDNumber::Flat);

    this->displayFz = new QLCDNumber();
    this->displayFz->setDecMode();
    this->displayFz->setDigitCount(10);
    this->displayFz->display(0.0);
    this->displayFz->setSegmentStyle(QLCDNumber::Flat);

    this->displayTx = new QLCDNumber();
    this->displayTx->setDecMode();
    this->displayTx->setDigitCount(10);
    this->displayTx->display(0.0);
    this->displayTx->setSegmentStyle(QLCDNumber::Flat);

    this->displayTy = new QLCDNumber();
    this->displayTy->setDecMode();
    this->displayTy->setDigitCount(10);
    this->displayTy->display(0.0);
    this->displayTy->setSegmentStyle(QLCDNumber::Flat);

    this->displayTz = new QLCDNumber();
    this->displayTz->setDecMode();
    this->displayTz->setDigitCount(10);
    this->displayTz->display(0.0);
    this->displayTz->setSegmentStyle(QLCDNumber::Flat);

    /* running average displays */
    this->displayFxAvg = new QLCDNumber();
    this->displayFxAvg->setDecMode();
    this->displayFxAvg->setDigitCount(10);
    this->displayFxAvg->display(0.0);
    this->displayFxAvg->setSegmentStyle(QLCDNumber::Flat);

    this->displayFyAvg = new QLCDNumber();
    this->displayFyAvg->setDecMode();
    this->displayFyAvg->setDigitCount(10);
    this->displayFyAvg->display(0.0);
    this->displayFyAvg->setSegmentStyle(QLCDNumber::Flat);

    this->displayFzAvg = new QLCDNumber();
    this->displayFzAvg->setDecMode();
    this->displayFzAvg->setDigitCount(10);
    this->displayFzAvg->display(0.0);
    this->displayFzAvg->setSegmentStyle(QLCDNumber::Flat);

    this->displayTxAvg = new QLCDNumber();
    this->displayTxAvg->setDecMode();
    this->displayTxAvg->setDigitCount(10);
    this->displayTxAvg->display(0.0);
    this->displayTxAvg->setSegmentStyle(QLCDNumber::Flat);

    this->displayTyAvg = new QLCDNumber();
    this->displayTyAvg->setDecMode();
    this->displayTyAvg->setDigitCount(10);
    this->displayTyAvg->display(0.0);
    this->displayTyAvg->setSegmentStyle(QLCDNumber::Flat);

    this->displayTzAvg = new QLCDNumber();
    this->displayTzAvg->setDecMode();
    this->displayTzAvg->setDigitCount(10);
    this->displayTzAvg->display(0.0);
    this->displayTzAvg->setSegmentStyle(QLCDNumber::Flat);

    /* Bias displays */
    this->displayFxBias = new QLCDNumber();
    this->displayFxBias->setDecMode();
    this->displayFxBias->setDigitCount(10);
    this->displayFxBias->display(0.0);
    this->displayFxBias->setSegmentStyle(QLCDNumber::Flat);

    this->displayFyBias = new QLCDNumber();
    this->displayFyBias->setDecMode();
    this->displayFyBias->setDigitCount(10);
    this->displayFyBias->display(0.0);
    this->displayFyBias->setSegmentStyle(QLCDNumber::Flat);

    this->displayFzBias = new QLCDNumber();
    this->displayFzBias->setDecMode();
    this->displayFzBias->setDigitCount(10);
    this->displayFzBias->display(0.0);
    this->displayFzBias->setSegmentStyle(QLCDNumber::Flat);

    this->displayTxBias = new QLCDNumber();
    this->displayTxBias->setDecMode();
    this->displayTxBias->setDigitCount(10);
    this->displayTxBias->display(0.0);
    this->displayTxBias->setSegmentStyle(QLCDNumber::Flat);

    this->displayTyBias = new QLCDNumber();
    this->displayTyBias->setDecMode();
    this->displayTyBias->setDigitCount(10);
    this->displayTyBias->display(0.0);
    this->displayTyBias->setSegmentStyle(QLCDNumber::Flat);

    this->displayTzBias = new QLCDNumber();
    this->displayTzBias->setDecMode();
    this->displayTzBias->setDigitCount(10);
    this->displayTzBias->display(0.0);
    this->displayTzBias->setSegmentStyle(QLCDNumber::Flat);
}

void ATIForceTorqueSensorWidget::create_layouts()
{
    // group boxes
    this->biasGroupBox = new QGroupBox(tr("Bias"));
    this->biasGroupBox->setAlignment(Qt::AlignHCenter);

    this->bufferGroupBox = new QGroupBox(tr("Buffer"));
    this->bufferGroupBox->setAlignment(Qt::AlignHCenter);

    this->measurementGroupBox = new QGroupBox(tr("Running Average"));
    this->measurementGroupBox->setAlignment(Qt::AlignHCenter);

    this->axisGroupBox = new QGroupBox(tr("Axes"));
    this->axisGroupBox->setAlignment(Qt::AlignHCenter);

    // buffer group
    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addWidget(this->displayFx);
    this->vlayout1->addWidget(this->displayFy);
    this->vlayout1->addWidget(this->displayFz);
    this->vlayout1->addWidget(this->displayTx);
    this->vlayout1->addWidget(this->displayTy);
    this->vlayout1->addWidget(this->displayTz);
    this->vlayout1->addWidget(this->updateButton);
    this->bufferGroupBox->setLayout(this->vlayout1);

    // bias group
    this->vlayout2 = new QVBoxLayout();
    this->vlayout2->addWidget(this->displayFxBias);
    this->vlayout2->addWidget(this->displayFyBias);
    this->vlayout2->addWidget(this->displayFzBias);
    this->vlayout2->addWidget(this->displayTxBias);
    this->vlayout2->addWidget(this->displayTyBias);
    this->vlayout2->addWidget(this->displayTzBias);
    this->vlayout2->addWidget(this->biasButton);
    this->biasGroupBox->setLayout(this->vlayout2);

    // running average
    this->vlayout3 = new QVBoxLayout();
    this->vlayout3->addWidget(this->displayFxAvg);
    this->vlayout3->addWidget(this->displayFyAvg);
    this->vlayout3->addWidget(this->displayFzAvg);
    this->vlayout3->addWidget(this->displayTxAvg);
    this->vlayout3->addWidget(this->displayTyAvg);
    this->vlayout3->addWidget(this->displayTzAvg);
    this->vlayout3->addWidget(this->measureButton);
    this->measurementGroupBox->setLayout(this->vlayout3);

    // axes labels
    this->vlayout4 = new QVBoxLayout();
    this->vlayout4->addWidget(this->fxLabel);
    this->vlayout4->addWidget(this->fyLabel);
    this->vlayout4->addWidget(this->fzLabel);
    this->vlayout4->addWidget(this->txLabel);
    this->vlayout4->addWidget(this->tyLabel);
    this->vlayout4->addWidget(this->tzLabel);
    this->axisGroupBox->setLayout(this->vlayout4);

    //
    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->axisGroupBox);
    this->hlayout1->addWidget(this->bufferGroupBox);
    this->hlayout1->addWidget(this->biasGroupBox);
    this->hlayout1->addWidget(this->measurementGroupBox);

    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->autoUpdateCheckBox);
    this->hlayout2->addWidget(this->writeOutputCheckBox);

    this->layout = new QVBoxLayout();
    this->layout->addLayout(this->hlayout1);
    this->layout->addLayout(this->hlayout2);

    this->setLayout(layout);
}

void ATIForceTorqueSensorWidget::emit_bias()
{
    emit this->signal_bias();
}

void ATIForceTorqueSensorWidget::emit_measure()
{
    emit this->signal_measure();
}

void ATIForceTorqueSensorWidget::emit_update()
{
    emit this->signal_update();
}

bool ATIForceTorqueSensorWidget::is_verbose()
{
    return this->verbose;
}

void ATIForceTorqueSensorWidget::set_verbose(bool flg)
{
    this->verbose = flg;
}
