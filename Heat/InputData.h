#pragma once
#include <math.h>
/*!
 * \brief The InputData class
 * stores the values that are required for solving the heat transfer problem
 * \author Łukasz Dyraga
 * \version 1.0
 */
class InputData
{
public:
	InputData();
	void calculateTheRemainingData();
	//Geometry of pipe and isolator
	double innerDiameterOfPipe;
	double outerDiameterOfPipe;
    /*!
     * \brief outer diameter of pipe adding the thickness of isolator
     */
    double overallDiameterOfPipe;
	double thicknessOfPipe;
	double thicknessOfIsolator;
	double lengthOfPipe;
	double areaOfOuterPipe;
	double areaOfPipe;
	//Properties of liquid
	double meanVelocityOfLiquid;
	double meanTemperatureOfLiquid;
	//Properties of isolator
	double emissivityOfIsolator;
	double thermalConductivityOfIsolator;
	//Properties of environment
	const double emissivityOfEnvironment{1};
	double temperatureOfEnvironment;
	double ratioOfRadiantEnergyExchange;
	//Constant values of forced convection
	double forcedConvectionConstValueC;
	double forcedConvectionConstValueA;
	double forcedConvectionConstValueB;
	//Fundamental Physcial Constants
	const double accelerationOfGravity{ 9.80665 };
	const double StefanBoltzmannConstant{5.670367};
	const double PI{ 3.141592653589793238 };
private:
	void calculateGeometry();
	void calculateRadiantEnergyExchange();
};

