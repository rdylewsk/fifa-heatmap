/*
    Racheal Dylewski
    matchdata.h
    9/26/2021
*/
#include<string>
#include<vector>
#include<map>
#include "../json-develop/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

#define COMPETITION "FIFA World Cup"
const std::string COMPETITION_ID = "43";
const int PITCH_WIDTH = 120;
const int PITCH_HEIGHT = 80;

class MatchData {
    private:
        std::vector<std::vector<double> > pass_map; // 2D grid of number of passes at a coordinate
        std::multimap<std::vector<std::string>, std::string> match_map;
        std::vector<int> x;
        std::vector<int> y;
        std::string player;

        void get_players(std::string); // Gets starting lineup of match for user selection
        void get_matches(); // Reads in all matches into matches grid
        void parse_pass_data(std::string);
        void create_densities(); // Prepares match map for jgraph
        std::string assign_heatmap_color(int); // Assigns RBG values for heatmap

    public:
        MatchData();

        void set_game(std::string, std::string, bool); // Creates new game 
        void create_jgraph(std::string, std::string, std::string); // Creates jgraph heatmap
};

double kde_quadratic(double d, float h);