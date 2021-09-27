/*
    Racheal Dylewski
    matchdata.cpp
    9/26/2021
*/
#include "matchdata.h"
#include<iterator>
#include<stdexcept>
#include<fstream>
#include<iostream>
#include<map>
#include<cmath>
#include<numeric>
#include<cstdio>
#include<algorithm>

double kde_quadratic(double d, float h){
    double dn = d/h;
    double P = pow((15.0/16.0),2) * pow((1-dn*dn), 2);

    return P;
}

MatchData::MatchData(){
    player = "";
    get_matches();
}

void MatchData::get_matches(){
    /* Reads in json and fills matches grid */
    std::string filename = "open-data-master/data/matches/43/3.json";
    std::ifstream ifs(filename);
    json match_json = json::parse(ifs);
    
    // read all match data into match multimap byt storing home/away team and match id
    for(int i=0; i<match_json.size(); i++){      

        std::string match_id = match_json[i]["match_id"].dump();
        match_id.erase(remove(match_id.begin(), match_id.end(), '\"'), match_id.end());
        
        std::string home_team = match_json[i]["home_team"]["country"]["name"].dump();
        home_team.erase(remove(home_team.begin(), home_team.end(), '\"'), home_team.end());
        
        std::string away_team = match_json[i]["away_team"]["country"]["name"].dump();
        away_team.erase(remove(away_team.begin(), away_team.end(), '\"'), away_team.end());
        
        std::vector<std::string> teams = {home_team, away_team};

        match_map.insert({teams, match_id});
    }
}

void MatchData::set_game(std::string team1, std::string team2, bool set_player){
    // Find id of the teams (find reverse or set order?)
    std::vector<std::string> teams = {team1, team2};
    std::multimap<std::vector<std::string>, std::string>::iterator it;

    it = match_map.find(teams);
    if(it == match_map.end()){
        fprintf(stderr, "Invalid team combination. Are you sure you have the correct home team/away team combination?\n");
        exit(0);
    }
   
    if(set_player) get_players(it->second);
    parse_pass_data(it->second);
}

void MatchData::get_players(std::string game_id){
    /* Gets player list and lets user select which player to map */
    std::string match_id = game_id + ".json";
    std::string filename, p, country;
    std::vector<std::string> players;
    
    filename = "open-data-master/data/lineups/" + match_id;
    std::ifstream ifs(filename);

    json event_json = json::parse(ifs);
    
    // Get all players from match lineup and print
    for(int i=0;i<event_json.size();i++){ 
        for(int j=0;j<event_json[i]["lineup"].size();j++){
            p = event_json[i]["lineup"][j]["player_name"].dump();
            country = event_json[i]["lineup"][j]["country"]["name"].dump();

            // remove string quotes
            p.erase(remove(p.begin(), p.end(), '\"'), p.end());
            country.erase(remove(country.begin(), country.end(), '\"'), country.end());
            players.push_back(p);

            printf("%s - %s\n", p.c_str(), country.c_str());
        }
    }

    printf("\nEnter a player's name: ");
    while(true){
        std::getline(std::cin, player);
        if(std::find(players.begin(), players.end(), player) != players.end()){
            break;
        }else{
            printf("\nPlayer not found. Enter a new player: ");
            fflush(stdout);
        }
    }
}

void MatchData::parse_pass_data(std::string game_id){
    /* Parse match json and store pass coordinates */
    std::string p;
    const std::string match_id = game_id + ".json";
    std::string filename = "open-data-master/data/events/" + match_id;
    std::ifstream ifs(filename);
    std::vector<int> xx, yy, coordinates;
    json event_json = json::parse(ifs);
   
    for(int i=0; i<event_json.size(); i++){
        std::string type = event_json[i]["type"]["name"].dump();
        type.erase(remove(type.begin(), type.end(), '\"'), type.end());
  
        if(type.compare("Pass") == 0){
            // If player was set, filter by player name
            if(player.compare("")){
                p = event_json[i]["player"]["name"].dump();
                p.erase(remove(p.begin(), p.end(), '\"'), p.end());
   
                if(p.compare(player)) continue;
            }

            // Get coordinates
            coordinates = event_json[i]["location"].get<std::vector<int>>();
            
            // Pitch is transposed on jgraph --> coordinates = (y,x)
            y.push_back(coordinates[0] - 1);
            x.push_back(coordinates[1] - 1);
        }
    }

    create_densities();
}

void MatchData::create_densities(){
    /* Calculates heatmap densities*/
    std::vector<std::vector<int> > v;
    std::vector<std::vector<double> > intensity_list;

    double h, p;
    h = 10.0;

    // Calculate density for each grid position on map
    for(int j=0;j<PITCH_WIDTH;j++){
        std::vector<double> intensity_row;
        
        for(int k=0;k<PITCH_HEIGHT;k++){
            std::vector<double> kde_value_list;
            
            // Loop through passes and calculate kernel density based on current grid coordinate
            for(int i=0;i<x.size();i++){
                double centerx = k + 3/2;
                double centery = j + 3/2;
                double d = sqrt(pow(centerx - x[i], 2) + pow(centery - y[i], 2));
               
                if(d<=h) p = kde_quadratic(d, h);
                else p = 0;
            
                kde_value_list.push_back(p);
            
            }
            // Sum all the density values
            double total = 0;
            for(int m=0;m<kde_value_list.size();m++){
                total = total + kde_value_list[m];
            }
            
            intensity_row.push_back(total);
        }
        intensity_list.push_back(intensity_row);
    }
   
    pass_map = intensity_list;

}

std::string MatchData::assign_heatmap_color(int value){
    /* Assigns an rbg based on density value */
    double rbg[3] = {1, 1, 0};
    rbg[1] = rbg[1] - (value * 0.3);
    
    if(rbg[1] <= 0) {
        rbg[1] = 0;
        rbg[0] = rbg[0] - (value * 0.3);
        rbg[2] = rbg[2] + (value * 0.05);
    }


    return std::to_string(rbg[0]) + " " + std::to_string(rbg[1]) + " " + std::to_string(rbg[2]);
}

void MatchData::create_jgraph(std::string outfile, std::string team1, std::string team2){
    std::string jgraph_filename = "FifaHeatMap_Jgraph.jgr";

    std::ofstream jgraph_file(jgraph_filename);
    // Create standard pitch
    jgraph_file << "newgraph\nxaxis min 0 max 81 nodraw size 6\nyaxis min 0 max 121 nodraw size 9\nnewcurve marktype box marksize 80 120 cfill 0 .7 0 pts 40 60\n";

    // loop through pass map and add point color coded to density
    // if rbg is 0,0,0, then don't write a point
    std::string rbg;
   
    for(int i=0;i<pass_map.size();i++){
        for(int j=0;j<pass_map[i].size();j++){
            if(pass_map[i][j] != 0){
                jgraph_file << "newcurve marktype box marksize 1 1 color " << assign_heatmap_color(pass_map[i][j]) << " pts " << (j) << " " << (i) << "\n";

            }
        }
    }

    // Create penalty boxes
    jgraph_file << "newline linethickness 3 gray 1 pts 22.3 0 22.3 14.6\n" << "newline linethickness 3 gray 1 pts 57.6 0 57.6 14.6\n" << "newline linethickness 3 gray 1 pts 22.3 14.6 57.6 14.6\n";

    jgraph_file << "newline linethickness 3 gray 1 pts 22.3 120 22.3 105.4\n" << "newline linethickness 3 gray 1 pts 57.6 120 57.6 105.4\n" << "newline linethickness 3 gray 1 pts 22.3 105.4 57.6 105.4\n";
    // Create 6 yard boxes
    jgraph_file << "newline linethickness 3 gray 1 pts 32 0 32 4.9\n" << "newline linethickness 3 gray 1 pts 48 0 48 4.9\n" << "newline linethickness 3 gray 1 pts 32 4.9 48 4.9\n";
    
    jgraph_file << "newline linethickness 3 gray 1 pts 32 120 32 115.1\n" << "newline linethickness 3 gray 1 pts 48 120 48 115.1\n" << "newline linethickness 3 gray 1 pts 32 115.1 48 115.1\n";
    // Create center field and center circle
    jgraph_file << "newline linethickness 3 gray 1 pts 0 60 80 60\n" << "newline linethickness 3 bezier  gray 1 pts 23.8 60 25 41.8 55 41.8 56.2 60\n" << "newline linethickness 3 bezier gray 1 pts 23.8 60 25 78.2 55 78.2 56.2 60\n";
    // Create graph title
    jgraph_file << "newstring fontsize 28 hjc vjt x 40 y 141\n" << "\t: " << team1 << " vs " << team2 << "\n";
    jgraph_file << "newstring fontsize 20 hjc vjt x 40 y 131\n" << "\t: " << player << "\n";

    jgraph_file.close();

    char run_jgraph[300];
    sprintf(run_jgraph, "jgraph/jgraph -P FifaHeatMap_Jgraph.jgr | ps2pdf - | convert -density 300 - -quality 100 %s-%s-heatmap.jpg", team1.c_str(), team2.c_str());
    system(run_jgraph);
}
