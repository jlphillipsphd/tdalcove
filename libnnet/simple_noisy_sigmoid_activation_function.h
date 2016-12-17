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
#ifndef SIMPLE_NOISY_SIGMOID_ACTIVATION_FUNCTION_H
#define SIMPLE_NOISY_SIGMOID_ACTIVATION_FUNCTION_H

#include "simple_activation_function.h"
#include "noisy_sigmoid_activation_function.h"
#include "random_number_generator.h"
#include <stddef.h>

class SimpleNoisySigmoidActivationFunction : public SimpleActivationFunction, public NoisySigmoidActivationFunction {

public:
	SimpleNoisySigmoidActivationFunction();
	SimpleNoisySigmoidActivationFunction(double min, double max, RandomNumberGenerator* rand);
	SimpleNoisySigmoidActivationFunction(double min, double max, double sigmoid_gain, RandomNumberGenerator* rand);

	double Compute(double x);
	double Derivative(double x);

	double Compute(double x, double position);
	double Derivative(double x, double position);

	bool setGain(double g);
	double getGain();

private:
	double* process(double x);

};

#endif
