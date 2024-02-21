/**
  * @file Sensoray.h
  * @brief Declaration of the Sensoray class
  * @author Lisandro Leon
  */

#ifndef _SENSORAY_H_
#define _SENSORAY_H_

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

// Sensoray has defined their own data types; need to include header
#include "DAQ/s626types.h"

#define HIGH_VOLTS      8191                    // +10 V
#define OFF_VOLTS       0                       // 0 V
#define LOW_VOLTS       -8191                   // -10v
#define RANGE_10V	0x00			// Range code for ADC ±10V range.
#define RANGE_5V	0x10			// Range code for ADC ±5V range.
#define EOPL		0x80			// ADC end-of-poll-list marker.
#define CHANMASK	0x0F			// ADC channel number mask.

class Sensoray
{
private:
    /// List of items to be digitized.
    BYTE * poll_list;

    /// Buffer to receive digitized data.
    SHORT * databuf;

    /// scale factor based on the poll list
    float analogInputScale[16];

public:
    /// default constructor
    Sensoray();

    /// default destructor
    ~Sensoray();

    /// Convert from double volts to integer values
    LONG convertVoltsOut(double val);

    /// reads the card saves to buffer
    void readIn();

    /// resets the DAC channels to zero.
    void resetDAC();

    /// returns the analog scale for all buffers (usually +- 10V)
    float * returnAnalogInputScale();

    /// returns the pointer to the buffer
    SHORT * returnBufferPointer();

    /// Returns the value of the buffer array
    /**
      * @param unsigned int chn: determines which analog-to-digital channel to examine
      * @return float: the value of the buffer array corresponding to unsigned int chn
      */
    float returnBufferValue(unsigned int chn);

    /// sends a value through the digital to analog channel
    /**
      * @param WORD channel: identifies the channel to send out (there are 4 output channels on the S626 Card)
      * @param double val: is the value sent through the channel
      * @return bool: to indicate if operation was successful or not
      */
    bool sendOut(WORD channel, double val);
    //bool sendOut(unsigned short channel, double val);
};

#endif // _SENSORAY_H_
