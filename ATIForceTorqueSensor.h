/**
  * @file ATIForceTorqueSensor.h
  * @brief Declaration of the ATIForceTorqueSensor class
  */

#ifndef _ATI_FORCE_TORQUE_SENSOR_H_
#define _ATI_FORCE_TORQUE_SENSOR_H_

#include <iostream>
#include <comedilib.h>
#include "ftconfig.h"

using namespace std;


/**
 * A class that wraps up functions for obtaining data from an ATI force torque sensor
 * with it's associated AtoD card.
 *
 */
class ATIForceTorqueSensor {
	private:
	Calibration *cal;

	comedi_t *daq;
	comedi_range *rangefx, *rangefy, *rangefz;
	comedi_range *rangetx, *rangety, *rangetz;
	int mfx, mfy, mfz;
	int mtx, mty, mtz;
	int subdevice;
	
	bool biased;
        bool initialized;

	float bias[6];

	public:
	// Constructors.

        /*
         * in Art Mahoney's original files, he passed the argumenets into the constructor
         * in lleon's implementation, the constructor is empty.
         *      the arguments are passed to the initialize member function
         * ATIForceTorqueSensor(const char *comedi_device, const char *calibration_file);
         * ATIForceTorqueSensor(const char *comedi_device, const char *calibration_file, const char *bias_file);
         */

        ATIForceTorqueSensor();
        ~ATIForceTorqueSensor();

        bool initialize(const char *comedi_device, const char *calibration_file);
        bool initialize(const char *comedi_device, const char *calibration_file, float * tool_transform);
        //bool initialize(const char *comedi_device, const char *calibration_file, const char *bias_file);

	bool getForceTorque(float *forcetorque); // gets the force & torque from the card and biases
	bool getUnbiasedForceTorque(float *forcetorque); // gets the force & torque from the card without bias
	bool performBias(); // takes 200 samples to produce an average bias for the force & torque
	bool performBias(int); // takes a user defined number of samples to produce an average bias for the force & torque
	bool performBias(const char *bias_file); // sets the bias to that given by the file
	//bool getBias(float *bias); // gets the files
	bool saveBias(const char *bias_file); // saves the bias to a file
        float* getBias();

        bool is_biased();
        bool is_initialized();
};


#endif
