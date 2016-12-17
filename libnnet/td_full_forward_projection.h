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
#include "bp_full_forward_projection.h"
#include "td_layer.h"
#include "layer.h"

#ifndef TD_FULL_FORWARD_PROJECTION_H
#define TD_FULL_FORWARD_PROJECTION_H

class TDFullForwardProjection : public BpFullForwardProjection {

private:
	double*** e_traces;
	double** bias_e_traces;
	double lambda;
	double* special;
	TDLayer* out_layer;
	TDLayer* f_layer;
	TDLayer* t_layer;

public:
	TDFullForwardProjection();
	~TDFullForwardProjection();
	TDFullForwardProjection(TDLayer* from_layer, TDLayer* to_layer, int dimension, double lambda_value, TDLayer* output_layer);

	bool setLambda(double val);
	double getLambda();

	bool computeWeightChanges(int output_unit);

	bool clearEligibilityTraces();
};

#endif
