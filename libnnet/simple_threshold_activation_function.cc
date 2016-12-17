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
#include "simple_threshold_activation_function.h"
#include "simple_activation_function.h"
#include <stddef.h>

SimpleThresholdActivationFunction::SimpleThresholdActivationFunction() : ThresholdActivationFunction(), SimpleActivationFunction() {
}

SimpleThresholdActivationFunction::SimpleThresholdActivationFunction(double min, double max) : ThresholdActivationFunction(1, min, max), SimpleActivationFunction() {
}

SimpleThresholdActivationFunction::SimpleThresholdActivationFunction(double min, double max, double threshold) : ThresholdActivationFunction(1, min, max, process(threshold)), SimpleActivationFunction() {
}

bool SimpleThresholdActivationFunction::setThreshold(double g) {
	return ((ThresholdActivationFunction*) this)->setThreshold(0, g);
}

double SimpleThresholdActivationFunction::getThreshold() {
	return ((ThresholdActivationFunction*) this)->getThreshold(0);
}

double* SimpleThresholdActivationFunction::process(double x) {
	work_vec[0] = x;
	return work_vec;
}

double SimpleThresholdActivationFunction::Compute(double x) {
    work_vec[0] = x;
	return ((ThresholdActivationFunction*) this)->Compute(work_vec);
}

double SimpleThresholdActivationFunction::Derivative(double x) {
	    work_vec[0] = x;
	    return ((ThresholdActivationFunction*) this)->Derivative(0, work_vec);
}

double SimpleThresholdActivationFunction::Compute(double x, double position) {
	    return Compute(x - position);
}

double SimpleThresholdActivationFunction::Derivative(double x, double position) {
	    return Derivative(x - position);
}
