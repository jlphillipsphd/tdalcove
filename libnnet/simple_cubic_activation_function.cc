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
#include "simple_cubic_activation_function.h"
#include "simple_activation_function.h"
#include <stddef.h>

SimpleCubicActivationFunction::SimpleCubicActivationFunction() : CubicActivationFunction(), SimpleActivationFunction() {
}

SimpleCubicActivationFunction::SimpleCubicActivationFunction(double min, double max) : CubicActivationFunction(1, min, max), SimpleActivationFunction() {
}

SimpleCubicActivationFunction::SimpleCubicActivationFunction(double min, double max, double domain) : CubicActivationFunction(1, min, max, process(domain)), SimpleActivationFunction() {
}

bool SimpleCubicActivationFunction::setDomain(double d) {
	return ((CubicActivationFunction*) this)->setDomain(0, d);
}

double SimpleCubicActivationFunction::getDomain() {
	return ((CubicActivationFunction*) this)->getDomain(0);
}

double* SimpleCubicActivationFunction::process(double x) {
	work_vec[0] = x;
	return work_vec;
}

double SimpleCubicActivationFunction::Compute(double x) {
    work_vec[0] = x;
	return ((CubicActivationFunction*) this)->Compute(work_vec);
}

double SimpleCubicActivationFunction::Derivative(double x) {
	    work_vec[0] = x;
	    return ((CubicActivationFunction*) this)->Derivative(0, work_vec);
}

double SimpleCubicActivationFunction::Compute(double x, double position) {
	    return Compute(x - position);
}

double SimpleCubicActivationFunction::Derivative(double x, double position) {
	    return Derivative(x - position);
}
