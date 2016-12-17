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
#include "simple_noisy_sigmoid_activation_function.h"

SimpleNoisySigmoidActivationFunction::SimpleNoisySigmoidActivationFunction() : SimpleActivationFunction(), NoisySigmoidActivationFunction() {
}

SimpleNoisySigmoidActivationFunction::SimpleNoisySigmoidActivationFunction(double min, double max, RandomNumberGenerator* rand) : SimpleActivationFunction(), NoisySigmoidActivationFunction(1, min, max, rand) {
}

SimpleNoisySigmoidActivationFunction::SimpleNoisySigmoidActivationFunction(double min, double max, double sigmoid_gain, RandomNumberGenerator* rand) : SimpleActivationFunction(), NoisySigmoidActivationFunction(1, min, max, process(sigmoid_gain), rand) {
}

bool SimpleNoisySigmoidActivationFunction::setGain(double g) {
	return ((SigmoidActivationFunction*) this)->setGain(0, g);
}

double SimpleNoisySigmoidActivationFunction::getGain() {
	return ((SigmoidActivationFunction*) this)->getGain(0);
}

double* SimpleNoisySigmoidActivationFunction::process(double x) {
	work_vec[0] = x;
	return work_vec;
}

double SimpleNoisySigmoidActivationFunction::Compute(double x) {
    work_vec[0] = x;
	return ((SigmoidActivationFunction*) this)->Compute(work_vec);
}

double SimpleNoisySigmoidActivationFunction::Derivative(double x) {
	    work_vec[0] = x;
	    return ((SigmoidActivationFunction*) this)->Derivative(0, work_vec);
}

double SimpleNoisySigmoidActivationFunction::Compute(double x, double position) {
	    return Compute(x - position);
}

double SimpleNoisySigmoidActivationFunction::Derivative(double x, double position) {
	    return Derivative(x - position);
}
