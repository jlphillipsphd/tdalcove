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
#include "cmacs_td_alcove_input_layer.h"

CMACS_TD_ALCOVEInputLayer::CMACS_TD_ALCOVEInputLayer() : ALCOVEInputLayer(), CMACS_CoarseCodeLayerSet() {
	bias_layer = NULL;
}

CMACS_TD_ALCOVEInputLayer::~CMACS_TD_ALCOVEInputLayer() {

	int x;

	if (bias_layer != NULL) {
		for (x = 0; x < tiles; x++) {
			delete td_proj[x];
		}

		delete saf;
		delete bias_layer;
		delete td_proj;
	}
}

ActivationFunction* CMACS_TD_ALCOVEInputLayer::getNewActFunc() {
	saf = new SimpleSigmoidActivationFunction();
	return saf;
}

CMACS_TD_ALCOVEInputLayer::CMACS_TD_ALCOVEInputLayer(int number_of_units,
		int number_of_tilings,
		int number_of_units_per_tile_dimension,
		double minimum_per_dimension,
		double maximum_per_dimension,
		double td_learning_rate) : ALCOVEInputLayer(number_of_units), CMACS_CoarseCodeLayerSet(number_of_units, number_of_tilings, number_of_units_per_tile_dimension, minimum_per_dimension, maximum_per_dimension, getNewActFunc()) {

	if (number_of_units <= 0 || layers == NULL) {
		bias_layer = NULL;
		delete saf;
		return;
	}

	int x;
	lrate = td_learning_rate;
	bias_layer = new Layer(1, saf);
	bias_layer->getUnit(0)->setAct(1.0);
	td_proj = new FullForwardProjection*[tiles];

	for (x = 0; x < tiles; x++) {
		td_proj[x] = new FullForwardProjection(bias_layer, layers[x], 0);
	}
}

CMACS_TD_ALCOVEInputLayer::CMACS_TD_ALCOVEInputLayer(int number_of_units, double td_learning_rate) : ALCOVEInputLayer(number_of_units), CMACS_CoarseCodeLayerSet(number_of_units, 9, 5, getNewActFunc()) {
	if (number_of_units <= 0 || layers == NULL) {
		bias_layer = NULL;
		delete saf;
		return;
	}

	int x;
	lrate = td_learning_rate;
	bias_layer = new Layer(1, saf);
	bias_layer->getUnit(0)->setAct(1.0);
	td_proj = new FullForwardProjection*[tiles];

	for (x = 0; x < tiles; x++) {
		td_proj[x] = new FullForwardProjection(bias_layer, layers[x], 0);
	}
}

bool CMACS_TD_ALCOVEInputLayer::initializeTDWeights(RandomNumberGenerator* rng) {
	if (rng == NULL || bias_layer == NULL)
		return false;

	for (int x = 0; x < tiles; x++)
		td_proj[x]->initializeWeights(rng);

	return true;
}

bool CMACS_TD_ALCOVEInputLayer::computeAttentionalStrengths() {

	if (bias_layer == NULL)
		return false;

	for (int x = 0; x < tiles; x++) {
		td_proj[x]->computeNets(false);
		layers[x]->computeActivations();
	}

	getCoarseActs(as);
	setAttentionalStrengths(as);

	return true;

}

bool CMACS_TD_ALCOVEInputLayer::adjustTDWeights(double reward) {

	if (bias_layer == NULL)
		return false;

	for (int z = 0; z < tiles; z++) {
		layers[z]->getUnit(the_one[z])->getNets(as);
		as[0] = saf->Derivative(as[0]);
		double w_val = lrate * (reward - layers[z]->getUnit(the_one[z])->getAct()) * as[0];
		td_proj[z]->setWeight(0, the_one[z], td_proj[z]->getWeight(0, the_one[z]) + w_val);
	} // end for (z)

	return true;
}

bool CMACS_TD_ALCOVEInputLayer::biasAroundPosition(double* pos, double spread, RandomNumberGenerator* noise_generator) {

	if (pos == NULL || bias_layer == NULL)
		return false;

	double spreads[Size()];
	for (int i = 0; i < Size(); i++)
		spreads[i] = spread;

	NoisyGaussianActivationFunction gauss(Size(), -0.05, 0.05, spreads, noise_generator);

	for (int z = 0; z < tiles; z++)
		for (int x = 0; x < layers[z]->Size(); x++) {
			for (int y = 0; y < Size(); y++)
				as[y] = pos[y] - layers[z]->getUnit(x)->getPosition(y);

			td_proj[z]->setWeight(0, x, gauss.Compute(as));
		} // end for (x)

	return true;
}
