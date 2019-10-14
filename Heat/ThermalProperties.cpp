#include "ThermalProperties.h"
/*!
 * \brief constructor, starts the openFile function
 * \param file_path file path where the values of properties are stored
 */
ThermalProperties::ThermalProperties(std::string file_path):
	file_path{file_path}
{
	openFile();
}
/*!
 * \brief default constructor
 */
ThermalProperties::ThermalProperties(){

}
/*!
 * \brief copy constructor
 * \param other object that will be copied
 */
ThermalProperties::ThermalProperties(ThermalProperties& other){
    this->temperature=other.temperature;
    this->thermalConductivity=other.thermalConductivity;
    this->kinematicViscosity=other.kinematicViscosity;
    this->prandtlNumber=other.prandtlNumber;
}
/*!
 * \brief move constructor
 * \param other object that will be copied
 */
ThermalProperties::ThermalProperties(ThermalProperties &&other){
    this->temperature=other.temperature;
    this->thermalConductivity=other.thermalConductivity;
    this->kinematicViscosity=other.kinematicViscosity;
    this->prandtlNumber=other.prandtlNumber;
}
/*!
 * \brief copy assignment operator
 * \param other object that will be copied
 * \return object with set attributes
 */
ThermalProperties& ThermalProperties::operator=(ThermalProperties &other){
    if(this!=&other){
        this->temperature=other.temperature;
        this->thermalConductivity=other.thermalConductivity;
        this->kinematicViscosity=other.kinematicViscosity;
        this->prandtlNumber=other.prandtlNumber;
    }
    return *this;
}
/*!
 * \brief move assignment operator
 * \param other object that will be copied
 * \return object with set attributes
 */
ThermalProperties& ThermalProperties::operator=(ThermalProperties &&other){
    if(this!=&other){
        this->temperature=other.temperature;
        this->thermalConductivity=other.thermalConductivity;
        this->kinematicViscosity=other.kinematicViscosity;
        this->prandtlNumber=other.prandtlNumber;
    }
    return *this;
}
/*!
 * \brief
 * calculates the value of thermal propertie using the interpolation,
 * the thermal propertie depends on the temperature value
 * \param temperature  value
 * \param type property type
 * \return value of chosen propertie
 */
double ThermalProperties::valueAt(const double & temperature, const PropertyType type)const
{
	switch (type)
	{
	case PropertyType::conductivity:
		return interpolation.calculate(temperature, this->temperature, thermalConductivity);
	case PropertyType::viscosity:
		return interpolation.calculate(temperature, this->temperature, kinematicViscosity);
	case PropertyType::prandtl:
		return interpolation.calculate(temperature, this->temperature, prandtlNumber);;
	}
}
/*!
 * \brief displays all data using iostream library, this function is used for tests
 */
void ThermalProperties::displayAllData()
{
	display(temperature, "temperature");
	display(thermalConductivity, "thermalConductivity");
	display(kinematicViscosity, "kinematicViscosity");
	display(prandtlNumber, "prandtlNumber");
}
/*!
 * \brief display chosen thermal propertie
 * \param data values of thermal properties
 * \param header type of the properties
 */
void ThermalProperties::display(std::vector<double> data, std::string header)
{
	std::cout << header << ": ";
	for (auto value:data)
	{
		std::cout << "\t" << value;
	}
	std::cout << std::endl;
}
/*!
 * \brief default destructor
 */
ThermalProperties::~ThermalProperties()
{
}
/*!
 * \brief
 * opens the file which stores the thermal properties data
 * if it fails the program will stop and display the error
 */
void ThermalProperties::openFile()
{
	file.open(file_path);
	if (file.is_open()) {
		loadDataFromFile();
	}
	else {
        //assert(false); //visual studio
        //below, qt
        std::string text="Couldn't open file which stores properties, "+file_path;
        QMessageBox::critical(nullptr,"file corrupted",
                              QString::fromStdString(text));
       exit(EXIT_FAILURE);
	}
}
/*!
 * \brief loads data form file
 */
void ThermalProperties::loadDataFromFile()
{
	std::string textLine = {};
	while (std::getline(file, textLine))
	{
		loadedData.push_back(textLine);
        }
        getLoadedData();
}
/*!
 * \brief reads the file
 */
void ThermalProperties::getLoadedData()
{
	std::string number = ""; //container for number from file
	std::string lineText = "";//contains line from file
	int numberOfWords = 0;/*counts the words, the value is equal to the number of columns, 
							0=temperature, 1=thermalConductivity, 
							2=kinematicViscosity, 3=prandtlNumber*/
	for (size_t i = 1; i < loadedData.size(); i++){//starting from i=1, because first line contains headers
		lineText = loadedData[i];
		numberOfWords = 0;
		for (size_t j = 0; j < lineText.length(); j++)
		{
			if (lineText[j] != '\t'){
				if (lineText[j] == ',') { lineText[j] = '.'; }
				number += lineText[j];
			}
			if( lineText[j]=='\t' || j==(lineText.length()-1) ){
				insertLoadedValue(number, numberOfWords);
				++numberOfWords;
				number = "";
			}
		}

	}
	file.close();
}
/*!
 * \brief sets the value from file into the class attributes
 * \param number value form file
 * \param typeOfData 0-temperature, 1-conductivity, 2-kinematicViscosity, 3-prandtlNumber
 * \throw display critical error which close the application if the data in file are corrupted:
 * value in not a number (std::invalid_argument),
 * value is too long for double type (std::out_of_range)
 */
void ThermalProperties::insertLoadedValue(std::string number, int typeOfData)
{
    try {
        double value = std::stod(number);
        switch (typeOfData)
        {
        case 0:
            temperature.push_back(value);
            break;
        case 1:
            thermalConductivity.push_back(value);
            break;
        case 2:
            kinematicViscosity.push_back(value);
            break;
        case 3:
            prandtlNumber.push_back(value);
            break;
        default:
            break;
        }
    }
    catch (std::invalid_argument) {
        std::string text="Data in "+file_path+" are corrupted.";
        QMessageBox::critical(nullptr,"Error while loading data",
                              QString::fromStdString(text));
        exit(EXIT_FAILURE);
    }
    catch(std::out_of_range){
        std::string text="Data in "+file_path+" are too long for double type.";
        QMessageBox::critical(nullptr,"Error while loading data",
                              QString::fromStdString(text));
        exit(EXIT_FAILURE);
    }
}
