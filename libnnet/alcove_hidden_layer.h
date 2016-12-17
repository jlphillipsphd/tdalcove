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
#ifndef ALCOVE_HIDDEN_LAYER_H
#define ALCOVE_HIDDEN_LAYER_H

#include "alcove_input_layer.h"
#include "linear_activation_function.h"
#include "layer.h"
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

class ALCOVEHiddenLayer : public Layer {

private:
	double** node_pos;
	double* specificity;
	ALCOVEInputLayer* in_layer;
	double* some_vec;
	double* local;
	double learning_rate;
	LinearActivationFunction* laf;
	LinearActivationFunction* createAct();

public:
	ALCOVEHiddenLayer();
	~ALCOVEHiddenLayer();
	ALCOVEHiddenLayer(int number_of_units, ALCOVEInputLayer* ail, double* min_positions, double* max_positions); // COVEring map constructor
	ALCOVEHiddenLayer(int number_of_units, ALCOVEInputLayer* ail, double** hidden_node_positions);								// Exemplar constructor

	bool computeActivations();
	bool computeActivations(double q, double r);

	bool setHiddenUnitPositions(double** positions);

	bool setSpecificities(double* vals);
	bool getSpecificities(double* vals);

	bool setSpecificity(int pos, double val);
	double getSpecificity(int pos);
	bool setSpecificity(double val);

	bool setLearningRate(double val);
	double getLearningRate();

	bool adjustDeltaAttentionalStrengths();

        // TD-learning hook
        bool setDeltaAttentionalStrengths(double td_error);
  
};

#endif
