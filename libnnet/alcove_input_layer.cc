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
#include "alcove_input_layer.h"
#include "linear_activation_function.h"
#include "layer.h"
#include <stddef.h>
#include <iostream>

using namespace std;

ALCOVEInputLayer::ALCOVEInputLayer() : Layer() {
	attention_strengths = NULL;
	d_attention_strengths = NULL;
	laf = NULL;
}

ALCOVEInputLayer::~ALCOVEInputLayer() {
	if (attention_strengths != NULL)
		delete attention_strengths;
	if (d_attention_strengths != NULL)
		delete d_attention_strengths;
	if (laf != NULL)
		delete laf;
}

ALCOVEInputLayer::ALCOVEInputLayer(int number_of_units) : Layer(number_of_units, createAct()) {

	attention_strengths = NULL;
	d_attention_strengths = NULL;

	if (number_of_units > 0) {
		attention_strengths = new double[number_of_units];
		d_attention_strengths = new double[number_of_units];
		for (int x = 0; x < number_of_units; x++) {
			attention_strengths[x] = 0.0;
			d_attention_strengths[x] = 0.0;
		} // end for
	} // end if
}

ALCOVEInputLayer::ALCOVEInputLayer(int number_of_units, double* attentional_strengths) : Layer(number_of_units, createAct()) {

	attention_strengths = NULL;
	d_attention_strengths = NULL;

	if (number_of_units > 0) {
		attention_strengths = new double[number_of_units];
		d_attention_strengths = new double[number_of_units];

		if (attentional_strengths != NULL) {
			for (int x = 0; x < number_of_units; x++) {
				attention_strengths[x] = attentional_strengths[x];
				d_attention_strengths[x] = 0.0;
			}
		}
		else {
			for (int x = 0; x < number_of_units; x++) {
				attention_strengths[x] = 0.0;
				d_attention_strengths[x] = 0.0;
			}
		}
	} // end if
}

bool ALCOVEInputLayer::setAttentionalStrengths(double* as) {
	
	if (as == NULL || attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++) {
		attention_strengths[x] = as[x];
		if (as[x] < 0.0)
			attention_strengths[x] = 0.0;
		// Addition to ALCOVE
		if (as[x] > 1.0)
			attention_strengths[x] = 1.0;
	}

	return true;
}

bool ALCOVEInputLayer::getAttentionalStrengths(double* as) {
	
	if (as == NULL || attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		as[x] = attention_strengths[x];

	return true;
}

bool ALCOVEInputLayer::setAttentionalStrength(int pos, double val) {
	if (pos < 0 || pos >= Size() || attention_strengths == NULL)
		return false;

	attention_strengths[pos] = val;

	if (val < 0.0)
		attention_strengths[pos] = 0.0;
	// Addition to ALCOVE
	if (val > 1.0)
		attention_strengths[pos] = 1.0;

	return true;
}

double ALCOVEInputLayer::getAttentionalStrength(int pos) {
	if (pos < 0 || pos >= Size() || attention_strengths == NULL)
		return 0.0;

	return attention_strengths[pos];
}

bool ALCOVEInputLayer::setDeltaAttentionalStrengths(double* as) {
	
	if (as == NULL || attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++) {
		d_attention_strengths[x] = as[x];
		if (as[x] < 0.0)
			attention_strengths[x] = 0.0;
		// Addition to ALCOVE
		if (as[x] > 1.0)
			attention_strengths[x] = 1.0;
	}

	return true;
}

bool ALCOVEInputLayer::getDeltaAttentionalStrengths(double* as) {
	
	if (as == NULL || attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		as[x] = d_attention_strengths[x];

	return true;
}

bool ALCOVEInputLayer::setDeltaAttentionalStrength(int pos, double val) {
	if (pos < 0 || pos >= Size() || attention_strengths == NULL)
		return false;

	d_attention_strengths[pos] = val;

	return true;
}

double ALCOVEInputLayer::getDeltaAttentionalStrength(int pos) {
	if (pos < 0 || pos >= Size() || attention_strengths == NULL)
		return 0.0;

	return d_attention_strengths[pos];
}

bool ALCOVEInputLayer::updateAttentionalStrengths() {

	if (attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++) {
		attention_strengths[x] += d_attention_strengths[x];
		if (attention_strengths[x] < 0.0)
			attention_strengths[x] = 0.0;
		// Addition to ALCOVE
		if (attention_strengths[x] > 1.0)
			attention_strengths[x] = 1.0;
	}

	clearDeltaAttentionalStrengths();

	return true;
}

bool ALCOVEInputLayer::clearDeltaAttentionalStrengths() {
	if (d_attention_strengths == NULL)
		return false;

	for (int x = 0; x < Size(); x++)
		d_attention_strengths[x] = 0.0;

	return true;
}

LinearActivationFunction* ALCOVEInputLayer::createAct() {
	laf = new LinearActivationFunction();
	return laf;
}
