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
// Not all of these functions are used, but they are provided if wanted.
// Some are quite useful however.

#ifndef NNET_MATH_H
#define NNET_MATH_H

#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

void increment(int dim1, int dim2, double** matrix1, double** matrix2);

double multiply(int vector_size, double* vector);

void multiply(int vector_size, double* vector, int second_dimension, double** matrix, double* new_vector);

void transpose(int dim1, int dim2, double** matrix, double** new_matrix);

// For these the final argument can be the same object as the first two
void add(int size, double* vec1, double* vec2, double* new_vec);

void subtract(int size, double* vec1, double* vec2, double* new_vec);

void multiply_elements(int size, double* vec1, double* vec2, double* new_vec);

// Strange to put these here, but usefull debugging tools so what the hey...
void print_vector(int size, double* vec);
void print_matrix(int dim1, int dim2, double** matrix);

// Gibb's Softwmx Function
void applySoftmax(int size, double* vector, double* target_vector);
void applySoftmax(double scaling_parameter, int size, double* vector, double* target_vector);

int getIndexOfMaximum(int size, double* vector);

//Simple clip
double clip(double val, double min, double max);

// Number to string conversion routines - I guess they will work best in
// this header file?
string ftoa(double val);
string itoa(int val);

#endif
