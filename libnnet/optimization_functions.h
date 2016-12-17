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
#ifndef OPTIMIZATION_FUNCTIONS_H
#define OPTIMIZATION_FUNCTIONS_H

#include "parameter.h"
#include "double_parameter.h"
#include "integer_parameter.h"
#include "string_parameter.h"
#include "data_pair.h"
#include "double_data_pair.h"
#include "integer_data_pair.h"
#include "string_data_pair.h"
#include <stdlib.h>
#include <stddef.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>

using namespace std;

list<Parameter*>* readParameters(string filename);
// Creates a list of parameters based on an input file like so...

// number_of_parameters
// parameter_type initial_value minimum_value maximum_value
// ...			  ...			...			  ...
//
// To handle string parameters do this:
// parameter_type initial_value number_of_legal_values value1 value2 ...

// Where:
// number_of_parameters - int
// parameter_type - int (see parameter.h for type assignments)
// initial_value - double, int, or string(no whitespace)
// minimum_value - double or int
// maximum_value - double or int
// number_of_legal_values - int
// value1 - string(no whitespace)

list<DataPair*>* readDataPairs(string filename);
// Creates a list of data input and output pairs based on an input file...

// number_of_pairs
// data_pair_type number_of_input_values number_of_output_values
// input_value1 input_value2 ...
// output_value1 output_value2 ...
// ... 			  ...					 ...
// ...		    ...			 ...
// ...			 ...		   ...

// Where:
// number_of_pairs - int
// data_pair_type - int (see data_pair.h for type assignments)
// number_of_input_values - int
// number_of_output_values - int
// input_valuex - double, int, or string(no whitespace)
// output_valuex - double, int, or string(no whitespace)

bool randomizeParameterValues(list<Parameter*> &parameters);
// Performs a randomization of the parameters within their legal
// bounds. Used to perform the classic "random restart" of many
// optimization schemes.

void printParameterList(list<Parameter*> &parameters);
// Displays the list of parameters (used for debugging mainly)

void printDataPairList(list<DataPair*> &pairs);
// Displays the list of data pairs (used for debugging mainly)

bool writeDataPairList(list<DataPair*> &pairs, string filename);
//Writes the data pair list out in a format compatible with the
//function readDataPairList().

bool writeParameterList(list<Parameter*> &parameters, string filename);
//Writes the pararmeter list out in a format compatible with the
//function readParameterList().

#endif
