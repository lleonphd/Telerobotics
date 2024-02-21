/**
  * @file TrajectoryWindow.cpp
  * @brief Code for the TrajectoryWindow class methods
  * @author Lisandro Leon
  */

#include "TrajectoryWindow.h"

TrajectoryWindow::TrajectoryWindow(QWidget * parent) : TrajectoryWidget(parent)
{
    this->manualMode = false;
    this->connect_signals();
}

TrajectoryWindow::~TrajectoryWindow()
{
    qDebug() << "TrajectoryWindow::~TrajectoryWindow()";
}

void TrajectoryWindow::abort_trajectories()
{
    qDebug() << "ABORT BUTTON PRESSED ...";
}

void TrajectoryWindow::append_trajectory()
{
    int row = (this->tableWidget->currentRow())+1;
    this->tableWidget->insertRow(row);
    this->set_trajectory(row);
    this->set_controls();
}

void TrajectoryWindow::connect_signals()
{
    connect(this, SIGNAL(signal_abort()), this, SLOT(abort_trajectories()));
    connect(this, SIGNAL(signal_append()), this, SLOT(append_trajectory()));
    connect(this, SIGNAL(signal_delete()), this, SLOT(delete_trajectory()));
    connect(this, SIGNAL(signal_go()), this, SLOT(go_trajectory()));
    connect(this, SIGNAL(signal_insert()), this, SLOT(insert_trajectory()));
    connect(this, SIGNAL(signal_load()), this, SLOT(load_trajectories()));
    connect(this, SIGNAL(signal_replace()), this, SLOT(replace_trajectory()));
    connect(this, SIGNAL(signal_save()), this, SLOT(save_trajectories()));
    connect(this, SIGNAL(signal_manual(bool)), this, SLOT(update_manual_mode(bool)));
}

void TrajectoryWindow::delete_trajectory()
{
    int row = this->tableWidget->currentRow();
    this->tableWidget->removeRow(row);
    this->set_controls();
}

float TrajectoryWindow::get_amc_ref(int val)
{
    return this->get_contents(val,4);
}

float TrajectoryWindow::get_insertion_ref(int val)
{
    return this->get_contents(val,2);
}

float TrajectoryWindow::get_insertion_speed_ref(int val)
{
    return this->get_contents(val,3);
}

float TrajectoryWindow::get_manipulator_ref(int val)
{
    return this->get_contents(val,0);
}

float TrajectoryWindow::get_manipulator_speed_ref(int val)
{
    return this->get_contents(val,1);
}

void TrajectoryWindow::go_trajectory()
{
    int row = this->tableWidget->currentRow();

    qDebug() << "Go to trajectory at row " << (row + 1);

    emit this->signal_go_here(row);
}

void TrajectoryWindow::insert_trajectory()
{
    int row = this->tableWidget->currentRow();
    this->tableWidget->insertRow(row);
    this->set_trajectory(row);
    this->set_controls();
}

bool TrajectoryWindow::is_manual_mode()
{
    return this->manualMode;
}

void TrajectoryWindow::load_trajectories()
{
    QString fname = QFileDialog::getOpenFileName(this, "Choose file to load trajectories from", QDir::currentPath(), tr("*.csv"));

    // define variables temporarary storage of data
    std::string tmpString;
    float tmpManipulator, tmpManipulatorSpeed;
    float tmpInsertion, tmpInsertionSpeed;
    int tmpAMC;
    int count = 0; // increments with each line read from file

    if (fname.isEmpty())
    {
        qDebug() << "No file selected";
    }
    else
    {
        qDebug() << "Loading trajectories from " << fname;
        fstream trajectoryFile(fname.toStdString().c_str(), ios::in);

        // open the file
        if (trajectoryFile.is_open())
        {
            this->reset_table(); // clears table contents prior to loading new values

            // read the file
            while(!trajectoryFile.eof())
            {
                getline(trajectoryFile, tmpString);

                int readSize = sscanf(tmpString.c_str(),
                                      "%f,%f,%f,%f,%d",
                                      &tmpManipulator, &tmpManipulatorSpeed,
                                      &tmpInsertion, &tmpInsertionSpeed,
                                      &tmpAMC);

                if (readSize < 0) // no more data to parse; exit while loop
                {
                    break;
                }
                else
                {
                    this->tableWidget->insertRow(count);
                    this->set_contents(count, 0, tmpManipulator);
                    this->set_contents(count, 1, tmpManipulatorSpeed);
                    this->set_contents(count, 2, tmpInsertion);
                    this->set_contents(count, 3, tmpInsertionSpeed);
                    this->set_contents(count, 4, tmpAMC);
                    count++;
                }
            }
        }
    }
}

void TrajectoryWindow::replace_trajectory()
{
    int row = this->tableWidget->currentRow();
    this->set_trajectory(row);
    this->set_controls();
}

void TrajectoryWindow::save_trajectories()
{
    QString date = QDateTime::currentDateTime().toString();
    date.replace(' ', '-');
    date.replace(':', '.');

    QString name = "Trajectory-AutoSave-";
    QString extension = ".csv";

    name += date + extension;

    //QString fname = QFileDialog::getSaveFileName(this, "Choose file to save trajectories to", QDir::currentPath(), fname);

    QString fname = QFileDialog::getSaveFileName(this, "Choose file to save trajectories to", QDir::currentPath(), tr("*.csv"));

    if (fname.isEmpty())
    {
        qDebug() << "No file selected";
    }
    else
    {
        qDebug() << "Saving trajectories to " << fname;
        fstream trajectoryFile(fname.toStdString().c_str(), ios::out);
        for (int step = 0; step < this->tableWidget->rowCount(); step++)
        {
            trajectoryFile << this->get_manipulator_ref(step) << ",";
            trajectoryFile << this->get_manipulator_speed_ref(step) << ",";
            trajectoryFile << this->get_insertion_ref(step) << ",";
            trajectoryFile << this->get_insertion_speed_ref(step) << ",";
            trajectoryFile << this->get_amc_ref(step) << std::endl;
        }
        trajectoryFile.close();
    }
}

void TrajectoryWindow::set_row(int row)
{
    this->tableWidget->setCurrentCell(row,0);
}

void TrajectoryWindow::set_trajectory(int row)
{
    emit signal_query_buffers(row);
}

void TrajectoryWindow::update_manual_mode(bool flg)
{
    this->manualMode = flg;
    //qDebug() << "hello manual mode to " << this->is_manual_mode();
}
