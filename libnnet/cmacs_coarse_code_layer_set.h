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
#ifndef CMACS_COARSE_CODE_LAYER_SET_H
#define CMACS_COARSE_CODE_LAYER_SET_H

// BE CAREFULL!!! when using sets.
// All layers can be used freely, but altering anything other
// than values (like position or input dimensionality) can lead
// to misbehavior. Also, DO NOT delete the individual layers.
// They will be tended to when the set class is deleted.

#include "cubic_coarse_code_layer.h"
#include "cubic_activation_function.h"
#include "activation_function.h"
#include "nnet_math.h"
#include <stddef.h>
#include <math.h>
#include <iostream>

using namespace std;

class CMACS_CoarseCodeLayerSet : public Object {

public:
	CMACS_CoarseCodeLayerSet();
	~CMACS_CoarseCodeLayerSet();
	CMACS_CoarseCodeLayerSet(int dimensionality,
							 int number_of_tilings,
							 int number_of_units_per_tile_dimension,
							 ActivationFunction* af);
	CMACS_CoarseCodeLayerSet(int dimensionality,
							 int number_of_tilings,
							 int number_of_units_per_tile_dimension,
							 double minimum_per_dimension,
							 double maximum_per_dimension,
							 ActivationFunction* af);
	CMACS_CoarseCodeLayerSet(int dimensionality,
							 int number_of_tilings,
							 int* number_of_units_per_tile_dimension,
							 double* minimums_per_dimension,
							 double* maximums_per_dimension,
							 ActivationFunction* af);

	bool getCoarseActs(double* pos);
	bool setCoarseActs(double* pos);

	int getNumberOfTilings();
	CubicCoarseCodeLayer* getTiling(int val);
	bool getWinningUnitIndexes(int* the_ones);

protected:
	CubicCoarseCodeLayer** layers;
	CubicActivationFunction* caf;
	int cmac_dim;
	int tiles;
	int* side_units;
	double* code_domain;
	double* code_step;
	double** a_pos;
	double** a_pos2;
	int* candidate;
	double* as;
	double* real_pos;
	int the_tile;
	int* the_one;
	double some_dist;
	int bound_corner, new_corner;
	double* layer_work_vec;
	int power_size;
	double* temp;
};

#endif
