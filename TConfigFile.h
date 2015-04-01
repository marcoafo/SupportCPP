#ifndef TConfigFileH
#define TConfigFileH

#include <map>
#include <string>

//---------------------------------------------------------------------------

/*!
 * \brief Class to read configuration files in a Linux style.
 */
class TConfigFile
{
private:
	// support functions
	std::string Trim(const char* Expression);

public:
	std::multimap<std::string,std::string> Parameters;  /*!< Parameters and values. */

	// constructos and destructor
	TConfigFile();
	TConfigFile(const TConfigFile &Copy);
	~TConfigFile();

	// operators
	const TConfigFile& operator = (const TConfigFile &Copy);

	// file operations
	bool ReadFile(const char* ConfigFile);
	bool WriteFile(const char* ConfigFile);
};

//---------------------------------------------------------------------------

#endif


