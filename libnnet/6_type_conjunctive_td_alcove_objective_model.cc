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
#include "6_type_conjunctive_td_alcove_objective_model.h"

SixTypeConjunctiveTD_ALCOVE_ObjectiveModel::SixTypeConjunctiveTD_ALCOVE_ObjectiveModel() : ObjectiveModel() {
}

SixTypeConjunctiveTD_ALCOVE_ObjectiveModel::SixTypeConjunctiveTD_ALCOVE_ObjectiveModel(list<unsigned int> &seed_list) : ObjectiveModel(seed_list) {
}

double SixTypeConjunctiveTD_ALCOVE_ObjectiveModel::singleRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data) {

	// Unfortunately the static arrays do not work in tandem with
	// the dynamic decrees needed by the Neural Network Library.
	// As a result I simply copy these values into two dynamically
	// created arrays and use the resultant dynamic arrays during
	// processing. However, the simplicity of including them here
	// gives enough reason for the wasted memory.
	double patterns[8][3] = { { 0.0, 0.0, 0.0 },
							  { 0.0, 0.0, 1.0 },
							  { 0.0, 1.0, 0.0 },
							  { 0.0, 1.0, 1.0 },
							  { 1.0, 0.0, 0.0 },
							  { 1.0, 0.0, 1.0 },
							  { 1.0, 1.0, 0.0 },
							  { 1.0, 1.0, 1.0 } };

	double targets[6][8][2] = { { { 1.0, -1.0 }, { 1.0, -1.0 },
								  { 1.0, -1.0 }, { 1.0, -1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 } },
								{ { 1.0, -1.0 }, { 1.0, -1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 },
								  { 1.0, -1.0 }, { 1.0, -1.0 } },
								{ { 1.0, -1.0 }, { 1.0, -1.0 },
								  { 1.0, -1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { 1.0, -1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 } },
								{ { 1.0, -1.0 }, { 1.0, -1.0 },
								  { 1.0, -1.0 }, { -1.0, 1.0 },
								  { 1.0, -1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 } },
								{ { 1.0, -1.0 }, { 1.0, -1.0 },
								  { 1.0, -1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { 1.0, -1.0 } },
								{ { 1.0, -1.0 }, { -1.0, 1.0 },
								  { -1.0, 1.0 }, { 1.0, -1.0 },
								  { -1.0, 1.0 }, { 1.0, -1.0 },
								  { 1.0, -1.0 }, { -1.0, 1.0 } } };

	// Parameters
	list<Parameter*>::iterator p_itr = parameters.begin();
	double w_learning_rate = ((DoubleParameter*) (*p_itr))->getValue();
	p_itr++;
	double td_learning_rate = ((DoubleParameter*) (*p_itr))->getValue();

	double correct_tolerance = 0.5;
	double weight_span = 0.05;
	double specificity = 6.5;
	double mapping_constant = 2.0;

	list<DataPair*> our_data;

	double return_value;

	int x, y, z;
	double** patterns_pointer;
	double*** targets_pointer;
	ConjunctiveTD_ALCOVEInputLayer* in_layer;
	ALCOVEHiddenLayer* hidden_layer;
	ALCOVEOutputLayer* out_layer;
	BpFullForwardProjection* hidden_out_proj;
	UniformRandomNumberGenerator* urng;

	double a_learning_rate = 0.0033;
	//a_learning_rate = 0.0;
	int number_of_input_dimensions = 3;
	int number_of_categories = 2;
	int number_of_hidden_units = 8;
	double** activation_array = new double*[8];
	double** softmax_array = new double*[8];
	for (x = 0; x < 8; x++) {
		activation_array[x] = new double[number_of_categories];
		softmax_array[x] = new double[number_of_categories];
	}
	double initial_attention_strengths = 1.0 / 3.0;

	double min_weights = -weight_span;
	double max_weights = weight_span;

	int number_of_epochs = 51;
	int test_every = 5;
	int epoch = 0;
	bool past_threshold;

	double* as = new double[number_of_input_dimensions];
	for (x = 0; x < number_of_input_dimensions; x++)
		as[x] = initial_attention_strengths;

	patterns_pointer = new double*[number_of_hidden_units];

	for (x = 0; x < number_of_hidden_units; x++) {
		patterns_pointer[x] = new double[number_of_input_dimensions];
		for (y = 0; y < 3; y++)
			patterns_pointer[x][y] = patterns[x][y];
	} // end for

	targets_pointer = new double**[6];

	for (x = 0; x < 6; x++) {
		targets_pointer[x] = new double*[8];
		for (y = 0; y < 8; y++) {
			targets_pointer[x][y] = new double[2];
			for (z = 0; z < 2; z++)
				targets_pointer[x][y][z] = targets[x][y][z];
		} // end for
	} // end for

	// Create Network
	in_layer = new ConjunctiveTD_ALCOVEInputLayer(number_of_input_dimensions, 9, 0.01, 1.0, td_learning_rate);
	out_layer = new ALCOVEOutputLayer(number_of_categories);
	hidden_layer = new ALCOVEHiddenLayer(number_of_hidden_units, in_layer, patterns_pointer);
	hidden_out_proj = new BpFullForwardProjection(hidden_layer, out_layer, 0);

	// Random number generator for small random initial weights
	urng = new UniformRandomNumberGenerator(min_weights, max_weights);

	int problem_type = 0;
	ofstream graph;
	graph.open("alcove_graph.jgr");
	graph << "newgraph" << endl;
	graph << "border" << endl;
	graph << "xaxis size 3.0 min 0 max " << (number_of_epochs - 1) << endl;
	graph << "hash_labels fontsize 14" << endl;
	graph << "label fontsize 16 : epoch" << endl;
	graph << "yaxis size 2.5 min 0.5 max 1" << endl;
	graph << "label fontsize 16 : Pr(correct)" << endl;
	graph << endl;

	for (problem_type = 0; problem_type < 6; problem_type++) {

	graph << "newcurve marktype box fill 1 linetype solid color ";

	switch (problem_type) {
		case 0:
			graph << "0 0 0" << endl;
			graph << "label : Type 1" << endl;
			break;
		case 1:
			graph << "0 0 1" << endl;
			graph << "label : Type 2" << endl;
			break;
		case 2:
			graph << "0 1 0" << endl;
			graph << "label : Type 3" << endl;
			break;
		case 3:
			graph << "0 1 1" << endl;
			graph << "label : Type 4" << endl;
			break;
		case 4:
			graph << "1 0 0" << endl;
			graph << "label : Type 5" << endl;
			break;
		case 5:
			graph << "1 0 1" << endl;
			graph << "label : Type 6" << endl;
			break;
	}

	graph << "pts" << endl;

	hidden_layer->setSpecificity(specificity);

	hidden_out_proj->setLearningRate(w_learning_rate);
	hidden_layer->setLearningRate(a_learning_rate);

	in_layer->setAttentionalStrengths(as);
	in_layer->clearDeltaAttentionalStrengths();
	hidden_out_proj->clearWeights();
	//in_layer->initializeTDWeights(urng);
	in_layer->biasAroundPosition(as, 1.0, NULL);

	past_threshold = false;
	int t_number = number_of_epochs;

	for (epoch = 0; epoch <= number_of_epochs; epoch++) {

		// Test output
		if (epoch % test_every == 0) {
		
			double average_correct = 0.0;

			// Find attention strengths
			in_layer->computeAttentionalStrengths();

			for (x = 0; x < 8; x++) {

				// Forward propagation
				in_layer->setActs(patterns[x]);
				hidden_layer->computeActivations();
				hidden_out_proj->computeNets(false);
				out_layer->computeActivations();

				out_layer->getActs(activation_array[x]);
				applySoftmax(mapping_constant, number_of_categories, activation_array[x], softmax_array[x]);
				for (y = 0; y < number_of_categories; y++) {
					if (targets_pointer[problem_type][x][y] == 1.0) {
						average_correct += softmax_array[x][y];
					}
				}
			} // end for

			average_correct /= 8.0;
			graph << epoch << " " << average_correct << endl;

			double ep_temp = (double) epoch;
			DoubleDataPair* new_pair = new DoubleDataPair(1, &ep_temp, 1, &average_correct);
			our_data.push_back(new_pair);

			if (average_correct >= 0.95 && !past_threshold) {
				past_threshold = true;
				t_number = epoch;
			} // end if
		} // end if

		for (x = 0; x < 8; x++) {

			// Find attention strengths
			in_layer->computeAttentionalStrengths();

			// Forward propagation
			in_layer->setActs(patterns[x]);
			hidden_layer->computeActivations();
			hidden_out_proj->computeNets(false);
			out_layer->computeActivations();
			out_layer->setTargets(targets_pointer[problem_type][x]);

			// Learning
			out_layer->computeBpHumbleDeltas(-1.0, 1.0);
			hidden_out_proj->computeNetDeltas();
			
			hidden_out_proj->computeWeightChanges();
			//hidden_layer->adjustDeltaAttentionalStrengths();

			// Online update
			hidden_out_proj->updateWeights();
			//in_layer->updateAttentionalStrengths();

			// Adjust TD weights
			double reward;
			double one_val[1];
			bool good = true;

			double f_1;
			for (y = 0; y < 2; y++) {
				out_layer->getUnit(y)->getDeltas(one_val);
				f_1 = one_val[0];
				out_layer->getUnit(y)->getDerivatives(one_val);
				if (fabs(f_1 / one_val[0]) > correct_tolerance)
					good = false;
			}

			if (good)
				reward = 1.0;
			else
				reward = -1.0;

			in_layer->adjustTDWeights(reward);

		} // end for

		// Batch update
		//hidden_out_proj->updateWeights();
		//in_layer->updateAttentionalStrengths();
	} // end epochs for

	double* an_array = new double[number_of_input_dimensions];
	in_layer->getAttentionalStrengths(an_array);

	//cout << t_number << " ";
	//for (x = 0; x < number_of_input_dimensions; x++)
		//cout << an_array[x] << " ";
	//cout << endl;

	delete an_array;

	} // end problem for

	for (x = 0; x < number_of_hidden_units; x++)
		delete patterns_pointer[x];
	delete patterns_pointer;

	for (x = 0; x < 8; x++) {
		delete activation_array[x];
		delete softmax_array[x];
	}
	delete activation_array;
	delete softmax_array;

	for (x = 0; x < 6; x++) {
		for (y = 0; y < 8; y++)
			delete targets_pointer[x][y];
		delete targets_pointer[x];
	}
	delete targets_pointer;

	delete as;

	delete urng;
	delete hidden_out_proj;
	delete hidden_layer;
	delete out_layer;
	delete in_layer;

	graph.close();


	return_value = obj_func->evaluate(real_data, our_data);

	//writeDataPairList(our_data, "our_data");

	for (list<DataPair*>::iterator d_itr = our_data.begin(); d_itr != our_data.end(); d_itr++)
		delete (*d_itr);

	return return_value;

} // end main()
