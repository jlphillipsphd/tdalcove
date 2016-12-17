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
#include "alcove_hidden_layer.h"

ALCOVEHiddenLayer::ALCOVEHiddenLayer() : Layer() {
	node_pos = NULL;
	specificity = NULL;
	some_vec = NULL;
	local = NULL;
	learning_rate = 0.0;
	laf = NULL;
}

ALCOVEHiddenLayer::~ALCOVEHiddenLayer() {
	if (node_pos != NULL) {
		for (int x = 0; x < Size(); x++)
			delete node_pos[x];

		delete node_pos;
	}

	if (specificity != NULL)
		delete specificity;

	if (some_vec != NULL)
		delete some_vec;

	if (local != NULL)
		delete local;

	if (laf != NULL)
		delete laf;
}

ALCOVEHiddenLayer::ALCOVEHiddenLayer(int number_of_units, ALCOVEInputLayer* ail, double** hidden_node_positions) : Layer(number_of_units, createAct()) {
	if (number_of_units < 1 || hidden_node_positions == NULL || ail == NULL || ail->Size() < 1) {
		node_pos = NULL;
		specificity = NULL;
		some_vec = NULL;
		local = NULL;
		learning_rate = 0.0;
		return;
	}

	in_layer = ail;
	specificity = new double[number_of_units];
	local = new double[number_of_units];
	node_pos = new double*[number_of_units];
	some_vec = new double[in_layer->Size()];
	learning_rate = 0.01;

	for (int x = 0; x < number_of_units; x++) {
		node_pos[x] = new double[in_layer->Size()];
		specificity[x] = 1.0;
		for (int y = 0; y < in_layer->Size(); y++) {
			node_pos[x][y] = hidden_node_positions[x][y];
		}
	}

}

ALCOVEHiddenLayer::ALCOVEHiddenLayer(int number_of_units, ALCOVEInputLayer* ail, double* min_positions, double* max_positions) : Layer(number_of_units, createAct()) {

	if (ail == NULL || number_of_units < 1 || min_positions == NULL, max_positions == NULL) {
		node_pos = NULL;
		specificity = NULL;
		some_vec = NULL;
		learning_rate = 0.0;
		return;
	}

	in_layer = ail;
	some_vec = new double[in_layer->Size()];
	specificity = new double[Size()];
	local = new double[number_of_units];
	node_pos = new double*[Size()];
	learning_rate = 0.01;

	int x,y,z;

	for (x = 0; x < Size(); x++) {
		node_pos[x] = new double[in_layer->Size()];
		specificity[x] = 1.0;
		for (y = 0; y < in_layer->Size(); y++) {
			double scale = max_positions[y] - min_positions[y];
			double val = drand48();
			val *= scale;
			val += min_positions[y];
			node_pos[x][y] = val;
		}
	}
}

bool ALCOVEHiddenLayer::computeActivations() {
	return computeActivations(1.0, 1.0);
}

bool ALCOVEHiddenLayer::computeActivations(double q, double r) {
	if (node_pos == NULL || specificity == NULL)
		return false;

	in_layer->getActs(some_vec);

	for (int x = 0; x < Size(); x++) {
		double val = 0.0;

		for (int y = 0; y < in_layer->Size(); y++)
			val += in_layer->getAttentionalStrength(y) * pow(fabs(node_pos[x][y] - some_vec[y]), r);

		if (val < 0.0)
			cerr << "Nan here!" << endl;

		val = pow(val, q / r);
		val = exp(-specificity[x] * val);

		local[x] = val;
	}

	setActs(local);

	return true;
}

bool ALCOVEHiddenLayer::setHiddenUnitPositions(double** positions) {

	if (node_pos == NULL || positions == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		for (int y = 0; y < in_layer->Size(); y++)
			node_pos[x][y] = positions[x][y];

	return true;
}

bool ALCOVEHiddenLayer::setSpecificities(double* vals) {
	if (specificity == NULL || vals == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		specificity[x] = vals[x];

	return true;
}

bool ALCOVEHiddenLayer::getSpecificities(double* vals) {
	if (specificity == NULL || vals == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		vals[x] = specificity[x];

	return true;
}

bool ALCOVEHiddenLayer::setSpecificity(int pos, double val) {
	if (specificity == NULL || pos < 0 || pos >= Size())
		return false;

	specificity[pos] = val;

	return true;
}

double ALCOVEHiddenLayer::getSpecificity(int pos) {
	if (specificity == NULL || pos < 0 || pos >= Size())
		return 0.0;

	return specificity[pos];
}

double ALCOVEHiddenLayer::getLearningRate() {
	return learning_rate;
}

bool ALCOVEHiddenLayer::setLearningRate(double val) {
	learning_rate = val;
	return true;
}

bool ALCOVEHiddenLayer::adjustDeltaAttentionalStrengths() {
	if (node_pos == NULL)
		return false;

	in_layer->getActs(some_vec);
	getActs(local);

	for (int y = 0; y < in_layer->Size(); y++) {
	  double adjustment = 0.0;
	
		for (int x = 0; x < Size(); x++) {
			adjustment += getUnit(x)->getNetDelta() * local[x] * specificity[x] * fabs(node_pos[x][y] - some_vec[y]);
		}

		adjustment *= -learning_rate;
		in_layer->setDeltaAttentionalStrength(y, in_layer->getDeltaAttentionalStrength(y) + adjustment);
	}
}

bool ALCOVEHiddenLayer::setDeltaAttentionalStrengths(double td_error) {
	if (node_pos == NULL)
		return false;

	in_layer->getActs(some_vec);

	for (int y = 0; y < in_layer->Size(); y++) {
	  in_layer->setDeltaAttentionalStrength(y,learning_rate*td_error*some_vec[y]);
	}
}

bool ALCOVEHiddenLayer::setSpecificity(double val) {
	if (specificity == NULL)
		return false;

	bool return_val;

	for (int x = 0; x < Size(); x++) {
		return_val = setSpecificity(x, val);
	}

	return return_val;
}

LinearActivationFunction* ALCOVEHiddenLayer::createAct() {
	laf = new LinearActivationFunction();
	return laf;
}
