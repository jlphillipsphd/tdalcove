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
#include "noisy_gaussian_activation_function.h"
#include "simple_noisy_gaussian_activation_function.h"
#include "simple_activation_function.h"
#include <stddef.h>

SimpleNoisyGaussianActivationFunction::SimpleNoisyGaussianActivationFunction() : NoisyGaussianActivationFunction(), SimpleActivationFunction() {
}

SimpleNoisyGaussianActivationFunction::SimpleNoisyGaussianActivationFunction(double min, double max, RandomNumberGenerator* rand) : NoisyGaussianActivationFunction(1, min, max, rand), SimpleActivationFunction() {
}

SimpleNoisyGaussianActivationFunction::SimpleNoisyGaussianActivationFunction(double min, double max, double gaussian_gain, RandomNumberGenerator* rand) : NoisyGaussianActivationFunction(1, min, max, process(gaussian_gain), rand), SimpleActivationFunction() {
}

bool SimpleNoisyGaussianActivationFunction::setGain(double g) {
	return ((GaussianActivationFunction*) this)->setGain(0, g);
}

double SimpleNoisyGaussianActivationFunction::getGain() {
	return ((GaussianActivationFunction*) this)->getGain(0);
}

double* SimpleNoisyGaussianActivationFunction::process(double x) {
	work_vec[0] = x;
	return work_vec;
}

double SimpleNoisyGaussianActivationFunction::Compute(double x) {
    work_vec[0] = x;
	return ((GaussianActivationFunction*) this)->Compute(work_vec);
}

double SimpleNoisyGaussianActivationFunction::Derivative(double x) {
	    work_vec[0] = x;
	    return ((GaussianActivationFunction*) this)->Derivative(0, work_vec);
}

double SimpleNoisyGaussianActivationFunction::Compute(double x, double position) {
	    return Compute(x - position);
}

double SimpleNoisyGaussianActivationFunction::Derivative(double x, double position) {
	    return Derivative(x - position);
}
