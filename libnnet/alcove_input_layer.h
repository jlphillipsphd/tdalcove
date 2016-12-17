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
#include "layer.h"
#include "linear_activation_function.h"

#ifndef ALCOVE_INPUT_LAYER_H
#define ALCOVE_INPUT_LAYER_H

class ALCOVEInputLayer : public Layer {

private:
	double* attention_strengths;
	double* d_attention_strengths;
	LinearActivationFunction* laf;
	LinearActivationFunction* createAct();

public:
	ALCOVEInputLayer();
	~ALCOVEInputLayer();
	ALCOVEInputLayer(int number_of_units);
	ALCOVEInputLayer(int number_of_units, double* attentional_strengths);

	bool setAttentionalStrengths(double* as);
	bool getAttentionalStrengths(double* as);

	bool setAttentionalStrength(int pos, double val);
	double getAttentionalStrength(int pos);

	bool setDeltaAttentionalStrengths(double* as);
	bool getDeltaAttentionalStrengths(double* as);

	bool setDeltaAttentionalStrength(int pos, double val);
	double getDeltaAttentionalStrength(int pos);

	bool updateAttentionalStrengths();

	bool clearDeltaAttentionalStrengths();

};

#endif
