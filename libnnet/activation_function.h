//
// 
//                This source code is part of
// 
//                        TD-ALCOVE
// 
//	         Temporal Difference - ALCOVE
// 
//                        VERSION 1.0.0
// Written by Joshua L. Phillips.
// Copyright (c) 2004-2016, Joshua L. Phillips.
// Check out http://www.cs.mtsu.edu/~jphillips/software.html for more
// information.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
// 
// If you want to redistribute modifications, please consider that
// derived work must not be called official TDALCOVE. Details are found
// in the README & LICENSE files - if they are missing, get the
// official version at github.com/jlphillipsphd/tdalcove/.
// 
// To help us fund TDALCOVE development, we humbly ask that you cite the
// papers on the package - you can find them in the top README file.
// 
// For more info, check our website at
// http://www.cs.mtsu.edu/~jphillips/software.html
// 
//
#ifndef ACTIVATION_FUNCTION_H
#define ACTIVATION_FUNCTION_H

#include "object.h"
#include <stddef.h>

class ActivationFunction : public Object {

public:
	ActivationFunction();
	ActivationFunction(int number_of_dimensions);

	virtual double Compute(double* variables) = 0;
	virtual double Derivative(int dimension, double* variables) = 0; //partial
	virtual double Derivative(double* variables) = 0; //total

	//Computes the function values relative to the position provided
	double Compute(double* variables, double* position);
	double Derivative(int dimension, double* variables, double* position);
	double Derivative(double* variables, double* position);
	//NOTE: The positional information is assumed to be of the same
	//dimensionality as the input space (number of input dimensions).
	//These functions allow for all derived activation functions to act as
	//radial basis activation functions if desired.

	int getNumberOfInputDimensions();

protected:
	int dims;

};

#endif
