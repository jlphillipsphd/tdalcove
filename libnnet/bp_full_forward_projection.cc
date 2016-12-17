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
#include "full_forward_projection.h"
#include "layer.h"
#include "nnet_math.h"
#include <stddef.h>

BpFullForwardProjection::BpFullForwardProjection() : FullForwardProjection() {
	learning_rate = 0.3;
	from_work_vec1 = NULL;
	from_work_vec2 = NULL;
	to_work_vec1 = NULL;
	to_work_vec2 = NULL;
}

BpFullForwardProjection::BpFullForwardProjection(Layer* from_layer, Layer* to_layer, int dimension) : FullForwardProjection(from_layer, to_layer, dimension) {
	learning_rate = 0.3;

	if (from_layer->Size() > 0)
		from_work_vec1 = new double[from_layer->Size()];

	if (from_layer->Size() > 0)
		from_work_vec2 = new double[from_layer->Size()];

	if (to_layer->Size() > 0)
		to_work_vec1 = new double[to_layer->Size()];

	if (to_layer->Size() > 0)
		to_work_vec2 = new double[to_layer->Size()];

}

BpFullForwardProjection::~BpFullForwardProjection() {

	if (from_work_vec1 != NULL)
		delete from_work_vec1;

	if (from_work_vec2 != NULL)
		delete from_work_vec2;

	if (to_work_vec1 != NULL)
		delete to_work_vec1;

	if (to_work_vec2 != NULL)
		delete to_work_vec2;
}

bool BpFullForwardProjection::setLearningRate(double val) {
	learning_rate = val;
	return true;
}

double BpFullForwardProjection::getLearningRate() {
	return learning_rate;
}

bool BpFullForwardProjection::computeNetDeltas() {

	if (weights == NULL || d_weights == NULL)
		return false;

	for (int x = 0; x < getToLayer()->Size(); x++) {
		getToLayer()->getUnit(x)->getDeltas(work_vec3);
		to_work_vec1[x] = work_vec3[dim];
	}

	multiply(getToLayer()->Size(), to_work_vec1, getFromLayer()->Size(), w_transpose, from_work_vec1);

	getFromLayer()->setNetDeltas(from_work_vec1);

	return true;
}

bool BpFullForwardProjection::adjustNetDeltas() {

	if (weights == NULL || d_weights == NULL)
		return false;

	for (int x = 0; x < getToLayer()->Size(); x++) {
		getToLayer()->getUnit(x)->getDeltas(work_vec3);
		to_work_vec1[x] = work_vec3[dim];
	}

	multiply(getToLayer()->Size(), to_work_vec1, getFromLayer()->Size(), w_transpose, from_work_vec1);

	getFromLayer()->getNetDeltas(from_work_vec2);

	for (int y = 0; y < getFromLayer()->Size(); y++)
		from_work_vec1[y] += from_work_vec2[y];

	getFromLayer()->setNetDeltas(from_work_vec1);

	return true;
}

bool BpFullForwardProjection::computeWeightChanges() {

	if (weights == NULL || d_weights == NULL)
		return false;

	int x,y;

	for (x = 0; x < getToLayer()->Size(); x++) {
		getToLayer()->getUnit(x)->getDeltas(work_vec3);
		to_work_vec1[x] = work_vec3[dim];
		getToLayer()->getUnit(x)->getDeltaBiases(work_vec3);
		to_work_vec2[x] = work_vec3[dim];
	}
	getFromLayer()->getActs(from_work_vec1);

	for (x = 0; x < getFromLayer()->Size(); x++)
		for (y = 0; y < getToLayer()->Size(); y++)
			d_weights[x][y] += learning_rate * to_work_vec1[y] * from_work_vec1[x];

	for (x = 0; x < getToLayer()->Size(); x++) {
		to_work_vec2[x] += learning_rate * to_work_vec1[x];
		getToLayer()->getUnit(x)->getDeltaBiases(work_vec3);
		work_vec3[dim] = to_work_vec2[x];
		getToLayer()->getUnit(x)->setDeltaBiases(work_vec3);
	}

	return true;

}

