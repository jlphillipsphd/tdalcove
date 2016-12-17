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
#include "threshold_activation_function.h"
#include <stddef.h>
#include <math.h>

ThresholdActivationFunction::ThresholdActivationFunction() : ActivationFunction() {
	gains = new double[1];
	gains[0] = 0.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

ThresholdActivationFunction::~ThresholdActivationFunction() {
	delete gains;
}

ThresholdActivationFunction::ThresholdActivationFunction(int number_of_input_dimensions) : ActivationFunction(number_of_input_dimensions) {
	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 0.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

ThresholdActivationFunction::ThresholdActivationFunction(int number_of_input_dimensions, double min, double max) : ActivationFunction(number_of_input_dimensions) {

	int x;
	high = true;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 0.0;

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}

ThresholdActivationFunction::ThresholdActivationFunction(int number_of_input_dimensions, double min, double max, double* thresholds) : ActivationFunction(number_of_input_dimensions) {

	int x;
	high = true;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 0.0;

	if (thresholds != NULL)
		for (x = 0; x < dims; x++)
			gains[x] = thresholds[x];

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}


double ThresholdActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	int x;
	double value = mx;
	bool meets_criterion = true;

	if (high) {
		for (x = 0; x < dims && meets_criterion; x++)
			if (variables[x] < gains[x])
				meets_criterion = false;
	}
	else {
		for (x = 0; x < dims && meets_criterion; x++)
			if (variables[x] <= gains[x])
				meets_criterion = false;
	}

	if (!meets_criterion)
		value = mn;

	return value;
}

double ThresholdActivationFunction::Derivative(int dimension, double* variables) {
	return 0.0;
}

double ThresholdActivationFunction::Derivative(double* variables) {
	return 0.0;
}

bool ThresholdActivationFunction::setThreshold(int dim, double t) {
	if (dim < 0 || dim >= dims)
		return false;

	gains[dim] = t;
	return true;
}

double ThresholdActivationFunction::getThreshold(int dim) {
	if (dim < 0 || dim >= dims)
		return 0.0;

	return gains[dim];
}

bool ThresholdActivationFunction::setThresholds(double* t) {
	if (t == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		gains[x] = t[x];

	return true;
}

bool ThresholdActivationFunction::getThresholds(double* t) {
	if (t == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		t[x] = gains[x];

	return true;
}

bool ThresholdActivationFunction::setMin(double min) {
	if (min >= mx)
		return false;

	mn = min;
	return true;
}

double ThresholdActivationFunction::getMin() {
	return mn;
}

void ThresholdActivationFunction::biasMin() {
	high = false;
	return;
}

bool ThresholdActivationFunction::isBiasedMin() {
	return !high;
}

bool ThresholdActivationFunction::setMax(double max) {
	if (max <= mn)
		return false;

	mx = max;
	return true;
}

double ThresholdActivationFunction::getMax() {
	return mx;
}

void ThresholdActivationFunction::biasMax() {
	high = true;
	return;
}

bool ThresholdActivationFunction::isBiasedMax() {
	return high;
}

