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
#include "td_cubic_coarse_code_layer.h"

TDCubicCoarseCodeLayer::TDCubicCoarseCodeLayer() : TDCoarseCodeLayer() {
	return;
}

TDCubicCoarseCodeLayer::TDCubicCoarseCodeLayer(CubicActivationFunction* caf,
					int* number_of_units_per_dimension,
					double* minimums_per_dimension,
					double* maximums_per_dimension,
					ActivationFunction* af, double discount_rate) :
	TDCoarseCodeLayer(caf, number_of_units_per_dimension, minimums_per_dimension, maximums_per_dimension, af, discount_rate) {
	if (caf != NULL && minimums_per_dimension != NULL && maximums_per_dimension != NULL) {
		int dims = caf->getNumberOfInputDimensions();
		for (int x = 0; x < dims; x++)
			caf->setDomain(x, (computeStep(number_of_units_per_dimension[x], minimums_per_dimension[x], maximums_per_dimension[x]) / 2.0));
		caf->biasMax();
		cc_max = caf->getMax();
		cc_min = caf->getMin();
	}
}

bool TDCubicCoarseCodeLayer::setCoarseActs(double* position) {
	bool return_value = ((TDCoarseCodeLayer*) this)->setCoarseActs(position);
	
	if (!return_value)
		return false;

	applyWTA(cc_min, cc_max);

	return true;
}
