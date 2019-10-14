#include "InputData.h"

InputData::InputData()
{
}
/*!
 * \brief calculates the geometry parameters and radiant energy exchange value
 */
void InputData::calculateTheRemainingData()
{
	calculateGeometry();
	calculateRadiantEnergyExchange();
}
/*!
 * \brief calculates the geometry parameters: diameters and areas
 */
void InputData::calculateGeometry()
{
	outerDiameterOfPipe = innerDiameterOfPipe + 2*thicknessOfPipe;
	overallDiameterOfPipe = outerDiameterOfPipe + 2*thicknessOfIsolator;
	areaOfOuterPipe = outerDiameterOfPipe * pow(PI, 2);
	areaOfPipe = overallDiameterOfPipe * pow(PI, 2);
}
/*!
 * \brief calculates the radiant energy exchange value
 */
void InputData::calculateRadiantEnergyExchange()
{
	double quotientOfArea = areaOfOuterPipe / areaOfPipe;
	double radiantRatio = 1 / emissivityOfIsolator + 
		quotientOfArea * ((1 / emissivityOfEnvironment)-1);
	ratioOfRadiantEnergyExchange = pow(radiantRatio, -1);
}
