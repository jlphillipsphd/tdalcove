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
#include "td_layer.h"
#include <stddef.h>
#include <iostream>
#include <iomanip>

using namespace std;

TDLayer::TDLayer() : Layer() {
	gamma = 1.0;
	last_acts = NULL;
	last_nets = NULL;
	td_error = NULL;
	some_vec = NULL;
	first_step = true;
}

TDLayer::~TDLayer() {
	cout << "Start TDLayer" << endl;
	if (some_vec != NULL)
		delete some_vec;
	cout << "Mark" << endl;

	if (td_error != NULL)
		delete td_error;
	cout << "Mark" << endl;

	if (last_acts != NULL)
		delete last_acts;
	cout << "Mark" << endl;

	if (last_nets != NULL) {
		for (int x = 0; x < Size(); x++)
			delete last_nets[x];
		delete last_nets;
	}
	cout << "Finish TDLayer" << endl;
} // destructor

TDLayer::TDLayer(int number_of_units, ActivationFunction*
saf, double discount_rate) : Layer(number_of_units, saf) {

	if (number_of_units <= 0) {
		gamma = 1.0;
		last_acts = NULL;
		last_nets = NULL;
		td_error = NULL;
		some_vec = NULL;
		first_step = true;
		return;
	}

	gamma = discount_rate;
	some_vec = new double[number_of_units];
	td_error = new double[number_of_units];
	last_acts = new double[number_of_units];
	last_nets = new double*[number_of_units];
	for (int x = 0; x < number_of_units; x++)
		last_nets[x] = new double[in_dims];
	first_step = true;

}

bool TDLayer::computeTDError(double* reward) {
	return computeTDError(reward, false);
}

bool TDLayer::computeTDError(double* reward, bool final_round) {

	int x, y;

	for (x = 0; x < Size(); x++)
		td_error[x] = 0.0;

	if (first_step) {
		first_step = false;
	}
	else {
		getActs(some_vec);
		for (x = 0; x < Size(); x++) {
			if (final_round) {
				td_error[x] = reward[x] - last_acts[x]; }
			else {
				td_error[x] = reward[x] + (gamma * some_vec[x]) - last_acts[x]; }
		}

		if (final_round)
			first_step = true;
	}

	return true;
}

bool TDLayer::Reset() {

	if (first_step)
		return false;

	first_step = true;
	return false;
}

bool TDLayer::setDiscountRate(double g) {
	gamma = g;
	return true;
}

double TDLayer::getDiscountRate() {
	return gamma;
}

bool TDLayer::getTDError(double* vals) {
	if (td_error == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		vals[x] = td_error[x];

	return true;;
}

bool TDLayer::setTDError(int index, double val) {
	if (td_error == NULL || index < 0 || index >= Size())
		return false;

	td_error[index] = val;
	return true;
}

double TDLayer::getTDError(int index) {
	if (td_error == NULL || index < 0 || index >= Size())
		return false;

	return td_error[index];
}

bool TDLayer::getLastActs(double* vals) {
	if (last_acts == NULL || vals == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		vals[x] = last_acts[x];

	return true;
}

bool TDLayer::setLastActs(double* vals) {
	if (vals == NULL || last_acts == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		last_acts[x] = vals[x];

	return true;
}

bool TDLayer::computeTDDelta(int unit) {
	if (last_acts == NULL || unit < 0 || unit >= Size())
		return false;

	int x, y;

	for (x = 0; x < Size(); x++) {
		for (y = 0; y < in_dims; y++)
			work_vec2[y] = 0;
		if (unit == x) {
			for (y = 0; y < in_dims; y++)
				work_vec2[y] = act_func->Derivative(y, last_nets[x]);
			getUnit(x)->setDerivatives(work_vec2);
			getUnit(x)->setDeltas(work_vec2);
		}
		else {
			getUnit(x)->setDerivatives(work_vec2);
			getUnit(x)->setDeltas(work_vec2);
		}
	} // end for

	return true;
}

bool TDLayer::incrementTimer() {
		if (last_acts == NULL || last_nets == NULL)
			return false;

		getActs(last_acts);
		getNets(last_nets);

		return true;
}
