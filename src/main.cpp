/*
    Racheal Dylewski
    matchdata.cpp
    9/26/2021
*/
#include "matchdata.h"
#include<cstdio>
#include<string>
#include<cctype>
#include<iostream>

using namespace std;

int main(int argc, char** argv){
    std::string home, away, outfile;
    bool set_player;
    if(argc < 4){
        fprintf(stderr, "Usage: home away player(Y/N)");
        exit(0);
    }

    home = argv[1];
    away = argv[2];

    if(string(argv[3]).compare("y") == 0 || string(argv[3]).compare("Y") == 0) set_player = true;
    else if(string(argv[3]).compare("n") == 0 || string(argv[3]).compare("N") == 0) set_player = false;
    else{
        cout << argv[3] << endl;
        fprintf(stderr, "Usage: (Y/N) create heatmap based on player\n");
        exit(0);
    }

    MatchData *match_data = new MatchData;

    outfile = "FifaHeatmap.jpg";
    
    match_data->get_matches();
    match_data->set_game(home, away, set_player);
    match_data->create_jgraph(outfile, home, away);

    return 0;
}