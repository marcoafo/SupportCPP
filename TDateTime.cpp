#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "TDateTime.h"

//---------------------------------------------------------------------------

/*!
 * \brief Extension for the C++ mktime() function, so it can use UTC time instead.
 *
 * Since there are functions in ctime that doesn't deal with DST very well, I prefer
 * to move to UTC. For that, the input time need to be UTC, directly calculated from
 * the system. The trick is to set environment to UTC, get the time, and switch back
 * to local time. Also, there's a problem on how the tzset() is called, so I used a
 * macro to call correctly function. In Windows, it's _tzset(), but in Linux is tzset().
 * This function is local, since I don't need it outside the class.
 *
 * \param timeinfo Time structure from ctime from which we want ITC time.
 * \return The timestamp UNIX to the defined time, but defined to UTC.
 */
time_t mktimeUTC(struct tm* timeinfo)
{
	// get local time, and chance environment to UTC
	char* oldTZ = getenv("TZ");
	putenv("TZ=UTC");
#ifdef WIN32
	_tzset();
#else
	tzset();
#endif
	// now get the datetime
	time_t ret = mktime(timeinfo);
	// return local time
	if(oldTZ == NULL)  // check if there's actually a GMT
	{
		putenv("TZ=");
	}
	else
	{
		char buff[255];
		sprintf(buff,"TZ=%s",oldTZ);
		putenv(buff);
	}
#ifdef WIN
	_tzset();
#else
	tzset();
#endif
	// retornar time_t no UTC
	return ret;
}

//---------------------------------------------------------------------------

/*!
 * \brief Split a string in chunks, based in a delimiter for the division.
 * \param Text       String text that will the split into chunks.
 * \param Delimiter  Character that is used to split the text into chunks.
 * \return Vector of strings that will contain the resulting chunks (it'll be cleared first).
 */
std::vector<std::string> TDateTime::Split(const std::string &Text, char Delimiter) const
{
    std::vector<std::string> chunks;
    std::stringstream ss(Text);
    std::string item;
    while(std::getline(ss, item, Delimiter))  chunks.push_back(item);
    return chunks;
}

/*!
 * \brief Replace occurrences of a substring (needle) in a text string (haystack).
 * \param  Haystack  Text string that will be searched for the substring.
 * \param  Needle    Substring that will be searched.
 * \param  Replace   Expression that will be placed in the position of the needle.
 * \return  A copy of the text string with the needles replaced by the replacement text.
 */
std::string TDateTime::Replace(const char* Haystack, const char* Needle, const char* Replace) const
{
	std::string texto = Haystack;
	std::string search = Needle;
	std::string replace = Replace;
	if(search != replace)
	{
		std::string::size_type pos = 0;
		while( (pos = texto.find(search, pos)) != std::string::npos )
		{
			texto.replace(pos, search.size(), replace);
			pos++;
		}
	}
	return texto;
}

//---------------------------------------------------------------------------

/*!
 * \brief Default constructor, without parameters.
 */
TDateTime::TDateTime()
{
	Time = 0;
}

/*!
 * \brief Constructor with member attribution.
 * \param  Year     Year of the attributed date/time.
 * \param  Month    Month of the attributed date/time.
 * \param  Day      Day of the attributed date/time.
 * \param  Hours    Hours of the attributed date/time.
 * \param  Minutes  Minutes of the attributed date/time.
 * \param  Seconds  Seconds of the attributed date/time.
 */
TDateTime::TDateTime(int Year, int Month, int Day, int Hours, int Minutess, int Secondss)
{
    this->Set(Year,Month,Day,Hours,Minutess,Secondss);
}

/*!
 * \brief Copy constructor.
 * \param Copy  Origin object from which the properties will be copied.
 */
TDateTime::TDateTime(const TDateTime &Copy)
{
	this->Time = Copy.Time;
}

/*!
 * \brief Destructor (no pointers here, so nothing to declare).
 */
TDateTime::~TDateTime()
{
}

//---------------------------------------------------------------------------

/*!
 * \brief Null operator (defines what is "not" of this class).
 * \return  Return true if the element can be considered a null.
 */
bool TDateTime::operator ! () const
{
	return (this->Time == 0);  // it's not a *truly* null state, but we'll consider that being in UNIX era is equals to null
}

/*!
 * \brief Equal operator (defines when two objects can be considered equal of this class).
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if the two objects can be considered equal, false otherwise.
 */
bool TDateTime::operator == (const TDateTime &Direita) const
{
	return (this->Time == Direita.Time);
}

/*!
 * \brief Less operator (defines when an object can be considered less than other of this class).
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered less than the other one, false otherwise.
 */
bool TDateTime::operator < (const TDateTime &Direita) const
{
	return (this->Time < Direita.Time);
}

/*!
 * \brief Less or equal operator (defines when an object can be considered less or equal than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered less or equal than the other one, false otherwise.
 */
bool TDateTime::operator <= (const TDateTime &Direita) const
{
	return ((*this) < Direita) || ((*this) == Direita);
}

/*!
 * \brief Greater operator (defines when an object can be considered greater than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered greater than the other one, false otherwise.
 */
bool TDateTime::operator > (const TDateTime &Direita) const
{
	return !((*this) <= Direita);
}

/*!
 * \brief Greater or equal operator (defines when an object can be considered greater or equal than other of this class).
 *
 * Note that this operator can be defined from the equal and less operators indirectly.
 *
 * \param  Right  The other object which is being compared to this object (the one we are dealing with).
 * \return  True if this object can be considered greater or equal than the other one, false otherwise.
 */
bool TDateTime::operator >= (const TDateTime &Direita) const
{
	return ((*this) > Direita) || ((*this) == Direita);
}

/*!
 * \brief Attribution operator (no overlap function).
 * \param  Copy  Origin object from which the properties will be copied.
 * \return  Self-reference for this object to allow cascading or operators.
 */
const TDateTime& TDateTime::operator = (const TDateTime &Copy)
{
	Time = Copy.Time;
	return *this;
}

/*!
 * \brief Bitwise right shift operator, where we will use only two valid formats, DD/MM/YYYY or YYYY-MM-DD.
 * \param  InStream  Reference of the streamer from which the date will be read.
 * \param  DateTime  Reference to the object of this class which will receive the read date.
 * \return  Self-reference for this object to allow cascading or operators.
 */
std::istream& operator >> (std::istream& InStream, TDateTime& DateTime)
{
    int dia, mes, Year;  // formato em DD/MM/AAAA ou AAAA-MM-DD
    char ch;  // listar a "/"
    InStream >> dia;
    if(!InStream) return InStream;
    InStream >> ch;
    if(!InStream || (ch != '/' && ch != '-'))
    {
    	InStream.clear(InStream.rdstate() | std::ios::failbit);
        return InStream;
    }
    InStream >> mes;
    if(!InStream) return InStream;
    InStream >> ch;
    if(!InStream || (ch != '/' && ch != '-'))
    {
    	InStream.clear(InStream.rdstate() | std::ios::failbit);
        return InStream;
    }
    InStream >> Year;
    if(!InStream) return InStream;
    if(ch == '-') DateTime.Set(dia,mes,Year,0,0,0);  // AAAA-MM-DD
    else if(ch == '/') DateTime.Set(Year,mes,dia,0,0,0);  // DD/MM/AAAA
    return InStream;
}

/*!
 * \brief Bitwise left shift operator, where we will output only the date part in the format DD/MM/YYYY.
 * \param  InStream  Reference of the streamer to which the date will be written.
 * \param  DateTime  Reference to the object of this class from which the date will be read.
 * \return  Self-reference for this object to allow cascading or operators.
 */
std::ostream& operator << (std::ostream& OutStream, const TDateTime& DateTime)
{
	OutStream << DateTime.Get("dd/mm/YY");
    return OutStream;
}

//---------------------------------------------------------------------------

/*!
 * \brief Try to add a period to the currently date/time.
 * \param Value  Integer number of the intervals to add (can be negative if you want to subtract the intervals).
 * \param Interval  Type of interval to be added/subtracted; must be: year, month, day, hours, minutes or seconds.
 * \return True if it was able to add the interval, false if there wasn't possible for the insertion (no message is output).
 */
bool TDateTime::Add(const int &Value, EDateTime Interval)
{
	try
	{
		struct std::tm * timeinfo = std::gmtime(&Time);
		timeinfo->tm_isdst = -1;
		if(Interval == dtYear) timeinfo->tm_year += Value;
		else if(Interval == dtMonth) timeinfo->tm_mon += Value;
		else if(Interval == dtDay) timeinfo->tm_mday += Value;
		else if(Interval == dtHours) timeinfo->tm_hour += Value;
		else if(Interval == dtMinutes) timeinfo->tm_min += Value;
		else if(Interval == dtSeconds) timeinfo->tm_sec += Value;
		if(mktimeUTC(timeinfo) == -1)  return false;
		Time = mktimeUTC(timeinfo);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

/*!
 * \brief Calculate the interval between two objects of this class.
 * \param Right  Date/time object from which the interval will be calculated against the reference object.
 * \param Interval  Type of interval for the output; must be: seconds, minutes, hours or days (makes no sense to use Gregorian calendar for intervals).
 * \return The interval between the date/time objects (can be negative as well), or the constant DBL_MAX if the interval type is invalid.
 */
double TDateTime::Diff(const TDateTime &Right, EDateTime Interval)
{
	double diferenca = std::difftime(this->Time,Right.Time);
	switch(Interval)
	{
		case dtDay:  return std::ceil(diferenca / (60.0 * 60.0 * 24.0));
		case dtHours: return std::ceil(diferenca / (60.0 * 60.0));
		case dtMinutes: return std::ceil(diferenca / (60.0));
		case dtSeconds: return diferenca;
		default: return DBL_MIN;
	}
}

/*!
 * \brief Set the date/time of the object using discrete parameters.
 * \param  Year     Year of the attributed date/time.
 * \param  Month    Month of the attributed date/time.
 * \param  Day      Day of the attributed date/time.
 * \param  Hours    Hours of the attributed date/time.
 * \param  Minutes  Minutes of the attributed date/time.
 * \param  Seconds  Seconds of the attributed date/time.
 * \return True if the parameters form a valid date/time and it was attributed, false otherwise (no message will the thrown).
 */
bool TDateTime::Set(const int &Year, const int &Month, const int &Day, const int &Hours, const int &Minutes, const int &Seconds)
{
	if(Hours > 23 || Hours < 0 || Minutes > 59 || Minutes < 0 || Seconds > 59 || Seconds < 0)  return false;
	if(Month > 12 || Month < 1 || Day > 31 || Day < 1)  return false;
	try
	{
		struct std::tm timeinfo;
        timeinfo.tm_isdst = 0;
		timeinfo.tm_year = Year - 1900;
		timeinfo.tm_mon = Month - 1;
		timeinfo.tm_mday = Day;
		timeinfo.tm_hour = Hours;
		timeinfo.tm_min = Minutes;
		timeinfo.tm_sec = Seconds;
		if(mktimeUTC(&timeinfo) == -1)  return false;
		Time = mktimeUTC(&timeinfo);
		return true;
	}
	catch(...)
	{
		return false;
	}
}

/*!
 * \brief Set the date/time of the object using a input string with an expected formatted date (YYYY-MM-DD, YYYYMMDD or DD/MM/YYYY) and a time separated from date by space in in format of HH:MM:SS;
 * \param  DataANSI  String of expected format date, with an optional time (the date is mandatory, however).
 * \return Verdadeiro se conseguiu atribuir o horário com sucesso, falso caso contrário (se a data for inválida ou incompleta, algo assim).
 */
bool TDateTime::Set(const char* DataANSI)
{
    int Year=0, mes=0, dia=0, hrs=0, min=0, seg=0;
    std::vector<std::string> tmp = Split(DataANSI,' ');
    std::string data,hora;
    data = tmp[0];
    if(tmp.size() > 1) hora = tmp[1];
	// ver se existe pelo menos uma informação (a priori, é data)
    if(!data.empty())
    {
        tmp = Split(data,'/');
        if(tmp.size() > 1)  // ver se é formato DD/MM/YYYY
        {
            dia = std::atoi(tmp[0].c_str());
            mes = std::atoi(tmp[1].c_str());
            Year = std::atoi(tmp[2].c_str());
        }
        else
        {
            tmp = Split(data,'-');
            if(tmp.size() > 1)  // ver se é formato YYYY-MM-DD
            {
                dia = std::atoi(tmp[2].c_str());
                mes = std::atoi(tmp[1].c_str());
                Year = std::atoi(tmp[0].c_str());
            }
            else if(data.size() == 8)  // horário ou formato YYYYMMDD
            {
                if(Replace(data.c_str(),":","").size() == 8)  // só informação de data no estilo YYYYMMDD
                {
                    Year = std::atoi(data.substr(0,4).c_str());
                    mes = std::atoi(data.substr(4,2).c_str());
                    dia = std::atoi(data.substr(6,2).c_str());
                }
                else  // só informação de horário, repassar para a variável
                {
                    hora = data;
                }
            }
        }
    }
	// ver se existe informação de horário
    if(!hora.empty())
    {
        tmp = Split(hora,':');
        hrs = std::atoi(tmp[0].c_str());
        min = std::atoi(tmp[1].c_str());
        seg = std::atoi(tmp[2].c_str());
    }
	// setar a data e retornar se conseguiu ou não pela função da classe
    return Set(Year,mes,dia,hrs,min,seg);
}

/*!
 * \brief Set the current date/time to this object.
 */
void TDateTime::SetNow()
{
	std::time(&Time);
}

/*!
 * \brief Get the date/time period with a custom formatting.
 *
 * Mask used by this class are:
 * yy - two digits year
 * YY - four digits year
 * mm - two digits month
 * dd - two digits day
 * hh - two digits hours
 * ii - two digits minutes
 * ss - two digits seconds
 *
 * \param  Format  Masked string with the desired format of the period.
 * \return String with the mask formats replaced by the parts of the period, if avaliable.
 */
std::string TDateTime::Get(const char* Format) const
{
	char Year4d[5];  std::sprintf(Year4d,"%04d",Get(dtYear));
	char Year2d[3];  std::sprintf(Year2d,"%02d",(Get(dtYear) % 100));
	char mes2d[3];  std::sprintf(mes2d,"%02d",Get(dtMonth));
	char dia2d[3];  std::sprintf(dia2d,"%02d",Get(dtDay));
	char hora2d[3];  std::sprintf(hora2d,"%02d",Get(dtHours));
	char min2d[3];  std::sprintf(min2d,"%02d",Get(dtMinutes));
	char seg2d[3];  std::sprintf(seg2d,"%02d",Get(dtSeconds));
	std::string saida = Format;
	saida = Replace(saida.c_str(),"yy",Year2d);
	saida = Replace(saida.c_str(),"YY",Year4d);
	saida = Replace(saida.c_str(),"mm",mes2d);
	saida = Replace(saida.c_str(),"dd",dia2d);
	saida = Replace(saida.c_str(),"hh",hora2d);
	saida = Replace(saida.c_str(),"ii",min2d);
	saida = Replace(saida.c_str(),"ss",seg2d);
	return saida;
}

/*!
 * \brief Get a part of the date/time from the object as an integer.
 * \param  Part  Which part is desired from the period.
 * \return The integer corresponding to the part of the period asked for, or -1 if the requisition was invalid.
 */
int TDateTime::Get(const EDateTime &Part) const
{
	struct std::tm * timeinfo = std::gmtime(&Time);
    timeinfo->tm_isdst = 0;
	switch(Part)
	{
		case dtYear: return (timeinfo->tm_year + 1900);
		case dtMonth: return (timeinfo->tm_mon + 1);
		case dtDay: return (timeinfo->tm_mday);
		case dtHours: return (timeinfo->tm_hour);
		case dtMinutes: return (timeinfo->tm_min);
		case dtSeconds: return (timeinfo->tm_sec);
        case dtDayOfWeek: return (timeinfo->tm_wday + dtSunday);
		case dtDST: return (timeinfo->tm_isdst);
		default: return -1;
	}
}

//---------------------------------------------------------------------------

/*!
 * \brief Define the period of the object based in the serial date defined by the Julian calendar (mostly used by derivates from Lotus 1-2-3, such as Excel).
 *
 * Please note that exists a bug in the Julian calendar which wasn't fixed until now. The
 * Julian calendar takes 1900 as a leap year, but since it's century, it's not.
 *
 * \param  SerialDate  Serial date in Julian calendar that will set the period of the object.
 */
void TDateTime::SetJulian(int SerialDate)
{
	int day, month, year;
    // Excel/Lotus 123 have a bug with 29-02-1900. 1900 is not a
    // leap year, but Excel/Lotus 123 think it is...
    if(SerialDate == 60)
    {
		day = 29;
        month = 2;
        year = 1900;
    }
    else if(SerialDate < 60)
    {
        // Because of the 29-02-1900 bug, any serial date
        // under 60 is one off... Compensate.
        SerialDate++;
    }
    // Modified Julian to DMY calculation with an addition of 2415019
    int l = SerialDate + 68569 + 2415019;
    int n = int(( 4 * l ) / 146097);
    l = l - int(( 146097 * n + 3 ) / 4);
    int i = int(( 4000 * ( l + 1 ) ) / 1461001);
    l = l - int(( 1461 * i ) / 4) + 31;
    int j = int(( 80 * l ) / 2447);
    day = l - int(( 2447 * j ) / 80);
    l = int(j / 11);
    month = j + 2 - ( 12 * l );
    year = 100 * ( n - 49 ) + i + l;
	Set(year,month,day,0,0,0);
}

/*!
 * \brief Get the date/time defined by the object in the Julian calendar.
 * \return  Return the period of the object in the Julian calendar.
 * \sa SetJulian
 */
int TDateTime::GetJulian()
{
	int day, month, year;
	day = Get(dtDay);
	month = Get(dtMonth);
	year = Get(dtYear);
    // Excel/Lotus 123 have a bug with 29-02-1900. 1900 is not a
    // leap year, but Excel/Lotus 123 think it is...
    if (day == 29 && month == 02 && year==1900)  return 60;
    // DMY to Modified Julian calculatie with an extra substraction of 2415019.
    long serialdate = int(( 1461 * ( year + 4800 + int(( month - 14 ) / 12) ) ) / 4) +
                       int(( 367 * ( month - 2 - 12 * ( ( month - 14 ) / 12 ) ) ) / 12) -
                       int(( 3 * ( int(( year + 4900 + int(( month - 14 ) / 12) ) / 100) ) ) / 4) +
                       day - 2415019 - 32075;
    if (serialdate <= 60)
    {
        // Because of the 29-02-1900 bug, any serial date
        // under 60 is one off... Compensate.
        serialdate--;
    }
    return (int)serialdate;
}

//---------------------------------------------------------------------------

/*!
 * \brief Function to define the period of the class using a timestamp from UNIX era.
 * \param TimeStamp Number of seconds elapsed since UNIX era that defines the period to apply to the object.
 */
void TDateTime::SetTimestamp(unsigned long int Timestamp)
{
    Time = Timestamp;
}

/*!
 * \brief Obtain the current timestamp from UNIX era of the period of the object.
 * \return Number of seconds elapsed since UNIX era that defines the period of the object.
 */
unsigned long int TDateTime::GetTimestamp()
{
    return Time;
}

//---------------------------------------------------------------------------

/*!
 * \brief Get the last day of the month, which can be defined also as the numbers of days in the month.
 * \return The last day of month, or zero if the period is invalid.
 */
int GetLastDay(int Year, int Month)
{
	TDateTime aux;
	aux.Set(Year,Month,1,0,0,0);
	aux.Add(1,TDateTime::dtMonth);
	aux.Add(-1,TDateTime::dtDay);
	return aux.Get(TDateTime::dtDay);
}

std::string GetNow(const char* Format)
{
	TDateTime now;
	now.SetNow();
	return now.Get(Format);
}