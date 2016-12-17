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
#include "projection.h"
#include "layer.h"
#include <stddef.h>
#include <iostream>

using namespace std;

Projection::Projection() : Object() {
	from = NULL;
	to = NULL;
	dim = -1;
}

Projection::Projection(Layer* from_layer, Layer* to_layer, int dimension) : Object() {
	if (from_layer == NULL || to_layer == NULL) {
		from = NULL;
		to = NULL;
		dim = -1;
		return;
	}

	if (dimension < 0 || dimension >= to_layer->getNumberOfInputDimensions()) {
		from == NULL;
		to == NULL;
		dim = -1;
		return;
	}

	dim = dimension;
	from = from_layer;
	to = to_layer;
	tl_size = to_layer->Size();
	fl_size = from_layer->Size();
}

Layer* Projection::getFromLayer() {
	return from;
}

Layer* Projection::getToLayer() {
	return to;
}

