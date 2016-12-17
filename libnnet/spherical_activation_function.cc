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
#include "spherical_activation_function.h"
#include <stddef.h>
#include <math.h>

SphericalActivationFunction::SphericalActivationFunction() : ActivationFunction() {
	radius = 1.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

SphericalActivationFunction::SphericalActivationFunction(int number_of_input_dimensions) : ActivationFunction(number_of_input_dimensions) {
	radius = 1.0;
	mn = 0.0;
	mx = 1.0;
	high = true;
}

SphericalActivationFunction::SphericalActivationFunction(int number_of_input_dimensions, double min, double max) : ActivationFunction(number_of_input_dimensions) {
	radius = 1.0;
	high = true;

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}

SphericalActivationFunction::SphericalActivationFunction(int number_of_input_dimensions, double min, double max, double s_radius) : ActivationFunction(number_of_input_dimensions) {

	high = true;
	s_radius = 1.0;

	if (s_radius > 0.0)
		radius = s_radius;

	if (max <= min) {
		mn = 0.0;
		mx = 1.0;
		return;
	}

	mn = min;
	mx = max;
}

double SphericalActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	int x;
	double value = mx;
	double dist = 0.0;

	if (high) {
		for (x = 0; x < dims; x++)
			dist += variables[x]*variables[x];
		dist = sqrt(dist);

		if (dist > radius)
			value = mn;
	}
	else {
		for (x = 0; x < dims; x++)
			dist += variables[x]*variables[x];
		dist = sqrt(dist);

		if (dist >= radius)
			value = mn;
	}

	return value;
}

double SphericalActivationFunction::Derivative(int dimension, double* variables) {
	return 0.0;
}

double SphericalActivationFunction::Derivative(double* variables) {
	return 0.0;
}

bool SphericalActivationFunction::setRadius(double r) {
	if (r <= 0.0)
		return false;

	radius = r;
	return true;
}

double SphericalActivationFunction::getRadius() {
	return radius;
}

bool SphericalActivationFunction::setMin(double min) {
	if (min >= mx)
		return false;

	mn = min;
	return true;
}

double SphericalActivationFunction::getMin() {
	return mn;
}

void SphericalActivationFunction::biasMin() {
	high = false;
	return;
}

bool SphericalActivationFunction::isBiasedMin() {
	return !high;
}

bool SphericalActivationFunction::setMax(double max) {
	if (max <= mn)
		return false;

	mx = max;
	return true;
}

double SphericalActivationFunction::getMax() {
	return mx;
}

void SphericalActivationFunction::biasMax() {
	high = true;
	return;
}

bool SphericalActivationFunction::isBiasedMax() {
	return high;
}

