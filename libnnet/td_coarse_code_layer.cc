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
#include "td_coarse_code_layer.h"
#include <stddef.h>
#include <math.h>
#include <iostream>

using namespace std;

TDCoarseCodeLayer::TDCoarseCodeLayer() : TDLayer() {
	cc_func = NULL;
	cc_work_vec = NULL;
	cc_work_vec2 = NULL;
}

TDCoarseCodeLayer::~TDCoarseCodeLayer() {

	if (cc_work_vec != NULL)
		delete cc_work_vec;
	if (cc_work_vec2 != NULL)
		delete cc_work_vec2;
}

TDCoarseCodeLayer::TDCoarseCodeLayer(ActivationFunction* coarse_code_function,
			            int* number_of_units_per_dimension,
				        double* minimums_per_dimension,
				        double* maximums_per_dimension,
				        ActivationFunction* af, double discount_rate) : 
						TDLayer(computeUnits(coarse_code_function,
							number_of_units_per_dimension), af, discount_rate) {

	if (coarse_code_function == NULL || number_of_units_per_dimension == NULL || minimums_per_dimension == NULL || maximums_per_dimension == NULL) {
		cc_func = NULL;
		cc_work_vec = NULL;
		cc_work_vec2 = NULL;
		return;
	}

	cc_func = coarse_code_function;
	int number_of_positional_dimensions = coarse_code_function->getNumberOfInputDimensions();
	int x, y, z;
	double step[number_of_positional_dimensions];
	int* pos = new int[number_of_positional_dimensions];
	double* position = new double[number_of_positional_dimensions];

	cc_work_vec = new double[number_of_positional_dimensions];
	cc_work_vec2 = new double[number_of_positional_dimensions];

	for (x = 0; x < num_units; x++) {
		getUnit(x)->setPositionDimensions(number_of_positional_dimensions);
	}

	for (x = 0; x < number_of_positional_dimensions; x++) {
		step[x] = computeStep(number_of_units_per_dimension[x], minimums_per_dimension[x], maximums_per_dimension[x]);
	}

	for (x = 0; x < num_units; x++) {
		codeLocation(number_of_positional_dimensions,
				number_of_units_per_dimension, pos, x);
		for (y = 0; y < number_of_positional_dimensions; y++) {
			position[y] = minimums_per_dimension[y] + (pos[y] * step[y]);
		}
		getUnit(x)->setPosition(position);
	}

	delete position;
	delete pos;
}


int TDCoarseCodeLayer::computeUnits(ActivationFunction* af, int* vals) {
	if (vals == NULL)
		return 0;

	int value = 1;

	for (int x = 0; x < af->getNumberOfInputDimensions(); x++)
		value *= vals[x];

	return value;
}

double TDCoarseCodeLayer::computeStep(int n, double min, double max) {
	double scale = fabs(max - min);
	return (scale / ((double) (n-1)));
}

bool TDCoarseCodeLayer::setCoarseActs(double* position) {
	if (cc_func == NULL)
		return false;

	for (int x = 0; x < num_units; x++) {
		getUnit(x)->getPosition(cc_work_vec);
		getUnit(x)->setAct(cc_func->Compute(cc_work_vec, position));
	}

	return true;
}

bool TDCoarseCodeLayer::setCoarseTargets(double* position) {
	if (cc_func == NULL)
		return false;

	for (int x = 0; x < num_units; x++) {
		getUnit(x)->getPosition(cc_work_vec);
		getUnit(x)->setTarget(cc_func->Compute(cc_work_vec, position));
	}

	return true;
}

bool TDCoarseCodeLayer::setCoarseNets(double* position) {
	if (cc_func == NULL || in_dims != 1)
		return false;

	for (int x = 0; x < num_units; x++) {
		getUnit(x)->getPosition(cc_work_vec);
		cc_work_vec2[0] = cc_func->Compute(cc_work_vec, position);
		getUnit(x)->setNets(cc_work_vec2);
	}

	return true;
}

void TDCoarseCodeLayer::codeLocation(int num_dims, int* dims, int* pos, int index) {
	for (int x = 0; x < num_dims; x++) {
		int division = 1;
		for (int dim = 0; dim < x; dim++) {
			division *= dims[dim];
		} // end for

		pos[x] = (index / division) % dims[x];
	} // end for
};
