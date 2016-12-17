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
#ifndef INTEGRAL_CMACS_TD_ALCOVE_OBJECTIVE_MODEL_H
#define INTEGRAL_CMACS_TD_ALCOVE_OBJECTIVE_MODEL_H

#include "objective_model.h"
#include "cmacs_td_alcove_input_layer.h"
#include "alcove_hidden_layer.h"
#include "alcove_output_layer.h"
#include "bp_full_forward_projection.h"
#include "uniform_random_number_generator.h"
#include "nnet_math.h"
#include "parameter.h"
#include "double_parameter.h"
#include "data_pair.h"
#include "double_data_pair.h"
#include "objective_function.h"
#include "optimization_functions.h" //printing
#include "index_permuter.h"
#include <list>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

class IntegralCMACS_TD_ALCOVE_ObjectiveModel : public ObjectiveModel {

public:
	IntegralCMACS_TD_ALCOVE_ObjectiveModel();
	IntegralCMACS_TD_ALCOVE_ObjectiveModel(int problem_number);
	IntegralCMACS_TD_ALCOVE_ObjectiveModel(int problem_number, list<unsigned int> &seed_list);

	double  singleRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data, unsigned int seed);

private:
	int problem;

};

#endif
