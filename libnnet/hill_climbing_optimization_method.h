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
#ifndef HILL_CLIMBING_METHOD_H
#define HILL_CLIMBING_METHOD_H

#define DOUBLE_DIVISIONS 70
#define INTEGER_STEP 1

// Set to zero to not print intermittent results
#define DISPLAY_OBJECTIVE_VALUES 0

#include "optimization_method.h"
#include "parameter.h"
#include "double_parameter.h"
#include "integer_parameter.h"
#include "string_parameter.h"
#include "data_pair.h"
#include "objective_model.h"
#include "objective_function.h"
#include "optimization_functions.h"
#include <math.h>
#include <list>
#include <string>
#include <iostream>

using namespace std;

class HillClimbingOptimizationMethod : public OptimizationMethod {

private:
	int current_parameter;
	double d_last_val;
	double d_last_adj;
	int i_last_val;
	int i_last_adj;
	string s_last_val;
	list<string> s_done;
	bool first_run;
	int run_type;
	int* last_run;

	double runFunc();

public:
	HillClimbingOptimizationMethod();
	~HillClimbingOptimizationMethod();
	HillClimbingOptimizationMethod(list<Parameter*> *model_parameters, ObjectiveModel* objective_model, list<DataPair*> *real_data, ObjectiveFunction* objective_function, int the_run_type);

	bool maximize();
	bool minimize();

	bool maximize(int number_of_starts);
	bool minimize(int number_of_starts);

};

#endif
