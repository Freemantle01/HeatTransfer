#pragma once
#include <string>
#include "Interpolation.h"
#include <vector>
#include <fstream>
#include <assert.h>
#include <QMessageBox>
/*!
 * \brief The PropertyType enum
 * class stores the properties which is used in ThermalProperties class
 */
enum class PropertyType { conductivity, viscosity, prandtl };
/*!
 * \brief The ThermalProperties class
 * stores the thermal properties of the fluids
 */
class ThermalProperties
{
public:
    ThermalProperties();
    ThermalProperties(ThermalProperties &&);
    ThermalProperties(ThermalProperties &);
    ThermalProperties& operator=(ThermalProperties &&);
    ThermalProperties& operator=(ThermalProperties &);
    ~ThermalProperties();
	ThermalProperties(const std::string file_path);
    double valueAt(const double &temperature, PropertyType type)const;
	void displayAllData();
    /*!
     * \brief stores the values of temperature
     */
    std::vector<double> temperature;
    /*!
     * \brief stores the values of thermal conductivity
     */
    std::vector<double> thermalConductivity;
    /*!
     * \brief stores the values of kinematic viscosity
     */
    std::vector<double> kinematicViscosity;
    /*!
     * \brief stores the values of Prandtl number
     */
    std::vector<double> prandtlNumber;
private:
    /*!
     * \brief
     * holds the file path to the data
     * which stores the thermal properties
     */
	const std::string file_path;
    /*!
     * \brief array which stores the rows of text from file
     */
	std::vector<std::string> loadedData;
    /*!
     * \brief object which is used for input operations on file (reading)
     */
	std::ifstream file;
    /*!
     * \brief object used for Lagrange interpolation method
     */
	Interpolation interpolation;
	void openFile();
	void loadDataFromFile();
	void getLoadedData();
	void insertLoadedValue(std::string number, int typeOfData );
	void display(std::vector<double> data, std::string header);
};

 
