/**
  * @file TrajectoryWidget.cpp
  * @brief Code for the TrajectoryWidget class methods
  * @author Lisandro Leon
  */

#include "TrajectoryWidget/TrajectoryWidget.h"

TrajectoryWidget::TrajectoryWidget(QWidget *parent)
    : QWidget(parent)
{
    this->create_buttons();
    this->create_checkboxes();
    this->create_layouts();
    this->connect_slots();
    this->initialize_members();
}

TrajectoryWidget::~TrajectoryWidget()
{
    qDebug() << "TrajectoryWidget::~TrajectoryWidget()";
}

void TrajectoryWidget::closeEvent(QCloseEvent *event)
{
    qDebug() << "TrajectoryWidget::closeEvent(QCloseEvent *event)";
    if (this->tableWidget->rowCount() > 0)
    {
        // give the user a chance to save the trajectories before closing
        //this->emit_save();
        this->saveButton->click();
    }
    event->accept();
}

void TrajectoryWidget::connect_slots()
{
    connect(this->abortButton, SIGNAL(clicked()), this, SLOT(emit_abort()));
    connect(this->appendButton, SIGNAL(clicked()), this, SLOT(emit_append()));
    connect(this->deleteButton, SIGNAL(clicked()), this, SLOT(emit_delete()));
    connect(this->discreteButton, SIGNAL(clicked()), this, SLOT(emit_discrete()));
    connect(this->goButton, SIGNAL(clicked()), this, SLOT(emit_go()));
    connect(this->insertButton, SIGNAL(clicked()), this, SLOT(emit_insert()));
    connect(this->loadButton, SIGNAL(clicked()), this, SLOT(emit_load()));
    connect(this->replaceButton, SIGNAL(clicked()), this, SLOT(emit_replace()));
    connect(this->runButton, SIGNAL(clicked()), this, SLOT(emit_run()));
    connect(this->saveButton, SIGNAL(clicked()), this, SLOT(emit_save()));
    connect(this->manualAdvanceCheckBox, SIGNAL(clicked(bool)), this, SLOT(emit_manual(bool)));
}

void TrajectoryWidget::create_buttons()
{
    this->appendButton = new QPushButton();
    this->appendButton->setText("Append");

    this->deleteButton = new QPushButton();
    this->deleteButton->setText("Delete");

    this->goButton = new QPushButton();
    this->goButton->setText("Go");

    this->insertButton = new QPushButton();
    this->insertButton->setText("Insert");

    this->replaceButton = new QPushButton();
    this->replaceButton->setText("Replace");

    this->abortButton = new QPushButton();
    this->abortButton->setText("Abort");
    this->abortButton->setStyleSheet("QPushButton { color: red; font: bold 14px }");

    this->discreteButton = new QPushButton();
    this->discreteButton->setText("Discrete");

    this->runButton = new QPushButton();
    this->runButton->setText("Run");

    this->loadButton = new QPushButton();
    this->loadButton->setText("Load");

    this->saveButton = new QPushButton();
    this->saveButton->setText("Save");
}

void TrajectoryWidget::create_checkboxes()
{
    this->manualAdvanceCheckBox = new QCheckBox();
    this->manualAdvanceCheckBox->setText("Enable Manual Advance to Next Sequence");
}

void TrajectoryWidget::create_layouts()
{
    this->editGroupBox = new QGroupBox(tr("Edit Trajectory"));
    this->executeGroupBox = new QGroupBox(tr("Execute Trajectory"));

    // layout the "edit" pushbuttons vertically
    this->vlayout1 = new QVBoxLayout();
    this->vlayout1->addWidget(this->loadButton);
    this->vlayout1->addWidget(this->saveButton);
    this->vlayout1->addWidget(this->insertButton);
    this->vlayout1->addWidget(this->appendButton);
    this->vlayout1->addWidget(this->replaceButton);
    this->vlayout1->addWidget(this->deleteButton);
    this->editGroupBox->setLayout(this->vlayout1);

    // layout the "execute" pushbuttons horizontally
    this->hlayout2 = new QHBoxLayout();
    this->hlayout2->addWidget(this->goButton);
    this->hlayout2->addWidget(this->runButton);
    this->hlayout2->addWidget(this->discreteButton);
    this->hlayout2->addWidget(this->abortButton);
    this->executeGroupBox->setLayout(this->hlayout2);

    // combine tableWidget with "edit" pushbuttons
    this->hlayout1 = new QHBoxLayout();
    this->hlayout1->addWidget(this->editGroupBox);
    this->initialize_table();
    this->hlayout1->addWidget(this->tableWidget);

    // combine everything into the top-most layout
    this->layout = new QVBoxLayout();
    this->layout->addLayout(this->hlayout1);
    this->layout->addWidget(this->executeGroupBox);
    this->layout->addWidget(this->manualAdvanceCheckBox);

    this->setLayout(this->layout);
}

void TrajectoryWidget::disable_action()
{
    this->discreteButton->setEnabled(false);
    this->goButton->setEnabled(false);
    this->runButton->setEnabled(false);
    //this->abortButton->setDefault(true);
}

void TrajectoryWidget::emit_abort()
{
    emit this->signal_abort();
}

void TrajectoryWidget::emit_append()
{
    emit this->signal_append();
}

void TrajectoryWidget::emit_delete()
{
    emit this->signal_delete();
}

void TrajectoryWidget::emit_discrete()
{
    emit this->signal_discrete();
}

void TrajectoryWidget::emit_go()
{
    emit this->signal_go();
}

void TrajectoryWidget::emit_insert()
{
    emit this->signal_insert();
}

void TrajectoryWidget::emit_load()
{
    emit this->signal_load();
}

void TrajectoryWidget::emit_manual(bool flg)
{
    emit this->signal_manual(flg);
}

void TrajectoryWidget::emit_replace()
{
    emit this->signal_replace();
}

void TrajectoryWidget::emit_run()
{
    emit this->signal_run();
}

void TrajectoryWidget::emit_save()
{
    emit this->signal_save();
}

void TrajectoryWidget::enable_action()
{
    this->discreteButton->setEnabled(true);
    this->goButton->setEnabled(true);
    this->runButton->setEnabled(true);
    //this->abortButton->setDefault(true);
}

float TrajectoryWidget::get_contents(int row, int col)
{
    return this->tableWidget->item(row, col)->text().toFloat();
}

int TrajectoryWidget::get_current_row()
{
    return this->tableWidget->currentRow();
}

int TrajectoryWidget::get_row_count()
{
    return this->tableWidget->rowCount();
}

void TrajectoryWidget::initialize_members()
{
    this->set_verbose(false);
}

void TrajectoryWidget::initialize_table()
{
    this->headerStringList <<
            "Manipulator (mm)" << "Manipulator Speed (mm/s)" <<
            "Insertion (mm)" << "Insertion Speed (mm/s)" <<
            "AMC (deg)";

    this->tableWidget = new QTableWidget(this);
    this->tableWidget->setColumnCount(5);
    this->tableWidget->setRowCount(0);
    this->tableWidget->setHorizontalHeaderLabels(this->headerStringList);
    this->set_controls();
}

bool TrajectoryWidget::is_verbose()
{
    return this->verbose;
}

void TrajectoryWidget::reset_table()
{
    this->tableWidget->clearContents();
    this->tableWidget->setRowCount(0);
    this->set_controls();
}

void TrajectoryWidget::set_contents(int row, int col, float val)
{
    QString newString;
    newString.setNum(val);
    QTableWidgetItem *newContents = new QTableWidgetItem(newString);

    //newContents->setFlags(Qt::NoItemFlags); // make read only
    this->tableWidget->setItem(row, col, newContents);
    this->set_controls();
}

void TrajectoryWidget::set_controls()
{
    if (this->tableWidget->rowCount() < 1)
    {
        this->abortButton->setEnabled(false);
        this->deleteButton->setEnabled(false);
        this->goButton->setEnabled(false);
        this->insertButton->setEnabled(false);
        this->replaceButton->setEnabled(false);
        this->runButton->setEnabled(false);
        this->discreteButton->setEnabled(false);
        this->saveButton->setEnabled(false);
    }
    else
    {
        this->abortButton->setEnabled(true);
        this->deleteButton->setEnabled(true);
        this->goButton->setEnabled(true);
        this->insertButton->setEnabled(true);
        this->replaceButton->setEnabled(true);
        this->runButton->setEnabled(true);
        this->discreteButton->setEnabled(true);
        this->saveButton->setEnabled(true);

        // set the current cell to row 1, col 1 if not already set to something
        if ((this->tableWidget->currentRow()) < 0)
        {
            this->tableWidget->setCurrentCell(0,0);
        }
    }
    this->tableWidget->resizeColumnsToContents();
}

void TrajectoryWidget::set_focus()
{
    this->abortButton->setFocus();
}

void TrajectoryWidget::set_verbose(bool flg)
{
    this->verbose = flg;
}
