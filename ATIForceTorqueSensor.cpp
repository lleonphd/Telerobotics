#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ATIForceTorqueSensor.h"

using namespace std;


/**
 * Class constructor that takes in the comedi device (probably /dev/comedi0) as well as ATI's calibration
 * file name.
 *
 */

ATIForceTorqueSensor::ATIForceTorqueSensor(){
    std::cout << "ATIForceTorqueSensor::ATIForceTorqueSensor()" << std::endl;
}

ATIForceTorqueSensor::~ATIForceTorqueSensor(){

    std::cout << "ATIForceTorqueSensor::~ATIForceTorqueSensor()" << std::endl;
}


/**
 * Retrieves the force & torque from the AtoD card and saves it in the given float pointer
 * without applying a bias.
 *
 */
bool ATIForceTorqueSensor::getUnbiasedForceTorque(float *forcetorque) {
	if (!daq)
		return false;
		
	lsampl_t datafx, datafy, datafz;
	lsampl_t datatx, dataty, datatz;
	float data[6];	
		
	comedi_data_read(daq, subdevice, 0, 0, AREF_GROUND, &datafx);
	comedi_data_read(daq, subdevice, 1, 0, AREF_GROUND, &datafy);
	comedi_data_read(daq, subdevice, 2, 0, AREF_GROUND, &datafz);
	comedi_data_read(daq, subdevice, 3, 0, AREF_GROUND, &datatx);
	comedi_data_read(daq, subdevice, 4, 0, AREF_GROUND, &dataty);
	comedi_data_read(daq, subdevice, 5, 0, AREF_GROUND, &datatz);

	data[0] = comedi_to_phys(datafx, rangefx, mfx);
	data[1] = comedi_to_phys(datafy, rangefy, mfy);
	data[2] = comedi_to_phys(datafz, rangefz, mfz);
	data[3] = comedi_to_phys(datatx, rangetx, mtx);
	data[4] = comedi_to_phys(dataty, rangety, mty);
	data[5] = comedi_to_phys(datatz, rangetz, mtz);		

	/* used to output the temperature compensation boolean
	cout << "temp comp = " << cal->cfg.TempCompEnabled << endl;
	*/

	ConvertToFT(cal, data, forcetorque);	

	/* OUTPUT VOLTAGES FOR TROUBLESHOOTING
	forcetorque[0] = comedi_to_phys(datafx, rangefx, mfx);
	forcetorque[1] = comedi_to_phys(datafy, rangefy, mfy);
	forcetorque[2] = comedi_to_phys(datafz, rangefz, mfz);
	forcetorque[3] = comedi_to_phys(datatx, rangetx, mtx);
	forcetorque[4] = comedi_to_phys(dataty, rangety, mty);
	forcetorque[5] = comedi_to_phys(datatz, rangetz, mtz);	
	*/

	return true;
}

float * ATIForceTorqueSensor::getBias(){
    return bias;
}


/**
 * Retrieves the force & torque from the AtoD card and saves it in the given float pointer.
 * This function applies a bias.
 *
 */
bool ATIForceTorqueSensor::getForceTorque(float *forcetorque) {
	if (!daq)
		return false;

	getUnbiasedForceTorque(forcetorque);	

	if (biased) {
		for (int aa=0; aa<6; ++aa) {
			forcetorque[aa] -= bias[aa];	
			
		}
	}

	return true;
}

//ATIForceTorqueSensor::ATIForceTorqueSensor(const char *comedi_device, const char *calibration_file) {
bool ATIForceTorqueSensor::initialize(const char *comedi_device, const char *calibration_file) {

        subdevice = 0;
        biased = false;

        daq = NULL;
        daq = comedi_open(comedi_device);

        rangefx = comedi_get_range(daq, subdevice, 0, 0);
        rangefy = comedi_get_range(daq, subdevice, 1, 0);
        rangefz = comedi_get_range(daq, subdevice, 2, 0);
        rangetx = comedi_get_range(daq, subdevice, 3, 0);
        rangety = comedi_get_range(daq, subdevice, 4, 0);
        rangetz = comedi_get_range(daq, subdevice, 5, 0);

        mfx = comedi_get_maxdata(daq, subdevice, 0);
        mfy = comedi_get_maxdata(daq, subdevice, 1);
        mfz = comedi_get_maxdata(daq, subdevice, 2);
        mtx = comedi_get_maxdata(daq, subdevice, 3);
        mty = comedi_get_maxdata(daq, subdevice, 4);
        mtz = comedi_get_maxdata(daq, subdevice, 5);

        cal = NULL;
        if(!(cal = createCalibration(calibration_file, 1))) {
                cout << "ATI F/T: callibration file could not be read." << endl;
                //return;
                return false;
        }

        // default at the tool interface
        //displacements and rotations in the order Dx, Dy, Dz, Rx, Ry, Rz
        float tool_transform[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

        short sts;

        SetForceUnits(cal, "N");
        SetTorqueUnits(cal, "N-m");
        sts = SetToolTransform(cal, tool_transform, "mm", "degrees");

        switch (sts) {
            case 0: printf("\nATIForceTorqueSensor::initialize()\n\t tool transform completed\n\n"); break;	// successful completion
            case 1: printf("Invalid Calibration struct"); return 0;
            case 2: printf("Invalid distance units"); return 0;
            case 3: printf("Invalid angle units"); return 0;
            default: printf("Unknown error"); return 0;
        }

        for (int aa=0; aa<6; ++aa)
                bias[aa] = 0.0;

        //return;
        this->initialized = true;
        return this->is_initialized();
}

bool ATIForceTorqueSensor::initialize(const char *comedi_device, const char *calibration_file,
                                      float * tool_transform) {

        subdevice = 0;
        biased = false;

        daq = NULL;
        daq = comedi_open(comedi_device);

        rangefx = comedi_get_range(daq, subdevice, 0, 0);
        rangefy = comedi_get_range(daq, subdevice, 1, 0);
        rangefz = comedi_get_range(daq, subdevice, 2, 0);
        rangetx = comedi_get_range(daq, subdevice, 3, 0);
        rangety = comedi_get_range(daq, subdevice, 4, 0);
        rangetz = comedi_get_range(daq, subdevice, 5, 0);

        mfx = comedi_get_maxdata(daq, subdevice, 0);
        mfy = comedi_get_maxdata(daq, subdevice, 1);
        mfz = comedi_get_maxdata(daq, subdevice, 2);
        mtx = comedi_get_maxdata(daq, subdevice, 3);
        mty = comedi_get_maxdata(daq, subdevice, 4);
        mtz = comedi_get_maxdata(daq, subdevice, 5);

        cal = NULL;
        if(!(cal = createCalibration(calibration_file, 1))) {
                cout << "ATI F/T: callibration file could not be read." << endl;
                //return;
                return false;
        }

        short sts;

        SetForceUnits(cal, "N");
        SetTorqueUnits(cal, "N-m");
        sts = SetToolTransform(cal, tool_transform, "mm", "degrees");

        std::cout << "initializing using the following tool transform\n";
        std::cout << "\t Dx = " << tool_transform[0] << std::endl;
        std::cout << "\t Dy = " << tool_transform[1] << std::endl;
        std::cout << "\t Dz = " << tool_transform[2] << std::endl;
        std::cout << "\t Rx = " << tool_transform[3] << std::endl;
        std::cout << "\t Ry = " << tool_transform[4] << std::endl;
        std::cout << "\t Rz = " << tool_transform[5] << std::endl;

        switch (sts) {
            case 0: printf("\t\t tool transform completed \n"); break;	// successful completion
            case 1: printf("Invalid Calibration struct"); return 0;
            case 2: printf("Invalid distance units"); return 0;
            case 3: printf("Invalid angle units"); return 0;
            default: printf("Unknown error"); return 0;
        }

        for (int aa=0; aa<6; ++aa)
                bias[aa] = 0.0;

        //return;
        this->initialized = true;
        return this->is_initialized();
}

bool ATIForceTorqueSensor::is_biased()
{
    return this->biased;
}

bool ATIForceTorqueSensor::is_initialized()
{
    return this->initialized;
}


/**
 * Gathers 200 consecutive samples from the AtoD card and averages them to make a bias.
 *
 */
bool ATIForceTorqueSensor::performBias() {
	if (!daq)
		return false;

	float tmp[6];
	int counts = 200;
	
	for (int aa=0; aa<counts; ++aa) {
	
		getUnbiasedForceTorque(tmp);
	
		for (int bb=0; bb<6; ++bb)
			bias[bb] += tmp[bb];
	
	}

	for (int cc=0; cc<6; ++cc)
		bias[cc] /= (float)counts;

	biased = true;

        cout << "\nATIForceTorqueSensor::performBias()\n\t " << counts << " counts used to bias sensor\n\n";
	return true;
}


/**
 * Gathers a user defined number of consecutive samples from the AtoD card and averages them to make a bias.
 * this shoddy part of this file was developed by lleon
 *
 */
bool ATIForceTorqueSensor::performBias(int counts) {
	if (!daq)
		return false;
	
	float tmp[6];
	//int counts = 200;
	
	for (int aa=0; aa<counts; ++aa) {
		
		getUnbiasedForceTorque(tmp);
		
		for (int bb=0; bb<6; ++bb)
			bias[bb] += tmp[bb];
		
	}
	
	for (int cc=0; cc<6; ++cc)
		bias[cc] /= (float)counts;
	
	biased = true;

        cout << "\nATIForceTorqueSensor::performBias(int counts)\n\t " << counts << " counts used to bias sensor\n\n";
	return true;
}


/**
 * Reads in the values from a bias file and stores them as the bias.
 *
 */
bool ATIForceTorqueSensor::performBias(const char *bias_file) {
	if (!daq)
		return false;

	ifstream file(bias_file);
	string line;

	if (!file.is_open()) {
		cout << "ATI F/T: couldn't read bias from file " << bias_file << endl;
		return false;
	}	

	cout << "ATI F/T: reading in bias file " << bias_file << ":" << endl;

	int counter = 0;
	while(getline(file, line) && (counter < 6)) {
		stringstream str(line);
		string text;

		str >> bias[counter];
        
        	cout << "     " << bias[counter] << endl;
        	
        	counter += 1;
    	}

	biased = true;

	return true;
}


bool ATIForceTorqueSensor::saveBias(const char *bias_file) {
	if (!daq)
		return false;

	fstream file(bias_file, ios::out);
	
	if (!file.is_open()) {
		cout << "ATI F/T: couldn't save bias to file " << bias_file << endl;
		return false;
	}
	
	cout << "ATI F/T: saving bias to file " << bias_file << endl;
	
	for (int aa=0; aa<6; ++aa) {
		file << bias[aa] << endl;
		cout << "     " << bias[aa] << endl;
	}
	
	file.close();

	return true;
}
