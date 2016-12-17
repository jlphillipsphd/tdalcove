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
#include "optimization_functions.h"

list<Parameter*>* readParameters(string filename) {

	ifstream infile;

	infile.open(filename.c_str());

	if (infile.bad() || infile.eof())
		return NULL;

	list<Parameter*> *parameter_list = new list<Parameter*>;

	int number_of_parameters;
	infile >> number_of_parameters;

	for (int x = 0; x < number_of_parameters; x++) {
		int parameter_type;
		Parameter* parameter = NULL;
		infile >> parameter_type;
		if (parameter_type == Parameter::TYPE_DOUBLE) {
			double init;
			double min;
			double max;

			infile >> init >> min >> max;
			parameter = new DoubleParameter(init, min, max);
			parameter_list->push_back(parameter);
		}
		else if(parameter_type == Parameter::TYPE_INTEGER) {
			int i_init;
			int i_min;
			int i_max;

			infile >> i_init >> i_min >> i_max;
			parameter = new IntegerParameter(i_init, i_min, i_max);
			parameter_list->push_back(parameter);
		}
		else if(parameter_type == Parameter::TYPE_STRING) {
			string s_init;
			int num;
			string now;
			list<string> s_list;

			infile >> s_init >> num;

			for (int y = 0; y < num; y++) {
				infile >> now;
				s_list.push_back(now);
			}
			parameter = new StringParameter(s_init, s_list);
			parameter_list->push_back(parameter);
		}
	}

	infile.close();

	return parameter_list;
}

list<DataPair*>* readDataPairs(string filename) {

	ifstream infile;

	infile.open(filename.c_str());

	if (infile.bad() || infile.eof())
		return NULL;

	list<DataPair*> *pair_list = new list<DataPair*>;

	int number_of_pairs;
	infile >> number_of_pairs;

	for (int x = 0; x < number_of_pairs; x++) {
		int pair_type;
		int num_in;
		int num_out;
		DataPair* pair = NULL;
		infile >> pair_type >> num_in >> num_out;

		if (pair_type == DataPair::TYPE_DOUBLE) {
			double* in_vals = new double[num_in];
			double* out_vals = new double[num_out];

			for (int y = 0; y < num_in; y++) {
				infile >> in_vals[y];
			}

			for (int z = 0; z < num_out; z++) {
				infile >> out_vals[z];
			}

			pair = new DoubleDataPair(num_in, in_vals, num_out, out_vals);
			pair_list->push_back(pair);

			delete in_vals;
			delete out_vals;
		}
		else if(pair_type == DataPair::TYPE_INTEGER) {
			int* i_in_vals = new int[num_in];
			int* i_out_vals = new int[num_out];

			for (int i = 0; i < num_in; i++)
				infile >> i_in_vals[i];
			for (int j = 0; j < num_out; j++)
				infile >> i_out_vals[j];

			pair = new IntegerDataPair(num_in, i_in_vals, num_out, i_out_vals);
			pair_list->push_back(pair);

			delete i_in_vals;
			delete i_out_vals;
		}
		else if(pair_type == DataPair::TYPE_STRING) {
			string* s_in_vals = new string[num_in];
			string* s_out_vals = new string[num_out];

			for (int k = 0; k < num_in; k++)
				infile >> s_in_vals[k];
			for (int l = 0; l < num_out; l++)
				infile >> s_out_vals[l];

			pair = new StringDataPair(num_in, s_in_vals, num_out, s_out_vals);
			pair_list->push_back(pair);

			delete s_in_vals;
			delete s_out_vals;
		}
	}

	infile.close();

	return pair_list;
}

bool randomizeParameterValues(list<Parameter*> &parameters) {

	int x;
	list<Parameter*>::iterator itr;

	for (itr = parameters.begin(); itr != parameters.end(); itr++) {

		if ((*itr)->getParameterType() == Parameter::TYPE_DOUBLE) {

			double min = ((DoubleParameter*) (*itr))->getMin();
			double max = ((DoubleParameter*) (*itr))->getMax();

			double val = drand48();
			val *= (max - min);
			val += min;

			((DoubleParameter*) (*itr))->setValue(val);

		}
		else if ((*itr)->getParameterType() == Parameter::TYPE_INTEGER) {

			int i_min = ((IntegerParameter*) (*itr))->getMin();
			int i_max = ((IntegerParameter*) (*itr))->getMax();

			int i_val = random();
			i_val %= (i_max - i_min);
			i_val += i_min;

			((IntegerParameter*) (*itr))->setValue(i_val);

		}
		else if ((*itr)->getParameterType() == Parameter::TYPE_STRING) {

			list<string> empty_list;
			((StringParameter*) (*itr))->getPossibleValues(empty_list);

			if (empty_list.empty())
				continue;

			int s_val = random();
			s_val %= empty_list.size();

			list<string>::iterator s_itr = empty_list.begin();

			for (x = 0; x < s_val; x++, s_itr++);

			((StringParameter*) (*itr))->setValue((*s_itr));

		}
	} // end itr for

	return true;
}

void printParameterList(list<Parameter*> &parameters) {

	int x;
	list<Parameter*>::iterator itr;

	for (itr = parameters.begin(); itr != parameters.end(); itr++) {

		if ((*itr)->getParameterType() == Parameter::TYPE_DOUBLE) {

			double min = ((DoubleParameter*) (*itr))->getMin();
			double max = ((DoubleParameter*) (*itr))->getMax();
			double val = ((DoubleParameter*) (*itr))->getValue();

			cout << "DBL " << val << " (" << min << ", " << max << ")" << endl;

		}
		else if ((*itr)->getParameterType() == Parameter::TYPE_INTEGER) {

			int i_min = ((IntegerParameter*) (*itr))->getMin();
			int i_max = ((IntegerParameter*) (*itr))->getMax();
			int i_val = ((IntegerParameter*) (*itr))->getValue();

			cout << "INT " << i_val << " (" << i_min << ", " << i_max << ")" << endl;

		}
		else if ((*itr)->getParameterType() == Parameter::TYPE_STRING) {

			list<string> empty_list;
			((StringParameter*) (*itr))->getPossibleValues(empty_list);

			string s_val = ((StringParameter*) (*itr))->getValue();

			cout << "STR " << s_val << " (";

			list<string>::iterator s_itr = empty_list.begin();

			for (; s_itr != empty_list.end(); s_itr++)
				cout << (*s_itr) << ", ";

			cout << ")" << endl;

		}
	} // end itr for

	return;
}

void printDataPairList(list<DataPair*> &pairs) {

	int x;
	int in;
	int out;
	list<DataPair*>::iterator itr;

	for (itr = pairs.begin(); itr != pairs.end(); itr++) {

		in = (*itr)->getInputSize();
		out = (*itr)->getOutputSize();

		if ((*itr)->getDataPairType() == DataPair::TYPE_DOUBLE) {

			double* inputs;
			double* outputs;

			if (in > 0)
				inputs = new double[in];
			if (out > 0)
				outputs = new double[out];

			((DoubleDataPair*) (*itr))->getInputValues(inputs);
			((DoubleDataPair*) (*itr))->getOutputValues(outputs);

			cout << "DBL Inputs: ";

			for (x = 0; x < in; x++)
				cout << inputs[x] << " ";

			cout << "DBL Outputs: ";

			for (x = 0; x < out; x++)
				cout << outputs[x] << " ";

			if (in > 0)
				delete inputs;
			if (out > 0)
				delete outputs;

		}
		else if ((*itr)->getDataPairType() == DataPair::TYPE_INTEGER) {

			int* i_inputs;
			int* i_outputs;

			if (in > 0)
				i_inputs = new int[in];
			if (out > 0)
				i_outputs = new int[out];

			((IntegerDataPair*) (*itr))->getInputValues(i_inputs);
			((IntegerDataPair*) (*itr))->getOutputValues(i_outputs);

			cout << "INT Inputs: ";

			for (x = 0; x < in; x++)
				cout << i_inputs[x] << " ";

			cout << "INT Outputs: ";

			for (x = 0; x < out; x++)
				cout << i_outputs[x] << " ";

			if (in > 0)
				delete i_inputs;
			if (out > 0)
				delete i_outputs;

		}
		else if ((*itr)->getDataPairType() == DataPair::TYPE_STRING) {

			string* s_inputs;
			string* s_outputs;

			if (in > 0)
				s_inputs = new string[in];
			if (out > 0)
				s_outputs = new string[out];

			((StringDataPair*) (*itr))->getInputValues(s_inputs);
			((StringDataPair*) (*itr))->getOutputValues(s_outputs);

			cout << "INT Inputs: ";

			for (x = 0; x < in; x++)
				cout << s_inputs[x] << " ";

			cout << "INT Outputs: ";

			for (x = 0; x < out; x++)
				cout << s_outputs[x] << " ";

			if (in > 0)
				delete s_inputs;
			if (out > 0)
				delete s_outputs;

		}
		cout << endl;
	} // end itr for

	return;
}

bool writeDataPairList(list<DataPair*> &pairs, string filename) {

	int x;
	ofstream outfile;
	outfile.open(filename.c_str());

	if (outfile.bad())
		return false;

	outfile << pairs.size() << endl;

	list<DataPair*>::iterator itr;
	for (itr = pairs.begin(); itr != pairs.end(); itr++) {

		int type = (*itr)->getDataPairType();
		int in_size = (*itr)->getInputSize();
		int out_size = (*itr)->getOutputSize();
		if (in_size < 0)
			in_size = 0;
		if (out_size < 0)
			out_size = 0;
		outfile << type << " " << in_size << " " << out_size << endl;

		if (type == DataPair::TYPE_DOUBLE) {
			double* d_vals;

			if (in_size > 0) {
				d_vals = new double[in_size];
				((DoubleDataPair*) (*itr))->getInputValues(d_vals);
				for (x = 0; x < in_size; x++)
					outfile << d_vals[x] << " ";
				delete d_vals;
				outfile << endl;
			}
			if (out_size > 0) {
				d_vals = new double[out_size];
				((DoubleDataPair*) (*itr))->getOutputValues(d_vals);
				for (x = 0; x < out_size; x++)
					outfile << d_vals[x] << " ";
				delete d_vals;
				outfile << endl;
			}
					
		}
		else if (type == DataPair::TYPE_INTEGER) {
			int* i_vals;

			if (in_size > 0) {
				i_vals = new int[in_size];
				((IntegerDataPair*) (*itr))->getInputValues(i_vals);
				for (x = 0; x < in_size; x++)
					outfile << i_vals[x] << " ";
				delete i_vals;
				outfile << endl;
			}
			if (out_size > 0) {
				i_vals = new int[out_size];
				((IntegerDataPair*) (*itr))->getOutputValues(i_vals);
				for (x = 0; x < out_size; x++)
					outfile << i_vals[x] << " ";
				delete i_vals;
				outfile << endl;
			}
		}
		else if (type == DataPair::TYPE_STRING) {
			string* s_vals;

			if (in_size > 0) {
				s_vals = new string[in_size];
				((StringDataPair*) (*itr))->getInputValues(s_vals);
				for (x = 0; x < in_size; x++)
					outfile << s_vals[x] << " ";
				delete s_vals;
				outfile << endl;
			}
			if (out_size > 0) {
				s_vals = new string[out_size];
				((StringDataPair*) (*itr))->getOutputValues(s_vals);
				for (x = 0; x < out_size; x++)
					outfile << s_vals[x] << " ";
				delete s_vals;
				outfile << endl;
			}
		}
	}

	outfile.close();

	return true;
}

bool writeParameterList(list<Parameter*> &parameters, string filename) {

	int x;
	ofstream outfile;
	outfile.open(filename.c_str());

	if (outfile.bad())
		return false;

	outfile << parameters.size() << endl;

	list<Parameter*>::iterator itr;

	for (itr = parameters.begin(); itr != parameters.end(); itr++) {
		int type = (*itr)->getParameterType();
		outfile << type << " ";

		if (type == Parameter::TYPE_DOUBLE) {
			double d_val = ((DoubleParameter*) (*itr))->getValue();
			double d_min = ((DoubleParameter*) (*itr))->getMin();
			double d_max = ((DoubleParameter*) (*itr))->getMax();

			outfile << d_val << " " << d_min << " " << d_max << endl;
		}
		else if (type == Parameter::TYPE_INTEGER) {
			double i_val = ((IntegerParameter*) (*itr))->getValue();
			double i_min = ((IntegerParameter*) (*itr))->getMin();
			double i_max = ((IntegerParameter*) (*itr))->getMax();

			outfile << i_val << " " << i_min << " " << i_max << endl;
		}
		else if (type == Parameter::TYPE_STRING) {
			string s_val = ((StringParameter*) (*itr))->getValue();
			list<string> possibles;
			((StringParameter*) (*itr))->getPossibleValues(possibles);

			outfile << s_val << " " << possibles.size() << " ";

			for (list<string>::iterator s_itr = possibles.begin();
					s_itr != possibles.end(); s_itr++)
				outfile << (*s_itr) << " ";

			outfile << endl;
		}
	}

	outfile.close();

	return true;
}
