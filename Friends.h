#define FriendsH

#include <vector>
#include <string>

#include "TDataHora.h"

//---------------------------------------------------------------------------

/*!
 * \page Funções e templates classes de apoio geral.
 */

//---------------------------------------------------------------------------

inline bool platform_is_little_endian(void)
{
	union
	{
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };
	return bint.c[0] != 1;
}


/*!
 * \brief Return the minimum between two vales.
 * \param x  First value to be analysed.
 * \param y  Sceond value to be analysed.
 * \return   Lower value between x and y,
 */
template <class Type> Type Min(Type x, Type y)
{
	return (x > y) ? y : x;
}

/*!
 * \brief Return the maximum between two vales.
 * \param x  First value to be analysed.
 * \param y  Seoond value to be analysed.
 * \return   Higher value between x and y,
 */
template <class Type> Type Max(Type x, Type y)
{
	return (x > y) ? x : y;
}

//---------------------------------------------------------------------------

// Funções de estatística e probabilidade.
double Media(const std::vector<double> &Valores);
double Moda(const std::vector<double> &Valores);
double DesvioPadrao(const std::vector<double> &Valores);
double AbcissaFreqAcumulada(double* Vetor, const unsigned int NPonto, const double P);
double AbcissaFreqAcumulada(const std::vector<double> &Vetor, const double P);
void Histograma(const std::vector<double> &Amostras, std::vector<double> &Frequencias, bool FreqRelativa = true);
double Percentil(const std::vector<double> &Vetor, const double P);
bool AjustePolinomioMMQ(const std::vector<double> &Xi, const std::vector<double> &Yi, double *Coeficientes, int OrdemMax);
void Binomio(int ValorMax, int NumCasas, std::vector<std::vector<int> > &Combinacoes);
double TendenciaLinear(const std::vector<double> &Valores);
double MediaMovel(const std::vector<double> &Valores, int Largura);

// Funções de apoio de programação relativos ao uso de strings.
std::string RemoverEspacos(const char* Expressao);
std::string Trim(const char* Expressao);
std::string UCase(const char* Expressao);
std::string LCase(const char* Expressao);
std::string Replace(const char* Expressao, const char* Busca, const char* Substitui);
std::vector<std::string> &Split(const std::string &Texto, char Delimitador, std::vector<std::string> &Elementos);
std::string Narrow(const std::wstring& Entrada);
std::string NumeroCEPEL(double Valor, int Tamanho);

// Funções de apoio diversas.
std::string NomeMes(int Mes, bool Abreviado);
std::string Base64_Encode(char const* BytesToEncode, unsigned int Len);
std::string Base64_Decode(std::string const& EncodedString);
double ArredondaValor(const double &Valor, const unsigned int &Digitos = 2);

//-----------------------------------------------------------------------------

#endif


