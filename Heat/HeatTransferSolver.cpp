#include "HeatTransferSolver.h"



/*!
 * \brief constructor, sets the value of class attributes
 * \param data stores the input data
 * \param liquid stores the properties of liquid which flows through pipe
 */
HeatTransferSolver::HeatTransferSolver(InputData &data,ThermalProperties &liquid):
    data{&data},liquid{&liquid},naturalTransfer{}, isWarningAlertDisplay{false}

{
	air = new ThermalProperties{ airFilePath };
	this->data->calculateTheRemainingData();//Check do u need it!
	calculateInitialValues();
}
/*!
 * \brief
 * calculates the initial values:
 * convection coefficient 1,
 * thermal conduction,
 * thermal penetration
 */
void HeatTransferSolver::calculateInitialValues()
{
	calculateConvectionCoefficient1();
	calculateResistanceOfThermalConduction();
	calculateResistanceOfThermalPenetration();
}
/*!
 * \brief starts the main solving algorithm
 */
void HeatTransferSolver::runTheSolver()
{
	std::vector<double> interval=getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
	double temperatureOnIsolator
		=getTheIntersectionPointOfFunction(&HeatTransferSolver::getDifferenceOfHeatFlows, interval[0], interval[1]);
	setResults(temperatureOnIsolator);
}
/*!
 * \brief finds the intersection point of function using bisection method
 * \param *fun  address to a function
 * \param upperInterval upper interval value
 * \param bottomInterval bottom interval value
 * \param tolerance value of tolerance
 * \return the value of intersection point
 */
double HeatTransferSolver::getTheIntersectionPointOfFunction(double(HeatTransferSolver::*fun)(const double &), const double & upperInterval,
															const double & bottomInterval, const double & tolerance)
{
	double a = bottomInterval;
	double b = upperInterval;
	double x0 = 0;//intersection point
	while (abs(a - b) > tolerance) {
		x0 = (a + b) / 2;
		if ( abs ((this->*fun)(x0))   <= tolerance) {
			break;
		}
		else if ((this->*fun)(x0)  *  (this->*fun)(a) < 0) {
			b = x0;
		}
		else {
			a = x0;
		}
	}
	return x0;// returns the intersection point
}
/*!
 * \brief sets the results into the OutputData object
 * \param temperatureOnIsolator the final value of temperature on isolator
 */
void HeatTransferSolver::setResults(double const & temperatureOnIsolator)
{
	double temp = temperatureOnIsolator;
	results.temperatureOnIsolator = temp;
	results.convectionCoefficient2 = getConvectionCeofficient2(temp);
	results.radiationCoefficient2 = getRadiationCoefficient2(temp);
	results.heatFlowByConvection2 = getHeatFlowByConvection2(temp);
	results.heatFlowByRadiation2 = getHeatFlowByRadiation2(temp);
	results.heatFlow1 = getHeatFlow1(temp);
	results.heatFlow2 = getHeatFlow2(temp);
}
/*!
 * \brief calculates the interval values
 * \return the interval values: the bottom and upper
 */
std::vector<double> HeatTransferSolver::getIntervalValues(double (HeatTransferSolver::* fun)(const double&))
{
	std::vector<double> interval{ 0,0 };//interval[0] bottom, interval[1] upper
	double temp{ 0 };//temporary
	double length = abs(data->meanTemperatureOfLiquid + data->temperatureOfEnvironment);
	if (length < 100) {
		length =  200;
	}
	for (size_t j = 0, i = 0; i < length; ++j, i=j*10)
	{
		temp = (this->*fun)(i);
		if (temp < interval[0]) {//setting bottom interval
			interval[0] = temp;
		}
		else if (temp > interval[1]) {//setting upper interval
			interval[1] = temp;
		}
	}
	return interval;
}
/*!
 * \brief calculates the value of convection coefficient 1
 */
void HeatTransferSolver::calculateConvectionCoefficient1()
{
	double TemperatureLiquid = data->meanTemperatureOfLiquid;
	double ReynoldsNumber = (data->meanVelocityOfLiquid*data->innerDiameterOfPipe) /
		liquid->valueAt(TemperatureLiquid, PropertyType::viscosity);
	double v = liquid->valueAt(TemperatureLiquid, PropertyType::viscosity);
	double PrandtlNumber = liquid->valueAt(TemperatureLiquid, PropertyType::prandtl);
	double A = data->forcedConvectionConstValueA;
	double B = data->forcedConvectionConstValueB;
	double C = data->forcedConvectionConstValueC;
	double NusseltNumber = C * getPower(ReynoldsNumber, A)*getPower(PrandtlNumber, B);
	results.convectionCoefficient1 = (NusseltNumber*liquid->valueAt(TemperatureLiquid, PropertyType::conductivity))
		/ data->innerDiameterOfPipe;	
}
/*!
 * \brief calculates the value of resistance of thermal conduction
 */
void HeatTransferSolver::calculateResistanceOfThermalConduction()
{
	double numerator = log(data->overallDiameterOfPipe / data->outerDiameterOfPipe);
	double denominator = 2 * data->PI*data->thermalConductivityOfIsolator * data->lengthOfPipe;
	results.resistanceOfThermalConduction = numerator / denominator;
}
/*!
 * \brief calculates the value of resistance of thermal penetration
 */
void HeatTransferSolver::calculateResistanceOfThermalPenetration()
{
	results.resistanceOfThermalPenetration = 1.0 /
		(results.convectionCoefficient1*data->PI*data->innerDiameterOfPipe);
}
/*!
 * \brief calculates the heat flow 1 value
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of heat flow 1
 */
double HeatTransferSolver::getHeatFlow1(const double &temperatureOnIsolator)
{
	double numerator = data->meanTemperatureOfLiquid - temperatureOnIsolator;
	double denominator = results.resistanceOfThermalConduction + 
		results.resistanceOfThermalPenetration;
	return getQuotient(numerator,denominator);
}
/*!
 * \brief calculates the heat flow 2 value
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of heat flow 2
 */
double HeatTransferSolver::getHeatFlow2(const double & temperatureOnIsolator)
{
	double a2r=getRadiationCoefficient2(temperatureOnIsolator);
	double a2k = getConvectionCeofficient2(temperatureOnIsolator);
	double heatFlow = data->PI*data->overallDiameterOfPipe;
	heatFlow *= (getRadiationCoefficient2(temperatureOnIsolator) +
		getConvectionCeofficient2(temperatureOnIsolator));
	heatFlow *= (temperatureOnIsolator - data->temperatureOfEnvironment);
	return heatFlow;
}
/*!
 * \brief calculates the difference between heat flow 2 and heat flow 1
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of difference between heat flows
 */
double HeatTransferSolver::getDifferenceOfHeatFlows(const double & temperatureOnIsolator)
{
	return getHeatFlow2(temperatureOnIsolator)-getHeatFlow1(temperatureOnIsolator);
}
/*!
 * \brief calculates the heat flow made by radiation 2
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of heat flow made by radiation 2
 */
double HeatTransferSolver::getHeatFlowByRadiation2(const double & temperatureOnIsolator)
{
	return data->PI*
		data->overallDiameterOfPipe*
		getRadiationCoefficient2(temperatureOnIsolator)*
		(temperatureOnIsolator - data->temperatureOfEnvironment);
}
/*!
 * \brief calculates the heat flow made by convection 2
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of heat flow made by convection 2
 */
double HeatTransferSolver::getHeatFlowByConvection2(const double & temperatureOnIsolator)
{
	return data->PI*
		data->overallDiameterOfPipe*
		getConvectionCeofficient2(temperatureOnIsolator)*
		(temperatureOnIsolator - data->temperatureOfEnvironment);
}
/*!
 * \brief calculates the radiation coefficient 2
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of radiation coefficient 2
 */
double HeatTransferSolver::getRadiationCoefficient2(const double & temperatureOnIsolator)
{
	double numerator = data->ratioOfRadiantEnergyExchange*data->StefanBoltzmannConstant;
	numerator *= (getPower(temperatureOnIsolator / 100, 4) - getPower(data->temperatureOfEnvironment / 100, 4));
	double denominator = temperatureOnIsolator - data->temperatureOfEnvironment;
	return getQuotient(numerator, denominator);
}
/*!
 * \brief calculates the convection coefficient 2
 * \param temperatureOnIsolator value of temperature on isolator
 * \return value of convection coefficient 2
 */
double HeatTransferSolver::getConvectionCeofficient2(const double & temperatureOnIsolator)
{
	double meanTemperature = 0.5*(temperatureOnIsolator + data->temperatureOfEnvironment);
	double conductivityAir = air->valueAt(meanTemperature, PropertyType::conductivity);
	double prandtlAir = air->valueAt(meanTemperature, PropertyType::prandtl);
	double viscosityAir = air->valueAt(meanTemperature, PropertyType::viscosity);
	double GrashofNumber = getGrashofNumber(temperatureOnIsolator, viscosityAir);
	naturalTransfer.setValueOfAandC(GrashofNumber*prandtlAir);
	double numerator = naturalTransfer.C*conductivityAir;
	numerator *= getPower(GrashofNumber*prandtlAir, naturalTransfer.A);
	double denominator = data->overallDiameterOfPipe;
	return getQuotient(numerator, denominator);
}
/*!
 * \brief calculates the Grashof's value
 * \param temperatureOnIsolator value of temperature on isolator
 * \param viscosityOfAir value of viscosity of air
 * \return value of Grashof's number
 */
double HeatTransferSolver::getGrashofNumber(const double &temperatureOnIsolator, const double &viscosityOfAir)
{
	double beta = 2.0 / (temperatureOnIsolator + data->temperatureOfEnvironment);
	double numerator = data->accelerationOfGravity*beta;
	numerator *= (temperatureOnIsolator - data->temperatureOfEnvironment);
	numerator *= getPower(data->overallDiameterOfPipe, 3);
	double denominator = getPower(viscosityOfAir, 2);
	return getQuotient(numerator, denominator);
}
/*!
 * \brief calculates the quotient
 * \param numerator value of numerator
 * \param denominator value of denominator
 * \return value of quotient
 */
double HeatTransferSolver::getQuotient(const double & numerator, const double & denominator)
{
	if (numerator != 0 && denominator != 0) {
		return numerator / denominator;
	}
	else {
		return 0.0;
	}
}
/*!
 * \brief calculates exponentiation
 * \param base value of base
 * \param exponent value of exponent
 * \throw if the results is NaN or inf it will displays warning message on application window
 * \return value of exponentiation
 */
double HeatTransferSolver::getPower(double base, double exponent)
{
    double result = pow(base, exponent);
    try {
        if ( isinf( abs(result) ) || isnan(result) ) {
            throw std::invalid_argument("Results may be incorrect.");
        }
    }
    catch (std::invalid_argument error) {
        if(!isWarningAlertDisplay){
            QMessageBox::warning(nullptr,"Warning from solver.",
                                 QString::fromStdString(error.what()));
            isWarningAlertDisplay=true;
        }
    }
    return result;
}
/*!
 * \brief returns the results obtained from solver
 * \return values of results
 */
OutputData * HeatTransferSolver::getResults()
{
	return &results;
}
/*!
 * \brief destructor that release dynamic allocated data
 */
HeatTransferSolver::~HeatTransferSolver()
{
	delete air;
}
