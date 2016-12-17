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
#include "hill_climbing_optimization_method.h"

HillClimbingOptimizationMethod::HillClimbingOptimizationMethod() : OptimizationMethod() {
	run_type = 0;
	last_run = NULL;
}

HillClimbingOptimizationMethod::~HillClimbingOptimizationMethod() {

	if (last_run != NULL)
		delete last_run;
}

HillClimbingOptimizationMethod::HillClimbingOptimizationMethod(list<Parameter*> *model_parameters, ObjectiveModel* objective_model, list<DataPair*> *real_data, ObjectiveFunction* objective_function, int the_run_type) : OptimizationMethod(model_parameters, objective_model, real_data, objective_function) {
	first_run = true;
	run_type = 0;

	if (the_run_type >= 0 && the_run_type < 4)
		run_type = the_run_type;

	last_run = NULL;

	if (model_parameters != NULL)
		last_run = new int[model_parameters->size()];
}

bool HillClimbingOptimizationMethod::maximize() {
	return maximize(1);
}

bool HillClimbingOptimizationMethod::minimize() {
	return minimize(1);
}

bool HillClimbingOptimizationMethod::maximize(int number_of_starts) {
	if (number_of_starts < 1)
		return false;
	if (parameter_list == NULL)
		return false;

	int x;
	list<Parameter*>::iterator itr;
	bool not_finished = true;
	double current_value = runFunc();

	for (x = 0; x < parameter_list->size(); x++)
		last_run[x] = 0;

	while (not_finished) {

		not_finished = false;

		int count = 0;

		for (itr = parameter_list->begin(); itr != parameter_list->end(); itr++) {
			if ((*itr)->getParameterType() == Parameter::TYPE_DOUBLE) {
				double min = ((DoubleParameter*) (*itr))->getMin();
				double max = ((DoubleParameter*) (*itr))->getMax();
				double adj = fabs(max - min) / (double) DOUBLE_DIVISIONS;
				double val = ((DoubleParameter*) (*itr))->getValue();

				double now = current_value;
				double up = now;
				double down = now;

				if (last_run[count] == 1) {
					((DoubleParameter*) (*itr))->adjustValue(adj);
					up = runFunc();
					((DoubleParameter*) (*itr))->setValue(val);

					if (up <= now) {
						((DoubleParameter*) (*itr))->adjustValue(-adj);
						down = runFunc();
						((DoubleParameter*) (*itr))->setValue(val);
					}
				}

				if (last_run[count] == 0) {
					((DoubleParameter*) (*itr))->adjustValue(-adj);
					down = runFunc();
					((DoubleParameter*) (*itr))->setValue(val);

					if (down <= now) {
						((DoubleParameter*) (*itr))->adjustValue(adj);
						up = runFunc();
						((DoubleParameter*) (*itr))->setValue(val);
					}
				}
			
				if (up > now || down > now)
					not_finished = true;

				current_value = now;
				if (up > now) {
					((DoubleParameter*) (*itr))->adjustValue(adj);
					current_value = up;
					last_run[count] = 1;
				}
				else if (down > now) {
					((DoubleParameter*) (*itr))->adjustValue(-adj);
					current_value = down;
					last_run[count] = 0;
				}

			}
			else if ((*itr)->getParameterType() == Parameter::TYPE_INTEGER) {
				int i_min = ((IntegerParameter*) (*itr))->getMin();
				int i_max = ((IntegerParameter*) (*itr))->getMax();
				int i_adj = INTEGER_STEP;
				int i_val = ((IntegerParameter*) (*itr))->getValue();

				double i_now = current_value;
				double i_up = i_now;
				double i_down = i_now;

				if (last_run[count] == 1) {
					((IntegerParameter*) (*itr))->adjustValue(i_adj);
					i_up = runFunc();
					((IntegerParameter*) (*itr))->setValue(i_val);

					if (i_up <= i_now) {
						((IntegerParameter*) (*itr))->adjustValue(-i_adj);
						i_down = runFunc();
						((IntegerParameter*) (*itr))->setValue(i_val);
					}
				}

				if (last_run[count] == 0) {
					((IntegerParameter*) (*itr))->adjustValue(-i_adj);
					i_down = runFunc();
					((IntegerParameter*) (*itr))->setValue(i_val);

					if (i_down <= i_now) {
						((IntegerParameter*) (*itr))->adjustValue(i_adj);
						i_up = runFunc();
						((IntegerParameter*) (*itr))->setValue(i_val);
					}
				}
				
				if (i_up > i_now || i_down > i_now)
					not_finished = true;

				current_value = i_now;
				if (i_up > i_now) {
					((IntegerParameter*) (*itr))->adjustValue(i_adj);
					current_value = i_up;
					last_run[count] = 1;
				}
				else if (i_down > i_now) {
					((IntegerParameter*) (*itr))->adjustValue(-i_adj);
					current_value = i_down;
					last_run[count] = 0;
				}
			}
			else if ((*itr)->getParameterType() == Parameter::TYPE_STRING) {

				list<string> possible;
				list<string>::iterator s_itr;

				int pos_num = ((StringParameter*) (*itr))->getNumberOfPossibleValues();
				((StringParameter*) (*itr))->getPossibleValues(possible);

				double* possible_values = new double[pos_num];

				int s_now_pos = 0;
				double s_now = current_value;
				string curr_s = ((StringParameter*) (*itr))->getValue();

				for (x = 0, s_itr = possible.begin(); s_itr != possible.end(); x++, s_itr++) {
					if (curr_s == (*s_itr))
						s_now_pos = x;
				}

				possible_values[s_now_pos] = s_now;

				for (x = 0, s_itr = possible.begin(); s_itr != possible.end(); x++, s_itr++) {
					if (x != s_now_pos) {
						((StringParameter*) (*itr))->setValue((*s_itr));
						possible_values[x] = runFunc();
					}
				}

			
				double new_s_val = s_now;
				int new_s_pos = s_now_pos;
				for (x = 0; x < pos_num; x++) {
					if (x != s_now_pos && possible_values[x] > s_now) {
						not_finished = true;
						new_s_val = possible_values[x];
						new_s_pos = x;
					}
				}

				for (x = 0, s_itr = possible.begin(); x < new_s_pos; x++, s_itr++);

				current_value = possible_values[new_s_pos];
				((StringParameter*) (*itr))->setValue((*s_itr));
				
			}

			count++;
		}

		if (DISPLAY_OBJECTIVE_VALUES == 1) {
			cout << "Objective function value: " << current_value << endl;
			cout << "Parameters:" << endl;
			printParameterList(*parameter_list);
		}
	}

	return true;
}

bool HillClimbingOptimizationMethod::minimize(int number_of_starts) {
	if (number_of_starts < 1)
		return false;
	if (parameter_list == NULL)
		return false;

	int x;
	list<Parameter*>::iterator itr;
	bool not_finished = true;
	double current_value = runFunc();

	for (x = 0; x < parameter_list->size(); x++)
		last_run[x] = 0;

	while (not_finished) {

		not_finished = false;
		int count = 0;

		for (itr = parameter_list->begin(); itr != parameter_list->end(); itr++) {
			if ((*itr)->getParameterType() == Parameter::TYPE_DOUBLE) {
				double min = ((DoubleParameter*) (*itr))->getMin();
				double max = ((DoubleParameter*) (*itr))->getMax();
				double adj = fabs(max - min) / (double) DOUBLE_DIVISIONS;
				double val = ((DoubleParameter*) (*itr))->getValue();

				double now = current_value;
				double up = now;
				double down = now;

				if (last_run[count] == 1) {

					((DoubleParameter*) (*itr))->adjustValue(adj);
					up = runFunc();
					((DoubleParameter*) (*itr))->setValue(val);

					if (up >= now) {
						((DoubleParameter*) (*itr))->adjustValue(-adj);
						down = runFunc();
						((DoubleParameter*) (*itr))->setValue(val);
					}
				}

				if (last_run[count] == 0) {
					((DoubleParameter*) (*itr))->adjustValue(-adj);
					down = runFunc();
					((DoubleParameter*) (*itr))->setValue(val);

					if (down >= now) {
						((DoubleParameter*) (*itr))->adjustValue(adj);
						up = runFunc();
						((DoubleParameter*) (*itr))->setValue(val);
					}
				}
			
				if (up < now || down < now)
					not_finished = true;

				current_value = now;
				if (up < now) {
					((DoubleParameter*) (*itr))->adjustValue(adj);
					current_value = up;
					last_run[count] = 1;
				}
				else if (down < now) {
					((DoubleParameter*) (*itr))->adjustValue(-adj);
					current_value = down;
					last_run[count] = 0;
				}

			}
			else if ((*itr)->getParameterType() == Parameter::TYPE_INTEGER) {
				int i_min = ((IntegerParameter*) (*itr))->getMin();
				int i_max = ((IntegerParameter*) (*itr))->getMax();
				int i_adj = INTEGER_STEP;
				int i_val = ((IntegerParameter*) (*itr))->getValue();

				double i_now = current_value;
				double i_up = i_now;
				double i_down = i_now;

				if (last_run[count] == 1) {
					((IntegerParameter*) (*itr))->adjustValue(i_adj);
					i_up = runFunc();
					((IntegerParameter*) (*itr))->setValue(i_val);

					if (i_up >= i_now) {
						((IntegerParameter*) (*itr))->adjustValue(-i_adj);
						i_down = runFunc();
						((IntegerParameter*) (*itr))->setValue(i_val);
					}
				}

				if (last_run[count] == 0) {
					((IntegerParameter*) (*itr))->adjustValue(-i_adj);
					i_down = runFunc();
					((IntegerParameter*) (*itr))->setValue(i_val);

					if (i_down >= i_now) {
						((IntegerParameter*) (*itr))->adjustValue(i_adj);
						i_up = runFunc();
						((IntegerParameter*) (*itr))->setValue(i_val);
					}
				}
				
				if (i_up < i_now || i_down < i_now)
					not_finished = true;

				current_value = i_now;
				if (i_up < i_now) {
					((IntegerParameter*) (*itr))->adjustValue(i_adj);
					current_value = i_up;
					last_run[count] = 1;
				}
				else if (i_down < i_now) {
					((IntegerParameter*) (*itr))->adjustValue(-i_adj);
					current_value = i_down;
					last_run[count] = 0;
				}
			}
			else if ((*itr)->getParameterType() == Parameter::TYPE_STRING) {

				list<string> possible;
				list<string>::iterator s_itr;

				int pos_num = ((StringParameter*) (*itr))->getNumberOfPossibleValues();
				((StringParameter*) (*itr))->getPossibleValues(possible);

				double* possible_values = new double[pos_num];

				int s_now_pos = 0;
				double s_now = current_value;
				string curr_s = ((StringParameter*) (*itr))->getValue();

				for (x = 0, s_itr = possible.begin(); s_itr != possible.end(); x++, s_itr++) {
					if (curr_s == (*s_itr))
						s_now_pos = x;
				}

				possible_values[s_now_pos] = s_now;

				for (x = 0, s_itr = possible.begin(); s_itr != possible.end(); x++, s_itr++) {
					if (x != s_now_pos) {
						((StringParameter*) (*itr))->setValue((*s_itr));
						possible_values[x] = runFunc();
					}
				}

			
				double new_s_val = s_now;
				int new_s_pos = s_now_pos;
				for (x = 0; x < pos_num; x++) {
					if (x != s_now_pos && possible_values[x] < s_now) {
						not_finished = true;
						new_s_val = possible_values[x];
						new_s_pos = x;
					}
				}

				for (x = 0, s_itr = possible.begin(); x < new_s_pos; x++, s_itr++);

				current_value = possible_values[new_s_pos];
				((StringParameter*) (*itr))->setValue((*s_itr));

				delete possible_values;
				
			}

			count++;
		}

		if (DISPLAY_OBJECTIVE_VALUES == 1) {
			cout << "Objective function value: " << current_value << endl;
			cout << "Parameters:" << endl;
			printParameterList(*parameter_list);
		}
	}

	return true;
}

double HillClimbingOptimizationMethod::runFunc() {

	if (run_type == 1)
		return obj_model->avgRun(*parameter_list, obj_func, *data);
	if (run_type == 2)
		return obj_model->maxRun(*parameter_list, obj_func, *data);
	if (run_type == 3)
		return obj_model->minRun(*parameter_list, obj_func, *data);

	return obj_model->sumRun(*parameter_list, obj_func, *data);
}

