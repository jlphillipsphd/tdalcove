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
#include "nnet_math.h"

double multiply(int vector_size, double* vector) {

	if (vector_size < 1)
		return 0.0;

	double val = 1.0;

	for (int x = 0; x < vector_size; x++)
		val *= vector[x];

	return val;
	
};

void multiply(int vector_size, double* vector, int second_dimension, double** matrix, double* new_vector) {

	int x, y;
	for (x = 0; x < second_dimension; x++)
		new_vector[x] = 0.0;

	for (x = 0; x < second_dimension; x++)
		for (y = 0; y < vector_size; y++)
			new_vector[x] += vector[y] * matrix[y][x];

	return;
}

void transpose(int dim1, int dim2, double** matrix, double** new_matrix) {

	int x, y;
	for (x = 0; x < dim2; x++) {
		for (y = 0; y < dim1; y++)
			new_matrix[x][y] = matrix[y][x];
	}

	return;
}

void add(int size, double* vec1, double* vec2, double* new_vec) {

	for (int x = 0; x < size; x++)
		new_vec[x] = vec1[x] + vec2[x];

	return;
}

void subtract(int size, double* vec1, double* vec2, double* new_vec) {

	for (int x = 0; x < size; x++)
		new_vec[x] = vec1[x] - vec2[x];

	return;
}

void multiply_elements(int size, double* vec1, double* vec2, double* new_vec) {

	for (int x = 0; x < size; x++)
		new_vec[x] = vec1[x] * vec2[x];

	return;
}

void increment(int dim1, int dim2, double** matrix1, double** matrix2) {

	for (int x = 0; x < dim1; x++)
		for (int y = 0; y < dim2; y++)
			matrix1[x][y] += matrix2[x][y];

	return;
}

void print_vector(int size, double* vec) {

	printf("[ ");

	for (int x = 0; x < size; x++)
		printf("%f ", vec[x]);

	printf(" ]");

	return;
}

void print_matrix(int dim1, int dim2, double** matrix) {

	for(int x = 0; x < dim1; x++) {

		printf("[ ");

		for (int y = 0; y < dim2; y++)
			printf("%f ", matrix[x][y]);

		printf(" ]\n");

	}

	return;

}

void applySoftmax(int size, double* vector, double* target_vector) {
	applySoftmax(1.0, size, vector, target_vector);
}

void applySoftmax(double scaling_parameter, int size, double* vector, double* target_vector) {
	
	int x;
	double sum = 0.0;

	for (x = 0; x < size; x++)
		sum += exp(scaling_parameter * vector[x]);

	for (x = 0; x < size; x++)
		target_vector[x] = exp(scaling_parameter * vector[x]) / sum;

	return;
}

int getIndexOfMaximum(int size, double* vector) {

	int index = 0;

	for (int x = 0; x < size; x++)
		if (vector[x] > vector[index]) {
			index = x;
		}
	
	return index;
}

double clip(double val, double min, double max) {
	if (val < min)
		val = min;
	if (val > max)
		val = max;

	return val;
}

string itoa(int val) {
	stringstream ss;
	string r_val;
	ss << val;
	ss >> r_val;
	return r_val;
}

string ftoa(double val) {
	stringstream ss;
	string r_val;
	ss << val;
	ss >> r_val;
	return r_val;
}

// I am trying to make a version of these that doesn't use the string stream
// library, but I guess they can do it faster then I can. Actually, these
// functions work just fine, but the double converter doesn't round at all.
// There are lots of values that cannot be accurately represented in binary
// format. A value like 0.33 has a lot of trailing values due the attempt to
// make an accurate representation that is as close as possible. However, this
// function works well if you don't want any internal rounding!!!

/*
string itoa(int val) {

	if (val == 0) {
		return "0";
	}

	char holder;
	string r_val = "";
	bool positive = true;

	if (val < 0) {
		positive = false;
		val = val * -1;
	}

	while (val != 0) {
		holder = val % 10;
		holder += 48;
		val = val / 10;
		r_val = holder + r_val;
	}

	if (!positive)
		r_val = "-" + r_val;

	return r_val;
}

string ftoa(double val) {

	if (val == 0.0) {
		return "0";
	}

	bool positive = true;
	if (val < 0.0) { 
		positive = false;
		val = val * -1.0;
	}

	double pre = nearbyint(floor(val));
	double post = val - pre;
	string r_val = "";
	char holder;

	if (pre == 0.0)
		r_val = "0";

	while (pre != 0.0) {
		holder = (int) nearbyint(((pre / 10.0) - floor(pre / 10.0)) * 10.0);
		holder += 48;
		pre = nearbyint(floor(pre / 10.0));
		r_val = holder + r_val;
	}

	if (post != 0.0) {
		
		r_val = r_val + ".";

		while (post != 0.0) {
			holder = (int) nearbyint(floor(post * 10.0));
			holder += 48;
			post = (post * 10.0) - floor(post * 10.0);
			r_val = r_val + holder;
		}
	}

	if (!positive)
		r_val = "-" + r_val;

	return r_val;
}
*/
