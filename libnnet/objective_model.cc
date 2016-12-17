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
#include "objective_model.h"

ObjectiveModel::ObjectiveModel() : Object() {
}

ObjectiveModel::ObjectiveModel(list<unsigned int> &seed_list) : Object() {
	for (list<unsigned int>::iterator itr = seed_list.begin();
			itr != seed_list.end(); itr++)
		seeds.push_back((*itr));
}

double ObjectiveModel::sumRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data) {
	if (seeds.size() == 0)
		return singleRun(parameters, obj_func, real_data, 0);

	double total = 0.0;

	for (list<unsigned int>::iterator itr = seeds.begin();
			itr != seeds.end(); itr++) {
		total += singleRun(parameters, obj_func, real_data, *itr);
	}

	return total;
}

double ObjectiveModel::avgRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data) {
	if (seeds.size() == 0)
		return singleRun(parameters, obj_func, real_data, 0);

	double total = 0.0;

	for (list<unsigned int>::iterator itr = seeds.begin();
			itr != seeds.end(); itr++) {
		total += singleRun(parameters, obj_func, real_data, *itr);
	}

	return total / (double) seeds.size();
}

double ObjectiveModel::maxRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data) {
	if (seeds.size() == 0)
		return singleRun(parameters, obj_func, real_data, 0);

	double total = -50000000.0;

	for (list<unsigned int>::iterator itr = seeds.begin();
			itr != seeds.end(); itr++) {
		double val = singleRun(parameters, obj_func, real_data, *itr);
		if (val > total)
			total = val;
	}

	return total;
}

double ObjectiveModel::minRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data) {
	if (seeds.size() == 0)
		return singleRun(parameters, obj_func, real_data, 0);

	double total = 50000000.0;

	for (list<unsigned int>::iterator itr = seeds.begin();
			itr != seeds.end(); itr++) {
		double val = singleRun(parameters, obj_func, real_data, *itr);
		if (val < total)
			total = val;
	}

	return total;
}

