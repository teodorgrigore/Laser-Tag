// Copyright 2017
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>

struct Player {
	std::string name;
	bool team;
	int health;
	int score;
	int distance;
	std::vector<int> sensors;
	std::vector<int> unique_sensors;
	explicit Player(std::string nume) {
		name = nume;
		score = 0;
		distance = 0;
	}
	~Player() {}

	void newgame() {
		health = 2;
		sensors.clear();
		unique_sensors.clear();
	}
    void add_sensor(int x){
        sensors.push_back(x);
    }
	void get_unique_sensors() {
		int ok;
		for (unsigned int i = 0; i < sensors.size(); ++i){
			ok = 1;
			for(unsigned int j = 0; j < unique_sensors.size(); ++j){
				if(sensors[i] == unique_sensors[j])
					ok = 0;
			}
			if(ok != 0)
				unique_sensors.push_back(sensors[i]);
		}
	}
	int total_distance(int mat_sensors[][100]){
		if(sensors.size() > 0){
			for(unsigned int i = 0; i < sensors.size()-1; ++i){
				distance += mat_sensors[sensors[i]][sensors[i+1]];
			}
			distance += 3*unique_sensors.size();
		}
		return distance;
	}
	bool isAlive(){
		if(health != 0)
			return true;
		else
			return false;
	}
	void playerScore(const Player &target) {
		if(this->team != target.team){
			this->score += 2;
		}
		if(this->team == target.team){
			this->score -= 5;
		}
	}
};

bool sortByName(const Player &p1, const Player &p2) {
	return (p1.name < p2.name);
}

bool sortByScoreAndName(const Player &p1, const Player &p2) {
	if( p1.score != p2.score)
		return (p1.score > p2.score);
	return (p1.name < p2.name);
}
bool sortByDistanceAndName(const Player &p1, const Player &p2) {
	if( p1.distance != p2.distance)
		return (p1.distance > p2.distance);
	return (p1.name < p2.name);
}

int get_max_fireExchange(int mat_duels[][200], int &i_max, int &j_max,
 std::vector<Player> players){
	int max = 0;
	for(unsigned int i = 0; i < players.size(); i++){
		for(unsigned int j = 0; j < players.size(); j++){
			if(i<j && mat_duels[i][j] > max){
				max = mat_duels[i][j];
				i_max = i;
				j_max = j;
			}
		}
	}
	return max;
}

int main(){
	int  mat_sensors[100][100], sensor, mat_duels[200][200];
	int score_team0 = 0, score_team1 = 0, health_team0, health_team1;
	int check_score, ok;
	int bestShootScore, bestExploreScore, i_max, j_max, check_duel;
	int count_duel, max_fireExchange, next_max_fireExchange;
	float winning_chance0, winning_chance1;
	unsigned int i, j, n_sensors, n_players_team0, n_players_team1;

	std::string p, simbol, p2;
	std::vector<Player> players;

	std::cin >> n_sensors;
	for(i = 0; i < n_sensors; i++){
		mat_sensors[i][i] = 0;
		for(j = 0; j < n_sensors; j++){
			if(i != j)
				std::cin >> mat_sensors[i][j];
		}
	}

	std::cin >> n_players_team0;
	std::cin >> n_players_team1;
	for(i = 0; i < n_players_team0; i++){
		std::cin >> p;
		players.push_back(Player(p));
		players[i].team = 0;
	}
	for(j = i; j < (n_players_team0 + n_players_team1); j++){
		std::cin >> p;
		players.push_back(Player(p));
		players[j].team = 1;
	}
	health_team0 = 2*n_players_team0;
    health_team1 = 2*n_players_team1;


sort(players.begin(), players.end(), sortByName);

    for (i = 0; i < players.size(); i++){
    	for(j = 0; j < players.size(); j++){
    		mat_duels[i][j] = 0;
    	}
    }
    std::cin >> p;
    while ( p != "END_CHAMPIONSHIP" ){
    	ok = 1;
    	if(p.size() > 3 && p[0] == 'J' && p[1] == 'O' &&
    		p[2] == 'C' && p[3] == '_'){
    			for(i = 0; i < players.size(); i++){
					players[i].get_unique_sensors();
					players[i].total_distance(mat_sensors);
    				players[i].newgame();
    			}
    				if(health_team0 == 0){
    					score_team1++;
    				}
    				if(health_team1 == 0){
    					score_team0++;
    				}
    				health_team0 = 2*n_players_team0;
    				health_team1 = 2*n_players_team1;
    	}


    	if(p[1] == ':') {
    		sensor = atoi(p.c_str());
    		std::cin >> p;
    		for(i = 0; i < players.size(); i++){
                if (players[i].name == p)
                    players[i].add_sensor(sensor);
			}
			ok = 0;
		}

    	if(p[0] != 'J' && p[1] != 'O' && p[2] != 'C' && p[3] != '_' && ok != 0){
    		std::cin >> simbol;
    		if(simbol[0] == '-' && simbol[1] == '>' && simbol.size() == 2){
    			std::cin >> p2;
    			for(i = 0; i < players.size(); i++){
    				for(j = 0; j < players.size(); j++){
                		if(players[i].name == p && players[j].name == p2){
                			mat_duels[i][j]++;
                			players[i].playerScore(players[j]);
                			players[j].health = players[j].health - 1;
                			if(players[j].team ==0){
                				health_team0--;
                			} else{
                			health_team1--;
                			}
               			}
               		}
               	}
			}
		}
		std::cin >> p;
    }

	if(p == "END_CHAMPIONSHIP"){
		std::cout << "I. Top shooters" << std::endl;

		std::sort(players.begin(), players.end(), sortByScoreAndName);

		bestShootScore = players[0].score;
		if(players.size() < 6){
			for(i = 0; i < players.size(); i++){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].score << 'p' << std::endl;
			}
		} else{
			for(i = 0; i < 5; i++){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].score << 'p' << std::endl;
			}
			while(i < players.size() && players[i-1].score == players[i].score){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].score << 'p' << std::endl;
				i++;
			}
		}
		std::cout << std::endl;

		std::cout << "II. Top explorers" << std::endl;
		for(i = 0; i < players.size(); i++){
			players[i].get_unique_sensors();
			players[i].total_distance(mat_sensors);
		}
		std::sort(players.begin(), players.end(), sortByDistanceAndName);
		bestExploreScore = players[0].distance;

		if(players.size() < 6){
			for(i = 0; i < players.size(); i++){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].distance << 'p' << std::endl;
			}
		} else{
			for(i = 0; i < 5; i++){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].distance << 'p' << std::endl;
			}
			while(i < players.size() && players[i-1].distance == players[i].distance){
				std::cout << i+1 << ". " << players[i].name << ' ';
				std::cout << players[i].distance << 'p' << std::endl;
				i++;
			}
		}
		std::cout << std::endl;

		std::cout << "III. Top fire exchange" << std::endl;
		sort(players.begin(), players.end(), sortByName);

		for(i = 0; i < players.size(); i++){
			for(j = 0; j < players.size(); j++){
				if(i < j)
					mat_duels[i][j] += mat_duels[j][i];
			}
		}
		check_duel = 1;
		count_duel = 0;
		check_score = 1;
		while(check_duel != 0 && check_score == 1){
			max_fireExchange = get_max_fireExchange(mat_duels, i_max, j_max, players);
			count_duel++;
			std::cout << count_duel << ". " << players[i_max].name << " - ";
			std::cout << players[j_max].name << ' ' << max_fireExchange << std::endl;
			mat_duels[i_max][j_max] = -1;

			check_duel = 0;
			for(i = 0; i < players.size(); i++){
				for(j = 0; j < players.size(); j++){
				if(i < j && mat_duels[i][j] != -1)
					check_duel = 1;
				}
			}
			if(count_duel >= 5 && check_duel != 0){
				next_max_fireExchange = get_max_fireExchange(mat_duels,
					i_max, j_max, players);
				if(next_max_fireExchange < max_fireExchange)
					check_score = 0;
			}
		}
		std::cout << std::endl;
		std::cout << "IV. Final score" << std::endl;
		winning_chance0 = 0.0;
		winning_chance1 = 0.0;
		for(i = 0; i < players.size(); i++){
			if(players[i].team == 0 && players[i].isAlive() != 0){
				winning_chance0 += ((float)players[i].score / (float)bestShootScore +
				(float)players[i].distance / (float)bestExploreScore);
			}
			if(players[i].team == 1 && players[i].isAlive() != 0){
				winning_chance1 += ((float)players[i].score/(float)bestShootScore +
				(float)players[i].distance/(float)bestExploreScore);
			}
		}
			if(winning_chance0 > winning_chance1){
				score_team0++;
			} else{
				score_team1++;
			}
			std::cout << score_team0 << " - " << score_team1;
		}

return 0;
}
