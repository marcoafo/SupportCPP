#include "TYearMonth.h"

//---------------------------------------------------------------------------

/*!
 * \brief Default constructor, without parameters.
 */
TYearMonth::TYearMonth()
{
	Month = 1;
	Year = 1900;
}

/*!
 * \brief Copy constructor.
 * \param Copy  Origin object from which the properties will be copied.
 */
TYearMonth::TYearMonth(const TYearMonth &Copy)
{
	Month = Copy.Month;
	Year = Copy.Year;
}

/*!
 * \brief Constructor with parameters to initializate members.
 * \param YearRefer   Year of reference for this object.
 * \param MonthRefer  Month of reference for this object.
 */
TYearMonth::TYearMonth(const int YearRefer, const int MonthRefer)
{
	Month = MonthRefer;
    Year = YearRefer;
}

/*!
 * \brief Destructor (no pointers here, so nothing to declare).
 */
TYearMonth::~TYearMonth()
{
}

//---------------------------------------------------------------------------

/*!
 * \brief Attribution operator (no overlap function).
 * \param  Copy  Origin object from which the properties will be copied.
 * \return  Self-reference for this object to allow cascading or operators.
 */
const TYearMonth& TYearMonth::operator = (const TYearMonth &Copy)
{
	Month = Copy.Month;
	Year = Copy.Year;
	return *this;
}

/*!
 * \brief Null operator (defines what is "not" of this class).
 * \return  Return true if the element can be considered a null.
 */
bool TYearMonth::operator ! () const
{
	return (Year == 0 && Month == 0);
}

/*!
 * \brief Equal operator (defines when two objects can be considered equal of this class).
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if the two objects can be considered equal, false otherwise. 
 */
bool TYearMonth::operator == (const TYearMonth &Right) const
{
	return (Year == Right.Year && Month == Right.Month);
}

/*!
 * \brief Less operator (defines when an object can be considered less than other of this class).
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered less than the other one, false otherwise.
 */
bool TYearMonth::operator < (const TYearMonth &Right) const
{
	if(Year == Right.Year)
		return Month < Right.Month;
	else
		return Year < Right.Year;
}

/*!
 * \brief Less or equal operator (defines when an object can be considered less or equal than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered less or equal than the other one, false otherwise.
 */
bool TYearMonth::operator <= (const TYearMonth &Right) const
{
	return ((*this) < Right) || ((*this) == Right);
}

/*!
 * \brief Greater operator (defines when an object can be considered greater than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered greater than the other one, false otherwise.
 */
bool TYearMonth::operator > (const TYearMonth &Right) const
{
	return !((*this) <= Right);
}

/*!
 * \brief Greater or equal operator (defines when an object can be considered greater or equal than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered greater or equal than the other one, false otherwise.
 */
bool TYearMonth::operator >= (const TYearMonth &Right) const
{
	return ((*this) > Right) || ((*this) == Right);
}

/*!
 * \brief Prefixed incremental operator (the element is predefined before the return).
 * \return  Self-reference for this object to allow cascading or operators.
 */
TYearMonth& TYearMonth::operator ++ ()
{
	// alter the actual state
	if(Month == 12)
	{
		Month = 1;
		Year++;
	}
	else
	{
		Month++;
    }
	// return the actual state
	return *this;
}

/*!
 * \brief Postfixed incremental operator (the element is post-defined before the return).
 * \return  Later state (before the increment).
 */
TYearMonth TYearMonth::operator ++ (int)
{
	TYearMonth before = *this;  // later state
	// alter actual state, not the later!
	if(Month == 12)
	{
		Month = 1;
		Year++;
	}
	else
	{
		Month++;
    }
	// return then the later state
	return before;
}

/*!
 * \brief Prefixed decremental operator (the element is predefined before the return).
 * \return  Self-reference for this object to allow cascading or operators.
 */
TYearMonth& TYearMonth::operator -- ()
{
	// alter the actual state
	if(Month == 1)
	{
		Month = 12;
		Year--;
	}
	else
	{
		Month--;
	}
	// return the actual state
	return *this;
}

/*!
 * \brief Postfixed decremental operator (the element is post-defined before the return).
 * \return  Later state (before the increment).
 */
TYearMonth TYearMonth::operator -- (int)
{
	TYearMonth before = *this;  // later state
	// alter actual state, not the later!
	if(Month == 1)
	{
		Month = 12;
		Year--;
	}
	else
	{
		Month--;
    }
	// return then the later state
	return before;
}
