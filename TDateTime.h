#ifndef TDateTimeH
#define TDateTimeH

#include <ctime>
#include <iostream>
#include <string>
#include <vector>

//---------------------------------------------------------------------------

/*!
 * \brief Date/time class operator, OS independent and cast various types.
 *
 * Uses only STL libraries, so it can be compiled with any decent compiler. You just
 * need to note on how tzset is implemented in your system (there's a macro in the
 * implementation of this class to help). Also it casts from and to many types. Masks
 * for formatting is quite limited, but it can hold even the bugged Julian calendar from Excel.
 */
class TDateTime
{
private:
	std::time_t Time;  /*!< Relative time of the class, from ctime library, which marks the time passed since Unix era (01/01/1970 00:00:00). */

    // support functions from the Friends repository
    std::string Replace(const char* Haystack, const char* Needle, const char* Replace) const;
    std::vector<std::string> Split(const std::string &Text, char Delimiter) const;

public:
	enum EDateTime  /*!< Parts of a date/time. */
	{
		dtInvalid = 0, /*!< Invalid item.. */
		dtYear,        /*!< Year. */
		dtMonth,       /*!< Month. */
		dtDay,         /*!< Day. */
		dtHours,       /*!< Hours. */
		dtMinutes,     /*!< Minutes. */
		dtSeconds,     /*!< Seconds. */
		dtDST,         /*!< Daylight saving time (usually a boolean to inform if there isn't one applied). */
		dtDayOfWeek,   /*!< Day of the week. Uses below notation because there isn't a consent in C++ implementations. */
		dtSunday,      /*!< Sunday. */
		dtMonday,      /*!< Monday. */
		dtTuesday,     /*!< Tuesday. */
		dtWednesday,   /*!< Wednesday. */
		dtThursday,    /*!< Thursday. */
		dtFriday,      /*!< Friday. */
		dtSaturday     /*!< Saturday. */
	};

	// constructors and destructor
	TDateTime();
	TDateTime(int Year, int Month, int Day, int Hours, int Minutes, int Seconds);
	TDateTime(const TDateTime &Copy);
	virtual ~TDateTime();

    // operators (friends or members)
	bool operator ! () const;
	bool operator == (const TDateTime &Right) const;
	bool operator < (const TDateTime &Right) const;
	bool operator <= (const TDateTime &Right) const;
	bool operator > (const TDateTime &Right) const;
	bool operator >= (const TDateTime &Right) const;
	const TDateTime& operator = (const TDateTime &Copy);
	friend std::istream& operator >> (std::istream& InStream, TDateTime& DateTime);
	friend std::ostream& operator << (std::ostream& OutStream, const TDateTime& DateTime);

    // date/time operations
	bool Add(const int &Value, EDateTime Interval);
	double Diff(const TDateTime &Right, EDateTime Interval);

    // attribution functions
	bool Set(const int &Year, const int &Month, const int &Day, const int &Hours, const int &Minutes, const int &Seconds);
	bool Set(const char* DateANSI);
	void SetNow();

    // output functions
	std::string Get(const char* Format) const;
	int Get(const EDateTime &Format) const;

	// functions to work with the bugged Julian calendar from Excel
	void SetJulian(int SerialDate);
	int GetJulian();

	// functions to work with timestamp
	void SetTimestamp(unsigned long int Timestamp);
	unsigned long int GetTimestamp();

	// friends
	friend int GetLastDay(int Year, int Month);
	friend std::string GetNow(const char* Format);
};

//---------------------------------------------------------------------------

#endif


