/**
  * @file Timer.cpp
  * @brief Code for the Timer class methods
  */

#include <iostream>
#include <math.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <limits.h>
#include "Timer.h"

using namespace std;

extern int errno; // external integer used by the OS to store errors


/**
 * This class constructor initializes the stored time to the current time when called.
 *
 */
Timer::Timer() {
    //std::cout << "Timer::Timer() \t\t constructing\n";
        ticking = false;

        getTime();

        return;
}


/**
 * This class constructor stores the time given by input_seconds plus the number of input_milliseconds.
 *
 */
Timer::Timer(int input_seconds, double input_milliseconds) {

    //std::cout << "Timer::Timer(int input_seconds, double input_milliseconds)\n";
	ticking = false;

	secs = input_seconds;
	millisecs = 0;
		
	increment(input_milliseconds);
			
	return;
}



/**
 * The class destructor makes sure that the timing thread isn't running before closing down.
 *
 */
Timer::~Timer() {

    //std::cout << "Timer::~Timer()\t\t destructing\n";
	if (ticking)
		stopTicking(); 

	return;
}


/**
 * Returns the total number of milliseconds stored in the timer.  (Warning: you could have problems with
 * overflow if the number of seconds is too big.)
 *
 */
double Timer::milliseconds() const {
	return 1000.0*secs + millisecs;
}


/**
 * Returns the total number of seconds stored in the timer.  (Warning: you could have problems with
 * overflow if the number of seconds is too big.)
 *
 */
double Timer::seconds() const {
	return secs + millisecs/1000.0;
}


/**
 * Begins executing the function 'signal_handler' at the given timestep. (See the comment in the Timer 
 * class definition for more info.)
 *
 */
bool Timer::startTicking(bool (*signal_handler)(double), double timestep_ms) {

	NormalFunctionThreadArguments *args = new NormalFunctionThreadArguments;
	args->signal_handler = signal_handler;
	args->timestep_ms = timestep_ms;
	args->timer = this;

	// Start up a new thread executed which executes the runNormalFunction() function with the provided arguments.
	if (pthread_create(&thread_handle, NULL, &Timer::runNormalFunction, (void *)args) != 0) {
		cout << "TIMER: there was an error starting the thread for the timer." << endl;
		return false;
	}

	ticking = true;

	return true;
}


void *Timer::runNormalFunction(void *args) {
	Timer time_keeper;

	NormalFunctionThreadArguments *my_args = (NormalFunctionThreadArguments *)args;

	double timestep_ms = my_args->timestep_ms;
	Timer *timer = my_args->timer;
	bool (*signal_handler)(double) = my_args->signal_handler;
	
	timer->getTime(); // store the current time
		
	// Keep looping until boolean ticking variable is set to false, then stop.
        //std::cout << "Timer::runNormalFunction(void *args) \t\t prior to while loop\n";
	while (timer->isTicking() && (*signal_handler)(time_keeper.elapsedTime().milliseconds())) {
		timer->increment(timestep_ms); // increment the time to the time interval

		timer->waitUntilPassed(); // wait until the time interval has passed
	}
        //std::cout << "Timer::runNormalFunction(void *args) \t\t after while loop\n";
	delete my_args;

	timer->ticking = false;

	pthread_exit(NULL); 
}



/**
 * Returns true if the function provided to startTicking() is still being called repeatedly at the desired
 * time interval.
 *
 */
bool Timer::isTicking() {
	return ticking;
}


/**
 * Stops repeatedly calling the function provided to startTicking().
 *
 */
bool Timer::stopTicking() {
	
	ticking = false;
	
	if (pthread_join(thread_handle, NULL) != 0) {
		cout << "TIMER: there was an error stopping the thread for the timer: " << strerror(errno) << "." << endl;
		return false;
	}

	return true;
}



/**
 * Saves the current time.  Returns false in case of an error.
 *
 */
bool Timer::getTime() {

	timespec tmp_time;

	if (clock_gettime(CLOCK_MONOTONIC, &tmp_time) < 0) {
		cout << "TIMER: an error occured when getting the current time: " << strerror(errno) << "." << endl;
		
		secs = 0;
		millisecs = 0;	
		
		return false;
	}
	
	secs = tmp_time.tv_sec;
	millisecs = tmp_time.tv_nsec/1000000.0;

	return true;
}


/**
 * Updates the stored time to the current time and returns the time elapsed.
 *
 */
Timer Timer::updateTime() {

	Timer old_time = *this;
	Timer new_time;
	
	*this = new_time;

	return new_time-old_time;
}


/**
 * Sets the time stored in the Timer.
 *
 */
void Timer::setTime(int input_seconds, double input_milliseconds) {

	secs = input_seconds;
	millisecs = 0;
		
	increment(input_milliseconds);

	return;
}


/**
 * Returns a timer that stores the elapsed time since the time that is stored in this
 * instance of the timer.
 *
 */
Timer Timer::elapsedTime() { 
    //std::cout << "Timer Timer::elapsedTime() \t\t Timer current_time\n";
	Timer current_time;
    //std::cout << "Timer Timer::elapsedTime() \t\t Timer old_time = *this\n";
	Timer old_time = *this;
    //std::cout << "Timer Timer::elapsedTime() \t\t Timer diff = current_time - old_time\n";
	Timer diff = current_time - old_time;
	
	return diff;	
}


/**
 * Holds the calling thread until the time stored in the Timer class instance has passed.  If the time
 * has already passed, then it simply returns.  Returns false in case of error.
 *
 */
bool Timer::waitUntilPassed() {
	int ret = 0;

	if (fabs(millisecs) > MAX_TIMESPEC_MILLISECONDS) { // protect against overflow
		cout << "TIMER: the stored milliseconds of " << millisecs << " will cause a timespec overflow - cannot exceed " << MAX_TIMESPEC_MILLISECONDS << "!" << endl;
		return false;
	}

	timespec tmp_time = {secs, int(millisecs*1000000.0)}; // overflow could happen here

	if ((ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &tmp_time, NULL))) {
		cout << "TIMER: an error occured while waiting for time to pass: " << strerror(ret) << "." << endl;
		return false;
	}
	
	return true;
}


/**
 * Adds the given number of milliseconds to the time stored in the Timer.
 *
 */
bool Timer::increment(double input_milliseconds) {
	int input_seconds = 0;

	if (input_milliseconds < 0.0)
		return decrement(-1.0*input_milliseconds);
	
	// Add the input milliseconds above 1000 to the seconds storage, and reduce
	// the input milliseconds to less than 1000.
	if (input_milliseconds > 1000.0) {
		input_seconds = int(input_milliseconds/1000.0);
		
		if (secs > (MAX_TIMESPEC_INTEGER-input_seconds)) { // guard against overflow	
			cout << "TIMER: adding " << input_milliseconds << " milliseconds to the Timer will cause an overflow in the stored seconds!" << endl;
			return false;
		}
		
		secs += input_seconds;	
		input_milliseconds -= input_seconds*1000.0;
	}	

	millisecs += input_milliseconds;   

	if (millisecs >= 1000.0) {
	
		if (secs > (MAX_TIMESPEC_INTEGER-1)) { // guard against overflow	
			cout << "TIMER: adding 1 millisecond to the Timer will cause an overflow in the stored seconds!" << endl;
			return false;
		}
	
		secs += 1;
		millisecs -= 1000.0;
	}	

	simplifyTime();

	return true;
}


/**
 * Subtracts the given number of milliseconds to the time stored in the Timer.
 *
 */
bool Timer::decrement(double input_milliseconds) {
	int input_seconds = 0;

	if (input_milliseconds < 0.0)
		return increment(-1.0*input_milliseconds);
	
	if (input_milliseconds > 1000.0) {
		input_seconds = int(input_milliseconds/1000.0);
		
		if (secs < (-MAX_TIMESPEC_INTEGER+input_seconds)) { // guard against overflow	
			cout << "TIMER: subtracting " << input_milliseconds << " milliseconds from the Timer will cause an overflow in the stored seconds!" << endl;
			return false;
		}
		
		secs -= input_seconds;	
		input_milliseconds -= input_seconds*1000.0;
	}	

	millisecs -= input_milliseconds;   

	if (millisecs <= -1000.0) {
	
		if (secs < (-MAX_TIMESPEC_INTEGER+1)) { // guard against overflow	
			cout << "TIMER: subtracting 1 millisecond from the Timer will cause an overflow in the stored seconds!" << endl;
			return false;
		}
	
	
		secs -= 1;
		millisecs += 1000.0;
	}
	
	simplifyTime();

	return true;
}





/**
 * This function finds the representation of time that reduces the magnitude of the seconds
 * and maximizes the magnitude of the remainder in the nanoseconds of the timespec.  In the 
 * end, the second and the nanosecond values of the timespec will have the same sign. (Note
 * that it assumes the magnitude of milliseconds is less than 1000.)
 *
 */
void Timer::simplifyTime() {

	if ((secs > 0) && (millisecs < 0.0)) {	
		millisecs += 1000.0;
		secs -= 1;
	}
	else if ((secs < 0) && (millisecs > 0.0)) {
		millisecs -= 1000.0;
		secs += 1;
	}
	

	return;
}


/**
 * Overloaded operator that increments the number of milliseconds stored by the Timer,
 * it's short-hand for using increment() or decrement().
 *
 */
Timer &Timer::operator+=(double input_milliseconds) {

	increment(input_milliseconds);

	return *this;
}


/**
 * Overloaded operator that decrements the number of milliseconds stored by the Timer,
 * it's short-hand for using increment() or decrement().
 *
 */
Timer &Timer::operator-=(double input_milliseconds) {

	decrement(input_milliseconds);

	return *this;
}


/**
 * Overloaded operator that increments this Timer by another.
 *
 */
Timer &Timer::operator+=(const Timer &timer) {

	Timer tmp = timer;
	tmp.simplifyTime(); // the milliseconds needs to be reduced before this occurs

	secs += tmp.secs;
	
	increment(tmp.millisecs);

	return *this;
}


/**
 * Overloaded operator that decrements this Timer by another.
 *
 */
Timer &Timer::operator-=(const Timer &timer) {
    //std::cout << "Timer Timer::operator-=(const Timer &timer)\n";
	Timer tmp = timer;
	tmp.simplifyTime();

	secs -= tmp.secs;
	
	decrement(tmp.millisecs);

	return *this;
}


/**
 * Overloaded operator that computes the sum of a Timer and an integer that represents milliseconds.
 *
 */
Timer Timer::operator+(double input_milliseconds) {

	Timer tmp = *this;
	
	tmp += input_milliseconds;

	return tmp;
}


/**
 * Overloaded operator that computes the difference of a Timer and an integer that represents milliseconds.
 *
 */
Timer Timer::operator-(double input_milliseconds) {

	Timer tmp = *this;
	
	tmp -= input_milliseconds;

	return tmp;
}


Timer Timer::operator+(const Timer &timer) {

    //std::cout << "Timer Timer::operator+(const Timer &timer)\n";
	Timer new_timer = *this;
	new_timer += timer; 

	return new_timer;
}

Timer Timer::operator-(const Timer &timer) {
    //std::cout << "Timer Timer::operator-(const Timer &timer)\n";
	Timer new_timer = *this;
	new_timer -= timer; 

	return new_timer;
}




Timer Timer::operator=(double input_milliseconds) {
	int input_seconds = int(input_milliseconds/1000.0);
	
	if (input_seconds < 0)
		input_milliseconds = -1.0*input_milliseconds + input_seconds*1000;
	else
		input_milliseconds = input_milliseconds - input_seconds*1000;
		
		
		
	return Timer(input_seconds, input_milliseconds);
}



/**
 * Print out the stored number of seconds and milliseconds to the provided output stream.
 *
 */
ostream &operator<<(ostream &stream, const Timer &timer) {

	cout << "(s: " << timer.secs << ", ms: " << timer.millisecs << ")";

	return stream;
}





