#include <nlopt.hpp>

#include "TMultiFit.h"

//---------------------------------------------------------------------------

/*!
 * \brief Class constructor, with no parameters.
 */
TMultiFit::TMultiFit()
{
}

/*!
 * \brief Class destructor, empty because there's no pointers.
 */
TMultiFit::~TMultiFit()
{
}

//---------------------------------------------------------------------------

/*!
 * \brief Set the matrices of the undependable and dependable variables (signals) of the problem.
 * \param Xi Matrix with the values of every dependable variable, being the rows as the set of values for each variable (matrix-vector notation).
 * \param Yi Vector with the values of the undependable variable for each set of values of Xi.
 * \return True if the variables were correctly assigned, false if a dimension verification failed.
 */
bool TMultiFit::SetValues(const std::vector<std::vector<double> > &Xi, const std::vector<double> &Yi)
{
	if(Yi.size() == 0 || Xi.size() == 0) return false;
	if(Yi.size() != Xi.size()) return false;
	if(Xi[0].size() == 0) return false;
	Y.clear();
	X.clear();
	B.clear();
	unsigned int n = Yi.size();
	unsigned int m = Xi[0].size();
	for(unsigned int j = 0; j < m; j++) B.push_back(1);
	for(unsigned int i = 0; i < n; i++)
	{
		Y.push_back(Yi[i]);
		std::vector<double> aux;
		for(unsigned int j = 0; j < m; j++)
		{
			aux.push_back(Xi[i][j]);
		}
		X.push_back(aux);
	}
	return true;
}

/*!
 * \brief Get a copy of the coefficients vector.
 * \return Copy of the coefficients, being assigned to the order of dependable variables in Xi.
 */
std::vector<double> TMultiFit::GetCoefficients()
{
    return B;
}

/*!
 * \brief calculate the square error of the current state of the problem (the current coefficient vector).
 * \return Square error of the current state, or zero if there's no problem set.
 */
double TMultiFit::SquareError()
{
	if(Y.size() == 0 || X.size() == 0) return 0;
	unsigned int n = Y.size();
	unsigned int m = B.size();
	double erro = 0;
	for(unsigned int i = 0; i < n; i++)
	{
		double e = 0;
		for(unsigned int j = 0; j < m; j++)
		{
			e += B[j] * X[i][j];
		}
		e = Y[i] - e;
		erro += e*e;
	}
	return erro;
}

/*!
 * \brief Use a solver to reduce the square error of this object, in order to acquire the best coefficient matrix.
 *
 * Currently it's using the BOBYQA (http://en.wikipedia.org/wiki/BOBYQA) solver in order to get the best approach
* to the coefficients vector. Since the least square error method is unconstrained, one can assume that the boundary
* doesn't exists, hence the quadratic approximation can act a quasi Newton method.
*/
void TMultiFit::Reduce()
{
    std::vector<double> b(B);
    nlopt::opt opt(nlopt::LN_BOBYQA,B.size());
    opt.set_min_objective(TMultiFit::SquareErrorWrapper,this);
    opt.set_ftol_rel(1E-5);
    opt.set_xtol_rel(1E-5);
    double minf;
    opt.optimize(b, minf);
    B = b;
}
