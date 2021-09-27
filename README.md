# FIFA 2018 World Cup Pass Heatmap
- Racheal Dylewski
- COSC 594 Lab 2 Jgraph
- September 28, 2021
<br />

Heatmap is a tool that creates a density map of passes made in particular matches of the 2018 FIFA World Cup. Given 2 teams, the program will create a JPEG heatmap of where the passes occured on the field. The program can visual passes between both teams over the entire game, as well as for a particular player provided by the user.

-----

### Set Up
Heatmap relies on several outside projects to make its magic possible and uses **curl** to download zip files for data and dependancies.
To set up, run
`sh setup.sh`

To build, run
`make`

To build and generate 5 beautiful images, run
`make run`

### Usage
Heatmap takes 3 command line arguments:
`./heatmap HOME_TEAM AWAY_TEAM PLAYER_CHOICE(Y/N)`

`HOME_TEAM` is the home team
`AWAY_TEAM` is the away team 
`PLAYER_CHOICE` is a simple 'y' or 'n'. If the user provides 'n', then the program will generate a heatmap based on all players passes for both teams. If 'y' is provided, then the program will provide the starting lineups for the teams, and will prompt the user for the **exact** name of a player. The program will generate the heatmap based only on passes completed by the specified player.

##### Requirements & Input Reference
Home team and away team must be capitalized and in specific order. A list of all games in the competition can be found in **matches.txt**. The program takes country spellings exactly as they appear in the file. 
Player input must be spelled exactly as it appears on program output. If there are accents or special character in a player's name, it is recommended to copy & paste.

All JPEG images will be sent to the folder **output_graphs** once generated.

### Examples
`./heatmap Croatia England y`
Input: `Luka Modrić`

![image](https://user-images.githubusercontent.com/46689828/134852738-2fbfd307-7d44-4b1a-9cab-3e4035213ca5.png)




### Dependancies
- [nlohmann/json](https://github.com/nlohmann/json)
- [StatsBomb Open Data](https://github.com/statsbomb/open-data)
- Dr. Plank's Jgraph
