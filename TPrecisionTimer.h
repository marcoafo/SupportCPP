#ifndef TPrecisionTimerH
#define TPrecisionTimerH

//---------------------------------------------------------------------------

/*!
 * \brief Support class for timing process.
 *
 * This class uses the clocks from the processor to measure time of some
 * parts of the code. It's just a wrapper for the C++ functions, but I use
 * it to benchmark some parts of the code. It might slow down things a little bit,
 * so I wouldn't recommend for release versions.
 *
 */
class TPrecisionTimer
{
private:
	LARGE_INTEGER lFreq;   /*!< Clock frequency per second. */
	LARGE_INTEGER lStart;  /*!< How many clocks counted from the start to stop. */

public:
	TPrecisionTimer();
	virtual ~TPrecisionTimer();
	void Start();
	double Stop();
};

//---------------------------------------------------------------------------

#endif


