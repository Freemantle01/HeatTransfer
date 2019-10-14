#include "Interpolation.h"


Interpolation::Interpolation()
{
}

Interpolation::~Interpolation()
{
}
/*!
 * \brief calculates the polynomial interpolation by Lagrange method
 * \param arg the X value for which the Y value will be calculated
 * \param valueX points for which the interpolation function is created
 * \param valueY points for which the interpolation function is created
 * \return value of Y for arg value
 */
double Interpolation::calculate(const double &arg,
                                const std::vector<double> &valueX,
                                const std::vector<double> &valueY)const
{
	double result{ 0 }, fraction{ 0 };
	for (size_t i = 0; i < valueX.size(); i++)
	{
		fraction = valueY[i];
		for (size_t j = 0; j < valueX.size(); j++)
		{
			if (j != i) {
				fraction *= ((arg - valueX[j])
					/ (valueX[i] - valueX[j]));
			}
		}
		result += fraction;
		fraction = 0;
	}
	return result;
}
