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
#ifndef LAYER_H
#define LAYER_H

#include "object.h"
#include "neural_unit.h"
#include "activation_function.h"
#include "nnet_math.h"
#include <stddef.h>
#include <math.h>
#include <iostream>

using namespace std;

class Layer : public Object {

public:
	Layer();
	~Layer();
	Layer(int number_of_units, ActivationFunction* af);

	NeuralUnit* getUnit(int index);
	int Size();

	ActivationFunction* getActivationFunction();
	int getNumberOfInputDimensions();

	bool computeActivations();
	bool computePositionalActivations();

	bool applyWTA();										// Uses current activations: off - 0.0, on - 1.0
	bool applyWTA(double off, double on);					// Uses current activations
	bool applySoftWTA();
	bool applySoftWTA(double off);

	bool computeBpDeltas();									// Assumes hidden layer
	bool computeBpDeltas(bool use_targets);					// Explicitize for output layer

	bool computeBpHumbleDeltas(); 							// Assumes output layer; min 0.0, max 1.0
	bool computeBpHumbleDeltas(double t_min, double t_max);	// Assumes output layer

	bool setActs(double* vals);
	bool getActs(double* vals);

	bool setTargets(double* vals);
	bool getTargets(double* vals);

	bool setNetDeltas(double* vals);
	bool getNetDeltas(double* vals);
	bool clearNetDeltas();

	bool setNets(double** vals);
	bool getNets(double** vals);
	bool clearNets();

	bool setDeltas(double** vals);
	bool getDeltas(double** vals);

	bool setBiases(double** vals);
	bool getBiases(double** vals);

	bool setDeltaBiases(double** vals);
	bool getDeltaBiases(double** vals);
	bool clearDeltaBiases();

	bool setDerivatives(double** vals);
	bool getDerivatives(double** vals);

	bool computeDerivatives();

	double computeSSE();

	void Print();

protected:
	int num_units;
	int in_dims;
	double* work_vec2;
	double* work_vec;
	ActivationFunction* act_func;

private:
	NeuralUnit** units;
};

#endif
