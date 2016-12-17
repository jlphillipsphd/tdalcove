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
#include "layer.h"

Layer::Layer() : Object() {
	units = NULL;
	act_func = NULL;
	in_dims = 0;
	num_units = 0;
	work_vec = NULL;
	work_vec2 = NULL;
}

Layer::Layer(int number_of_units, ActivationFunction* af ) : Object() {

	if (number_of_units <= 0 || af == NULL) {
		units = NULL;
		act_func = NULL;
		in_dims = 0;
		num_units = 0;
		work_vec = NULL;
		work_vec2 = NULL;
		return;
	}

	num_units = number_of_units;
	in_dims = af->getNumberOfInputDimensions();
	act_func = af;

	units = new NeuralUnit*[num_units];
	for (int x = 0; x < num_units; x++)
		units[x] = new NeuralUnit(in_dims);

	work_vec = new double[in_dims];
	work_vec2 = new double[in_dims];
}

Layer::~Layer() {
	if (units != NULL) {
		for (int x = 0; x < num_units; x++)
			delete units[x];
		delete units;
	}

	if (work_vec != NULL)
		delete work_vec;

	// I should delete this, but there is a problem with CMACS when I
	// do so. I haven't identified the problem yet. It's a complex bug.
	if (work_vec2 != NULL)
		delete work_vec2;
}

int Layer::Size() {
	return num_units;
}

NeuralUnit* Layer::getUnit(int index) {
	if (index < 0 || index >= num_units)
		return NULL;

	return units[index];
}

ActivationFunction* Layer::getActivationFunction() {
	return act_func;
}

int Layer::getNumberOfInputDimensions() {
	return in_dims;
}

bool Layer::clearNets() {
	if (units == NULL)
		return false;

	for (int y = 0; y < in_dims; y++)
		work_vec[y] = 0.0;

	for (int x = 0; x < num_units; x++)
		units[x]->setNets(work_vec);

	return true;
}

bool Layer::computeActivations() {

	if (units == NULL)
		return false;

	for (int x = 0; x < num_units; x++) {
		units[x]->getNets(work_vec);
		units[x]->setAct(act_func->Compute(work_vec));
	}

	return true;
}

bool Layer::computePositionalActivations() {
	if (units == NULL)
		return false;

	for (int x = 0; x < num_units; x++) {
		if (units[x]->getPositionDimensions() == in_dims) {
			units[x]->getNets(work_vec);
			units[x]->getPosition(work_vec2);
			units[x]->setAct(act_func->Compute(work_vec, work_vec2));
		}
	}

	return true;
}

bool Layer::computeDerivatives() {

	if (units == NULL)
		return false;

	for (int x = 0; x < num_units; x++) {
		units[x]->getNets(work_vec);

		for (int y = 0; y < in_dims; y++)
			work_vec2[y] = act_func->Derivative(y, work_vec);

		units[x]->setDerivatives(work_vec2);
	}

	return true;
}

bool Layer::setDerivatives(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setDerivatives(vals[x]);

	return true;
}

bool Layer::getDerivatives(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->getDerivatives(vals[x]);

	return true;
}

bool Layer::computeBpDeltas() {
	return computeBpDeltas(false);
}

bool Layer::computeBpDeltas(bool use_targets) {

	int x, y;

	if (units == NULL)
		return false;

	// Needed for both computations
	computeDerivatives();

	if (use_targets) {
		for (x = 0; x < num_units; x++) {
			double targ = units[x]->getTarget();
			double act = units[x]->getAct();
			units[x]->getDerivatives(work_vec);

			for (y = 0; y < in_dims; y++)
				work_vec2[y] = (targ - act) * work_vec[y];

			units[x]->setDeltas(work_vec2);
		} // end for
	}
	else {
		for (x = 0; x < num_units; x++) {
			units[x]->getDerivatives(work_vec);
			double net_delta = units[x]->getNetDelta();

			for (y = 0; y < in_dims; y++)
				work_vec2[y] = work_vec[y] * net_delta;

			units[x]->setDeltas(work_vec2);
		} // end for
	}

	return true;
}

bool Layer::computeBpHumbleDeltas() {
	return computeBpHumbleDeltas(0.0, 1.0);
}

bool Layer::computeBpHumbleDeltas(double t_min, double t_max) {

	int x, y;

	if (units == NULL)
		return false;

	computeDerivatives();

	for (x = 0; x < num_units; x++) {
		double targ = units[x]->getTarget();
		double act = units[x]->getAct();
		units[x]->getDerivatives(work_vec);

		if (act > t_max)
			act = t_max;
		if (act < t_min)
			act = t_min;
	
		for (y = 0; y < in_dims; y++)
			work_vec2[y] = (targ - act) * work_vec[y];

		units[x]->setDeltas(work_vec2);
	} // end for

	return true;
}

bool Layer::setActs(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setAct(vals[x]);

	return true;
}

bool Layer::getActs(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		vals[x] = units[x]->getAct();

	return true;
}

bool Layer::setNets(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setNets(vals[x]);

	return true;
}

bool Layer::getNets(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->getNets(vals[x]);

	return true;
}

bool Layer::setNetDeltas(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setNetDelta(vals[x]);

	return true;
}

bool Layer::getNetDeltas(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		vals[x] = units[x]->getNetDelta();

	return true;
}

bool Layer::clearNetDeltas() {

	for (int x = 0; x < num_units; x++)
		units[x]->setNetDelta(0.0);

	return true;
}

bool Layer::setTargets(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setTarget(vals[x]);

	return true;
}

bool Layer::getTargets(double* vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		vals[x] = units[x]->getTarget();

	return true;
}

bool Layer::setDeltas(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setDeltas(vals[x]);

	return true;
}

bool Layer::getDeltas(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->getDeltas(vals[x]);

	return true;
}

bool Layer::setBiases(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setBiases(vals[x]);

	return true;
}

bool Layer::getBiases(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->getBiases(vals[x]);

	return true;
}

bool Layer::setDeltaBiases(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->setDeltaBiases(vals[x]);

	return true;
}

bool Layer::getDeltaBiases(double** vals) {

	if (vals == NULL || units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->getDeltaBiases(vals[x]);

	return true;
}

bool Layer::clearDeltaBiases() {

	if (units == NULL)
		return false;

	for (int x = 0; x < num_units; x++)
		units[x]->clearDeltaBiases();

	return true;
}

double Layer::computeSSE() {

	double sse = 0.0;

	for (int x = 0; x < num_units; x++)
		sse += ((units[x]->getTarget() - units[x]->getAct()) * (units[x]->getTarget() - units[x]->getAct()));

	return sse;

}

void Layer::Print() {

	if (units == NULL)
		return;

	int x;
	double* vec = new double[num_units];
	double** vec2 = new double*[num_units];
	for (x = 0; x < num_units; x++)
		vec2[x] = new double[in_dims];

	cout << "Layer information:" << endl;

	cout << "Activations:\n";
	getActs(vec);
	print_vector(num_units, vec);
	cout << endl;

	cout << "Net Inputs:\n";
	getNets(vec2);
	print_matrix(num_units, in_dims, vec2);

	cout << "Net Deltas:\n";
	getNetDeltas(vec);
	print_vector(num_units, vec);
	cout << endl;

	cout << "Targets:\n";
	getTargets(vec);
	print_vector(num_units, vec);
	cout << endl;

	cout << "Deltas:\n";
	getDeltas(vec2);
	print_matrix(num_units, in_dims, vec2);

	cout << "Biases:\n";
	getBiases(vec2);
	print_matrix(num_units, in_dims, vec2);

	cout << "Delta Biases:\n";
	getDeltaBiases(vec2);
	print_matrix(num_units, in_dims, vec2);
	cout << endl;

	delete vec;

	for (x = 0; x < num_units; x++)
		delete vec2[x];
	delete vec2;
}

bool Layer::applyWTA() {
	return applyWTA(0.0, 1.0);
}

bool Layer::applyWTA(double off, double on) {
	
	int highest = 0;
	int x;

	for (x = 0; x < num_units; x++) {
		if (x != highest && units[x]->getAct() > units[highest]->getAct())
			highest = x;
	} // end for

	for (x = 0; x < num_units; x++) {
		if (x == highest)
			units[x]->setAct(on);
		else
			units[x]->setAct(off);
	}

	return true;
}

bool Layer::applySoftWTA() {
	return applySoftWTA(0.0);
}

bool Layer::applySoftWTA(double off) {

	int highest = 0;
	int x;

	for (x = 0; x < num_units; x++) {
		if (x != highest && units[x]->getAct() > units[highest]->getAct())
			highest = x;
	} // end for

	for (x = 0; x < num_units; x++) {
		if (x != highest)
			units[x]->setAct(off);
	}

	return true;
}
