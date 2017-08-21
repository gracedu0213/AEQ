//
//  main.cpp
//  AEQ-Q2
//
//  Created by Grace Du on 8/21/17.
//  Copyright © 2017 Grace Du. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <map>
#include <stack>
#include <limits.h>
#include <sstream>
#include <stdexcept>

//#include <NestedInteger>
#include <numeric> //iota
#include <cctype> //remove
using namespace std;




//part two: the transformation company
//Assumptions:
//1. All transformers can only fight in one battle
//2. All transformers are sorted by its rank, including Optimus Prime and Predaking
//3. In the output, the winning team will list all its members
//4. In the output, the survivors from the loser team includes both transformers that are skipped, and transformers that have won a battle
struct Transformer{
    int strength;
    int intelligence;
    int speed;
    int endurance;
    int rank;
    int courage;
    int firepower;
    int skill;
    int rating;
    string name;
    char team;
    
    Transformer(string name, char team, int strength, int intelligence, int speed,
                int endurance, int rank, int courage, int firepower, int skill){
        this->name = name;
        this->team = team;
        this->strength = strength;
        this->speed = speed;
        this->intelligence = intelligence;
        this->endurance = endurance;
        this->rank = rank;
        this->courage = courage;
        this->firepower = firepower;
        this->skill = skill;
        rating = strength + intelligence + speed + endurance + firepower;
    }
    
};

struct Result{
    int battle;
    string winningTeam;
    string losingTeam;
    vector<Transformer> survivors;
    vector<Transformer> winners;
};

//The teams should be sorted by rank
//For transformers with the same rank, sorting follows the order that transformers are entered
struct compare{
    bool operator()(Transformer t1, Transformer t2){
        return t1.rank < t2.rank;
    }
};

class Team{
private:
    priority_queue<Transformer, vector<Transformer>, compare> activeMembers;
    vector<Transformer> winners;
    vector<Transformer> losers;
    int size = 0;
    int activeSize = 0;
    int winnerCount = 0;
    int loserCount = 0;
public:
    void addMember(const Transformer &robot){
        activeMembers.push(robot);
        size++;
        activeSize++;
    }
    
    Transformer drawMember(){
        //throw exception if queue is empty; this should be rare as queue's size is checked in main
        if(activeMembers.empty())
            throw invalid_argument( "received negative value" );
        Transformer selectedMember = activeMembers.top();
        activeMembers.pop();
        activeSize--;
        return selectedMember;
    }
    
    void addWinner(const Transformer &t){
        winners.push_back(t);
        winnerCount++;
    }
    int getWinCount(){
        return winnerCount;
    }
    
    vector<Transformer> getWinners(){
        return winners;
    }
    
    void addLoser(const Transformer &t){
        losers.push_back(t);
        loserCount++;
    }
    int getLoserCount(){
        return loserCount;
    }
    
    vector<Transformer> getLosers(){
        return losers;
    }
    
    vector<Transformer> getAllMembers(){
        vector<Transformer> res;
        res = getActiveMembers();
        if(!losers.empty())
            res.insert(res.end(), losers.begin(), losers.end());
        if(!winners.empty())
            res.insert(res.end(), winners.begin(), winners.end());
        return res;
    }
    
    vector<Transformer> getSurvivors(){
        vector<Transformer> res;
        res = getActiveMembers();
        if(!winners.empty())
            res.insert(res.end(), winners.begin(), winners.end());
        return res;
    }
    
    vector<Transformer> getActiveMembers(){
        vector<Transformer> m;
        if(activeMembers.empty()) return m;
        while(!activeMembers.empty()){
            auto temp = activeMembers.top();
            activeMembers.pop();
            m.push_back(temp);
        }
        return m;
    }
    
    int getActiveSize(){
        return activeSize;
    }
    
    
    //all team destroy when either of the above face each other (or a duplicate of each other)
    //including winners of both team;
    void destroy(){
        while(!activeMembers.empty()){
            auto temp = activeMembers.top();
            activeMembers.pop();
            losers.push_back(temp);
            activeSize--;
        }
        losers.insert(losers.end(), winners.begin(), winners.end());
        winners.clear();
    }
};



void battle(const Transformer &a, const Transformer &d, Team &teamA, Team &teamD, Result &res){
    //if one or both fighters are either Optimus Prime or Predaking
    if(a.name == "Optimus Prime" && d.name == "Predaking"){
        res.winningTeam = "none";
        res.losingTeam = "none";
        teamA.destroy();
        teamD.destroy();
        res.survivors.clear();
    } else if(a.name == "Optimus Prime"){
        teamA.addWinner(a);
        teamD.addLoser(d);
    } else if(d.name == "Predaking"){
        teamA.addLoser(a);
        teamA.addWinner(d);
    }
    
    //special conditions
    if((a.courage - d.courage >= 4 && a.strength - d.strength >= 3) ||
       a.skill - d.skill >= 3){
        teamA.addWinner(a);
        teamD.addLoser(d);
        return;
    } else if ((d.courage - a.courage >= 4 && d.strength - a.strength >= 3)
               || d.skill - a.skill >= 3){
        teamA.addLoser(a);
        teamD.addWinner(d);
        return;
    }
    
    
    if(a.rating == d.rating) {
        //a tie occurs, both transformers are destroyeed
        teamA.addLoser(a);
        teamD.addLoser(d);
    } else if(a.rating > d.rating){
        teamA.addWinner(a);
        teamD.addLoser(d);
    } else {
        teamA.addLoser(a);
        teamD.addWinner(d);
    }
}

void printResult(const Result& res){
    if(res.battle >1){
        cout<< res.battle << " battles" << '\n';
    }else {
        cout<< res.battle << " battle" << '\n';
    }
    
    cout << "Winning team (" << res.winningTeam << "): ";
    if (res.winners.empty()) {
        cout << "None";
    } else {
        for(auto t : res.winners){
            cout << t.name << " ";
        }
    }
    cout << '\n';
    
    cout << "Survivors from the losing team (" << res.losingTeam << "): ";
    if(res.survivors.empty()){
        cout << "None";
    } else {
        for(auto t : res.survivors){
            cout << t.name << " ";
        }
    }
    cout << '\n';
    
}

bool checkTeamEmpty(Team &teamA, Team &teamD, Result &res){
    bool isEmpty = 0;
    
    if(teamA.getActiveSize() == 0 && teamD.getActiveSize() == 0){
        res.battle = 0;
        res.winningTeam = "none";
        res.losingTeam = "none";
        res.survivors.clear();
        isEmpty = 1;
    } else if(teamA.getActiveSize() == 0){
        res.battle = 0;
        res.winningTeam = "Decepticons";
        res.losingTeam = "Autobots";
        res.winners = teamD.getAllMembers();
        res.survivors.clear();
        isEmpty = 1;
        
    } else if(teamD.getActiveSize() == 0){
        res.battle = 0;
        res.winningTeam = "Autobots";
        res.losingTeam = "Decepticons";
        res.winners = teamA.getAllMembers();
        res.survivors.clear();
        isEmpty = 1;
    }
    return isEmpty;
    
}


int main(int argc, const char * argv[]) {
    
    //***************************INPUT DATA START****************************************//
    //function for data input is not included in this program;
    //guideline on input data format is required
    
    Transformer t1 = Transformer("SoundWave",  'D', 8, 9, 2, 6, 7, 5, 6, 10); //rating: 31
    Transformer t3 = Transformer("Hubcap",     'A', 4, 4, 4, 4, 4, 4, 4, 4);  //rating: 20
    Transformer t2 = Transformer("Bluestreak", 'A', 6, 6, 7, 9, 5, 2, 9, 7);  //rating: 37
    //Transformer t4 = Transformer("CutThroat" , 'D', 8, 4, 4, 9, 5, 8, 7, 7);  //rating: 32
    
    vector<Transformer> group;
    group.push_back(t1);
    group.push_back(t2);
    group.push_back(t3);
    //group.push_back(t4);
    
    //*************************INPUT DATA END*******************************************//
    
    //initiate results
    Result res;
    int count = 0; //number of battles
    Team teamA, teamD;
    
    for(auto t:group){
        if(t.team == 'A')
            teamA.addMember(t);
        else
            teamD.addMember(t);
    }
    
    //handle corner cases when empty team(s) present:
    //when one and only one empty team presents, the empty team forfeits
    //when both teams are empty, no winner or loser
    if (checkTeamEmpty(teamA, teamD, res)) {
        printResult(res);
        return 0;
    }
    
    while (teamA.getActiveSize() > 0 && teamD.getActiveSize() > 0) {
        count++;
        Transformer memberA = teamA.drawMember();
        Transformer memberD = teamD.drawMember();
        battle(memberA, memberD, teamA, teamD, res);
    }
    
    res.battle = count;
    
    //determine the winner
    if(teamA.getWinCount() > teamD.getWinCount()){
        res.winningTeam = "Autobots";
        res.losingTeam = "Decepticons";
        res.winners = teamA.getAllMembers();
        res.survivors = teamD.getSurvivors();
    } else if (teamA.getWinCount() < teamD.getWinCount()){
        res.winningTeam = "Decepticons";
        res.losingTeam = "Autobots";
        res.winners = teamD.getAllMembers();
        res.survivors = teamA.getSurvivors();
    } else {
        //when there is a tie, no winner or loser
        res.winningTeam = "none";
        res.losingTeam = "none";
        res.survivors.clear();
        res.winners.clear();
    }
    printResult(res);
    return 0;
}

