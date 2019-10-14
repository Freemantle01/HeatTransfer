#pragma once
#include <math.h>

/*!
 * \brief The NaturalConvection class
 * stores the parameters of natural convection
 * \author Łukasz Dyraga
 * \version 1.0
 */
class NaturalConvection
{
public:
	NaturalConvection();
	~NaturalConvection();
	void setValueOfAandC(const double productOfGrasshofAndPrandtl);
    /*!
     * \brief parameter used in natural convection equation
     */
	double A;
    /*!
     * \brief parameter used in natural convection equation
     */
	double C;
};

