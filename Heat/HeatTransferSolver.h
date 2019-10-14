#pragma once
#include "InputData.h"
#include "ThermalProperties.h"
#include <string>
#include "NaturalConvection.h"
#include "OutputData.h"
#include <QMessageBox>
#include <QString>
/*!
 * \brief The HeatTransferSolver class
 * solves the heat transfer problemm,
 * the index value near the functions and parameters names means:
 * 1: flowing into the surface of the isolator
 * 2: flowing away from the surface of isolator
 * \author Łukasz Dyraga
 * \version 1.0
 */
class HeatTransferSolver
{
public:
	HeatTransferSolver() = delete;
	explicit HeatTransferSolver(InputData &data,ThermalProperties &liquid);
	//Main algorithm functions
	void calculateInitialValues();
	void runTheSolver();
	double getTheIntersectionPointOfFunction(double (HeatTransferSolver::*fun)(const double &),const double &upperInterval, const double
										&bottomInterval,const double &tolerance =0.001);//Bisection method
	void setResults(double const &temperatureOnIsolator);
	std::vector<double> getIntervalValues(double (HeatTransferSolver::* fun)(const double&));
	OutputData* getResults();																					
	//Thermal Resistance functions
	void calculateResistanceOfThermalConduction();
	void calculateResistanceOfThermalPenetration();
	//Heat flow functions
	double getHeatFlow1(const double &temperatureOnIsolator);
	double getHeatFlow2(const double &temperatureOnIsolator);
	double getDifferenceOfHeatFlows(const double &temperatureOnIsolator);
	double getHeatFlowByRadiation2(const double &temperatureOnIsolator);
	double getHeatFlowByConvection2(const double &temperatureOnIsolator);
	//Coefficient functions
	void calculateConvectionCoefficient1();
	double getRadiationCoefficient2(const double &temperatureOnIsolator);
	double getConvectionCeofficient2(const double &temperatureOnIsolator);
	//Other functions
	double getGrashofNumber(const double &temperatureOnIsolator,const double &viscosityOfAir);
	double getQuotient(const double &numerator, const double &denominator);
	double getPower(double base, double exponent);
	~HeatTransferSolver();
private:
    /*!
     * \brief stores the input data
     */
	InputData *data;
    /*!
     * \brief properties of liquid which flows through pipe
     */
	const ThermalProperties *liquid;
    /*!
     * \brief properties of air
     */
	const ThermalProperties *air;
    /*!
     * \brief parameters of natural convection equation
     */
	NaturalConvection naturalTransfer;
    /*!
     * \brief file path for air properties
     */
	const std::string airFilePath{ "fluids_properties/air.txt" };
    /*!
     * \brief stores the output data (results)
     */
	OutputData results;
    /*!
     * \brief stores the output data (results)
     */
    bool isWarningAlertDisplay;
};

