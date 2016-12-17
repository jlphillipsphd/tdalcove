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
#include "linear_activation_function.h"
#include <stddef.h>
#include <math.h>

LinearActivationFunction::LinearActivationFunction() : ActivationFunction() {
	gains = new double[1];
	gains[0] = 1.0;
	offset = 0.0;
}

LinearActivationFunction::~LinearActivationFunction() {
	delete gains;
}

LinearActivationFunction::LinearActivationFunction(int number_of_input_dimensions) : ActivationFunction(number_of_input_dimensions) {
	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;
	offset = 0.0;
}

LinearActivationFunction::LinearActivationFunction(int number_of_input_dimensions, double intercept) : ActivationFunction(number_of_input_dimensions) {

	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	offset = intercept;
}

LinearActivationFunction::LinearActivationFunction(int number_of_input_dimensions, double intercept, double* slopes) : ActivationFunction(number_of_input_dimensions) {

	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	if (slopes != NULL)
		for (x = 0; x < dims; x++)
			gains[x] = slopes[x];

	offset = intercept;
}


double LinearActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	double value = 0.0;

	for (int x = 0; x < dims; x++)
		value += gains[x] * variables[x];

	return (value + offset);
}

double LinearActivationFunction::Derivative(int dimension, double* variables) {
	if (dimension < 0 || dimension >= dims || variables == NULL)
		return 0.0;

	return gains[dimension];
}

double LinearActivationFunction::Derivative(double* variables) {
	if (variables == NULL)
		return 0.0;

	double value = 0.0;

	for (int x = 0; x < dims; x++)
		value += gains[x];

	return value;
}

bool LinearActivationFunction::setSlope(int dim, double s) {
	if (dim < 0 || dim >= dims)
		return false;

	gains[dim] = s;
	return true;
}

double LinearActivationFunction::getSlope(int dim) {
	if (dim < 0 || dim >= dims)
		return 0.0;

	return gains[dim];
}

bool LinearActivationFunction::setSlopes(double* s) {
	if (s == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		gains[x] = s[x];

	return true;
}

bool LinearActivationFunction::getSlopes(double* s) {
	if (s == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		s[x] = gains[x];

	return true;
}

bool LinearActivationFunction::setIntercept(double val) {
	offset = val;
	return true;
}

double LinearActivationFunction::getIntercept() {
	return offset;
}

