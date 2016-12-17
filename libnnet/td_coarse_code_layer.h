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
#include "td_layer.h"
#include "activation_function.h"

#ifndef TD_COARSE_CODE_LAYER_H
#define TD_COARSE_CODE_LAYER_H

class TDCoarseCodeLayer : public TDLayer {

public:
	TDCoarseCodeLayer();
	~TDCoarseCodeLayer();
	TDCoarseCodeLayer(ActivationFunction* coarse_code_function,
			int* number_of_units_per_dimension,
			double* minimums_per_dimension,
			double* maximums_per_dimension,
			ActivationFunction* af, double discount_rate);

	bool setCoarseActs(double* position);
	bool setCoarseTargets(double* position);
	bool setCoarseNets(double* position);

protected:
	int computeUnits(ActivationFunction* af, int* vals);
	double computeStep(int n, double min, double max);
	void codeLocation(int num_dims, int* dims, int* pos, int index);
	ActivationFunction* cc_func;
	double* cc_work_vec;
	double* cc_work_vec2;

};

#endif
