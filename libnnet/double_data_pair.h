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
#ifndef DOUBLE_DATA_PAIR_H
#define DOUBLE_DATA_PAIR_H

#include "data_pair.h"
#include <stddef.h>
#include <iostream>

using namespace std;

class DoubleDataPair : public DataPair {

private:
	double* in_val;
	double* out_val;

public:
	DoubleDataPair();
	~DoubleDataPair();
	DoubleDataPair(int input_size, double* initial_input, int output_size, double* initial_output);

	bool getInputValues(double* values);
	bool setInputValues(int size, double* values);

	bool getOutputValues(double* values);
	bool setOutputValues(int size, double* values);

};

#endif
