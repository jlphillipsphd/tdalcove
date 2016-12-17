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
#include "cubic_activation_function.h"
#include <stddef.h>
#include <math.h>

CubicActivationFunction::CubicActivationFunction() : ActivationFunction() {
	gains = new double[1];
	gains[0] = 1.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

CubicActivationFunction::~CubicActivationFunction() {
	delete gains;
}

CubicActivationFunction::CubicActivationFunction(int number_of_input_dimensions) : ActivationFunction(number_of_input_dimensions) {
	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

CubicActivationFunction::CubicActivationFunction(int number_of_input_dimensions, double min, double max) : ActivationFunction(number_of_input_dimensions) {

	int x;
	high = true;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}

CubicActivationFunction::CubicActivationFunction(int number_of_input_dimensions, double min, double max, double* domains) : ActivationFunction(number_of_input_dimensions) {

	int x;
	high = true;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	if (domains != NULL)
		for (x = 0; x < dims; x++)
			gains[x] = fabs(domains[x]);

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}


double CubicActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	int x;
	double value = mx;
	bool meets_criterion = true;

	if (high) {
		for (x = 0; x < dims && meets_criterion; x++)
			if (variables[x] > gains[x] || variables[x] < -gains[x])
				meets_criterion = false;
	}
	else {
		for (x = 0; x < dims && meets_criterion; x++)
			if (variables[x] >= gains[x] || variables[x] <= -gains[x])
				meets_criterion = false;
	}

	if (!meets_criterion)
		value = mn;

	return value;
}

double CubicActivationFunction::Derivative(int dimension, double* variables) {
	return 0.0;
}

double CubicActivationFunction::Derivative(double* variables) {
	return 0.0;
}

bool CubicActivationFunction::setDomain(int dim, double d) {
	if (dim < 0 || dim >= dims)
		return false;

	gains[dim] = fabs(d);
	return true;
}

double CubicActivationFunction::getDomain(int dim) {
	if (dim < 0 || dim >= dims)
		return 0.0;

	return gains[dim];
}

bool CubicActivationFunction::setDomains(double* d) {
	if (d == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		gains[x] = fabs(d[x]);

	return true;
}

bool CubicActivationFunction::getDomains(double* d) {
	if (d == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		d[x] = gains[x];

	return true;
}

bool CubicActivationFunction::setMin(double min) {
	if (min >= mx)
		return false;

	mn = min;
	return true;
}

double CubicActivationFunction::getMin() {
	return mn;
}

void CubicActivationFunction::biasMin() {
	high = false;
	return;
}

bool CubicActivationFunction::isBiasedMin() {
	return !high;
}

bool CubicActivationFunction::setMax(double max) {
	if (max <= mn)
		return false;

	mx = max;
	return true;
}

double CubicActivationFunction::getMax() {
	return mx;
}

void CubicActivationFunction::biasMax() {
	high = true;
	return;
}

bool CubicActivationFunction::isBiasedMax() {
	return high;
}

