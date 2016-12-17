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
#include "gaussian_random_number_generator.h"

GaussianRandomNumberGenerator::GaussianRandomNumberGenerator() : RandomNumberGenerator() {
	min = -1.0;
	max = 1.0;
	make_new = true;
}

GaussianRandomNumberGenerator::GaussianRandomNumberGenerator(double minimum, double maximum) : RandomNumberGenerator() {
	min = -1.0;
	max = 1.0;
	make_new = true;

	if (minimum < maximum) {
		min = minimum;
		max = maximum;
	}
}

double GaussianRandomNumberGenerator::Number() {

	double return_value;
	double x1, x2, w, y1, y2;

	if (make_new) {
		/*
		do {
			x1 = 2.0 * drand48() - 1.0;
			x2 = 2.0 * drand48() - 1.0;
			w = x1 * x1 + x2 * x2;
		} while ( w >= 0.1 );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		*/

		// Incorrect way
		x1 = drand48();
		x2 = drand48();
		y1 = sqrt( -2.0 * log(x1) ) * cos( 2.0 * M_PI * x2 );
	    y2 = sqrt( -2.0 * log(x1) ) * sin( 2.0 * M_PI * x2 );

		return_value = clip(y1, -4.0, 4.0);
		val = clip(y2, -4.0, 4.0);

		return_value /= 8.0;
		val /= 8.0;

		return_value += 0.5;
		val += 0.5;

		double scale = (max - min);

		return_value = (scale * return_value) + min;
		val = (scale * val) + min;

		make_new = false;
	}
	else {
		return_value = val;
		make_new = true;
	}

	return return_value;
}
