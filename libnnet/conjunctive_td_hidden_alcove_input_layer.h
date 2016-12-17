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
#ifndef CONJUNCTIVE_TD_HIDDEN_ALCOVE_INPUT_LAYER_H
#define CONJUNCTIVE_TD_HIDDEN_ALCOVE_INPUT_LAYER_H

#include "alcove_input_layer.h"
#include "coarse_code_layer.h"
#include "activation_function.h"
#include "simple_sigmoid_activation_function.h"
#include "layer.h"
#include "gaussian_activation_function.h"
#include "full_forward_projection.h"
#include <stddef.h>
#include <iostream>

using namespace std;

class ConjunctiveTD_HiddenALCOVEInputLayer : public ALCOVEInputLayer {

public:
	ConjunctiveTD_HiddenALCOVEInputLayer();
	~ConjunctiveTD_HiddenALCOVEInputLayer();
	ConjunctiveTD_HiddenALCOVEInputLayer(int number_of_units, double td_learning_rate);
	ConjunctiveTD_HiddenALCOVEInputLayer(int number_of_units,
			int number_of_units_per_dimension,
			double minimum_per_dimension,
			double maximum_per_dimension,
			int hidden_layer_size,
			double td_learning_rate);

	bool initializeTDWeights(RandomNumberGenerator* rng);
	bool computeAttentionalStrengths();
	bool adjustTDWeights(double reward);
	bool biasAroundPosition(double* pos);

private:
	CoarseCodeLayer* td_layer;
	FullForwardProjection* proj;
	FullForwardProjection* proj2;
	Layer* hidden;
	SimpleSigmoidActivationFunction* saf;
	ActivationFunction* cc_func;
	double lrate;
	double* td_work_vec;
	int the_one;
	double* pos_work_vec;
};

#endif
