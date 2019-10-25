#include "pch.h"
#include "../Project1/Interpolation.cpp"
#include "../Project1/NaturalConvection.cpp"
#include "../Project1/ThermalProperties.cpp"
#include "../Project1/InputData.cpp" 
#include "../Project1/OutputData.cpp"
#include "../Project1/HeatTransferSolver.cpp"
#include <array>


TEST(Interpolation, LagrangeAlgorithm) {
	Interpolation lagrange;
	std::vector<double> X{ 300,400,500 };
	std::vector<double> Y{ 20,30,40 };
	EXPECT_EQ(20, lagrange.calculate(300, X, Y));
	EXPECT_EQ(30, lagrange.calculate(400, X, Y));
	EXPECT_EQ(40, lagrange.calculate(500, X, Y));
}

TEST(NaturalConvection, noFlow) {
	NaturalConvection naturalTransfer;
	naturalTransfer.setValueOfAandC(pow(10, -4));
	EXPECT_EQ(0, naturalTransfer.A);
	EXPECT_EQ(0.45, naturalTransfer.C);
}

TEST(NaturalConvection, laminarFlow) {
	NaturalConvection naturalTransfer;
	std::array<double,4> data{ pow(10,-3), pow(10,-2), 5 * pow(10,1), 4.9 * pow(10,2) };
	for (size_t i = 0; i < data.size(); i++)
	{
		naturalTransfer.setValueOfAandC(data[i]);
		EXPECT_EQ(0.125, naturalTransfer.A);
		EXPECT_EQ(1.18, naturalTransfer.C);
	}
}

TEST(NaturalConvection, transitionalFlow) {
	NaturalConvection naturalTransfer;
	std::array<double, 4> data{ 5*pow(10,2), 5*pow(10,3),7 * pow(10,5), 1.9* pow(10,7) };
	for (size_t i = 0; i < data.size(); i++)
	{
		naturalTransfer.setValueOfAandC(data[i]);
		EXPECT_EQ(0.25, naturalTransfer.A);
		EXPECT_EQ(0.54, naturalTransfer.C);
	}
}

TEST(NaturalConvection, turbulentFlow) {
	NaturalConvection naturalTransfer;
	std::array<double, 2> data{ 2 * pow(10,7), 2*pow(10,8) };
	for (size_t i = 0; i < data.size(); i++)
	{
		naturalTransfer.setValueOfAandC(data[i]);
		EXPECT_EQ(0.33, naturalTransfer.A);
		EXPECT_EQ(0.135, naturalTransfer.C);
	}
}

TEST(ThermalProperties, fileLoadedAir) {
	ThermalProperties air{ "fluids_properties/air.txt" };
	EXPECT_EQ(0.0265, air.valueAt(300, PropertyType::conductivity));
	EXPECT_EQ(0.0000159, air.valueAt(300, PropertyType::viscosity));
	EXPECT_EQ(0.702, air.valueAt(300, PropertyType::prandtl));
}

TEST(ThermalProperties, fileLoadedGlycerin) {
	ThermalProperties liquid{ "fluids_properties/glycerin.txt" };
	EXPECT_EQ(0.282, liquid.valueAt(273.15, PropertyType::conductivity));
	EXPECT_EQ(0.0082190, liquid.valueAt(273.15, PropertyType::viscosity));
	EXPECT_EQ(84.10, liquid.valueAt(273.15, PropertyType::prandtl));
}

TEST(ThermalProperties, fileLoadedWater) {
	ThermalProperties water{ "fluids_properties/water.txt" };
	EXPECT_EQ(0.612, water.valueAt(300, PropertyType::conductivity));
	EXPECT_EQ(0.00000086, water.valueAt(300, PropertyType::viscosity));
	EXPECT_EQ(5.85, water.valueAt(300, PropertyType::prandtl));
}

TEST(ThermalProperties, fileLoadedIsobutane) {
	ThermalProperties liquid{ "fluids_properties/isobutane.txt" };
	EXPECT_EQ(0.1068, liquid.valueAt(273.15, PropertyType::conductivity));
	EXPECT_EQ(0.0000003432, liquid.valueAt(273.15, PropertyType::viscosity));
	EXPECT_EQ(4.304, liquid.valueAt(273.15, PropertyType::prandtl));
}

TEST(ThermalProperties, fileLoadedMethanol) {
	ThermalProperties liquid{ "fluids_properties/methanol.txt" };
	EXPECT_EQ(0.1987, liquid.valueAt(293.15, PropertyType::conductivity));
	EXPECT_EQ(0.0000007429, liquid.valueAt(293.15, PropertyType::viscosity));
	EXPECT_EQ(7.414, liquid.valueAt(293.15, PropertyType::prandtl));
}

TEST(ThermalProperties, fileLoadedEngineOil) {
	ThermalProperties liquid{ "fluids_properties/engine_oil_unused.txt" };
	EXPECT_EQ(0.1469, liquid.valueAt(273.15, PropertyType::conductivity));
	EXPECT_EQ(0.004242, liquid.valueAt(273.15, PropertyType::viscosity));
	EXPECT_EQ(46.636, liquid.valueAt(273.15, PropertyType::prandtl));
}

TEST(InputData, geometryResult) {
	InputData data;
	data.innerDiameterOfPipe = 0.08;
	data.thicknessOfPipe = 0.004;
	data.thicknessOfIsolator = 0.03;
	data.calculateTheRemainingData();
	EXPECT_EQ(0.088, data.outerDiameterOfPipe);
	EXPECT_EQ(0.148, data.overallDiameterOfPipe);
	EXPECT_EQ(0.088*pow(data.PI, 2), data.areaOfOuterPipe);
	EXPECT_EQ(0.148*pow(data.PI, 2), data.areaOfPipe);
}

TEST(InputData, radiantEnergyExchangeResult) {
	InputData data;
	data.innerDiameterOfPipe = 0.08;
	data.thicknessOfPipe = 0.004;
	data.thicknessOfIsolator = 0.03;
	data.emissivityOfIsolator = 0.92;
	data.calculateTheRemainingData();
	EXPECT_EQ(0.92,data.ratioOfRadiantEnergyExchange);
}

InputData* getTestInputData() {
	InputData *data=new InputData;
	//Geometry
	data->innerDiameterOfPipe = 0.08;
	data->thicknessOfPipe = 0.004;
	data->thicknessOfIsolator = 0.03;
	data->lengthOfPipe = 1;
	//Liquid Properties
	data->meanTemperatureOfLiquid = 413;
	data->meanVelocityOfLiquid = 1;
	//Isolator Properties
	data->emissivityOfIsolator = 0.92;
	data->thermalConductivityOfIsolator = 0.093;
	//Forced convection values 
	data->forcedConvectionConstValueA = 0.8;
	data->forcedConvectionConstValueB = 0.4;
	data->forcedConvectionConstValueC = 0.023;
	//Environment Properties
	data->temperatureOfEnvironment = 286;
	data->calculateTheRemainingData();
	return data;
}

TEST(HeatTransferSolver, convectionCoefficient1) {
	InputData *data=getTestInputData();
	ThermalProperties liquid{"fluids_properties/water.txt"};
	HeatTransferSolver	exercise{ *data,liquid };
	exercise.calculateConvectionCoefficient1();
	OutputData *result = exercise.getResults();
	double value = result->convectionCoefficient1;
	EXPECT_GT(6350, value);
	EXPECT_LT(6250, value);
	delete data;
}

TEST(HeatTransferSolver, resistanceOfThermalConduction) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	exercise.calculateResistanceOfThermalConduction();
	OutputData *result = exercise.getResults();
	double value = result->resistanceOfThermalConduction;
	EXPECT_GT(0.8897, value);
	EXPECT_LT(0.8895, value);
	delete data;
}

TEST(HeatTransferSolver, resistanceOfThermalPenetration) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	exercise.calculateConvectionCoefficient1();
	exercise.calculateResistanceOfThermalPenetration();
	OutputData *result = exercise.getResults();
	double value = result->resistanceOfThermalPenetration;
	EXPECT_GT(0.00067, value);
	EXPECT_LT(0.00062, value);
	delete data;
}

TEST(HeatTransferSolver, heatFlow1) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	exercise.calculateResistanceOfThermalConduction();
	exercise.calculateResistanceOfThermalPenetration();
	OutputData *result = exercise.getResults();
	double value = exercise.getHeatFlow1(320);
	EXPECT_GT(104.7, value);
	EXPECT_LT(104.3, value);
	delete data;
}

TEST(HeatTransferSolver, radiationCoefficient2) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double value = exercise.getRadiationCoefficient2(320);
	EXPECT_GT(5.9, value);
	EXPECT_LT(5.7, value);
	delete data;
}

TEST(HeatTransferSolver, grashofNumber) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	ThermalProperties air{ "fluids_properties/air.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double temperatureOnIsolator = 320;
	double meanTemperature = 0.5*(temperatureOnIsolator + data->temperatureOfEnvironment);
	double beta = 1.0 / meanTemperature;
	double viscosityAir = air.valueAt(meanTemperature, PropertyType::viscosity);
	double value = exercise.getGrashofNumber(320, viscosityAir);
	EXPECT_GT(14210000, value);
	EXPECT_LT(13500000, value);
	delete data;
}

TEST(HeatTransferSolver, ConvectionCeofficient2) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double value = exercise.getConvectionCeofficient2(320);
	EXPECT_GT(5.45, value);
	EXPECT_LT(5.40, value);
	delete data;
}

TEST(HeatTransferSolver, heatFlow2) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double value = exercise.getHeatFlow2(320);
	EXPECT_GT(178, value);
	EXPECT_LT(177, value);
	delete data;
}

TEST(HeatTransferSolver, quotientFunction) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	EXPECT_EQ(0, exercise.getQuotient(0, 1));
	EXPECT_EQ(0, exercise.getQuotient(0, 0));
	EXPECT_EQ(0.5, exercise.getQuotient(2, 4));
	delete data;
}

TEST(HeatTransferSolver, heatFlowByConvection2) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double value = exercise.getHeatFlowByConvection2(320);
	EXPECT_GT(86, value);
	EXPECT_LT(85, value);
	delete data;
}

TEST(HeatTransferSolver, heatFlowByRadiation2) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver	exercise{ *data,liquid };
	double value = exercise.getHeatFlowByRadiation2(320);
	EXPECT_GT(93, value);
	EXPECT_LT(91, value);
	delete data;
}

bool checkIfValueIsInInterval(const double &value,const double &interval, const double &tolerance){
	if (value > (interval - tolerance) && value < (interval + tolerance)) {
		return true;
	}
	else {
		return false;
	}
}

TEST(HeatTransferSolver, setResults) {
	double Rlam = 0.890;
	double Ralfa1 = pow(6.43,-4);
	double T3 = 309.74;
	double ak1 = 6190.04;
	double a2r = 5.523;
	double a2k = 4.980;
	double q2k = 54.966;
	double q2r = 60.967;
	double q1 = 115.980;
	double q2 = 115.933;
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver example{ *data,liquid };
	example.runTheSolver();
	OutputData *results = example.getResults();
	ASSERT_TRUE(checkIfValueIsInInterval(results->resistanceOfThermalConduction,Rlam,0.1));
	ASSERT_TRUE(checkIfValueIsInInterval(results->resistanceOfThermalPenetration, Ralfa1, 0.00005));
	ASSERT_TRUE(checkIfValueIsInInterval(results->temperatureOnIsolator, T3, 0.5));
	ASSERT_TRUE(checkIfValueIsInInterval(results->convectionCoefficient1, ak1, 200));
	ASSERT_TRUE(checkIfValueIsInInterval(results->radiationCoefficient2, a2r, 0.1));
	ASSERT_TRUE(checkIfValueIsInInterval(results->convectionCoefficient2, a2k, 0.1));
	ASSERT_TRUE(checkIfValueIsInInterval(results->heatFlowByConvection2, q2k, 0.5));
	ASSERT_TRUE(checkIfValueIsInInterval(results->heatFlowByRadiation2, q2r, 0.5));
	ASSERT_TRUE(checkIfValueIsInInterval(results->heatFlow1, q1, 0.5));
	ASSERT_TRUE(checkIfValueIsInInterval(results->heatFlow2, q2, 0.5));
	delete data;
}

TEST(HeatTransferSolver, intersectionPointOfFunction) {
	double q1 = 115.980;
	double q2 = 115.933;
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver example{ *data,liquid };
	example.runTheSolver();
	OutputData *results = example.getResults();
	double x0 = results->temperatureOnIsolator;
	double q1x0 = example.getHeatFlow1(x0);
	double q2x0 = example.getHeatFlow2(x0);
	ASSERT_TRUE(checkIfValueIsInInterval(q1x0, q1, 0.5));
	ASSERT_TRUE(checkIfValueIsInInterval(q2x0, q2, 0.5));
}

TEST(HeatTransferSolver, differenceOfHeatFlows) {
	InputData *data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver example{ *data,liquid };
	double temp = 309.741592;//temperature on Isolator
	double diff = example.getDifferenceOfHeatFlows(temp);
	ASSERT_TRUE(checkIfValueIsInInterval(diff, 0, 0.05));
}

TEST(HeatTransferSolver, getIntervalValues_water) {
	InputData *data = getTestInputData();	
	ThermalProperties liquid{ "fluids_properties/water.txt" };
	HeatTransferSolver task(*data, liquid);
	for (size_t i = 0; i < 20; i++)
	{
		data->meanTemperatureOfLiquid = i*100.0;
		task.calculateInitialValues();
		task.runTheSolver();
		OutputData* results = task.getResults();
		double difference = results->heatFlow1 - results->heatFlow2;
		EXPECT_GT(1, difference);
		EXPECT_LT(-1, difference);
		std::vector<double> interval = task.getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
		EXPECT_GT(0, interval[0]);
		EXPECT_LE(0, interval[1]);
	}
}

TEST(HeatTransferSolver, getIntervalValues_engine_oil) {
	InputData* data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/engine_oil_unused.txt" };
	HeatTransferSolver task(*data, liquid);
	for (size_t i = 0; i < 20; i++)
	{
		data->meanTemperatureOfLiquid = i * 100.0;
		task.calculateInitialValues();
		task.runTheSolver();
		OutputData* results = task.getResults();
		double difference = results->heatFlow1 - results->heatFlow2;
		EXPECT_GT(1, difference);
		EXPECT_LT(-1, difference);
		std::vector<double> interval = task.getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
		EXPECT_GT(0, interval[0]);
		EXPECT_LE(0, interval[1]);
	}
}

TEST(HeatTransferSolver, getIntervalValues_glycerin){
	InputData* data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/glycerin.txt" };
	HeatTransferSolver task(*data, liquid);
	for (size_t i = 1; i < 20; i++)
	{
		data->meanTemperatureOfLiquid = i * 100.0;
		task.calculateInitialValues();
		task.runTheSolver();
		OutputData* results = task.getResults();
		double difference = results->heatFlow1 - results->heatFlow2;
		EXPECT_GT(1, difference);
		EXPECT_LT(-1, difference);
		std::vector<double> interval = task.getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
		EXPECT_GT(0, interval[0]);
		EXPECT_LE(0, interval[1]);
	}
}

TEST(HeatTransferSolver, getIntervalValues_isobutane) {
	InputData* data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/isobutane.txt" };
	HeatTransferSolver task(*data, liquid);
	for (size_t i = 0; i < 20; i++)
	{
		data->meanTemperatureOfLiquid = i * 100.0;
		task.calculateInitialValues();
		task.runTheSolver();
		OutputData* results = task.getResults();
		double difference = results->heatFlow1 - results->heatFlow2;
		EXPECT_GT(1, difference);
		EXPECT_LT(-1, difference);
		std::vector<double> interval = task.getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
		EXPECT_GT(0, interval[0]);
		EXPECT_LE(0, interval[1]);
	}
}

TEST(HeatTransferSolver, getIntervalValues_methanol) {
	InputData* data = getTestInputData();
	ThermalProperties liquid{ "fluids_properties/methanol.txt" };
	HeatTransferSolver task(*data, liquid);
	for (size_t i = 0; i < 40; i++)
	{
		data->meanTemperatureOfLiquid = i * 10;
		task.calculateInitialValues();
		task.runTheSolver();
		OutputData* results = task.getResults();
		double difference = results->heatFlow1 - results->heatFlow2;
		EXPECT_GT(1, difference);
		EXPECT_LT(-1, difference);
		std::vector<double> interval = task.getIntervalValues(&HeatTransferSolver::getDifferenceOfHeatFlows);
		EXPECT_GT(0, interval[0]);
		EXPECT_LE(0, interval[1]);
	}
}

