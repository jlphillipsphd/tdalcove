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
#include "sse_objective_function.h"

#define SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE 300

SSE_ObjectiveFunction::SSE_ObjectiveFunction() : ObjectiveFunction() {
}

double SSE_ObjectiveFunction::evaluate(list<DataPair*> &real_data, list<DataPair*> &model_data) {

	int x;
	double final_sse = 0.0;
	double d_output_data1[SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE];
	double d_output_data2[SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE];
	int i_output_data1[SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE];
	int i_output_data2[SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE];
	int m_size;
	int r_size;

	if (real_data.size() != model_data.size())
		return MAXFLOAT;

	list<DataPair*>::iterator r_itr;
	list<DataPair*>::iterator m_itr;

	for (r_itr = real_data.begin(), m_itr = model_data.begin();
			r_itr != real_data.end(); r_itr++, m_itr++) {

		m_size = (*m_itr)->getOutputSize();
		r_size = (*r_itr)->getOutputSize();

		if (m_size != r_size || m_size > SSE_OBJECTIVE_FUNCTION_MAX_OUTPUT_SIZE)
			continue;

		if ((*r_itr)->getDataPairType() == 0 && (*m_itr)->getDataPairType() == 0) {

			((DoubleDataPair*) (*m_itr))->getOutputValues(d_output_data1);
			((DoubleDataPair*) (*r_itr))->getOutputValues(d_output_data2);

			double d_temp = 0.0;

			for (x = 0; x < m_size; x++) {
				d_temp += (d_output_data1[x] - d_output_data2[x]) *
						  (d_output_data1[x] - d_output_data2[x]);
				if (SSE_OBJ_FUNC_VERBOSITY == 1)
					cout << "(" << d_output_data1[x] << " - " << d_output_data2[x] << ")^2 = " << d_temp << endl;
			}

			final_sse += d_temp;

		} // end normal double case

		if ((*r_itr)->getDataPairType() == 1 && (*m_itr)->getDataPairType() == 1) {

			((IntegerDataPair*) (*m_itr))->getOutputValues(i_output_data1);
			((IntegerDataPair*) (*r_itr))->getOutputValues(i_output_data2);

			int i_temp = 0;

			for (x = 0; x < m_size; x++) {
				i_temp += (i_output_data1[x] - i_output_data2[x]) *
						  (i_output_data1[x] - i_output_data2[x]);
				if (SSE_OBJ_FUNC_VERBOSITY == 1)
					cout << "(" << i_output_data1[x] << " - " << i_output_data2[x] << ")^2 = " << i_temp << endl;
			}

			final_sse += i_temp;

		} // end normal integer case
	}

	return final_sse;
}
