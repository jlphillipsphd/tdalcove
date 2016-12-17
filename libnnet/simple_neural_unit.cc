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
#include <stddef.h>
#include "neural_unit.h"
#include "simple_neural_unit.h"

SimpleNeuralUnit::SimpleNeuralUnit() : NeuralUnit() {
	work_vec = new double[1];
}

SimpleNeuralUnit::~SimpleNeuralUnit() {
	delete work_vec;
}

double SimpleNeuralUnit::getNet() {
	getNets(work_vec);
	return work_vec[0];
}

bool SimpleNeuralUnit::setNet(double val) {
	work_vec[0] = val;
	return setNets(work_vec);
}

double SimpleNeuralUnit::getDerivative() {
	getDerivatives(work_vec);
	return work_vec[0];
}

bool SimpleNeuralUnit::setDerivative(double val) {
	work_vec[0] = val;
	return setDerivatives(work_vec);
}

double SimpleNeuralUnit::getDelta() {
	getDeltas(work_vec);
	return work_vec[0];
}

bool SimpleNeuralUnit::setDelta(double val) {
	work_vec[0] = val;
	return setDeltas(work_vec);
}

double SimpleNeuralUnit::getBias() {
	getBiases(work_vec);
	return work_vec[0];
}

bool SimpleNeuralUnit::setBias(double val) {
	work_vec[0] = val;
	return setBiases(work_vec);
}

double SimpleNeuralUnit::getDeltaBias() {
	getDeltaBiases(work_vec);
	return work_vec[0];
}

bool SimpleNeuralUnit::setDeltaBias(double val) {
	work_vec[0] = val;
	return setDeltaBiases(work_vec);
}

void SimpleNeuralUnit::clearDeltaBias() {
	clearDeltaBiases();
	return;
}
