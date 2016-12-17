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
#include "integral_alcove_objective_model.h"

IntegralALCOVE_ObjectiveModel::IntegralALCOVE_ObjectiveModel() : ObjectiveModel() {
	problem = 0;
}

IntegralALCOVE_ObjectiveModel::IntegralALCOVE_ObjectiveModel(int problem_type) : ObjectiveModel() {
	problem = 0;

	if (problem_type > 0 && problem_type <= 6)
		problem = problem_type - 1;
}

IntegralALCOVE_ObjectiveModel::IntegralALCOVE_ObjectiveModel(int problem_type, list<unsigned int> &seed_list) : ObjectiveModel(seed_list) {
	problem = 0;

	if (problem_type > 0 && problem_type <= 6)
		problem = problem_type - 1;
}

double IntegralALCOVE_ObjectiveModel::singleRun(list<Parameter*> &parameters, ObjectiveFunction* obj_func, list<DataPair*> &real_data, unsigned int seed) {

	srand48(seed);

	// Evaluation Data List
	list<DataPair*> our_data;
	double return_value;

	// Parameters
	list<Parameter*>::iterator p_itr = parameters.begin();
	double w_learning_rate = ((DoubleParameter*) (*p_itr))->getValue();
	p_itr++;
	double a_learning_rate = ((DoubleParameter*) (*p_itr))->getValue();
	p_itr++;
	double mapping_constant = ((DoubleParameter*) (*p_itr))->getValue();
	p_itr++;
	double specificity = ((DoubleParameter*) (*p_itr))->getValue();

	// Unfortunately the static arrays do not work in tandem with
	// the dynamic decrees needed by the Neural Network Library.
	// As a result I simply copy these values into two dynamically
	// created arrays and use the resultant dynamic arrays during
	// processing. However, the simplicity of including them here
	// gives enough reason for the wasted memory.
	double patterns[12][2] = { { -2.543, 2.641 },
							   { 0.943, 4.341 },
							   { -1.092, 1.848 },
							   { 1.558, 2.902 },
							   { -2.258, 0.430 },
							   { 0.194, 0.572 },
							   { 2.806, 0.202 },
							   { -1.177, -1.038 },
							   { 1.543, -1.040 },
							   { -2.775, -3.149 },
							   { 0.528, -3.766 },
							   { 1.709, -3.773 } };

							   		//Criss-Cross
	double targets[6][12][2] = { { { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 0.0,  0.0 }, {  0.0, 0.0 },
								   { 0.0,  0.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 0.0,  0.0 }, { 1.0, -1.0 } },
								   	//Saturation (A)
								 { { 1.0, -1.0 }, { 0.0,  0.0 },
								   { 1.0, -1.0 }, { 0.0,  0.0 },
								   { 1.0, -1.0 }, { 0.0,  0.0 },
								   { -1.0, 1.0 }, { 0.0,  0.0 },
								   { -1.0, 1.0 }, { 1.0, -1.0 },
								   { -1.0, 1.0 }, { -1.0, 1.0 } },
								   //Diagonal
								 { { 1.0, -1.0 }, { 1.0, -1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { 1.0, -1.0 } },
								   	//Brightness
								 { { 0.0,  0.0 }, { 1.0, -1.0 },
								   { 0.0,  0.0 }, { 1.0, -1.0 },
								   { 0.0,  0.0 }, { 1.0, -1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { -1.0, 1.0 }, { -1.0, 1.0 },
								   { -1.0, 1.0 }, { -1.0, 1.0 } },
								   	//Saturation (B)
								 { { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { 0.0,  0.0 }, { -1.0, 1.0 },
								   { -1.0, 1.0 }, { 1.0, -1.0 },
								   { 0.0,  0.0 }, { 1.0, -1.0 },
								   { 0.0,  0.0 }, { 0.0,  0.0 } },
								   	//Pink-Brown
								 { { 1.0, -1.0 }, { -1.0, 1.0 },
								   { -1.0, 1.0 }, { -1.0, 1.0 },
								   { 1.0, -1.0 }, { -1.0, 1.0 },
								   { -1.0, 1.0 }, { 1.0, -1.0 },
								   { -1.0, 1.0 }, { 1.0, -1.0 },
								   { 1.0, -1.0 }, { 1.0, -1.0 } } };

	int x, y, z;
	double** patterns_pointer;
	double*** targets_pointer;
	double*** exemplars_pointer;
	int exemplar_count[6];
	ALCOVEInputLayer* in_layer;
	ALCOVEHiddenLayer* hidden_layer;
	ALCOVEOutputLayer* out_layer;
	BpFullForwardProjection* hidden_out_proj;

	// Uncomment this to analyze the network without attentional learning
	//a_learning_rate = 0.0;
	int number_of_input_dimensions = 2;
	int number_of_categories = 2;
	int number_of_hidden_units = 12;
	double** activation_array = new double*[number_of_hidden_units];
	double** softmax_array = new double*[number_of_hidden_units];
	for (x = 0; x < number_of_hidden_units; x++) {
		activation_array[x] = new double[number_of_categories];
		softmax_array[x] = new double[number_of_categories];
	}
	double initial_attention_strengths = 1.0 / (double) number_of_input_dimensions;

	double min_weights = -0.05;
	double max_weights = 0.05;

	int number_of_epochs[6];
	number_of_epochs[0] = 241;
    number_of_epochs[1] = 181;
    number_of_epochs[2] = 381;
    number_of_epochs[3] = 491;
    number_of_epochs[4] = 401;
    number_of_epochs[5] = 401;

	int test_every = 10;
	int epoch = 0;
	bool past_threshold;

	double* as = new double[number_of_input_dimensions];
	for (x = 0; x < number_of_input_dimensions; x++)
		as[x] = initial_attention_strengths;

	patterns_pointer = new double*[number_of_hidden_units];

	for (x = 0; x < number_of_hidden_units; x++) {
		patterns_pointer[x] = new double[number_of_input_dimensions];
		for (y = 0; y < number_of_input_dimensions; y++)
			patterns_pointer[x][y] = patterns[x][y];
	} // end for

	targets_pointer = new double**[6];
	exemplars_pointer = new double**[6];

	for (x = 0; x < 6; x++) {
		exemplar_count[x] = 0;
		targets_pointer[x] = new double*[number_of_hidden_units];
		exemplars_pointer[x] = new double*[number_of_hidden_units];
		for (y = 0; y < number_of_hidden_units; y++) {
			targets_pointer[x][y] = new double[2];
			for (z = 0; z < number_of_categories; z++)
				targets_pointer[x][y][z] = targets[x][y][z];
			//if (targets[x][y][0] != 0.0) {
			if (true) {
				exemplars_pointer[x][exemplar_count[x]] = new double[number_of_input_dimensions];
				for (z = 0; z < number_of_input_dimensions; z++)
					exemplars_pointer[x][exemplar_count[x]][z] = patterns_pointer[y][z];
				exemplar_count[x]++;
			}
		} // end for
	} // end for

	IndexPermuter permuter(0, number_of_hidden_units - 1);

	int problem_type = 0;

	/*
	ofstream graph;
	graph.open("alcove_graph.jgr");
	graph << "newgraph" << endl;
	graph << "border" << endl;
	graph << "xaxis size 3.0 min 0 max 1200" << endl;
	graph << "hash_labels fontsize 14" << endl;
	graph << "label fontsize 16 : epoch" << endl;
	graph << "yaxis size 2.5 min 0.5 max 1" << endl;
	graph << "label fontsize 16 : Pr(correct)" << endl;
	graph << endl;
	*/

	for (problem_type = problem; problem_type < problem + 1; problem_type++) {

	// Create Network
	in_layer = new ALCOVEInputLayer(number_of_input_dimensions, as);
	out_layer = new ALCOVEOutputLayer(number_of_categories);
	hidden_layer = new ALCOVEHiddenLayer(exemplar_count[problem_type], in_layer, exemplars_pointer[problem_type]);
	hidden_out_proj = new BpFullForwardProjection(hidden_layer, out_layer, 0);

	// Random number generator for small random initial weights

	/*
	graph << "newcurve marktype box fill 1 linetype solid color ";

	switch (problem_type) {
		case 0:
			graph << "0 0 0" << endl;
			graph << "label : Dimensional" << endl;
			break;
		case 1:
			graph << "0 0 1" << endl;
			graph << "label : Criss-Cross" << endl;
			break;
		case 2:
			graph << "0 1 0" << endl;
			graph << "label : Interior-Exterior" << endl;
			break;
		case 3:
			graph << "0 1 1" << endl;
			graph << "label : Diagonal" << endl;
			break;
	}

	graph << "pts" << endl;
	*/

	hidden_layer->setSpecificity(specificity);

	hidden_out_proj->setLearningRate(w_learning_rate);
	hidden_layer->setLearningRate(a_learning_rate);

	in_layer->setAttentionalStrengths(as);
	in_layer->clearDeltaAttentionalStrengths();
	hidden_out_proj->clearWeights();

	past_threshold = false;
	int t_number = number_of_epochs[problem_type];
	int tally;

	for (epoch = 0; epoch <= number_of_epochs[problem_type]; epoch++) {

		// Test output
		if (epoch % test_every == 0) {
		
			double average_correct = 0.0;

			int tally = 0;

			for (x = 0; x < number_of_hidden_units; x++) {

				// Forward propagation
				in_layer->setActs(patterns[x]);
				hidden_layer->computeActivations(1.0, 2.0);
				hidden_out_proj->computeNets(false);
				out_layer->computeActivations();

				out_layer->getActs(activation_array[x]);
				applySoftmax(mapping_constant, number_of_categories, activation_array[x], softmax_array[x]);

				/*
				double softsum = 0.0;
				softsum += exp(mapping_constant * activation_array[x][0]);
				softsum += exp((1.0 - mapping_constant) * activation_array[x][1]);
				softmax_array[x][0] = exp(mapping_constant * activation_array[x][0]) / softsum;
				softmax_array[x][1] = exp((1.0 - mapping_constant) * activation_array[x][1]) / softsum;
				*/

				for (y = 0; y < number_of_categories; y++) {
					if (targets_pointer[problem_type][x][y] == 1.0) {
						average_correct += softmax_array[x][y];
					}
				}

				if (targets[problem_type][x][0] != 0.0)
					tally++;

				if (epoch == number_of_epochs[problem_type] - 1) {
					DoubleDataPair* new_pair = new DoubleDataPair(0, NULL, 1, &softmax_array[x][0]);
					our_data.push_back(new_pair);
				}

			} // end for

			average_correct /= (double) tally;
			//graph << epoch << " " << average_correct << endl;

			if (average_correct >= 0.95 && !past_threshold) {
				past_threshold = true;
				t_number = epoch;
			} // end if
		} // end if

		permuter.reset();
		x = permuter.getNextIndex();
		while (x >= 0 && x < number_of_hidden_units) {

			if (targets_pointer[problem_type][x][0] != 0.0) {
				// Forward propagation
				in_layer->setActs(patterns[x]);
				hidden_layer->computeActivations(1.0, 2.0);
				hidden_out_proj->computeNets(false);
				out_layer->computeActivations();
				out_layer->setTargets(targets_pointer[problem_type][x]);

				// Learning
				out_layer->computeBpHumbleDeltas(-1.0, 1.0);
				hidden_out_proj->computeNetDeltas();
			
				hidden_out_proj->computeWeightChanges();
				hidden_layer->adjustDeltaAttentionalStrengths();

				// Online update
				hidden_out_proj->updateWeights();
				in_layer->updateAttentionalStrengths();
			}

			x = permuter.getNextIndex();
		} // end for

		// Batch update
		//hidden_out_proj->updateWeights();
		//in_layer->updateAttentionalStrengths();
	} // end epochs for


	delete hidden_out_proj;
	delete hidden_layer;
	delete in_layer;
	delete out_layer;

	} // end problem for

	for (x = 0; x < number_of_hidden_units; x++)
		delete patterns_pointer[x];
	delete patterns_pointer;

	for (x = 0; x < number_of_hidden_units; x++) {
		delete activation_array[x];
		delete softmax_array[x];
	}
	delete activation_array;
	delete softmax_array;

	for (x = 0; x < 6; x++) {
		for (y = 0; y < number_of_hidden_units; y++)
			delete targets_pointer[x][y];
		for (y = 0; y < exemplar_count[x]; y++)
			delete exemplars_pointer[x][y];
		delete targets_pointer[x];
		delete exemplars_pointer[x];
	}
	delete targets_pointer;
	delete exemplars_pointer;

	delete as;

	//graph.close();

	// Calculate the objective function value
	return_value = obj_func->evaluate(real_data, our_data);

	//writeDataPairList(our_data, "our_data");

	// Delete Data
	for (list<DataPair*>::iterator d_itr = our_data.begin(); d_itr != our_data.end(); d_itr++)
		delete (*d_itr);

	return return_value;

} // end main()
