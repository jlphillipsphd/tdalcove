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
#include "double_data_pair.h"

DoubleDataPair::DoubleDataPair() : DataPair(DataPair::TYPE_DOUBLE) {
	in_val = NULL;
	out_val = NULL;
	in_size = -1;
	out_size = -1;
}

DoubleDataPair::~DoubleDataPair() {
	if (in_val != NULL)
		delete in_val;
	if (out_val != NULL)
		delete out_val;
}

DoubleDataPair::DoubleDataPair(int input_size, double* initial_input, int output_size, double* initial_output) : DataPair(DataPair::TYPE_DOUBLE) {

	int x;

	in_val = NULL;
	out_val = NULL;

	if (input_size > 0 && initial_input != NULL) {
		in_size = input_size;
		in_val = new double[in_size];
		for (x = 0; x < in_size; x++)
			in_val[x] = initial_input[x];
	}
	else
		in_size = -1;

	if (output_size > 0 && initial_output != NULL) {
		out_size = output_size;
		out_val = new double[out_size];
		for (x = 0; x < out_size; x++)
			out_val[x] = initial_output[x];
	}
	else
		out_size = -1;
}

bool DoubleDataPair::getInputValues(double* values) {
	if (in_val == NULL || values == NULL)
		return false;

	for (int x = 0; x < in_size; x++)
		values[x] = in_val[x];

	return true;
}

bool DoubleDataPair::setInputValues(int size, double* values) {
	if (size <= 0 || values == NULL)
		return false;

	if (in_val != NULL)
		delete in_val;

	in_size = size;
	in_val = new double[in_size];

	for (int x = 0; x < in_size; x++)
		in_val[x] = values[x];

	return true;
}

bool DoubleDataPair::getOutputValues(double* values) {
	if (out_val == NULL || values == NULL)
		return false;

	for (int x = 0; x < out_size; x++)
		values[x] = out_val[x];

	return true;
}

bool DoubleDataPair::setOutputValues(int size, double* values) {
	if (size <= 0 || values == NULL)
		return false;

	if (out_val != NULL)
		delete out_val;

	out_size = size;
	out_val = new double[out_size];

	for (int x = 0; x < out_size; x++)
		out_val[x] = values[x];

	return true;
}
