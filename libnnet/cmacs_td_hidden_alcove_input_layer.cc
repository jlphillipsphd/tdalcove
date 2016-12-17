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
#include "cmacs_td_hidden_alcove_input_layer.h"

CMACS_TD_HiddenALCOVEInputLayer::CMACS_TD_HiddenALCOVEInputLayer() : ALCOVEInputLayer(), CMACS_CoarseCodeLayerSet() {
	td_proj = NULL;
}

CMACS_TD_HiddenALCOVEInputLayer::~CMACS_TD_HiddenALCOVEInputLayer() {

	int x;

	if (td_proj != NULL) {
		for (x = 0; x < tiles; x++) {
			delete td_proj[x];
		}

		delete saf;
		delete td_proj;
		delete proj2;
		delete hidden;
	}
}

ActivationFunction* CMACS_TD_HiddenALCOVEInputLayer::getNewActFunc() {
	saf = new SimpleSigmoidActivationFunction();
	return saf;
}

CMACS_TD_HiddenALCOVEInputLayer::CMACS_TD_HiddenALCOVEInputLayer(int number_of_units,
		int number_of_tilings,
		int number_of_units_per_tile_dimension,
		double minimum_per_dimension,
		double maximum_per_dimension,
		int number_of_hidden_units,
		double td_learning_rate) : ALCOVEInputLayer(number_of_units), CMACS_CoarseCodeLayerSet(number_of_units, number_of_tilings, number_of_units_per_tile_dimension, getNewActFunc()) {

	if (number_of_units <= 0 || layers == NULL || number_of_hidden_units < 1) {
		td_proj = NULL;
		delete saf;
		return;
	}

	int x;
	lrate = td_learning_rate;
	hidden = new Layer(number_of_hidden_units, saf);
	td_proj = new FullForwardProjection*[tiles];
	proj2 = new FullForwardProjection(this, hidden, 0);

	for (x = 0; x < tiles; x++) {
		td_proj[x] = new FullForwardProjection(hidden, layers[x], 0);
	}
}

CMACS_TD_HiddenALCOVEInputLayer::CMACS_TD_HiddenALCOVEInputLayer(int number_of_units, double td_learning_rate) : ALCOVEInputLayer(number_of_units), CMACS_CoarseCodeLayerSet(number_of_units, 9, 5, getNewActFunc()) {
	if (number_of_units <= 0 || layers == NULL) {
		td_proj = NULL;
		delete saf;
		return;
	}

	int x;
	lrate = td_learning_rate;
	hidden = new Layer(11, saf);
	td_proj = new FullForwardProjection*[tiles];
	proj2 = new FullForwardProjection(this, hidden, 0);

	for (x = 0; x < tiles; x++) {
		td_proj[x] = new FullForwardProjection(hidden, layers[x], 0);
	}
}

bool CMACS_TD_HiddenALCOVEInputLayer::initializeTDWeights(RandomNumberGenerator* rng) {
	if (rng == NULL || td_proj == NULL)
		return false;

	proj2->initializeWeights(rng);
	for (int x = 0; x < tiles; x++)
		td_proj[x]->initializeWeights(rng);

	return true;
}

bool CMACS_TD_HiddenALCOVEInputLayer::computeAttentionalStrengths() {

	if (td_proj == NULL)
		return false;

	proj2->computeNets(false);
	hidden->computeActivations();
	for (int x = 0; x < tiles; x++) {
		td_proj[x]->computeNets(false);
		layers[x]->computeActivations();
	}

	getCoarseActs(as);
	setAttentionalStrengths(as);

	return true;

}

bool CMACS_TD_HiddenALCOVEInputLayer::adjustTDWeights(double reward) {

	if (td_proj == NULL)
		return false;

	double val[1];
	int x,y;

	for (int z = 0; z < tiles; z++) {
		layers[z]->getUnit(the_one[z])->getNets(as);
		as[0] = saf->Derivative(as[0]);
		double w_val = lrate * (reward - layers[z]->getUnit(the_one[z])->getAct()) * as[0];
		for (x = 0; x < hidden->Size(); x++) {
			hidden->getUnit(x)->getNets(val);
			val[0] = w_val * saf->Derivative(val[0]) * td_proj[z]->getWeight(x, the_one[z]);
			hidden->getUnit(x)->setDeltas(val);
			for (int x = 0; x < Size(); x++) {
				td_proj[z]->setWeight(x, the_one[z], td_proj[z]->getWeight(x, the_one[z]) + (w_val * hidden->getUnit(x)->getAct()));
			}
		}

		for (x = 0; x < Size(); x++) {
			for (y = 0; y < hidden->Size(); y++) {
				hidden->getUnit(y)->getDeltas(val);
				w_val = val[0] * getUnit(x)->getAct();
				proj2->setWeight(x,y,proj2->getWeight(x,y) + w_val);
			}
		}

	} // end for (z)

	return true;
}

bool CMACS_TD_HiddenALCOVEInputLayer::biasAroundPosition(double* pos) {

	if (pos == NULL || td_proj == NULL)
		return false;

	GaussianActivationFunction gauss(Size(), -0.05, 0.05);
	double val[1];

	for (int z = 0; z < tiles; z++)
		for (int x = 0; x < layers[z]->Size(); x++) {
			for (int y = 0; y < Size(); y++)
				as[y] = pos[y] - layers[z]->getUnit(x)->getPosition(y);

			for (int i = 0; i < Size(); i++)
				td_proj[z]->setWeight(i, x, gauss.Compute(as));

			val[0] = gauss.Compute(as);
			layers[z]->getUnit(x)->setBiases(val);

		} // end for (x)

	return true;
}
