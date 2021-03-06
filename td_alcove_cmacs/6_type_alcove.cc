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
#include <alcove_input_layer.h>
#include <alcove_hidden_layer.h>
#include <alcove_output_layer.h>
#include <layer.h>
#include <cubic_coarse_code_layer.h>
#include <cubic_activation_function.h>
#include <gaussian_activation_function.h>
#include <simple_sigmoid_activation_function.h>
#include <bp_full_forward_projection.h>
#include <uniform_random_number_generator.h>
#include <full_forward_projection.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argc, char* argv[]) {


  // For individual differences
  srand48(time(NULL));

  // A instance with good performance
  srand48(9332874);

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

  int x, y, z, i, j, k;
  double** patterns_pointer;
  double*** targets_pointer;
  ALCOVEInputLayer* in_layer;
  ALCOVEHiddenLayer* hidden_layer;
  ALCOVEOutputLayer* out_layer;
  BpFullForwardProjection* hidden_out_proj;
  UniformRandomNumberGenerator* urng;

  // Original ALCOVE creation
  double correct_tolerance = 0.8;
  double a_learning_rate = 0.0033;
  double* one_val = new double[1];
  //a_learning_rate = 0.0;
  double w_learning_rate = 0.03;
  double specificity = 6.5;
  double mapping_constant = 2.0;
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

  double min_weights = -0.05;
  double max_weights = 0.05;

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
  in_layer = new ALCOVEInputLayer(number_of_input_dimensions, as);
  out_layer = new ALCOVEOutputLayer(number_of_categories);
  hidden_layer = new ALCOVEHiddenLayer(number_of_hidden_units, in_layer, patterns_pointer);
  hidden_out_proj = new BpFullForwardProjection(hidden_layer, out_layer, 0);

  // Random number generator for small random initial weights
  urng = new UniformRandomNumberGenerator(min_weights, max_weights);

  // TD vars
  CubicCoarseCodeLayer** td_layer;
  Layer* bias_layer;
  FullForwardProjection** td_proj;
  SimpleSigmoidActivationFunction* saf;
  GaussianActivationFunction* gaf;
  CubicActivationFunction* caf;

  double* code_mins = new double[3];
  double* code_maxes = new double[3];

  double code_min = -0.0625;
  double code_max = 0.9375;

  code_mins[0] = code_min;
  code_mins[1] = code_min;
  code_mins[2] = code_min;

  code_maxes[0] = code_max;
  code_maxes[1] = code_max;
  code_maxes[2] = code_max;

  int td_side_units = 9;
  int* units_per_dim = new int[3];
  int td_tiles = 5;

  double code_domain = ((code_max - code_min) / (double) (td_side_units - 1)) / 2.0;
  double code_step = code_domain / (double) (td_tiles - 1);

  units_per_dim[0] = td_side_units;
  units_per_dim[1] = td_side_units;
  units_per_dim[2] = td_side_units;

  double x_pos[8], y_pos[8], z_pos[8];
  double x_pos2[8], y_pos2[8], z_pos2[8];
  int td_candidate[8];
  double* td_as = new double[3];
  double* real_pos = new double[3];
  double td_lrate = 0.01;
  int the_tile;
  int the_one[td_tiles];
  double some_dist;
  int bound_corner, new_corner;

  // Build Conjunctive TD equipment
  saf = new SimpleSigmoidActivationFunction();
  gaf = new GaussianActivationFunction(3, -0.1, 0.1);
  caf = new CubicActivationFunction(3);

  bias_layer = new Layer(1, saf);
  bias_layer->getUnit(0)->setAct(1.0);

  td_layer = new CubicCoarseCodeLayer*[td_tiles];
  td_proj = new FullForwardProjection*[td_tiles];

  for (x = 0; x < td_tiles; x++) {
    td_layer[x] = new CubicCoarseCodeLayer(caf, units_per_dim, code_mins, code_maxes, saf);
    code_mins[0] += code_step;
    code_mins[1] += code_step;
    code_mins[2] += code_step;

    code_maxes[0] += code_step;
    code_maxes[1] += code_step;
    code_maxes[2] += code_step;

    td_proj[x] = new FullForwardProjection(bias_layer, td_layer[x], 0);

  }

  double* td_work_vec = new double[td_layer[0]->Size()];

  ofstream dim_data[6];
  dim_data[0].open("dim_data1.dat");
  dim_data[1].open("dim_data2.dat");
  dim_data[2].open("dim_data3.dat");
  dim_data[3].open("dim_data4.dat");
  dim_data[4].open("dim_data5.dat");
  dim_data[5].open("dim_data6.dat");

  ofstream reward_data[6];
  reward_data[0].open("reward_data1.dat");
  reward_data[1].open("reward_data2.dat");
  reward_data[2].open("reward_data3.dat");
  reward_data[3].open("reward_data4.dat");
  reward_data[4].open("reward_data5.dat");
  reward_data[5].open("reward_data6.dat");

  // Begin processing of ALCOVE network
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

    // TD Processing before each problem
    for (y = 0; y < td_tiles; y++)
      td_proj[y]->initializeWeights(urng);

    // Code for biasing the weights a particular manner
    /*
      for (y = 0; y < td_tiles; y++)
      for (z = 0; z < td_layer[y]->Size(); z++) {
      td_layer[y]->getUnit(z)->getPosition(td_as);
      td_as[0] -= (1.0 / 3.0);
      td_as[1] -= (1.0 / 3.0);
      td_as[2] -= (1.0 / 3.0);
      double a_w_val = gaf->Compute(td_as);
      td_proj[y]->setWeight(0, z, a_w_val);
      }
    */

    hidden_layer->setSpecificity(specificity);

    hidden_out_proj->setLearningRate(w_learning_rate);
    hidden_layer->setLearningRate(a_learning_rate);

    in_layer->setAttentionalStrengths(as);
    in_layer->clearDeltaAttentionalStrengths();
    hidden_out_proj->clearWeights();
    //hidden_out_proj->initializeWeights(urng);

    past_threshold = false;

    for (epoch = 0; epoch <= number_of_epochs; epoch++) {

      // Test output
      if (epoch % test_every == 0) {
		
	double average_correct = 0.0;

	// Find attention weights
	for (y = 0; y < td_tiles; y++) {
	  td_proj[y]->computeNets(false);
	  td_layer[y]->computeActivations();
	  td_layer[y]->getActs(td_work_vec);

	  code_step = 0.0;
	  for (z = 0; z < td_layer[y]->Size(); z++)
	    if (td_work_vec[z] > code_step) {
	      code_step = td_work_vec[z];
	      the_one[y] = z;
	    }
	} // end y for

	code_step = 0.0;
	for (z = 0; z < td_tiles; z++) {
	  td_proj[z]->computeNets(false);
	  td_layer[z]->computeActivations();
	  td_layer[z]->getActs(td_work_vec);

	  if (td_work_vec[the_one[z]] > code_step) {
	    code_step = td_work_vec[the_one[z]];
	    the_tile = z;
	  }
	}

	// Get the position of "the top one"
	td_layer[the_tile]->getUnit(the_one[the_tile])->getPosition(td_as);

	// Find bounding box -- complex, yet neccessary
	x_pos[0] = td_as[0] + code_domain;
	x_pos[1] = td_as[0] + code_domain;
	x_pos[2] = td_as[0] + code_domain;
	x_pos[3] = td_as[0] + code_domain;
	x_pos[4] = td_as[0] - code_domain;
	x_pos[5] = td_as[0] - code_domain;
	x_pos[6] = td_as[0] - code_domain;
	x_pos[7] = td_as[0] - code_domain;

	y_pos[0] = td_as[1] + code_domain;
	y_pos[1] = td_as[1] + code_domain;
	y_pos[2] = td_as[1] - code_domain;
	y_pos[3] = td_as[1] - code_domain;
	y_pos[4] = td_as[1] + code_domain;
	y_pos[5] = td_as[1] + code_domain;
	y_pos[6] = td_as[1] - code_domain;
	y_pos[7] = td_as[1] - code_domain;

	z_pos[0] = td_as[2] + code_domain;
	z_pos[1] = td_as[2] - code_domain;
	z_pos[2] = td_as[2] + code_domain;
	z_pos[3] = td_as[2] - code_domain;
	z_pos[4] = td_as[2] + code_domain;
	z_pos[5] = td_as[2] - code_domain;
	z_pos[6] = td_as[2] + code_domain;
	z_pos[7] = td_as[2] - code_domain;

	for (y = 0; y < td_tiles; y++)
	  if (y != the_tile) {
	    for (z = 0; z < 8; z++) {
	      td_as[0] = x_pos[z];
	      td_as[1] = y_pos[z];
	      td_as[2] = z_pos[z];

	      td_layer[y]->setCoarseActs(td_as);
	      td_layer[y]->getActs(td_work_vec);

	      for (i = 0; i < td_layer[y]->Size(); i++)
		if (td_work_vec[i] > 0.0)
		  td_candidate[z] = i;

	    }

	    td_proj[y]->computeNets(false);
	    td_layer[y]->computeActivations();
	    td_layer[y]->getActs(td_work_vec);

	    code_step = 0.0;
	    for (z = 0; z < 8; z++)
	      if (td_work_vec[td_candidate[z]] > code_step) {
		code_step = td_work_vec[td_candidate[z]];
		the_one[y] = td_candidate[z];
	      }

	    // Shrink the zone -- ???
	    td_layer[y]->getUnit(the_one[y])->getPosition(td_as);
	    x_pos2[0] = td_as[0] + code_domain;
	    x_pos2[1] = td_as[0] + code_domain;
	    x_pos2[2] = td_as[0] + code_domain;
	    x_pos2[3] = td_as[0] + code_domain;
	    x_pos2[4] = td_as[0] - code_domain;
	    x_pos2[5] = td_as[0] - code_domain;
	    x_pos2[6] = td_as[0] - code_domain;
	    x_pos2[7] = td_as[0] - code_domain;

	    y_pos2[0] = td_as[1] + code_domain;
	    y_pos2[1] = td_as[1] + code_domain;
	    y_pos2[2] = td_as[1] - code_domain;
	    y_pos2[3] = td_as[1] - code_domain;
	    y_pos2[4] = td_as[1] + code_domain;
	    y_pos2[5] = td_as[1] + code_domain;
	    y_pos2[6] = td_as[1] - code_domain;
	    y_pos2[7] = td_as[1] - code_domain;

	    z_pos2[0] = td_as[2] + code_domain;
	    z_pos2[1] = td_as[2] - code_domain;
	    z_pos2[2] = td_as[2] + code_domain;
	    z_pos2[3] = td_as[2] - code_domain;
	    z_pos2[4] = td_as[2] + code_domain;
	    z_pos2[5] = td_as[2] - code_domain;
	    z_pos2[6] = td_as[2] + code_domain;
	    z_pos2[7] = td_as[2] - code_domain;

	    // Compute mean of bounding box
	    for (z = 0; z < number_of_input_dimensions; z++)
	      real_pos[z] = 0.0;

	    for (z = 0; z < 8; z++) {
	      real_pos[0] += x_pos[z];
	      real_pos[1] += y_pos[z];
	      real_pos[2] += z_pos[z];
	    }

	    real_pos[0] /= 8.0;
	    real_pos[1] /= 8.0;
	    real_pos[2] /= 8.0;

	    // Find closest points to means
	    code_step = 80.0;

	    for (z = 0; z < 8; z++) {
	      some_dist = sqrt(pow(x_pos[z] - td_as[0], 2.0) +
			       pow(y_pos[z] - td_as[1], 2.0) +
			       pow(z_pos[z] - td_as[2], 2.0));

	      if (some_dist < code_step) {
		code_step = some_dist;
		bound_corner = z;
	      } // end if
	    } // end z for

	    code_step = 80.0;
	    for (z = 0; z < 8; z++) {
	      some_dist = sqrt(pow(x_pos2[z] - real_pos[0], 2.0) +
			       pow(y_pos2[z] - real_pos[1], 2.0) +
			       pow(z_pos2[z] - real_pos[2], 2.0));

	      if (some_dist < code_step) {
		code_step = some_dist;
		new_corner = z;
	      } // end if
	    } // end z for

	    // Original
	    real_pos[0] = x_pos[bound_corner];
	    real_pos[1] = y_pos[bound_corner];
	    real_pos[2] = z_pos[bound_corner];

	    // This tile
	    td_as[0] = x_pos2[new_corner];
	    td_as[1] = y_pos2[new_corner];
	    td_as[2] = z_pos2[new_corner];

	    // Rebuild box
	    x_pos[0] = real_pos[0];
	    y_pos[0] = real_pos[1];
	    z_pos[0] = real_pos[2];

	    x_pos[1] = td_as[0];
	    y_pos[1] = td_as[1];
	    z_pos[1] = td_as[2];

	    x_pos[2] = real_pos[0];
	    y_pos[2] = td_as[1];
	    z_pos[2] = td_as[2];

	    x_pos[3] = td_as[0];
	    y_pos[3] = real_pos[1];
	    z_pos[3] = real_pos[2];

	    x_pos[4] = td_as[0];
	    y_pos[4] = real_pos[1];
	    z_pos[4] = td_as[2];

	    x_pos[5] = real_pos[0];
	    y_pos[5] = td_as[1];
	    z_pos[5] = real_pos[2];

	    x_pos[6] = td_as[0];
	    y_pos[6] = td_as[1];
	    z_pos[6] = real_pos[2];

	    x_pos[7] = real_pos[0];
	    y_pos[7] = real_pos[1];
	    z_pos[7] = td_as[2];

	  }

	// Finished - Compute mean of bounding box
	for (z = 0; z < number_of_input_dimensions; z++)
	  real_pos[z] = 0.0;

	for (z = 0; z < 8; z++) {
	  real_pos[0] += x_pos[z];
	  real_pos[1] += y_pos[z];
	  real_pos[2] += z_pos[z];
	}

	real_pos[0] /= 8.0;
	real_pos[1] /= 8.0;
	real_pos[2] /= 8.0;

	in_layer->setAttentionalStrengths(real_pos);

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

	if (average_correct >= 0.95 && !past_threshold) {
	  cout << "Problem #" << (problem_type + 1) << " passed at " << epoch << " epochs." << endl;
	  past_threshold = true;
	} // end if

      } // end if

      for (x = 0; x < 8; x++) {

	// Find attention weights
	for (y = 0; y < td_tiles; y++) {
	  td_proj[y]->computeNets(false);
	  td_layer[y]->computeActivations();
	  td_layer[y]->getActs(td_work_vec);

	  code_step = 0.0;
	  for (z = 0; z < td_layer[y]->Size(); z++)
	    if (td_work_vec[z] > code_step) {
	      code_step = td_work_vec[z];
	      the_one[y] = z;
	    }
	} // end y for

	code_step = 0.0;
	for (z = 0; z < td_tiles; z++) {
	  td_layer[z]->getActs(td_work_vec);

	  if (td_work_vec[the_one[z]] > code_step) {
	    code_step = td_work_vec[the_one[z]];
	    the_tile = z;
	  }
	}

	// Get the position of "the top one"
	td_layer[the_tile]->getUnit(the_one[the_tile])->getPosition(td_as);

	// Find bounding box -- complex, yet neccessary
	x_pos[0] = td_as[0] + code_domain;
	x_pos[1] = td_as[0] + code_domain;
	x_pos[2] = td_as[0] + code_domain;
	x_pos[3] = td_as[0] + code_domain;
	x_pos[4] = td_as[0] - code_domain;
	x_pos[5] = td_as[0] - code_domain;
	x_pos[6] = td_as[0] - code_domain;
	x_pos[7] = td_as[0] - code_domain;

	y_pos[0] = td_as[1] + code_domain;
	y_pos[1] = td_as[1] + code_domain;
	y_pos[2] = td_as[1] - code_domain;
	y_pos[3] = td_as[1] - code_domain;
	y_pos[4] = td_as[1] + code_domain;
	y_pos[5] = td_as[1] + code_domain;
	y_pos[6] = td_as[1] - code_domain;
	y_pos[7] = td_as[1] - code_domain;

	z_pos[0] = td_as[2] + code_domain;
	z_pos[1] = td_as[2] - code_domain;
	z_pos[2] = td_as[2] + code_domain;
	z_pos[3] = td_as[2] - code_domain;
	z_pos[4] = td_as[2] + code_domain;
	z_pos[5] = td_as[2] - code_domain;
	z_pos[6] = td_as[2] + code_domain;
	z_pos[7] = td_as[2] - code_domain;

	for (y = 0; y < td_tiles; y++)
	  if (y != the_tile) {
	    for (z = 0; z < 8; z++) {
	      td_as[0] = x_pos[z];
	      td_as[1] = y_pos[z];
	      td_as[2] = z_pos[z];

	      td_layer[y]->setCoarseActs(td_as);
	      td_layer[y]->getActs(td_work_vec);

	      for (i = 0; i < td_layer[y]->Size(); i++)
		if (td_work_vec[i] > 0.0)
		  td_candidate[z] = i;

	    }

	    td_proj[y]->computeNets(false);
	    td_layer[y]->computeActivations();
	    td_layer[y]->getActs(td_work_vec);

	    code_step = 0.0;
	    for (z = 0; z < 8; z++)
	      if (td_work_vec[td_candidate[z]] > code_step) {
		code_step = td_work_vec[td_candidate[z]];
		the_one[y] = td_candidate[z];
	      }

	    // Shrink the zone -- ???
	    td_layer[y]->getUnit(the_one[y])->getPosition(td_as);
	    x_pos2[0] = td_as[0] + code_domain;
	    x_pos2[1] = td_as[0] + code_domain;
	    x_pos2[2] = td_as[0] + code_domain;
	    x_pos2[3] = td_as[0] + code_domain;
	    x_pos2[4] = td_as[0] - code_domain;
	    x_pos2[5] = td_as[0] - code_domain;
	    x_pos2[6] = td_as[0] - code_domain;
	    x_pos2[7] = td_as[0] - code_domain;

	    y_pos2[0] = td_as[1] + code_domain;
	    y_pos2[1] = td_as[1] + code_domain;
	    y_pos2[2] = td_as[1] - code_domain;
	    y_pos2[3] = td_as[1] - code_domain;
	    y_pos2[4] = td_as[1] + code_domain;
	    y_pos2[5] = td_as[1] + code_domain;
	    y_pos2[6] = td_as[1] - code_domain;
	    y_pos2[7] = td_as[1] - code_domain;

	    z_pos2[0] = td_as[2] + code_domain;
	    z_pos2[1] = td_as[2] - code_domain;
	    z_pos2[2] = td_as[2] + code_domain;
	    z_pos2[3] = td_as[2] - code_domain;
	    z_pos2[4] = td_as[2] + code_domain;
	    z_pos2[5] = td_as[2] - code_domain;
	    z_pos2[6] = td_as[2] + code_domain;
	    z_pos2[7] = td_as[2] - code_domain;

	    // Compute mean of bounding box
	    for (z = 0; z < number_of_input_dimensions; z++)
	      real_pos[z] = 0.0;

	    for (z = 0; z < 8; z++) {
	      real_pos[0] += x_pos[z];
	      real_pos[1] += y_pos[z];
	      real_pos[2] += z_pos[z];
	    }

	    real_pos[0] /= 8.0;
	    real_pos[1] /= 8.0;
	    real_pos[2] /= 8.0;

	    // Find closest points to means
	    code_step = 80.0;
	    for (z = 0; z < 8; z++) {
	      some_dist = sqrt(pow(x_pos[z] - td_as[0], 2.0) +
			       pow(y_pos[z] - td_as[1], 2.0) +
			       pow(z_pos[z] - td_as[2], 2.0));

	      if (some_dist < code_step) {
		code_step = some_dist;
		bound_corner = z;
	      } // end if
	    } // end z for

	    code_step = 80.0;
	    for (z = 0; z < 8; z++) {
	      some_dist = sqrt(pow(x_pos2[z] - real_pos[0], 2.0) +
			       pow(y_pos2[z] - real_pos[1], 2.0) +
			       pow(z_pos2[z] - real_pos[2], 2.0));

	      if (some_dist < code_step) {
		code_step = some_dist;
		new_corner = z;
	      } // end if
	    } // end z for

	    // Original
	    real_pos[0] = x_pos[bound_corner];
	    real_pos[1] = y_pos[bound_corner];
	    real_pos[2] = z_pos[bound_corner];

	    // This tile
	    td_as[0] = x_pos2[new_corner];
	    td_as[1] = y_pos2[new_corner];
	    td_as[2] = z_pos2[new_corner];

	    // Rebuild box
	    x_pos[0] = real_pos[0];
	    y_pos[0] = real_pos[1];
	    z_pos[0] = real_pos[2];

	    x_pos[1] = td_as[0];
	    y_pos[1] = td_as[1];
	    z_pos[1] = td_as[2];

	    x_pos[2] = real_pos[0];
	    y_pos[2] = td_as[1];
	    z_pos[2] = td_as[2];

	    x_pos[3] = td_as[0];
	    y_pos[3] = real_pos[1];
	    z_pos[3] = real_pos[2];

	    x_pos[4] = td_as[0];
	    y_pos[4] = real_pos[1];
	    z_pos[4] = td_as[2];

	    x_pos[5] = real_pos[0];
	    y_pos[5] = td_as[1];
	    z_pos[5] = real_pos[2];

	    x_pos[6] = td_as[0];
	    y_pos[6] = td_as[1];
	    z_pos[6] = real_pos[2];

	    x_pos[7] = real_pos[0];
	    y_pos[7] = real_pos[1];
	    z_pos[7] = td_as[2];

	  }

	// Finished - Compute mean of bounding box
	for (z = 0; z < number_of_input_dimensions; z++)
	  td_as[z] = 0.0;

	for (z = 0; z < 8; z++) {
	  td_as[0] += x_pos[z];
	  td_as[1] += y_pos[z];
	  td_as[2] += z_pos[z];
	}

	td_as[0] /= 8.0;
	td_as[1] /= 8.0;
	td_as[2] /= 8.0;

	double as_max = 0.0;
	for (z = 0; z < number_of_input_dimensions; z++) 
	  if (td_as[z] > as_max)
	    as_max = td_as[z];

	double dim_var = 0.0;
	for (z = 0; z < number_of_input_dimensions; z++)
	  dim_var += td_as[z] / as_max;

	dim_data[problem_type] << ((epoch * 8) + x) << " " << dim_var << endl;

	in_layer->setAttentionalStrengths(td_as);

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

	// TD reward value variable
	double reward;
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

	reward_data[problem_type] << ((epoch * 8) + x) << " " << reward << endl;

	for (z = 0; z < td_tiles; z++) {
	  td_layer[z]->getUnit(the_one[z])->getNets(td_as);
	  td_as[0] = saf->Derivative(td_as[0]);
	  double w_val = td_lrate * (reward - td_layer[z]->getUnit(the_one[z])->getAct()) * td_as[0];
	  td_proj[z]->setWeight(0, the_one[z], td_proj[z]->getWeight(0, the_one[z]) + w_val);
	} // end z for

      } // end for

      // Batch update
      //hidden_out_proj->updateWeights();
      //in_layer->updateAttentionalStrengths();
    } // end epochs for

    double* some_array = new double[number_of_input_dimensions];
    in_layer->getAttentionalStrengths(some_array);
    cout << "Attentional Strengths for Problem #" << (problem_type + 1) << ": ";
    for (x = 0; x < number_of_input_dimensions; x++)
      cout << some_array[x] << " ";
    cout << endl;
    delete some_array;

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
  delete one_val;

  graph.close();
  dim_data[0].close();
  dim_data[1].close();
  dim_data[2].close();
  dim_data[3].close();
  dim_data[4].close();
  dim_data[5].close();
  reward_data[0].close();
  reward_data[1].close();
  reward_data[2].close();
  reward_data[3].close();
  reward_data[4].close();
  reward_data[5].close();

  return 0;

} // end main()
