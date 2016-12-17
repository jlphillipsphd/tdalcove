# tdalcove
ALCOVE with TD-learning of Dimensional Attention

Basic usage (assumed linux environment):

1. Compile the libnnet.a library:
cd libnnet; make

2. Compile one of the three ALCOVE versions (alcove; td_alcove_conj; td_alcove_cmacs):
cd alcove; make

3. Train the neural net and construct the learning graph:
./6_type_alcove
./view.bash

Note -- you will need to install jgraph to produce the .ps files
and the view.bash script will use evince to view the .ps file
by default. You can skip the view step if desired.

Citation:
Phillips, J. L., & Noelle, D. C. (2004). Reinforcement Learning of Dimensional Attention for Categorization. In <i>Proceedings of the 26th Annual Meeting of the Cognitive Science Society</i>. Chicago, IL.

