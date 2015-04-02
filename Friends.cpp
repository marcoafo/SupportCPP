#include <cstdlib>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include <locale>

//-----------------------------------------------------------------------------

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

//-----------------------------------------------------------------------------

bool platform_is_little_endian(void)
{
	union
	{
		uint32_t i;
		char c[4];
	} bint = { 0x01020304 };
	return bint.c[0] != 1;
}

// comentar
double DesvioPadrao(const std::vector<double> &Valores)
{
    double media = Media(Valores);
    double n = 1E-9;
    double s = 0;
    for(std::vector<double>::const_iterator i = Valores.begin(); i != Valores.end(); i++)
    {
        s += std::pow(*i - media,2.0);
        n += 1.0;
    }
    if(n < 1.0) return 0;
    return std::sqrt(s / (n - 1.0));
}

/*!
 * \brief Ajustar um polin�mio por MMQ para um dado conjunto de pontos.
 * \param  Xi            Vetor constante dos valores da vari�vel independente da aproxima��o.
 * \param  Yi            Vetor constante dos valores da vari�vel dependente (deve ser pareado com a vari�vel independente).
 * \param  Coeficientes  Refer�ncia para o vetor na mem�ria (ser� alocado dinamicamente) que conter� os coeficientes do polin�mio calculado.
 * \param  OrdemMax      Ordem m�xima do polin�mio de aproxima��o.
 * \return  Verdadeiro se conseguir calcular os polin�mios, falso caso n�o consiga (ap�s avaliar dados de entrada, tais como vetores vazios).
 */
bool AjustePolinomioMMQ(const std::vector<double> &Xi, const std::vector<double> &Yi, double *Coeficientes, int OrdemMax)
{
	if(Xi.size() != Yi.size())  return false;
	int n = Xi.size();
	// determinar ordem m do polin�mio (lembrar que n >= m+1)
	int m = Min<int> (OrdemMax,n-1);
	// alocar matriz para localiza��o de coeficientes
	double ** matriz = (double**)std::malloc((m+2) * sizeof(double*));
	for(int i = 0; i <= (m+1); i++)
	{
		matriz[i] = (double*)std::malloc((m+3) * sizeof(double));
		for(int j = 0; j <= (m+2); j++)  matriz[i][j] = 0.0;
	}
	// montar matriz de Gauss do sistema linear do MMQ polinomial
	for(int k = 0; k <= m; k++)
	{
		double b = 0;
		double *A = (double*)std::malloc((m+1) * sizeof(double));
		for(int i = 0; i <= m; i++) A[i] = 0.0;
		for(int i = 0; i < n; i++)
		{
			b += (k == 0) ? Yi[i] : Yi[i] * std::pow(Xi[i],(double)k);
        }
		for(int j = 0; j <= m; j++)
		{
			A[j] = 0.0;
			for(int i = 0; i < n; i++)
			{
				A[j] += ((k+j) == 0) ? 1.0 : std::pow(Xi[i],(k+j));
        	}
        }
        for(int i = 0; i <= m; i++)
        {
        	matriz[k][i] = A[i];
        }
        matriz[k][m+1] = b;
        std::free(A);
	}
	// triangularizar por redu��o de Gauss a matriz do sistema linear
	for(int i = 0; i < m; i++)
	{
		for(int j = (i+1); j <= m; j++)
		{
			double fator = matriz[j][i] / matriz[i][i];
			for(int k = i; k <= (m+1); k++)
			{
				matriz[j][k] = matriz[j][k] - matriz[i][k] * fator;
			}
		}
	}
	// obter solu��es da redu��o de Gauss (o vetor j� deve estar alocado para, pelo menos, OrdemMax+1)
	Coeficientes[m] = matriz[m][m+1] / matriz[m][m];
	for(int i = (m-1); i >= 0; i--)
	{
		double soma = 0.0;
		for(int j = (i+1); j <= m; j++)  soma += matriz[i][j] * Coeficientes[j];
		Coeficientes[i] = (1.0/matriz[i][i])*(matriz[i][m+1] - soma);
	}
	// se chegou aqui, conseguir realizar o processo
	return true;
}

/*!
 * \brief Calcula o percentil P para variaveis discretas.
 * \param  Vetor double com valores n�o necess�riamente ordenados
 * \param P double, percentil a ser calculado numero de 0.0 a 1.0
 * \return Retorna o valor do percentil desejado, ou seja o valor menor ou igual a probabilidade P.
 */
double Percentil(const std::vector<double> &Amostras, const double P)
{
	double valor;
	long k;
	size_t NPonto = Amostras.size(); // Contagem inicia de 0
	//  Cria um vetor para armazenar o vetor ordenado.
	std::vector<double> vetorordenado(Amostras);
	k = (long)std::floor(P * NPonto);
	//  Ordena Vetor
	std::sort(vetorordenado.begin(),vetorordenado.end());
	valor = vetorordenado[k];
	return valor;
}

/*!
 * \brief Gerar todos os combinat�rios sem repeti��o do bin�mio (n,k), sendo n o m�ximo de cada posi��o e k o n�mero de posi��es.
 * \param ValorMax Valor m�ximo para cada posi��o (n).
 * \param NumCasas N�mero de posi��es (k).
 * \param Combinacoes Refer�ncia para o vetor que ter� as combina��es (ser� apagado antes!).
 */
void Binomio(int ValorMax, int NumCasas, std::vector<std::vector<int> > &Combinacoes)
{
    Combinacoes.clear();
    std::vector<int> permutacao(NumCasas,1);
    bool done;
    do
    {
        Combinacoes.push_back(permutacao);
        // incremental sempre pelo �ltimo d�gito, fazer rolagem quando este chegar no m�ximo
        done = true;  // a princ�pio, j� chegamos na combina��o (n,n,n,...)
        for(int i = (NumCasas-1); i >= 0; i--)
        {
            if(permutacao[i]++ > ValorMax)
            {
                permutacao[i] = 1;
                continue;  // reiniciou o �ltimo, partiu para o anterior
            }
            else
            {
                done = false;  // ainda h� o que incrementar, nem todos os d�gitos causaram rolagem
                break;
            }
        }
    } while(!done);
}

/*!
 * \brief Retornar o pr�ximo valor de uma s�rie num�rica por aproxima��o linear.
 * \param Valores Vetor com os valores da s�rie num�rica.
 * \return Pr�ximo valor linearmente aproximado ou zero se o vetor estiver vazio.
 */
double TendenciaLinear(const std::vector<double> &Valores)
{
    if(Valores.size() == 0) return 0;
    double mx = 0, my = 0;
    double soma1 = 0, soma2 = 0;
    double m = 0, b = 0;
    double k = 1;
    for(std::vector<double>::const_reverse_iterator i = Valores.rbegin(); i != Valores.rend(); i++)
    {
        double y = *i;
        my += y;
        mx += k;
        k += 1.0;
    }
    mx /= (double)Valores.size();
    my /= (double)Valores.size();
    k = 1;
    for(std::vector<double>::const_reverse_iterator i = Valores.rbegin(); i != Valores.rend(); i++)
    {
        double y = *i;
        soma1 += (k - mx) * (y - my);
        soma2 += std::pow(k - mx,2.0);
        k += 1.0;
    }
    m = soma1 / soma2;
    b = my - m * mx;
    return (m + b);
}

double MediaMovel(const std::vector<double> &Valores, int Largura)
{
    std::vector<double>::const_reverse_iterator it = Valores.rbegin();
    double soma = 0, n = 1E-8;
    for(int i = 0; i < Largura; i++)
    {
        if(it == Valores.rend()) break;
        soma += *it;
        n += 1.0;
        it++;
    }
    return(soma/n);
}


/*!
 * \brief Remover todos os espa�os de uma express�o string.
 * \param  Expressao String a ser analisada para exclus�o dos espa�os.
 * \return Retorna a string sem os espa�os no seu interior.
 */
std::string RemoverEspacos (const char* Expressao)
{
	std::string texto = Expressao;
	size_t found = texto.find(' ');
	while (found != std::string::npos)
	{
		texto.erase(found,1);
		found = texto.find(' ');
	}
	return texto;
}

/*!
 * \brief Converte os caracteres de uma string em mai�sculas.
 * \param  Expressao  A string a ser convertida em mai�sculas.
 * \return Retorna a string Valor com seus caracteres em ma��sculo.
 */
std::string UCase(const char* Expressao)
{
	std::string texto = Expressao;
	for (unsigned int i = 0;i < texto.size();i++)
	{
		texto[i] = toupper(texto[i]);
	}
	return texto;
}

/*!
 * \brief Converte os caracteres de uma string em min�sculas.
 * \param  Expressao  A string a ser convertida em min�sculas.
 * \return Retorna a string Valor com seus caracteres em min�sculo.
 */
std::string LCase(const char* Expressao)
{
	std::string texto = Expressao;
	for (unsigned int i = 0;i < texto.size();i++)
	{
		texto[i] = tolower(texto[i]);
	}
	return texto;
}

/*!
 * \brief Replace occurrences of a substring (needle) in a text string (haystack).
 * \param  Haystack  Text string that will be searched for the substring.
 * \param  Needle    Substring that will be searched.
 * \param  Replace   Expression that will be placed in the position of the needle.
 * \return  A copy of the text string with the needles replaced by the replacement text.
 */
std::string Replace(const char* Haystack, const char* Needle, const char* Replace)
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

/*!
 * \brief Dividir uma string em partes separadas por um dado caracter delimitador.
 * \param Texto        A string da qual se deseja obter substrings.
 * \param Delimitador  Caracter que separa as substrings dentro da string.
 * \param Elementos    Um vetor que ir� conter as strings encontradas na string dada separadas pelo delimitador dado.
 * \return Retorna uma auto-refer�ncia do objeto com as substrings, para aninhamento.
 */
std::vector<std::string> &Split(const std::string &Texto, char Delimitador, std::vector<std::string> &Elementos)
{
    Elementos.clear();
    std::stringstream ss(Texto);
    std::string item;
    while(std::getline(ss, item, Delimitador))  Elementos.push_back(item);
    return Elementos;
}

/*!
* \brief Converte UNICODE para std:string.
*
* Esta fun��o retorna uma string de uma string UNICODE. A solu��o apresentada pelo
* Builder apresenta altera��o da string original com a subtra��o de caracteres no final
* da string original. Fun��o substitui o uso do t_str(). Com essa fun��o voce converte
* o wstring (UNICODE) para o string padr�o.
*
* \param Entrada string em UNICODE.
* \return Retorna uma string padr�o std.
*/
std::string Narrow(const std::wstring& Entrada)
{
	std::ostringstream stm;
	const std::ctype<char>& ctfacet = std::use_facet< std::ctype<char> >( stm.getloc() );
	for( std::size_t i=0 ; i< Entrada.size() ; ++i )  stm << ctfacet.narrow( Entrada[i], 0 ) ;
	return stm.str() ;
}

/*!
 * \brief Formata um n�mero para um certo espa�o determinado.
 * \param  Valor    N�mero que se deseja formatar.
 * \param  Tamanho  Tamanho m�ximo do texto com o n�mero formatado.
 * \return  Retorna um texto com o n�mero colocado dentro do tamanho determinado.
 */
std::string NumeroCEPEL(double Valor, int Tamanho)
{
    char buf[Tamanho+1];
    std::sprintf(buf,"%*g",Tamanho,Valor);
    return buf;

}

//-----------------------------------------------------------------------------

/*!
 * \brief Obter nome do m�s.
 * \param Mes        N�mero do m�s que se deseja obter o nome (de 1 a 12).
 * \param Abreviado  Verdadeiro se o nome dever� ser retornado abreviado, e falso se o nome dever� ser retornado completo.
 * \return Retorna uma string com o nome (abreviado ou completo) do m�s, ou vazio se o m�s for inv�lido.
 */
std::string NomeMes(int Mes, bool Abreviado)
{
	if(Mes < 1 || Mes > 13)  return "";
	std::string meses_a[12] = {"Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez" };
	std::string meses[12] = {"Janeiro", "Fevereiro", "Mar�o", "Abril", "Maio", "Junho", "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro" };
	if(Abreviado) return std::string(meses_a[Mes-1]);
	return std::string(meses[Mes-1]);
}

/*! Caracteres v�lidos na codifica��o em base 64 (RFC 989). */
static const std::string Base64_Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*!
 * \brief Identifica se um dado caracter � v�lido para a RFC 989; escrito por Ren� Nyffenegger (rene.nyffenegger@adp-gmbh.ch).
 * \param Char  Caractere que se deseja validar como alfanum�rico, "/" ou "+".
 * \param Retorna verdadeiro se o caracter � alfanum�rico, "/" ou "+", e falso caso contr�rio.
 */
static bool IsBase64(char Char)
{
	return (isalnum(Char) || (Char == '+') || (Char == '/'));
}

/*!
 * \brief Codificar um texto em base 64 (RCF 989).
 * \param BytesToEncode  Vetor de caracteres que ser�o codificados.
 * \param Len  N�mero de bytes do vetor de caracteres que ser� codificado.
 * \return Retorna o n�mero de bytes do vetor de caracteres codificado em base 64.
 */
std::string Base64_Encode(const char* BytesToEncode, unsigned int Len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	char char_array_3[3];
	char char_array_4[4];
	while(Len--)
	{
		char_array_3[i++] = *(BytesToEncode++);
		if(i == 3)
		{
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;
			for(i = 0; (i <4) ; i++)  ret += Base64_Chars[char_array_4[i]];
			i = 0;
		}
	}
	if(i)
	{
		for(j = i; j < 3; j++)
		char_array_3[j] = '\0';
		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		char_array_4[3] = char_array_3[2] & 0x3f;
		for (j = 0; (j < i + 1); j++)  ret += Base64_Chars[char_array_4[j]];
		while((i++ < 3))  ret += '=';
	}
	return ret;
}

/*!
 * \brief Decodificar um texto em base 64 (RCF 989) para o formato ANSI (C++).
 * \param EncodedString  Texto em base 64 a ser decodificado para o formato ANSI do C++.
 * \return Retorna o texto decodificado de base 64 para o formato ANSI do C++.
 */
std::string Base64_Decode(std::string const& EncodedString)
{
	size_t in_len = EncodedString.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	char char_array_4[4], char_array_3[3];
	std::string ret;
	while(in_len-- && ( EncodedString[in_] != '=') && IsBase64(EncodedString[in_]))
	{
		char_array_4[i++] = EncodedString[in_]; in_++;
		if(i ==4)
		{
			for(i = 0; i < 4; i++)  char_array_4[i] = Base64_Chars.find(char_array_4[i]);
			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
			for (i = 0; (i < 3); i++)
			ret += char_array_3[i];
			i = 0;
		}
	}
	if (i)
	{
		for (j = i; j <4; j++)  char_array_4[j] = 0;
		for (j = 0; j <4; j++)  char_array_4[j] = Base64_Chars.find(char_array_4[j]);
		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}
	return ret;
}

/*!
 * \brief Arredonda um dado valor de forma euclidiana para um dado n�mero de casas decimais.
 * \param Valor    Valor a ser arredondado.
 * \param Digitos  N�mero de digitos para o arredondamento.
 * \return Retorna o valor arredondado para o n�mero de d�gitos solicitado.
 */
double ArredondaValor(const double &Valor, const unsigned int &Digitos)
{
    double v = Valor;
    long int f = 1; for(unsigned int i = 0; i < Digitos; i++) f *= 10;
	double t = Valor*f - std::floor(Valor*f);
	v *= f;
	v = (t >= 0.5) ? std::ceil(v) : std::floor(v);
	v /= f;
    return v;
}


