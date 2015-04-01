#include "TPrecisionTimer.h"

//---------------------------------------------------------------------------

/*!
 * \brief Default constructor of the class.
 *
 * Here we'll initializate the frequency counter with the maximum that the
 * hardware can support.
 *
 */
TPrecisionTimer::TPrecisionTimer()
{
	QueryPerformanceFrequency(&lFreq);
}

/*!
 * \brief Destructor of the class, empty since no pointers where used.
 */
TPrecisionTimer::~TPrecisionTimer()
{
}

//---------------------------------------------------------------------------

/*!
 * \brief Start the timer.
 *
 * Marks the currently clock counter, because we just want an interval rather
 * than the total amount since the beginning of the application.
 *
 */
void TPrecisionTimer::Start()
{
	QueryPerformanceCounter(&lStart);
}

/*!
 * \brief Stio the timer and return the measured time.
 *
 * Stops the counter of clocks, and returns the mark of this new position. Of
 * course it won't stop the timing of clocks, it justs get a new mark so we can
 * calculate the time elapsed.
 *
 * \return  A high precision decimal containing the interval measured, in seconds (the precision takes cares of milliseconds).
 *
 */
double TPrecisionTimer::Stop()
{
	LARGE_INTEGER lEnd;
	QueryPerformanceCounter(&lEnd);
	return (double(lEnd.QuadPart - lStart.QuadPart) / lFreq.QuadPart);
}

