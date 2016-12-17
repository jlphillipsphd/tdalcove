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
#ifndef NEURAL_UNIT_H
#define NEURAL_UNIT_H

#include "object.h"
#include "thread_mutex.h" // Adds multithreading capability
#include <stddef.h>
#include <iostream>

using namespace std;

class NeuralUnit : public Object {

private:
	double act;
	double target;

	double* nets;
	double net_delta;
	double* deltas;
	double* biases;
	double* derivatives;
	double* d_biases;
	int n_dims;

	double* position;
	int p_dims;

	ThreadMutex* mutex;

public:
	NeuralUnit();
	~NeuralUnit();
	NeuralUnit(int number_of_input_dimensions);

	bool setAct(double val);
	double getAct();

	bool setTarget(double val);
	double getTarget();

	bool setNets(double* vals);
	bool getNets(double* vals);

	bool setDerivatives(double* vals);
	bool getDerivatives(double* vals);

	bool setNetDelta(double val);
	double getNetDelta();

	bool setDeltas(double* vals);
	bool getDeltas(double* vals);

	bool setBiases(double* vals);
	bool getBiases(double* vals);

	bool setDeltaBiases(double* vals);
	bool getDeltaBiases(double* vals);
	bool clearDeltaBiases();

	bool setPositionDimensions(int val);
	int getPositionDimensions();
	double getPosition(int dim);
	bool setPosition(int dim, double val);
	bool getPosition(double* vals);
	bool setPosition(double* vals);

	int getInputDimensions();
};

#endif
