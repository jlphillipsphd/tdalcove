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
#include "noisy_sigmoid_activation_function.h"

NoisySigmoidActivationFunction::NoisySigmoidActivationFunction() : SigmoidActivationFunction() {
	rand = NULL;
}

NoisySigmoidActivationFunction::NoisySigmoidActivationFunction(int number_of_input_dimensions, RandomNumberGenerator* random_noise) : SigmoidActivationFunction(number_of_input_dimensions) {
	rand = random_noise;
}

NoisySigmoidActivationFunction::NoisySigmoidActivationFunction(int number_of_input_dimensions, double min, double max, RandomNumberGenerator* random_noise) : SigmoidActivationFunction(number_of_input_dimensions, min, max) {
	rand = random_noise;
}

NoisySigmoidActivationFunction::NoisySigmoidActivationFunction(int number_of_input_dimensions, double min, double max, double* sigmoid_gains, RandomNumberGenerator* random_noise) : SigmoidActivationFunction(number_of_input_dimensions, min, max, sigmoid_gains) {
	rand = random_noise;
}


double NoisySigmoidActivationFunction::Compute(double* variables) {
	if (variables == NULL)
		return 0.0;

	double value = ((scale * (1.0 / PreCompute(gains, variables))) + offset);

	if (rand == NULL)
		return value;

	return clip(value + rand->Number(), getMin(), getMax());
}


bool NoisySigmoidActivationFunction::setRandomNoise(RandomNumberGenerator* r_noise) {
	rand = r_noise;
	return true;
}
