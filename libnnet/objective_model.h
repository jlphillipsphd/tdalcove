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
#ifndef OBJECTIVE_MODEL_H
#define OBJECTIVE_MODEL_H

#include "object.h"
#include "parameter.h"
#include "objective_function.h"
#include "data_pair.h"
#include <list>
#include <stdlib.h>

using namespace std;

class ObjectiveModel : public Object {

private:
	list<unsigned int> seeds;

public:
	ObjectiveModel();
	ObjectiveModel(list<unsigned int> &seed_list);

	virtual double singleRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data, unsigned int seed) = 0;

	double sumRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data);
	double avgRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data);
	double maxRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data);
	double minRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data);
};

#endif
