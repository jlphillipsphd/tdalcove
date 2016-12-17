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
#include "sigmoid_activation_function.h"
#include <stddef.h>
#include <math.h>

SigmoidActivationFunction::SigmoidActivationFunction() : ActivationFunction() {
	gains = new double[1];
	gains[0] = 1.0;
	scale = 1.0;
	offset = 0.0;
	mn = 0.0;
	mx = 1.0;
}

SigmoidActivationFunction::~SigmoidActivationFunction() {
	delete gains;
}

SigmoidActivationFunction::SigmoidActivationFunction(int number_of_input_dimensions) : ActivationFunction(number_of_input_dimensions) {
	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;
	scale = 1.0;
	offset = 0.0;
	mn = 0.0;
	mx = 1.0;
}

SigmoidActivationFunction::SigmoidActivationFunction(int number_of_input_dimensions, double min, double max) : ActivationFunction(number_of_input_dimensions) {

	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	if (max <= min) {
		scale = 1.0;
		offset = 0.0;
		mn = 0.0;
		mx = 1.0;
		return;
	}

	scale = max - min;
	offset = min;
	mn = min;
	mx = max;
}

SigmoidActivationFunction::SigmoidActivationFunction(int number_of_input_dimensions, double min, double max, double* sigmoid_gains) : ActivationFunction(number_of_input_dimensions) {

	int x;
	gains = new double[dims];
	for (x = 0; x < dims; x++)
		gains[x] = 1.0;

	if (sigmoid_gains != NULL)
		for (x = 0; x < dims; x++)
			gains[x] = sigmoid_gains[x];

	if (max <= min) {
		scale = 1.0;
		offset = 0.0;
		mn = 0.0;
		mx = 1.0;
		return;
	}

	scale = max - min;
	offset = min;
	mn = min;
	mx = max;
}


double SigmoidActivationFunction::PreCompute(double* g, double* v) {
	int x;
	double value = 1.0;

	for (x = 0; x < dims; x++)
		value += exp(-1.0 * g[x] * v[x]);

	return value;
}

double SigmoidActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	return ((scale * (1.0 / PreCompute(gains, variables))) + offset);
}

double SigmoidActivationFunction::Derivative(int dimension, double* variables) {
	if (dimension < 0 || dimension >= dims || variables == NULL)
		return 0.0;

	return scale * ((gains[dimension] * exp(-1.0 * gains[dimension] * variables[dimension])) / pow(PreCompute(gains, variables), 2.0));
}

double SigmoidActivationFunction::Derivative(double* variables) {
	if (variables == NULL)
		return 0.0;

	double value = 0.0;

	for (int x = 0; x < dims; x++)
		value += (gains[x] * exp(-1.0 * gains[x] * variables[x]));

	return scale * (value / pow(PreCompute(gains, variables), 2.0));
}

bool SigmoidActivationFunction::setGain(int dim, double g) {
	if (dim < 0 || dim >= dims)
		return false;

	gains[dim] = g;
	return true;
}

double SigmoidActivationFunction::getGain(int dim) {
	if (dim < 0 || dim >= dims)
		return 0.0;

	return gains[dim];
}

bool SigmoidActivationFunction::setGains(double* g) {
	if (g == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		gains[x] = g[x];

	return true;
}

bool SigmoidActivationFunction::getGains(double* g) {
	if (g == NULL)
		return false;

	for (int x = 0; x < dims; x++)
		g[x] = gains[x];

	return true;
}

bool SigmoidActivationFunction::setMin(double min) {
	if (min >= mx)
		return false;

	mn = min;
	scale = mx - mn;
	offset = mn;
	return true;
}

double SigmoidActivationFunction::getMin() {
	return mn;
}

bool SigmoidActivationFunction::setMax(double max) {
	if (max <= mn)
		return false;

	mx = max;
	scale = mx - mn;
	return true;
}

double SigmoidActivationFunction::getMax() {
	return mx;
}

