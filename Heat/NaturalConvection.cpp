#include "NaturalConvection.h"



NaturalConvection::NaturalConvection()
{
}


NaturalConvection::~NaturalConvection()
{
}
/*!
 * \brief sets the parameters of natural convection depends on productOfGrAndPr
 * \param productOfGrAndPr product of Grashof number and Prandtl number
 */
void NaturalConvection::setValueOfAandC(const double productOfGrAndPr)
{
	if (productOfGrAndPr<pow(10,-3)){//no flow
		C = 0.45;
		A = 0;
	}
	else if (productOfGrAndPr >= pow(10, -3) && productOfGrAndPr < (5*pow(10, 2))) {//laminar flow
		C = 1.18;
		A = 0.125;
	}
	else if (productOfGrAndPr >= (5 * pow(10, 2)) && productOfGrAndPr < (2 * pow(10, 7))) {//transitional flow
		C=0.54;
		A=0.25;
	}
	else if (productOfGrAndPr >= (2 * pow(10, 7))) {//turbulent flow
		C = 0.135;
		A = 0.33;
	}
}

