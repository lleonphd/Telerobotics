/**
  * @file Sensoray.cpp
  * @brief Code for the Sensoray class methods
  * @author Lisandro Leon
  */

#include "Sensoray/Sensoray.h"

#include <iostream>

using namespace std;

extern "C"
{   /*
     * make sure the files are included in this order
     * you will get an error if you include s626core.h before s626drv.h
     */
    #include "DAQ/s626drv.h"
    #include "DAQ/App626.h"
    #include "DAQ/s626mod.h"
    #include "DAQ/s626core.h"
    #include "DAQ/s626api.h"
}

Sensoray::Sensoray()
{
    cout << "Sensoray::Sensoray()" << endl;
    cout << "Trying to open the s626 DAQ card" << endl;

    // OPEN THE 626 BOARD
    S626_OpenBoard(0, 0, NULL, 1);

    DWORD ErrCode = S626_GetErrors(0);

    if (ErrCode)
    {
            cout << "Error(s) opening the s626 DAQ card." << endl;

            // List errors.
            if ( ErrCode & 0x00000001 )
                    cout << " * Failed to open kernel-mode driver." << endl;
            if ( ErrCode & 0x00000002 )
                    cout << " * Can't detect/register board." << endl;
            if ( ErrCode & 0x00000004 )
                    cout << " * Memory allocation error." << endl;
            if ( ErrCode & 0x00000008 )
                    cout << " * Can't lock DMA buffer." << endl;
            if ( ErrCode & 0x00000010 )
                    cout << " * Can't launch interrupt thread." << endl;
            if ( ErrCode & 0x00000020 )
                    cout << " * Can't enable IRQ." << endl;
            if ( ErrCode & 0x00000040 )
                    cout << " * Missed interrupt." << endl;
            if ( ErrCode & 0x00000080 )
                    cout << " * Can't instantiate board object." << endl;
            if ( ErrCode & 0x00000100 )
                    cout << " * Unsupported kernel-mode driver version." << endl;
            if ( ErrCode & 0x00010000 )
                    cout << " * D/A communication timeout." << endl;
            if ( ErrCode & 0x00020000 )
                    cout << " * Illegal counter parameter." << endl;

            return;
    }
    else
    {
        cout << "No errors. Succeeded opening the s626 DAQ card" << endl;
    }

    // initialize DAC outputs
    this->resetDAC();

    /* setup the ADC channels */
    this->poll_list = new BYTE [16];            // List of items to be digitized.
    this->databuf = new SHORT [16];             // Buffer to receive digitized data.

    // Populate the poll list.
    poll_list[0] = 0 | RANGE_10V;               // Chan 0, +-10V range.
    poll_list[1] = 1 | RANGE_10V;               // Chan 1, +-10V range.
    poll_list[2] = 2 | RANGE_10V;               // Chan 2, +-10V range.
    poll_list[3] = 3 | RANGE_10V;               // Chan 3, +-10V range.
    poll_list[4] = 4 | RANGE_10V;               // Chan 4, +-10V range.
    poll_list[5] = 5 | RANGE_10V;               // Chan 5, +-10V range.
    poll_list[6] = 6 | RANGE_10V;               // Chan 6, +-10V range.
    poll_list[7] = 7 | RANGE_10V;               // Chan 7, +-10V range.
    poll_list[8] = 8 | RANGE_10V;               // Chan 8, +-10V range.
    poll_list[9] = 9 | RANGE_10V;               // Chan 9, +-10V range.
    poll_list[10] = 10 | RANGE_10V;             // Chan 10, +-10V range.
    poll_list[11] = 11 | RANGE_10V;             // Chan 11, +-10V range.
    poll_list[12] = 12 | RANGE_10V;             // Chan 12, +-10V range.
    poll_list[13] = 13 | RANGE_10V;             // Chan 13, +-10V range.
    poll_list[14] = 14 | RANGE_10V;             // Chan 14, +-10V range.
    poll_list[15] = 15 | RANGE_10V | EOPL;      // Chan 15, +-10V range.

    S626_ResetADC(0, poll_list);

    // set the analog input scale based on the poll list
    analogInputScale[0] = 10.0;
    analogInputScale[1] = 10.0;
    analogInputScale[2] = 10.0;
    analogInputScale[3] = 10.0;
    analogInputScale[4] = 10.0;
    analogInputScale[5] = 10.0;
    analogInputScale[6] = 10.0;
    analogInputScale[7] = 10.0;
    analogInputScale[8] = 10.0;
    analogInputScale[9] = 10.0;
    analogInputScale[10] = 10.0;
    analogInputScale[11] = 10.0;
    analogInputScale[12] = 10.0;
    analogInputScale[13] = 10.0;
    analogInputScale[14] = 10.0;
    analogInputScale[15] = 10.0;
}

Sensoray::~Sensoray()
{
    cout << "Sensoray::~Sensoray()" << endl;

    cout << "resetting DAQs" << endl;
    this->resetDAC();

    cout << "Closing s626 DAQ card" << endl;
    S626_CloseBoard(0);

    /* I don't think I have to do this */
    //cout << "free memory for analog inputs" << endl;
    //delete this->poll_list;
    //delete [] this->databuf;
}

LONG Sensoray::convertVoltsOut(double val)
{
    return (LONG) (val * (HIGH_VOLTS/10)); // Convert from double volts to integer values
}

void Sensoray::readIn()
{
    S626_ReadADC( 0, (WORD *)databuf );
}

void Sensoray::resetDAC() // resets all DAQs to zero
{
    S626_WriteDAC(0, 0, OFF_VOLTS);
    S626_WriteDAC(0, 1, OFF_VOLTS);
    S626_WriteDAC(0, 2, OFF_VOLTS);
    S626_WriteDAC(0, 3, OFF_VOLTS);
    cout << "DAQs successfully reset to zero" << endl;
}

float * Sensoray::returnAnalogInputScale()
{
    return this->analogInputScale;
}

SHORT * Sensoray::returnBufferPointer()
{
    return this->databuf;
}

float Sensoray::returnBufferValue(unsigned int chn)
{
    return ( (float) databuf[chn] ) * analogInputScale[chn] / 32767.0;
}

bool Sensoray::sendOut(WORD channel, double val)
{
    if (val >= 10.0)
    {
        S626_WriteDAC(0, channel, HIGH_VOLTS);
    }
    else if (val <= -10.0)
    {
        S626_WriteDAC(0, channel, LOW_VOLTS);
    }
    else
    {
        S626_WriteDAC(0, channel, this->convertVoltsOut(val));
    }
    return true;
}
