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
#include "activation_function.h"

#ifndef TD_LAYER_H
#define TD_LAYER_H

class TDLayer : public Layer {

private:
	double gamma;
	double* last_acts;
	double** last_nets;
	double* td_error;
	double* some_vec;
	bool first_step;

public:
	TDLayer();
	~TDLayer();
	TDLayer(int number_of_units, ActivationFunction* saf, double discount_rate);

	bool computeTDError(double* reward);
	bool computeTDError(double* reward, bool final_round);
	bool Reset();

	bool setDiscountRate(double g);
	double getDiscountRate();

	bool getTDError(double* vals);
	bool setTDError(int index, double val);
	double getTDError(int index);

	// After computing the TDError - this should be called for all
	// units (note that you will need to backprop the gradient back all
	// the way to the input layer and call the TD-weight update function
	// on the same unit as the computed gradient for each iteration.)
	bool computeTDDelta(int unit);

	bool getLastActs(double* vals);
	bool setLastActs(double* vals);

	bool incrementTimer(); // Important - shifts the current activations and
							// net inputs into the -last- acts and nets
							// categories. (Use after every time step.)
};

#endif
