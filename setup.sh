#!/bin/bash
curl -L -o jgraph.tar http://web.eecs.utk.edu/~jplank/plank/jgraph/2017-11-28-Jgraph.tar
tar -xvf jgraph.tar
cd jgraph
make
cd ..
curl -L -o data.zip https://github.com/statsbomb/open-data/archive/refs/heads/master.zip
unzip data.zip
curl -L -o json.zip https://github.com/nlohmann/json/archive/refs/heads/develop.zip
unzip json.zip
cd json
make 
cd ..