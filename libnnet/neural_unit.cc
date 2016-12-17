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
#include "neural_unit.h"

NeuralUnit::NeuralUnit() : Object() {
	act = 0.0;
	target = 0.0;
	net_delta = 0.0;
	nets = new double[1];
	nets[0] = 0.0;
	deltas = new double[1];
	deltas[0] = 0.0;
	biases = new double[1];
	biases[0] = 0.0;
	d_biases = new double[1];
	d_biases[0] = 0.0;
	derivatives = new double[1];
	derivatives[0] = 0.0;
	position = NULL;
	p_dims = 0;
	n_dims = 1;
	mutex = new ThreadMutex();
}

NeuralUnit::~NeuralUnit() {

	if (nets != NULL)
		delete nets;

	if (deltas != NULL)
		delete deltas;

	if (biases != NULL)
		delete biases;

	if (derivatives != NULL)
		delete derivatives;

	if (d_biases != NULL)
		delete d_biases;

	if (position != NULL)
		delete position;

	delete mutex;
} // destructor

NeuralUnit::NeuralUnit(int number_of_input_dimensions) : Object() {

	int x = number_of_input_dimensions;
	int i;

	if (x < 1) {
		act = 0.0;
		target = 0.0;
		net_delta = 0.0;
		nets = new double[1];
		nets[0] = 0.0;
		deltas = new double[1];
		deltas[0] = 0.0;
		biases = new double[1];
		biases[0] = 0.0;
		d_biases = new double[1];
		d_biases[0] = 0.0;
		derivatives = new double[1];
		derivatives[0] = 0.0;
		position = NULL;
		p_dims = 0;
		n_dims = 1;
		mutex = new ThreadMutex();
		return;
	}

	act = 0.0;
	target = 0.0;
	net_delta = 0.0;

	nets = new double[x];
	for (i = 0; i < x; i++)
		nets[i] = 0.0;

	deltas = new double[x];
	for (i = 0; i < x; i++)
		deltas[i] = 0.0;

	biases = new double[x];
	for (i = 0; i < x; i++)
		biases[i] = 0.0;

	derivatives = new double[x];
	for (i = 0; i < x; i++)
		derivatives[i] = 0.0;

	d_biases = new double[x];
	for (i = 0; i < x; i++)
		d_biases[i] = 0.0;

	position = NULL;
	p_dims = 0;
	n_dims = x;

	mutex = new ThreadMutex();
}

bool NeuralUnit::setAct(double val) {
	mutex->lock();
	act = val;
	mutex->unlock();
	return true;
}

double NeuralUnit::getAct() {
	double val;
	mutex->lock();
	val = act;
	mutex->unlock();
	return val;
}

bool NeuralUnit::setTarget(double val) {
	mutex->lock();
	target = val;
	mutex->unlock();
	return true;
}

double NeuralUnit::getTarget() {
	double val;
	mutex->lock();
	val = target;
	mutex->unlock();
	return val;
}

bool NeuralUnit::setNets(double* vals) {
	mutex->lock();

	if (nets == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		nets[x] = vals[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::getNets(double* vals) {
	mutex->lock();

	if (nets == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		vals[x] = nets[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::setDerivatives(double* vals) {
	mutex->lock();

	if (derivatives == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		derivatives[x] = vals[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::getDerivatives(double* vals) {
	mutex->lock();

	if (derivatives == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		vals[x] = derivatives[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::setNetDelta(double val) {
	mutex->lock();
	net_delta = val;
	mutex->unlock();
	return true;
}

double NeuralUnit::getNetDelta() {
	double val;
	mutex->lock();
	val = net_delta;
	mutex->unlock();
	return val;
}

bool NeuralUnit::setDeltas(double* vals) {
	mutex->lock();

	if (deltas == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		deltas[x] = vals[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::getDeltas(double* vals) {
	mutex->lock();

	if (deltas == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		vals[x] = deltas[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::setBiases(double* vals) {
	mutex->lock();

	if (biases == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		biases[x] = vals[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::getBiases(double* vals) {
	mutex->lock();

	if (biases == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		vals[x] = biases[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::setDeltaBiases(double* vals) {
	mutex->lock();

	if (d_biases == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		d_biases[x] = vals[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::getDeltaBiases(double* vals) {
	mutex->lock();

	if (d_biases == NULL || vals == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		vals[x] = d_biases[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::clearDeltaBiases() {
	mutex->lock();

	if (d_biases == NULL || n_dims < 1) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < n_dims; x++)
		d_biases[x] = 0.0;

	mutex->unlock();
	return true;
}

bool NeuralUnit::setPositionDimensions(int val) {

	if (val <= 0)
		return false;

	mutex->lock();

	if (position != NULL)
		delete position;

	position = new double[val];
	p_dims = val;

	mutex->unlock();
	return true;
}

int NeuralUnit::getPositionDimensions() {
	int val;
	mutex->lock();
	val = p_dims;
	mutex->unlock();
	return val;
}

double NeuralUnit::getPosition(int dim) {
	double val;
	mutex->lock();
	if (position == NULL || dim > p_dims || dim < 0) {
		mutex->unlock();
		return 0.0;
	}

	val = position[dim];
	mutex->unlock();
	return val;
}

bool NeuralUnit::setPosition(int dim, double val) {
	mutex->lock();

	if (position == NULL || dim > p_dims || dim < 0) {
		mutex->unlock();
		return false;
	}

	position[dim] = val;
	mutex->unlock();
	return true;
}

bool NeuralUnit::getPosition(double* vals) {
	mutex->lock();

	if (position == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < p_dims; x++)
		vals[x] = position[x];

	mutex->unlock();
	return true;
}

bool NeuralUnit::setPosition(double* vals) {
	mutex->lock();

	if (position == NULL) {
		mutex->unlock();
		return false;
	}

	for (int x = 0; x < p_dims; x++)
		position[x] = vals[x];

	mutex->unlock();
	return true;
}

int NeuralUnit::getInputDimensions() {
	int val;
	mutex->lock();
	val = n_dims;
	mutex->unlock();
	return val;
}
