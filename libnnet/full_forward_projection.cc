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
#include "full_forward_projection.h"
#include "nnet_math.h"
#include <stddef.h>
#include <iostream>
#include <fstream>

using namespace std;

FullForwardProjection::FullForwardProjection() : Projection() {
	min_w = -1.0;
	max_w = -1.0;
	w_bound = false;
	weights = NULL;
	d_weights = NULL;
	w_transpose = NULL;
	work_vec1 = NULL;
	work_vec2 = NULL;
	work_vec3 = NULL;
	work_vec4 = NULL;
	work_vec5 = NULL;
}

FullForwardProjection::~FullForwardProjection() {
	int x;

	if (weights != NULL) {
		for (x = 0; x < fl_size; x++)
			delete weights[x];

		delete weights;
	}

	if (d_weights != NULL) {
		for (x = 0; x < fl_size; x++)
			delete d_weights[x];

		delete d_weights;
	}

	if (w_transpose != NULL) {
		for (x = 0; x < tl_size; x++)
			delete w_transpose[x];

		delete w_transpose;
	}
	
	if (work_vec1 != NULL)
		delete work_vec1;

	if (work_vec2 != NULL)
		delete work_vec2;

	if (work_vec3 != NULL)
		delete work_vec3;

	if (work_vec4 != NULL)
		delete work_vec4;

	if (work_vec5 != NULL)
		delete work_vec5;
}

FullForwardProjection::FullForwardProjection(Layer* from_layer, Layer* to_layer, int dimension) : Projection(from_layer, to_layer, dimension) {

	min_w = -1.0;
	max_w = -1.0;
	w_bound = false;

	if (from_layer == NULL || to_layer == NULL) {
		weights = NULL;
		d_weights = NULL;
		w_transpose = NULL;
		work_vec1 = NULL;
		work_vec2 = NULL;
		work_vec3 = NULL;
		work_vec4 = NULL;
		work_vec5 = NULL;
		return;
	}

	int from_size = from_layer->Size();
	int to_size = to_layer->Size();
	int x, y;

	weights = new double*[from_size];
	d_weights = new double*[from_size];
	w_transpose = new double*[to_size];
	work_vec1 = new double[to_size];
	work_vec2 = new double[to_size];
	work_vec3 = new double[to_layer->getNumberOfInputDimensions()];
	work_vec4 = new double[to_layer->getNumberOfInputDimensions()];
	work_vec5 = new double[from_size];

	for (y = 0; y < to_size; y++)
		w_transpose[y] = new double[from_size];

	for (x = 0; x < from_size; x++) {
		weights[x] = new double[to_size];
		d_weights[x] = new double[to_size];
		for (y = 0; y < to_size; y++) {
			weights[x][y] = 0.0;
			w_transpose[y][x] = 0.0;
			d_weights[x][y] = 0.0;
		}
	}

	for (x = 0; x < to_size; x++) {
		NeuralUnit* unit = to_layer->getUnit(x);
		unit->getBiases(work_vec3);
		work_vec3[dim] = 0.0;
		unit->setBiases(work_vec3);
	}
}

bool FullForwardProjection::initializeWeights( RandomNumberGenerator* rand ) {
	if (rand == NULL || weights == NULL || d_weights == NULL)
		return false;

	int x,y;

	for (x = 0; x < getFromLayer()->Size(); x++)
		for (y = 0; y < getToLayer()->Size(); y++) {
			weights[x][y] = rand->Number();
			weights[x][y] = bound(weights[x][y]);
			w_transpose[y][x] = weights[x][y];
		}

	for (x = 0; x < getToLayer()->Size(); x++) {
		NeuralUnit* unit = getToLayer()->getUnit(x);
		unit->getBiases(work_vec3);
		work_vec3[dim] = rand->Number();
		work_vec3[dim] = bound(work_vec3[dim]);
		unit->setBiases(work_vec3);
	}

	return true;
}

bool FullForwardProjection::perturbWeights( RandomNumberGenerator* rand ) {
	if (rand == NULL || weights == NULL || d_weights == NULL)
		return false;

	int x,y;
	double val;

	for (x = 0; x < getFromLayer()->Size(); x++)
		for (y = 0; y < getToLayer()->Size(); y++) {
			val = rand->Number();
			weights[x][y] += val;
			weights[x][y] = bound(weights[x][y]);
			w_transpose[y][x] = weights[x][y];
		}

	for (x = 0; x < getToLayer()->Size(); x++) {
		val = rand->Number();
		NeuralUnit* unit = getToLayer()->getUnit(x);
		unit->getBiases(work_vec3);
		work_vec3[dim] += val;
		work_vec3[dim] = bound(work_vec3[dim]);
		unit->setBiases(work_vec3);
	}
	
	return true;
}

bool FullForwardProjection::updateWeights() {

	if (d_weights == NULL || weights == NULL)
		return false;

	increment(getFromLayer()->Size(), getToLayer()->Size(), weights, d_weights);

	for (int x = 0; x < getFromLayer()->Size(); x++)
		for (int y = 0; y < getToLayer()->Size(); y++) {
			weights[x][y] = bound(weights[x][y]);
			w_transpose[y][x] = weights[x][y];
		}

	for (int z = 0; z < getToLayer()->Size(); z++) {
		NeuralUnit* unit = getToLayer()->getUnit(z);
		unit->getDeltaBiases(work_vec3);
		unit->getBiases(work_vec4);
		work_vec4[dim] += work_vec3[dim];
		work_vec4[dim] = bound(work_vec4[dim]);
		unit->setBiases(work_vec4);
	}

	clearDeltaWeights();

	return true;
}

bool FullForwardProjection::clearDeltaWeights() {
	if (weights == NULL || d_weights == NULL)
		return false;

	for (int x = 0; x < getFromLayer()->Size(); x++)
		for (int y = 0; y < getToLayer()->Size(); y++)
			d_weights[x][y] = 0.0;

	for (int z = 0; z < getToLayer()->Size(); z++) {
		getToLayer()->getUnit(z)->getDeltaBiases(work_vec3);
		work_vec3[dim] = 0.0;
		getToLayer()->getUnit(z)->setDeltaBiases(work_vec3);
	} // end for

	return true;
}

bool FullForwardProjection::clearWeights() {
	if (weights == NULL || d_weights == NULL)
		return false;

	for (int x = 0; x < getFromLayer()->Size(); x++)
		for (int y = 0; y < getToLayer()->Size(); y++) {
			d_weights[x][y] = 0.0;
			weights[x][y] = 0.0;
			weights[x][y] = bound(weights[x][y]);
			w_transpose[y][x] = 0.0;
		}

	for (int z = 0; z < getToLayer()->Size(); z++) {
		getToLayer()->getUnit(z)->getBiases(work_vec3);
		work_vec3[dim] = 0.0;
		work_vec3[dim] = bound(work_vec3[dim]);
		getToLayer()->getUnit(z)->setBiases(work_vec3);
	} // end for

	return true;
}

void FullForwardProjection::Print() {

	cout << "Projection information:" << endl;
	cout << "Weights:" << endl;
	print_matrix(getFromLayer()->Size(), getToLayer()->Size(), weights);
	cout << "Next Weight Adjustments:" << endl;
	print_matrix(getFromLayer()->Size(), getToLayer()->Size(), d_weights);
	cout << endl;

}

bool FullForwardProjection::writeWeights(ofstream& output_file) {

	if (weights == NULL)
		return false;

	for (int x = 0; x < getFromLayer()->Size(); x++) {
		for (int y =0; y < getToLayer()->Size(); y++)
			output_file << weights[x][y] << " ";

		output_file << endl;

	} // end for

	for (int z = 0; z < getToLayer()->Size(); z++) {
		getToLayer()->getUnit(z)->getBiases(work_vec3);
		output_file << work_vec3[dim] << " ";
	} // end for

	output_file << endl;

	return true;
}

bool FullForwardProjection::readWeights(ifstream& input_file) {

	if (weights == NULL)
		return false;

	for (int x = 0; x < getFromLayer()->Size(); x++) {
		for (int y = 0; y < getToLayer()->Size(); y++) {
			input_file >> weights[x][y];
			weights[x][y] = bound(weights[x][y]);
			w_transpose[y][x] = weights[x][y];
		} // end for
	}

	for (int z = 0; z < getToLayer()->Size(); z++) {
		getToLayer()->getUnit(z)->getBiases(work_vec3);
		input_file >> work_vec3[dim];
		work_vec3[dim] = bound(work_vec3[dim]);
		getToLayer()->getUnit(z)->setBiases(work_vec3);
	} // end for

	return true;
}

bool FullForwardProjection::computeNets() {
	return computeNets(true);
}

bool FullForwardProjection::computeNets(bool use_biases) {

	if (weights == NULL || d_weights == NULL)
		return false;

	getFromLayer()->getActs(work_vec5);

	multiply(getFromLayer()->Size(), work_vec5, getToLayer()->Size(), weights, work_vec1);

	if (use_biases) {
	    for (int x = 0; x < getToLayer()->Size(); x++) {
			getToLayer()->getUnit(x)->getBiases(work_vec3);
			work_vec1[x] += work_vec3[dim];
		}
    }

	for (int y = 0; y < getToLayer()->Size(); y++) {
		getToLayer()->getUnit(y)->getNets(work_vec3);
		work_vec3[dim] = work_vec1[y];
		getToLayer()->getUnit(y)->setNets(work_vec3);
	}

    return true;

}

bool FullForwardProjection::adjustNets() {
	return adjustNets(true);
}

bool FullForwardProjection::adjustNets(bool use_biases) {

	if (weights == NULL || d_weights == NULL)
		return false;

	getFromLayer()->getActs(work_vec5);

	multiply(getFromLayer()->Size(), work_vec5, getToLayer()->Size(), weights, work_vec1);

	if (use_biases) {
	    for (int x = 0; x < getToLayer()->Size(); x++) {
			getToLayer()->getUnit(x)->getBiases(work_vec3);
			work_vec1[x] += work_vec3[dim];
		}
    }

	for (int y = 0; y < getToLayer()->Size(); y++) {
		getToLayer()->getUnit(y)->getNets(work_vec3);
		work_vec3[dim] += work_vec1[y];
		getToLayer()->getUnit(y)->setNets(work_vec3);
	}

    return true;

}

double FullForwardProjection::getWeight(int from_layer_pos, int to_layer_pos) {
	if (weights == NULL)
		return false;

	if (from_layer_pos < 0 || from_layer_pos >= getFromLayer()->Size())
		return false;

	if (to_layer_pos < 0 || to_layer_pos >= getToLayer()->Size())
		return false;

	return weights[from_layer_pos][to_layer_pos];
}

bool FullForwardProjection::setWeight(int from_layer_pos, int to_layer_pos, double val) {
	if (weights == NULL)
		return false;

	if (from_layer_pos < 0 || from_layer_pos >= getFromLayer()->Size())
		return false;

	if (to_layer_pos < 0 || to_layer_pos >= getToLayer()->Size())
		return false;

	val = bound(val);
	weights[from_layer_pos][to_layer_pos] = val;
	w_transpose[to_layer_pos][from_layer_pos] = val;

	return true;
}

double FullForwardProjection::bound(double val) {
	if (w_bound) {
		if (val < min_w)
			return min_w;
		if (val > max_w)
			return max_w;
	}
	else
		return val;
}

void FullForwardProjection::useWeightBounding(bool val) {
	if (val && w_bound)
		return;

	if (!val && !w_bound)
		return;

	w_bound = val;

	// Apply
	if (w_bound) {
		for (int x = 0; x < getFromLayer()->Size(); x++)
			for (int y = 0; y < getToLayer()->Size(); y++) {
				weights[x][y] = bound(weights[x][y]);
				w_transpose[y][x] = weights[x][y];
			}

		for (int z = 0; z < getToLayer()->Size(); z++) {
			getToLayer()->getUnit(z)->getBiases(work_vec3);
			work_vec3[dim] = bound(work_vec3[dim]);
			getToLayer()->getUnit(z)->setBiases(work_vec3);
		} // end for
	}

}

double FullForwardProjection::getWeightBoundMinimum() {
	return min_w;
}

double FullForwardProjection::getWeightBoundMaximum() {
	return max_w;
}

bool FullForwardProjection::setWeightBoundMinimum(double val) {
	if (val >= max_w)
		return false;

	min_w = val;

	if (w_bound) {
		for (int x = 0; x < getFromLayer()->Size(); x++)
			for (int y = 0; y < getToLayer()->Size(); y++) {
				weights[x][y] = bound(weights[x][y]);
				w_transpose[y][x] = weights[x][y];
			}

		for (int z = 0; z < getToLayer()->Size(); z++) {
			getToLayer()->getUnit(z)->getBiases(work_vec3);
			work_vec3[dim] = bound(work_vec3[dim]);
			getToLayer()->getUnit(z)->setBiases(work_vec3);
		} // end for
	}

	return true;
}

bool FullForwardProjection::setWeightBoundMaximum(double val) {
	if (val <= min_w)
		return false;

	max_w = val;

	if (w_bound) {
		for (int x = 0; x < getFromLayer()->Size(); x++)
			for (int y = 0; y < getToLayer()->Size(); y++) {
				weights[x][y] = bound(weights[x][y]);
				w_transpose[y][x] = weights[x][y];
			}

		for (int z = 0; z < getToLayer()->Size(); z++) {
			getToLayer()->getUnit(z)->getBiases(work_vec3);
			work_vec3[dim] = bound(work_vec3[dim]);
			getToLayer()->getUnit(z)->setBiases(work_vec3);
		} // end for
	}

	return true;
}

