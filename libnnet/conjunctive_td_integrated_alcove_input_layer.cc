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
#include "conjunctive_td_integrated_alcove_input_layer.h"

using namespace std;

ConjunctiveTD_IntegratedALCOVEInputLayer::ConjunctiveTD_IntegratedALCOVEInputLayer() : ALCOVEInputLayer() {
	td_layer = NULL;
	proj = NULL;
	saf = NULL;
	cc_func = NULL;
	td_work_vec = NULL;
	pos_work_vec = NULL;
}

ConjunctiveTD_IntegratedALCOVEInputLayer::~ConjunctiveTD_IntegratedALCOVEInputLayer() {
	if (td_layer != NULL)
		delete td_layer;

	if (proj != NULL)
		delete proj;

	if (cc_func != NULL)
		delete cc_func;

	if (saf != NULL)
		delete saf;

	if (td_work_vec != NULL)
		delete td_work_vec;

	if (pos_work_vec != NULL)
		delete pos_work_vec;
}

ConjunctiveTD_IntegratedALCOVEInputLayer::ConjunctiveTD_IntegratedALCOVEInputLayer(int number_of_units, double td_learning_rate) : ALCOVEInputLayer(number_of_units) {

	if (number_of_units <= 0) {
		td_layer = NULL;
		proj = NULL;
		saf = NULL;
		cc_func = NULL;
		td_work_vec = NULL;
		pos_work_vec = NULL;
		return;
	}

	int x;

	double code_min = 0.01;
	double code_max = 1.0;
	int td_side_units = 15;

	double* code_mins = new double[number_of_units];
	double* code_maxs = new double[number_of_units];
	int* units_per_dim = new int[number_of_units];

	for (x = 0; x < number_of_units; x++) {
		code_mins[x] = code_min;
		code_maxs[x] = code_max;
		units_per_dim[x] = td_side_units;
	}

	saf = new SimpleSigmoidActivationFunction();
	cc_func = new GaussianActivationFunction(number_of_units);
	td_layer = new CoarseCodeLayer(cc_func, units_per_dim, code_mins, code_maxs, saf);

	proj = new FullForwardProjection(this, td_layer, 0);
	td_work_vec = new double[td_layer->Size()];
	pos_work_vec = new double[number_of_units];

	delete code_mins;
	delete code_maxs;
	delete units_per_dim;

	the_one = 0;
	lrate = td_learning_rate;
}

ConjunctiveTD_IntegratedALCOVEInputLayer::ConjunctiveTD_IntegratedALCOVEInputLayer(int number_of_units, int number_of_units_per_dimension, double minimum_per_dimension, double maximum_per_dimension, double td_learning_rate) : ALCOVEInputLayer(number_of_units) {

	if (number_of_units <= 0 || minimum_per_dimension >= maximum_per_dimension || number_of_units_per_dimension <= 0) {
		td_layer = NULL;
		proj = NULL;
		saf = NULL;
		cc_func = NULL;
		td_work_vec = NULL;
		pos_work_vec = NULL;
		return;
	}

	int x;

	double code_min = minimum_per_dimension;
	double code_max = maximum_per_dimension;
	int td_side_units = number_of_units_per_dimension;

	double* code_mins = new double[number_of_units];
	double* code_maxs = new double[number_of_units];
	int* units_per_dim = new int[number_of_units];

	for (x = 0; x < number_of_units; x++) {
		code_mins[x] = code_min;
		code_maxs[x] = code_max;
		units_per_dim[x] = td_side_units;
	}

	saf = new SimpleSigmoidActivationFunction();
	cc_func = new GaussianActivationFunction(number_of_units);
	td_layer = new CoarseCodeLayer(cc_func, units_per_dim, code_mins, code_maxs, saf);
	proj = new FullForwardProjection(this, td_layer, 0);
	td_work_vec = new double[td_layer->Size()];
	pos_work_vec = new double[number_of_units];

	delete code_mins;
	delete code_maxs;
	delete units_per_dim;

	the_one = 0;
	lrate = td_learning_rate;
}

bool ConjunctiveTD_IntegratedALCOVEInputLayer::initializeTDWeights(RandomNumberGenerator* rng) {
	return proj->initializeWeights(rng);
}

bool ConjunctiveTD_IntegratedALCOVEInputLayer::computeAttentionalStrengths() {

	if (td_layer == NULL)
		return false;

	proj->computeNets();
	td_layer->computeActivations();
	td_layer->applySoftWTA(0.0);
	td_layer->getActs(td_work_vec);

	for (int z = 0; z < td_layer->Size(); z++)
		if (td_work_vec[z] > 0.0)
			the_one = z;

	td_layer->getUnit(the_one)->getPosition(pos_work_vec);
	setAttentionalStrengths(pos_work_vec);

	return true;

}

bool ConjunctiveTD_IntegratedALCOVEInputLayer::adjustTDWeights(double reward) {

	if (td_layer == NULL)
		return false;

	double val[1];
	td_layer->getUnit(the_one)->getNets(pos_work_vec);
	pos_work_vec[0] = saf->Derivative(pos_work_vec[0]);
	double w_val = lrate * (reward - td_layer->getUnit(the_one)->getAct()) * pos_work_vec[0];
	for (int x = 0; x < Size(); x++)
		proj->setWeight(x, the_one, proj->getWeight(x, the_one) + (w_val * getUnit(x)->getAct()));

	td_layer->getUnit(the_one)->getBiases(val);
	val[0] += w_val;
	td_layer->getUnit(the_one)->setBiases(val);

	return true;
}

bool ConjunctiveTD_IntegratedALCOVEInputLayer::biasAroundPosition(double* pos) {
	if (pos == NULL || td_layer == NULL)
		return false;

	double val[1];
	GaussianActivationFunction gauss(Size(), -0.05, 0.05);

	for (int x = 0; x < td_layer->Size(); x++) {
		for (int y = 0; y < Size(); y++)
			pos_work_vec[y] = pos[y] - td_layer->getUnit(x)->getPosition(y);

			for (int z = 0; z < Size(); z++)
				proj->setWeight(z, x, gauss.Compute(pos_work_vec));

			val[0] = gauss.Compute(pos_work_vec);
			td_layer->getUnit(x)->setBiases(val);
	}

	return true;
}
