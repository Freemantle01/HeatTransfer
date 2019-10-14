#pragma once
/*!
 * \brief The OutputData class
 * stores the results values,
 * the index near the parameters means for
 * 1: flowing into the surface of the isolator
 * 2: flowing away from the surface of isolator
 * \author Łukasz Dyraga
 * \version 1.0
 */
class OutputData
{
public:
	OutputData();
	~OutputData();
	double resistanceOfThermalConduction;
	double resistanceOfThermalPenetration;
	double temperatureOnIsolator;
	double convectionCoefficient1;
	double convectionCoefficient2;
	double radiationCoefficient2;
	double heatFlowByConvection2;
	double heatFlowByRadiation2;
	double heatFlow2;
	double heatFlow1;
};

