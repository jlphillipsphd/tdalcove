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
#include "cmacs_coarse_code_layer_set.h"

CMACS_CoarseCodeLayerSet::CMACS_CoarseCodeLayerSet() : Object() {
	layers = NULL;
}

CMACS_CoarseCodeLayerSet::~CMACS_CoarseCodeLayerSet() {

	int x;

	if (layers != NULL) {
		for (x = 0; x < tiles; x++) {
			delete layers[x];
		}

		for (x = 0; x < cmac_dim; x++) {
			delete a_pos[x];
			delete a_pos2[x];
		}

		delete layers;
		delete side_units;
		delete code_domain;
		delete code_step;
		delete a_pos;
		delete a_pos2;
		delete candidate;
		delete as;
		delete real_pos;
		delete the_one;
		delete caf;
		delete layer_work_vec;
		delete temp;

	}
}

CMACS_CoarseCodeLayerSet::CMACS_CoarseCodeLayerSet(int dimensionality,
		int number_of_tilings,
		int* number_of_units_per_tile_dimension,
		double* minimums_per_dimension,
		double* maximums_per_dimension,
		ActivationFunction* af) : Object() {

	int x;

	if (dimensionality < 1 || number_of_tilings < 1 || number_of_units_per_tile_dimension == NULL || minimums_per_dimension == NULL || maximums_per_dimension == NULL || af == NULL) {
		layers = NULL;
		return;
	}

	for (x = 0; x < number_of_tilings; x++)
		if (number_of_units_per_tile_dimension[x] < 2 || minimums_per_dimension[x] >= maximums_per_dimension[x]) {
			layers = NULL;
			return;
		}

	tiles = number_of_tilings;
	cmac_dim = dimensionality;
	power_size = (int) pow(2.0, (double) cmac_dim);
	side_units = new int[cmac_dim];
	double* code_span = new double[cmac_dim];
	double* code_mins = new double[cmac_dim];
	double* code_maxes = new double[cmac_dim];
	code_domain = new double[cmac_dim];
	code_step = new double[cmac_dim];
	a_pos = new double*[cmac_dim];
	a_pos2 = new double*[cmac_dim];
	as = new double[cmac_dim];
	real_pos = new double[cmac_dim];
	for (x = 0; x < cmac_dim; x++) {
		side_units[x] = number_of_units_per_tile_dimension[x];
		code_span[x] = (maximums_per_dimension[x] - minimums_per_dimension[x]);
		code_mins[x] = minimums_per_dimension[x] - ((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_maxes[x] = maximums_per_dimension[x] - ((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_domain[x] = ((code_maxes[x] - code_mins[x]) / (double) (side_units[x] - 1)) / 2.0;
		code_step[x] = code_domain[x] / (double) (tiles - 1);
		as[x] = 0;
		real_pos[x] = 0;
		a_pos[x] = new double[power_size];
		a_pos2[x] = new double[power_size];
		for (int y = 0; y < power_size; y++) {
			a_pos[x][y] = 0.0;
			a_pos2[x][y] = 0.0;
		}
	}

	candidate = new int[power_size];
	for (x = 0; x < power_size; x++)
		candidate[x] = 0;

	the_one = new int[tiles];
	temp = new double[tiles];
	caf = new CubicActivationFunction(cmac_dim);
	layers = new CubicCoarseCodeLayer*[tiles];
	for (x = 0; x < tiles; x++) {
		layers[x] = new CubicCoarseCodeLayer(caf, side_units, code_mins, code_maxes, af);
		the_one[x] = 0;
		temp[x] = 0;
		for (int y = 0; y < cmac_dim; y++) {
			code_mins[y] += code_step[y];
			code_maxes[y] += code_step[y];
		}
	}

	the_tile = 0;
	layer_work_vec = new double[layers[0]->Size()];

	delete code_span;
	delete code_mins;
	delete code_maxes;
}

CMACS_CoarseCodeLayerSet::CMACS_CoarseCodeLayerSet(int dimensionality,
												   int number_of_tilings,
												   int number_of_units_per_tile_dimension,
												   ActivationFunction* af) : Object() {

	int x;

	if (dimensionality < 1 || number_of_tilings < 1 || number_of_units_per_tile_dimension < 2 || af == NULL) {
		layers = NULL;
		return;
	}

	tiles = number_of_tilings;
	cmac_dim = dimensionality;
	power_size = (int) pow(2.0, (double) cmac_dim);
	side_units = new int[cmac_dim];
	double* code_span = new double[cmac_dim];
	double* code_mins = new double[cmac_dim];
	double* code_maxes = new double[cmac_dim];
	code_domain = new double[cmac_dim];
	code_step = new double[cmac_dim];
	a_pos = new double*[cmac_dim];
	a_pos2 = new double*[cmac_dim];
	as = new double[cmac_dim];
	real_pos = new double[cmac_dim];
	for (x = 0; x < cmac_dim; x++) {
		side_units[x] = number_of_units_per_tile_dimension;
		code_span[x] = 1.0;
		code_mins[x] = -((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_maxes[x] = 1.0 - ((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_domain[x] = ((code_maxes[x] - code_mins[x]) / (double) (side_units[x] - 1)) / 2.0;
		code_step[x] = code_domain[x] / (double) (tiles - 1);
		as[x] = 0;
		real_pos[x] = 0;
		a_pos[x] = new double[power_size];
		a_pos2[x] = new double[power_size];
		for (int y = 0; y < power_size; y++) {
			a_pos[x][y] = 0.0;
			a_pos2[x][y] = 0.0;
		}
	}

	candidate = new int[power_size];
	for (x = 0; x < power_size; x++)
		candidate[x] = 0;

	the_one = new int[tiles];
	temp = new double[tiles];
	caf = new CubicActivationFunction(cmac_dim);
	layers = new CubicCoarseCodeLayer*[tiles];
	for (x = 0; x < tiles; x++) {
		layers[x] = new CubicCoarseCodeLayer(caf, side_units, code_mins, code_maxes, af);
		the_one[x] = 0;
		temp[x] = 0.0;
		for (int y = 0; y < cmac_dim; y++) {
			code_mins[y] += code_step[y];
			code_maxes[y] += code_step[y];
		}
	}

	the_tile = 0;
	layer_work_vec = new double[layers[0]->Size()];

	delete code_span;
	delete code_mins;
	delete code_maxes;
}

CMACS_CoarseCodeLayerSet::CMACS_CoarseCodeLayerSet(int dimensionality,
												   int number_of_tilings,
												   int number_of_units_per_tile_dimension,
												   double minimum_per_dimension,
												   double maximum_per_dimension,
												   ActivationFunction* af) : Object() {

	int x;

	if (dimensionality < 1 || number_of_tilings < 1 || number_of_units_per_tile_dimension < 2 || minimum_per_dimension >= maximum_per_dimension || af == NULL) {
		layers = NULL;
		return;
	}

	tiles = number_of_tilings;
	cmac_dim = dimensionality;
	power_size = (int) pow(2.0, (double) cmac_dim);
	side_units = new int[cmac_dim];
	double* code_span = new double[cmac_dim];
	double* code_mins = new double[cmac_dim];
	double* code_maxes = new double[cmac_dim];
	code_domain = new double[cmac_dim];
	code_step = new double[cmac_dim];
	a_pos = new double*[cmac_dim];
	a_pos2 = new double*[cmac_dim];
	as = new double[cmac_dim];
	real_pos = new double[cmac_dim];
	for (x = 0; x < cmac_dim; x++) {
		side_units[x] = number_of_units_per_tile_dimension;
		code_span[x] = (maximum_per_dimension - minimum_per_dimension);
		code_mins[x] = minimum_per_dimension - ((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_maxes[x] = maximum_per_dimension - ((code_span[x] / ((double) side_units[x] - 1.0)) / 2.0);
		code_domain[x] = ((code_maxes[x] - code_mins[x]) / (double) (side_units[x] - 1)) / 2.0;
		code_step[x] = code_domain[x] / (double) (tiles - 1);
		as[x] = 0;
		real_pos[x] = 0;
		a_pos[x] = new double[power_size];
		a_pos2[x] = new double[power_size];
		for (int y = 0; y < power_size; y++) {
			a_pos[x][y] = 0.0;
			a_pos2[x][y] = 0.0;
		}
	}

	candidate = new int[power_size];
	for (x = 0; x < power_size; x++)
		candidate[x] = 0;

	the_one = new int[tiles];
	temp = new double[tiles];
	caf = new CubicActivationFunction(cmac_dim);
	layers = new CubicCoarseCodeLayer*[tiles];
	for (x = 0; x < tiles; x++) {
		layers[x] = new CubicCoarseCodeLayer(caf, side_units, code_mins, code_maxes, af);
		the_one[x] = 0;
		temp[x] = 0.0;
		for (int y = 0; y < cmac_dim; y++) {
			code_mins[y] += code_step[y];
			code_maxes[y] += code_step[y];
		}
	}

	the_tile = 0;
	layer_work_vec = new double[layers[0]->Size()];

	delete code_span;
	delete code_mins;
	delete code_maxes;
}

bool CMACS_CoarseCodeLayerSet::getCoarseActs(double* pos) {

	if (layers == NULL || pos == NULL)
		return false;

	double t_val;
	int x,y,z,i,j;

	for (y = 0; y < tiles; y++) {
		layers[y]->getActs(layer_work_vec);

		t_val = -200000.0;
		for (z = 0; z < layers[y]->Size(); z++)
			if (layer_work_vec[z] > t_val) {
				t_val = layer_work_vec[z];
				temp[y] = t_val;
				the_one[y] = z;
			}
	} // end for (y)

	t_val = -200000.0;
	for (z = 0; z < tiles; z++) {
		if (temp[z] > t_val) {
			t_val = temp[z];
			the_tile = z;
		}
	} // end for (z)

	//Get Position of "the top one"
	layers[the_tile]->getUnit(the_one[the_tile])->getPosition(as);

	//Find bounding box - very complex (HAHA!)
	int pivot_index = power_size / 2;
	for (x = 0; x < cmac_dim; x++) {
		for (y = 0; y < power_size; y++) {
			if ((y / pivot_index) % 2 == 0)
				a_pos[x][y] = as[x] + code_domain[x];
			else
				a_pos[x][y] = as[x] - code_domain[x];
		}

		pivot_index /= 2;
	}

	for (y = 0; y < tiles; y++) {
		if (y != the_tile) {
			for (z = 0; z < power_size; z++) {
				for (x = 0; x < cmac_dim; x++) {
					as[x] = a_pos[x][z];
				}

				t_val = 200000.0;
				// Find closest unit to winning unit's position
				for (i = 0; i < layers[y]->Size(); i++) {

					double a_dist = 0.0;
					layers[y]->getUnit(i)->getPosition(real_pos);
					for (j = 0; j < cmac_dim; j++) {
						a_dist += (as[j] - real_pos[j]) * (as[j] - real_pos[j]);
					}
					a_dist = sqrt(a_dist);

					if (a_dist < t_val) {
						t_val = a_dist;
						candidate[z] = i;
					}
				} // end for (i)
			} // end for (z)

			t_val = -200000.0;
			for (z = 0; z < power_size; z++) {
				if (layers[y]->getUnit(candidate[z])->getAct() > t_val) {
					t_val = layers[y]->getUnit(candidate[z])->getAct();
					the_one[y] = candidate[z];
				}
			} // end for (z)

			// Shrink the zone
			layers[y]->getUnit(the_one[y])->getPosition(as);
			
			pivot_index = power_size / 2;
			for (x = 0; x < cmac_dim; x++) {
				for (z = 0; z < power_size; z++) {
					if ((z / pivot_index) % 2 == 0) {
						a_pos2[x][z] = as[x] + code_domain[x];
					}
					else {
						a_pos2[x][z] = as[x] - code_domain[x];
					}
				} // end for (z)

				pivot_index /= 2;
			}

			// Compute mean of bounding box
			for (z = 0; z < cmac_dim; z++) {
				real_pos[z] = 0.0;
			} // end for (z)

			for (z = 0; z < power_size; z++) {
				for (x = 0; x < cmac_dim; x++) {
					real_pos[x] += a_pos[x][z];
				} // end for (x)
			} // end for (z)

			for (x = 0; x < cmac_dim; x++) {
				real_pos[x] /= pow(2.0, (double) cmac_dim);
			} // end for (x)

			// Find closest points to means
			t_val = 200000.0;
			for (z = 0; z < power_size; z++) {
				some_dist = 0.0;
				for (x = 0; x < cmac_dim; x++) {
					some_dist += (a_pos[x][z] - as[x]) * (a_pos[x][z] - as[x]);
				} // end for (x)

				some_dist = sqrt(some_dist);

				if (some_dist < t_val) {
					t_val = some_dist;
					bound_corner = z;
				} // end if
			} // end for (z)

			t_val = 200000.0;
			for (z = 0; z < power_size; z++) {
				some_dist = 0.0;
				for (x = 0; x < cmac_dim; x++) {
					some_dist += pow(a_pos2[x][z] - real_pos[x], 2.0);
				} // end for (x)

				some_dist = sqrt(some_dist);

				if (some_dist < t_val) {
					t_val = some_dist;
					new_corner = z;
				} // end if
			} // end for (z)

			// Original
			for (x = 0; x < cmac_dim; x++) {
				real_pos[x] = a_pos[x][bound_corner];
			}

			// This tile
			for (x = 0; x < cmac_dim; x++) {
				as[x] = a_pos2[x][new_corner];
			}

			// Rebuild box
			pivot_index = power_size / 2;
			for (x = 0; x < cmac_dim; x++) {
				for (z = 0; z < power_size; z++) {
					if ((z / pivot_index) % 2 == 0) {
						a_pos[x][z] = as[x];
					}
					else {
						a_pos[x][z] = real_pos[x];
					}
				} // end for (y)

				pivot_index /= 2;
			} // end for (x)
		} // end if
	} // end for (y)

	// Compute mean of bounding box
	for (z = 0; z < cmac_dim; z++) {
		as[z] = 0.0;
	} // end for (z)

	for (z = 0; z < power_size; z++) {
		for (x = 0; x < cmac_dim; x++) {
			as[x] += a_pos[x][z];
		} // end for (x)
	} // end for (z)

	for (z = 0; z < cmac_dim; z++) {
		as[z] /= pow(2.0, (double) cmac_dim);
	} // end for (z)

	for (z = 0; z < cmac_dim; z++) {
		pos[z] = as[z];
	}

	return true;

}

bool CMACS_CoarseCodeLayerSet::setCoarseActs(double* pos) {
	if (pos == NULL || layers == NULL)
		return false;

	for (int x = 0; x < tiles; x++)
		layers[x]->setCoarseActs(pos);

	return true;
}

int CMACS_CoarseCodeLayerSet::getNumberOfTilings() {
	if (layers == NULL)
		return -1;

	return tiles;
}

CubicCoarseCodeLayer* CMACS_CoarseCodeLayerSet::getTiling(int val) {
	if (layers == NULL || val < 0 || val >= tiles)
		return NULL;

	return layers[val];
}

bool CMACS_CoarseCodeLayerSet::getWinningUnitIndexes(int* the_ones) {
	if (layers == NULL || the_ones == NULL)
		return false;

	for (int x = 0; x < tiles; x++)
		the_ones[x] = the_one[x];

	return true;
}

