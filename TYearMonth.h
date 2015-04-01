#ifndef TYearMonthH
#define TYearMonthH

//---------------------------------------------------------------------------

/*!
 * \brief Struct to help dealing with year/month sorting.
 */
struct TYearMonth
{
	int Month;  /*!< Month (1 to 12) of the period. */
	int Year;   /*!< Year of the period. */

	// constructors and destructor
	TYearMonth();
	TYearMonth(const TYearMonth &Copia);
    TYearMonth(const int YearRefer, const int MonthRefer);
	virtual ~TYearMonth();

	// operators
	bool operator ! () const;
	bool operator == (const TYearMonth &Right) const;
	bool operator < (const TYearMonth &Right) const;
	bool operator <= (const TYearMonth &Right) const;
	bool operator > (const TYearMonth &Right) const;
	bool operator >= (const TYearMonth &Right) const;
	const TYearMonth& operator = (const TYearMonth &Right);
	TYearMonth& operator ++ ();
	TYearMonth operator ++ (int);
	TYearMonth& operator -- ();
	TYearMonth operator -- (int);
};

//---------------------------------------------------------------------------

#endif


