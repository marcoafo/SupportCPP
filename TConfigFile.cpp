#include <fstream>

#include "TConfigFile.h"

//---------------------------------------------------------------------------

/*!
 * \brief Remove escape characters (\r, \n e \t) from both sides of a string.
 * \param  Expression String to be trimmed (from both sides).
 * \return Return the string without escape characters in both sides.
 * \sa Friends repository.
 */
std::string TConfigFile::Trim(const char* Expression)
{
	std::string text = Expression;
	char const* delims = " \t\r\n";  // escape chars
	std::string::size_type corte = text.find_first_not_of(delims);
	text.erase(0,corte);
	corte = text.find_last_not_of(delims);
	text.erase(corte+1);
	return text;
}

//---------------------------------------------------------------------------

/*!
 * \brief Class constructor.
 */
TConfigFile::TConfigFile()
{
}

/*!
 * \brief Class copy constructor.
 * \param Copy Reference of the origin object, from which the members properties will be copied to newly created object.
 */
TConfigFile::TConfigFile(const TConfigFile &Copy)
{
	Parameters = Copy.Parameters;
}

/*!
 * \brief Class destructor.
 */
TConfigFile::~TConfigFile()
{
}

//---------------------------------------------------------------------------

/*!
 * \brief Copy (assign) operator.
 * \param Copy  Reference to the object from which the members properties will be copied.
 * \return Self-reference to the object, to allow child-inheritance.
 */
const TConfigFile& TConfigFile::operator = (const TConfigFile &Copy)
{
	Parameters = Copy.Parameters;
	return *this;
}

//---------------------------------------------------------------------------

/*!
 * \brief Read a configuration file and parse the variables to this class, cleaning its container first.
 * \param ConfigFile File name (may include full pathname) to the configuration file to be parsed.
 * \return True if parsed the file successfully, false if it couldn't open the file (no error message will be thrown).
 */
bool TConfigFile::ReadFile(const char* ConfigFile)
{
    std::fstream file;
    file.open(ConfigFile,std::fstream::in);
    if(!file.is_open()) return false;
    Parameters.clear();
    while(!file.eof())
    {
        std::string line;
        getline(file,line);
        line = Trim(line.c_str());
        if(line.empty()) continue;  // line em branco
        if(line.substr(0,1) == "#") continue;  // comentário
        std::string::size_type pos = 0;
		if((pos = line.find("=",pos)) == std::string::npos) continue;  // line inválida
		std::string param,val;
		param = Trim(line.substr(0,pos).c_str());
		val = Trim(line.substr(pos+1).c_str());
		Parameters.insert( std::pair<std::string,std::string>(param,val) );
    }
    file.close();
    return true;
}

/*!
 * \brief Write the parameters and their values in a standard way to a output file.
 * \param ConfigFile File name (may include full pathname) to the configuration file to be written.
 * \return True if wrote the configuration file, false if it couldn't open the file for output (no error message will be thrown).
 */
bool TConfigFile::WriteFile(const char* ConfigFile)
{
    std::fstream file;
    file.open(ConfigFile,std::fstream::out);
    if(!file.is_open()) return false;
    for(std::multimap<std::string,std::string>::iterator it = Parameters.begin(); it != Parameters.end(); it++)
    {
        file << it->first << " = " << it->second << std::endl;
    }
    file.close();
    return true;
}
