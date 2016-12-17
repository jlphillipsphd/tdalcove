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
#include "integer_parameter.h"

IntegerParameter::IntegerParameter() : Parameter(Parameter::TYPE_INTEGER) {
	p_val = 0;
	min = 0;
	max = 1;
}

IntegerParameter::IntegerParameter(int initial_value, int min_value, int max_value) : Parameter(Parameter::TYPE_INTEGER) {
	p_val = initial_value;
	min = 0;
	max = 1;

	if (min_value < max_value) {
		min = min_value;
		max = max_value;
	}

	if (p_val < min)
		p_val = min;

	if (p_val > max)
		p_val = max;

}

int IntegerParameter::getValue() {
	return p_val;
}

bool IntegerParameter::setValue(int value) {
	if (value < min || value > max)
		return false;

	p_val = value;
	return true;
}

int IntegerParameter::getMin() {
	return min;
}

int IntegerParameter::getMax() {
	return max;
}

bool IntegerParameter::adjustValue(bool up) {

	int adj;

	if (up)
		adj = 1;
	else
		adj = -1;

	if (p_val == max && p_val + adj > max)
		return false;

	if (p_val == min && p_val + adj < min)
		return false;

	if (p_val + adj > max) {
		p_val = max;
		return true;
	}

	if (p_val + adj < min) {
		p_val = min;
		return true;
	}

	p_val = p_val + adj;
	return true;
}

bool IntegerParameter::adjustValue(int adj) {

	if (p_val == max && p_val + adj > max)
		return false;

	if (p_val == min && p_val + adj < min)
		return false;

	if (p_val + adj > max) {
		p_val = max;
		return true;
	}

	if (p_val + adj < min) {
		p_val = min;
		return true;
	}

	p_val = p_val + adj;
	return true;
}

