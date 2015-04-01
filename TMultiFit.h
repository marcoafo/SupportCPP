#ifndef TMultiFitH
#define TMultiFitH

#include <vector>

//---------------------------------------------------------------------------

/*!
 * \brief Class to build a multivariate linear regression.
 *
 * Since GSL MultiFit only works if the variables are bayesian, it can't be used in
 * all problems. This class uses a non-linear optimization method, so any sort of signals
 * can be regressed. Please note that existing a linear regression doesn't mean it has
 * a good calculation error. The optimization is done using NLOpt.
 */
class TMultiFit
{
private:
	std::vector<std::vector<double> > X;  /*!< Matrix n x m with the values of the dependable variables. */
	std::vector<double> Y;  /*!< Vector n x 1 with the values of the undependable variable. */
	std::vector<double> B;  /*!< Vector 1 x m with the resulting regression coefficients. */

public:
	// constructors and destructor
	TMultiFit();
	virtual ~TMultiFit();

	// assign functions
	bool SetValues(const std::vector<std::vector<double> > &Xi, const std::vector<double> &Yi);
	std::vector<double> GetCoefficients();

    // calculation functions
	double SquareError();
	void Reduce();

    /*!<
     * \brief Wrapper function with the evaluation equation of the class, so it can be called in a global scope.
     * \param N Number of total dependable variables of X.
     * \param B Pointer to the coefficients vector of the dependable variables of X.
     * \param Data Pointer to the object of this class that is being evaluated, since this is a static function..
     * \return Evaluation of the requested state, using the class function to do the evaluation.
     */
	static double SquareErrorWrapper(unsigned int N, const double *B, double*, void *Data)
	{
        TMultiFit *obj = static_cast<TMultiFit*>(Data);
		obj->B.clear();
		for(unsigned int i = 0; i < N; i++) obj->B.push_back(B[i]);
        return obj->SquareError();
	}
};

//---------------------------------------------------------------------------

#endif


