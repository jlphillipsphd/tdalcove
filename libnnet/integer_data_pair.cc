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
#include "integer_data_pair.h"

IntegerDataPair::IntegerDataPair() : DataPair(DataPair::TYPE_DOUBLE) {
	in_val = NULL;
	out_val = NULL;
	in_size = -1;
	out_size = -1;
}

IntegerDataPair::~IntegerDataPair() {
	if (in_val != NULL)
		delete in_val;
	if (out_val != NULL)
		delete out_val;
}

IntegerDataPair::IntegerDataPair(int input_size, int* initial_input, int output_size, int* initial_output) : DataPair(DataPair::TYPE_DOUBLE) {

	int x;

	in_val = NULL;
	out_val = NULL;

	if (input_size > 0 && initial_input != NULL) {
		in_size = input_size;
		in_val = new int[in_size];
		for (x = 0; x < in_size; x++)
			in_val[x] = initial_input[x];
	}
	else
		in_size = -1;

	if (output_size > 0 && initial_output != NULL) {
		out_size = output_size;
		out_val = new int[out_size];
		for (x = 0; x < out_size; x++)
			out_val[x] = initial_output[x];
	}
	else
		out_size = -1;

}

bool IntegerDataPair::getInputValues(int* values) {
	if (in_val == NULL || values == NULL)
		return false;

	for (int x = 0; x < in_size; x++)
		values[x] = in_val[x];

	return true;
}

bool IntegerDataPair::setInputValues(int size, int* values) {
	if (size <= 0 || values == NULL)
		return false;

	if (in_val != NULL)
		delete in_val;

	in_size = size;
	in_val = new int[in_size];

	for (int x = 0; x < in_size; x++)
		in_val[x] = values[x];

	return true;
}

bool IntegerDataPair::getOutputValues(int* values) {
	if (out_val == NULL || values == NULL)
		return false;

	for (int x = 0; x < out_size; x++)
		values[x] = out_val[x];

	return true;
}

bool IntegerDataPair::setOutputValues(int size, int* values) {
	if (size <= 0 || values == NULL)
		return false;

	if (out_val != NULL)
		delete out_val;

	out_size = size;
	out_val = new int[out_size];

	for (int x = 0; x < out_size; x++)
		out_val[x] = values[x];

	return true;
}
