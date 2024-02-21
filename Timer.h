/**
 * This class can be used for timing in two different ways: 1) it can be used to call a provided function
 * at regular time intervals, and 2) it can be used for general timing.  In the first form of usage, the
 * class becomes a thread and repeatedly calls a provided function at a given time interval.  In the second
 * form of usage, the class basically becomes a stopwatch and a structure to store time.  In this form,
 * the Timer class stores time in the form of seconds (an integer with the same name) plus milliseconds
 * (a double with the same name).  Time can be added/subtracted to/from the time stored in the class. In this
 * form there are functions to find elapsed time or wait until time has passed.  The Timer class cannot
 * be used as a stopwatch and an interval timer at the same time.
 *
 * This function uses the high resolution realtime timers provided by Linux and your object file needs to be 
 * compiled with the -lrt option, it also uses pthreads so your object file also needs to include the -pthread
 * option.
 *
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <iostream>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

using namespace std;


class Timer {
private:
	bool ticking;
	
	int secs;
	double millisecs;

	pthread_t thread_handle;

	void simplifyTime();

	// This structure stores data that is passed to the timing thread when executing a callback
	// function that is a member of a class at regular intervals.
	//
	template<class T>
	struct ClassSubmemberThreadArguments {
		T *object;
		bool (T::*signal_handler)(double elapsed_ms);
		double timestep_ms;
		Timer *timer;
	};	
	
	// This function executes the function that was provided as input to the startTicking() function
	// with the timestep interval also specified to the startTicking() function.
	//
	template<class T> static void *runClassSubmember(void *args);
	
	// This structure stores data that is passed to the timing thread when an ordinary function is
	// provided as a callback.
	struct NormalFunctionThreadArguments {
		bool (*signal_handler)(double elapsed_ms);
		double timestep_ms;
		Timer *timer;
	};
	
	static void *runNormalFunction(void *args);
	
public:


	Timer();
	Timer(int seconds, double milliseconds = 0.0);
	~Timer(); // destructor
	
	// Danger: if the time stored in the Timer is too large, 
	// you could have problems with overflow with the following two functions:
	//
	double milliseconds() const; // returns the total time stored in units of milliseconds
	double seconds() const; // returns the total time stored in units of milliseconds (i.e. seconds() = milliseconds()/1000.0)

	/**
	 * The following functions are to be used for executing a call-back function at regular timed intervals.  
	 * You can't use these functions at the same time as the regular timing functions (see below). 
	 *
	 */
	
	// This function starts up a new thread that repeatedly calls the provided function until stopTicking()
	// is called, or if the provided function returns false.  The provided function must return a boolean
	// and has a void argument.  The timestep_ms argument specifies the time interval to call the function at.
	// The provided function is repeatedly called until stopTicking() is called or the provided function
	// returns false.
	//
	bool startTicking(bool (*signal_handler)(double), double timestep_ms);
	
	// This function starts up a new thread whose sole purpose is to repeatedly call the provided function
	// that is a member of the class 'object' every 'timestep_ms' milliseconds, until the Timer's destructor
	// is called or stopTicking() is called.  If you have an instance of a AnyClass class called 'foo' and it
	// has a member called runMe(), then the appropriate usage of this function to execute runMe() every 1000
	// milliseconds is: timerclass.startTicking(foo, &AnyClass::runMe, 1000).
	//
	template<class T>
	bool startTicking(T &object, bool (T::*signal_handler)(double), double timestep_ms);
	
	bool stopTicking(); // stops repeatedly calling the function provided to startTicking()
	bool isTicking(); // checks to see if the function provided to startTicking() is still being called at the desired time interval	

	/**
	 * These functions are to be used for your simple timing needs.  When using these functions the Timer
	 * class basically becomes a storage for time that holds seconds and milliseconds.  Time can be added
	 * to the Timer by using the increment() and decrement() functions or the overloaded operators.  See
	 * the functions below for the available functionality and usage.
	 *
	 */
	 
	bool getTime(); // stores the current time in the Timer class
	void setTime(int seconds, double milliseconds);
	Timer elapsedTime(); // returns a Timer class containing the time elapsed since the time stored in this instance of the Timer
	Timer updateTime(); // returns the elapsed time since the time stored in this instance of the Timer and then updates the stored time

	bool waitUntilPassed(); // blocks until the stored time has passed
	
	bool increment(double milliseconds); // adds time to the stored time
	bool decrement(double milliseconds); // subtracts time from the stored time
		
	Timer &operator+=(double milliseconds);
	Timer &operator-=(double milliseconds);
	Timer &operator+=(const Timer &timer); // adds another timer to the stored time
	Timer &operator-=(const Timer &timer); // subtracts a timer from the stored time
	Timer operator+(double milliseconds);
	Timer operator-(double milliseconds);
	Timer operator+(const Timer &timer);
	Timer operator-(const Timer &timer);
	Timer operator=(double milliseconds); // sets the stored time using milliseconds
	
	// These are used to check for integer underflow/overflow.
	static const int MAX_TIMESPEC_INTEGER = 2147483647;
	static const double MAX_TIMESPEC_MILLISECONDS = MAX_TIMESPEC_INTEGER/1000000.0; // max storable by an int divided by number of ns in a ms
	
	friend ostream &operator<<(ostream &stream, const Timer &time);	 		 
};

ostream &operator<<(ostream &stream, const Timer &timer); // print out the stored time to stdout


/**
 * Begins executing the function 'signal_handler' that is a member of the 'object' class repeatedly 
 * at the desired timestep.  (See the comment in the Timer class definition for more info.)
 *
 */
template<class T>
bool Timer::startTicking(T &object, bool (T::*signal_handler)(double), double timestep_ms) {

	// Create a new instance of the thread argument structure and populate it with information.
	ClassSubmemberThreadArguments<T> *args = new ClassSubmemberThreadArguments<T>;
	args->object = &object;
	args->signal_handler = signal_handler;
	args->timestep_ms = timestep_ms;
	args->timer = this;

        timespec clock_resolution;
        clock_getres( CLOCK_MONOTONIC, &clock_resolution );
        //std::cout << "Timer::startTicking(T &object, bool (T::*signal_handler)(double), double timestep_ms)" << std::endl;
        //std::cout << "\t\t clock_resolution is " << clock_resolution.tv_sec << " sec, " << clock_resolution.tv_nsec << " nsec" << std::endl;
        //std::cout << "\t\t pthread_t pointer is " << &thread_handle << std::endl;

	// Start up a new thread executed which executes the runClassSubmember() function with the provided arguments.
	if (pthread_create(&thread_handle, NULL, &Timer::runClassSubmember<T>, (void *)args) != 0) {
		cout << "TIMER: there was an error starting the thread for the timer." << endl;
		return false;
	}

	ticking = true;

	return true;
};


/**
 * This is run by the Timer thread when the user specifies a function that is a member of some class
 * to the startTicking() function.
 *
 */
template<class T>
void *Timer::runClassSubmember(void *args) {
	Timer time_keeper;
	
	ClassSubmemberThreadArguments<T> *my_args = (ClassSubmemberThreadArguments<T> *)args;

	T *object = my_args->object;
	bool (T::*signal_handler)(double) = my_args->signal_handler;
	double timestep_ms = my_args->timestep_ms;
	Timer *timer = my_args->timer;

	timer->getTime(); // store the current time
		
	// Keep looping until boolean ticking variable is set to false, then stop.
	while (timer->isTicking() && (object->*signal_handler)(time_keeper.elapsedTime().milliseconds()) ) {
		timer->increment(timestep_ms); // increment the time to the time interval

		timer->waitUntilPassed(); // wait until the time interval has passed
	}

	delete my_args;

	timer->ticking = false;

	pthread_exit(NULL);
}




#endif
