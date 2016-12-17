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
#include "projection.h"
#include "simple_activation_function.h"
#include "random_number_generator.h"
#include "layer.h"
#include <fstream>

using namespace std;

#ifndef FULL_FORWARD_PROJECTION_H
#define FULL_FORWARD_PROJECTION_H

class FullForwardProjection : public Projection {

private:
	double bound(double val);

protected:
	double** weights;
	double** d_weights;
	double** w_transpose;
	double* work_vec1;
	double* work_vec2;
	double* work_vec3;
	double* work_vec4;
	double* work_vec5;
	double min_w;
	double max_w;
	bool w_bound;
	
public:
	FullForwardProjection();
	~FullForwardProjection();
	FullForwardProjection(Layer* from_layer, Layer* to_layer, int dimension);

	bool initializeWeights(RandomNumberGenerator* rand);
	bool perturbWeights(RandomNumberGenerator* rand);
	bool updateWeights();
	bool clearDeltaWeights();
	bool clearWeights();

	bool computeNets();
	bool computeNets(bool use_biases);

	bool adjustNets();
	bool adjustNets(bool use_biases);

	bool setWeight(int from_layer_pos, int to_layer_pos, double val);
	double getWeight(int from_layer_pos, int to_layer_pos);

	void useWeightBounding(bool val);

	double getWeightBoundMinimum();
	bool setWeightBoundMinimum(double val);
	double getWeightBoundMaximum();
	bool setWeightBoundMaximum(double val);

	bool writeWeights(ofstream& output_file);
	bool readWeights(ifstream& input_file);
	void Print();
};

#endif
