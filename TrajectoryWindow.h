/**
  * @file TrajectoryWindow.h
  * @brief Declaration of the TrajectoryWindow class
  * @author Lisandro Leon
  */

#ifndef _TRAJECTORY_WINDOW_H_
#define _TRAJECTORY_WINDOW_H_

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <QWidget>

#include <fstream>
#include <vector>

#include "TrajectoryWidget/TrajectoryWidget.h"

using namespace std;

class TrajectoryWindow : public TrajectoryWidget
{
    Q_OBJECT

private:
    /// boolean flag for running experiments in manual vs automated mode
    bool manualMode;

    /**
      \fn void connect_signals()
      \brief connects the signals from TrajectoryWidget to slots
      \return void
      */
    void connect_signals();

public:
    TrajectoryWindow(QWidget * parent = 0);
    ~TrajectoryWindow();

    /**
      \fn float get_amc_ref(int val)
      \brief returns the AMC command from the trajectory table
      \return the value of the AMC command as a float
      \param val is the row from the trajectory table
      */
    float get_amc_ref(int val);

    /**
      \fn float get_insertion_ref(int val)
      \brief returns the insertion stage command from the trajectory table
      \return the value of the insertion stage command as a float
      \param val is the row from the trajectory table
      */
    float get_insertion_ref(int val);

    /**
      \fn float get_insertion_speed_ref(int val)
      \brief returns the insertion stage speed from the trajectory table
      \return the value of the insertion stage speed as a float
      \param val is the row from the trajectory table
      */
    float get_insertion_speed_ref(int val);

    /**
      \fn float get_manipulator_ref(int val)
      \brief returns the manipulator stage command from the trajectory table
      \return the value of the manipulator stage command as a float
      \param val is the row from the trajectory table
      */
    float get_manipulator_ref(int val);

    /**
      \fn float get_manipulator_speed_ref(int val)
      \brief returns the manipulator stage speed from the trajectory table
      \return the value of the insertion stage speed as a float
      \param val is the row from the trajectory table
      */
    float get_manipulator_speed_ref(int val);


    bool is_manual_mode();

    /**
      \fn void set_row(int val)
      \brief sets the active row in the Trajectory table
      \return void
      \param val is the row intended to be active
      */
    void set_row(int row);

    /**
      \fn void set_trajectory(int row)
      \brief emits a signal to update the values in the trajectory table

      this function is intended to emit a signal prompting the main window to query the hardware buffers for the current actuator positions.
        It is intended that the contents of trajectory table is updated with the actuator positions for the desired row.
      \return void
      \param row is the row to be updated from the trajectory table
      */
    void set_trajectory(int row);

public slots:
    void abort_trajectories();
    void append_trajectory();
    void delete_trajectory();
    void go_trajectory();
    void insert_trajectory();
    void load_trajectories();
    void replace_trajectory();
    void save_trajectories();
    void update_manual_mode(bool flg);

signals:
    void signal_go_here(int val);
    void signal_query_buffers(int val);
};

#endif // _TRAJECTORY_WINDOW_H_
