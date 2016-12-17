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
#include "double_parameter.h"

DoubleParameter::DoubleParameter() : Parameter(Parameter::TYPE_DOUBLE) {
	p_val = 0.0;
	min = 0.0;
	max = 1.0;
}

DoubleParameter::DoubleParameter(double initial_value, double min_value, double max_value) : Parameter(Parameter::TYPE_DOUBLE) {
	p_val = initial_value;
	min = 0.0;
	max = 1.0;

	if (min_value < max_value) {
		min = min_value;
		max = max_value;
	}

	if (p_val < min)
		p_val = min;

	if (p_val > max)
		p_val = max;
}

double DoubleParameter::getValue() {
	return p_val;
}

bool DoubleParameter::setValue(double value) {
	if (value < min || value > max)
		return false;

	p_val = value;
	return true;
}

double DoubleParameter::getMin() {
	return min;
}

double DoubleParameter::getMax() {
	return max;
}

bool DoubleParameter::adjustValue(double adj) {

	if (p_val == max && adj + p_val > max) {
		return false;
	}
	if (p_val == min && adj + p_val < min) {
		return false;
	}
	if (adj + p_val > max) {
		p_val = max;
		return true;
	}
	if (adj + p_val < min) {
		p_val = min;
		return true;
	}
	
	p_val = p_val + adj;
	return true;
}
