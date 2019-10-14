#pragma once
#include <vector>
#include <iostream>
/*!
 * \brief The Interpolation class
 * stores the polynomial interpolation methods
 * \author Łukasz Dyraga
 * \version 1.0
 */
class Interpolation//Lagrange 
{
public:
	Interpolation();
	double calculate(const double &arg, const std::vector<double> &valueX, const std::vector<double> &valueY)const;
	~Interpolation();
};

