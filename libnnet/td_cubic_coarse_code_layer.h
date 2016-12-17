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
#ifndef TD_CUBIC_COARSE_CODE_LAYER_H
#define TD_CUBIC_COARSE_CODE_LAYER_H

#include "td_coarse_code_layer.h"
#include "cubic_activation_function.h"
#include <stddef.h>

class TDCubicCoarseCodeLayer : public TDCoarseCodeLayer {

public:
	TDCubicCoarseCodeLayer();
	// This class alters the domains of the cubic activation function
	// in question in order to fill the space provided. However, all other
	// attributes for the function are maintained.
	TDCubicCoarseCodeLayer(CubicActivationFunction* caf,
			int* number_of_units_per_dimension,
			double* minimums_per_dimension,
			double* maximums_per_dimension,
			ActivationFunction* af, double discount_rate);

	bool setCoarseActs(double* position);

private:
	double cc_max;
	double cc_min;
};

#endif
