#!/bin/bash

GRAPHFILE=alcove_graph

jgraph < $GRAPHFILE.jgr > $GRAPHFILE.eps
evince $GRAPHFILE.eps
 
