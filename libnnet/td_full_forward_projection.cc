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
#include "td_full_forward_projection.h"
#include "layer.h"
#include "bp_full_forward_projection.h"
#include <stddef.h>
#include <iostream>

using namespace std;

TDFullForwardProjection::TDFullForwardProjection() : BpFullForwardProjection() {
	e_traces = NULL;
	bias_e_traces = NULL;
	lambda = 0.0;
	special = NULL;
	out_layer = NULL;
	f_layer = NULL;
	t_layer = NULL;
}

TDFullForwardProjection::~TDFullForwardProjection() {
	if (e_traces != NULL) {
		for (int x = 0; x < getFromLayer()->Size(); x++) {
			for (int y = 0; y < getToLayer()->Size(); y++)
				delete e_traces[x][y];
			delete e_traces[x];
		}
		delete e_traces;
	}

	if (bias_e_traces != NULL) {
		for (int z = 0; z < getToLayer()->Size(); z++)
			delete bias_e_traces[z];
		delete bias_e_traces;
	}

	if (special != NULL)
		delete special;
}

TDFullForwardProjection::TDFullForwardProjection(TDLayer* from_layer, TDLayer*
to_layer, int dimension, double lambda_value, TDLayer* output_layer) : BpFullForwardProjection(from_layer, to_layer, dimension) {

	int x,y,z;

	if (from_layer == NULL || to_layer == NULL || out_layer == NULL) {
		lambda = 0.0;
		e_traces = NULL;
		bias_e_traces = NULL;
		out_layer = NULL;
		f_layer = NULL;
		t_layer = NULL;
		return;
	}

	out_layer = output_layer;
	f_layer = from_layer;
	t_layer = to_layer;
	lambda = lambda_value;

	if (lambda_value < 0.0 || lambda_value > 1.0)
		lambda = 0.0;

	e_traces = new double**[from_layer->Size()];
	for (x = 0; x < from_layer->Size(); x++) {
		e_traces[x] = new double*[to_layer->Size()];
		for (y = 0; y < to_layer->Size(); y++) {
			e_traces[x][y] = new double[out_layer->Size()];
			for (z = 0; z < out_layer->Size(); z++)
				e_traces[x][y][z] = 0.0;
		}
	}
			
	bias_e_traces = new double*[to_layer->Size()];
	for (x = 0; x < to_layer->Size(); x++) {
		bias_e_traces[x] = new double[out_layer->Size()];
		for (y = 0; y < out_layer->Size(); y++)
			bias_e_traces[x][y] = 0.0;
	}

}

bool TDFullForwardProjection::setLambda(double val) {
	if (val < 0.0 || val > 1.0)
		return false;

	lambda = val;
	return true;
}

double TDFullForwardProjection::getLambda() {
	return lambda;
}

bool TDFullForwardProjection::computeWeightChanges(int output_unit) {

    if (weights == NULL || d_weights == NULL || e_traces == NULL || bias_e_traces == NULL)
	        return false;

	if (output_unit < 0 || output_unit >= out_layer->Size())
		return false;

	f_layer->getLastActs(from_work_vec1);

    int x,y;

	// Update e-traces
    for (y = 0; y < getToLayer()->Size(); y++) {
		t_layer->getUnit(y)->getDeltas(work_vec3);
    	for (x = 0; x < getFromLayer()->Size(); x++) {
           	e_traces[x][y][output_unit] = (lambda * e_traces[x][y][output_unit]) + (work_vec3[dim] * from_work_vec1[x]);
		}

       	bias_e_traces[y][output_unit] += (lambda * bias_e_traces[y][output_unit]) + (work_vec3[dim]);
	}

	double error = out_layer->getTDError(output_unit);

	// Compute weight updates
    for (x = 0; x < getFromLayer()->Size(); x++)
        for (y = 0; y < getToLayer()->Size(); y++)
            	d_weights[x][y] += getLearningRate() * error * e_traces[x][y][output_unit];

    for (x = 0; x < getToLayer()->Size(); x++) {
    	getToLayer()->getUnit(x)->getDeltaBiases(work_vec3);
        work_vec3[dim] += getLearningRate() * error * bias_e_traces[x][output_unit];
    	getToLayer()->getUnit(x)->setDeltaBiases(work_vec3);
	}

    return true;

}

bool TDFullForwardProjection::clearEligibilityTraces() {

	if (e_traces == NULL)
		return false;

	for (int x = 0; x < getFromLayer()->Size(); x++)
		for (int y = 0; y < getToLayer()->Size(); y++)
			for (int z = 0; z < out_layer->Size(); z++) {
				e_traces[x][y][z] = 0.0;
				bias_e_traces[y][z] = 0.0;
			}

	return true;
}
