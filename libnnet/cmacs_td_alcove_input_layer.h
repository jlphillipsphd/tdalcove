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
#ifndef CMACS_TD_ALCOVE_INPUT_LAYER_H
#define CMACS_TD_ALCOVE_INPUT_LAYER_H

#include "alcove_input_layer.h"
#include "simple_sigmoid_activation_function.h"
#include "gaussian_activation_function.h"
#include "layer.h"
#include "full_forward_projection.h"
#include "cmacs_coarse_code_layer_set.h"
#include "random_number_generator.h"
#include "noisy_gaussian_activation_function.h"
#include <stddef.h>
#include <math.h>
#include <iostream>

using namespace std;

class CMACS_TD_ALCOVEInputLayer : public ALCOVEInputLayer, CMACS_CoarseCodeLayerSet {

public:
	CMACS_TD_ALCOVEInputLayer();
	~CMACS_TD_ALCOVEInputLayer();
	CMACS_TD_ALCOVEInputLayer(int number_of_units, double td_learning_rate);
	CMACS_TD_ALCOVEInputLayer(int number_of_units,
			int number_of_tilings,
			int number_of_units_per_tile_dimension,
			double minimum_per_dimension,
			double maximum_per_dimension,
			double td_learning_rate);

	bool initializeTDWeights(RandomNumberGenerator* rng);
	bool computeAttentionalStrengths();
	bool adjustTDWeights(double reward);
	bool biasAroundPosition(double* pos, double spread, RandomNumberGenerator* noise_generator);

private:
	ActivationFunction* getNewActFunc();

	Layer* bias_layer;
	FullForwardProjection** td_proj;
	SimpleSigmoidActivationFunction* saf;
	double lrate;
};

#endif
