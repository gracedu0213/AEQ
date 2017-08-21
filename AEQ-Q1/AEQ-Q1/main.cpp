//
//  main.cpp
//  AEQ-Q1
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
#include <List>
#include <sstream>
#include <stdexcept>

//#include <NestedInteger>
#include <numeric> //iota
#include <cctype> //remove
using namespace std;


//part 1: the Castle Company
//additional assumption:
//1. one can alwasy build a castle at the end of the array, given that the array is non-empty
//      1.1 if the array has size of one, the first and the last element is the same, and thereby complying with the first assumption
//      1.2 if the last element n is different from the previous element, one castle can be built
//      1.3 if the last element is the same as the previous element, then one castle can be build on the sequence
//2. a sequence of elements with the same value is considered as "flat ground"

int buildCastle(vector<int> field){
    //if field is empty
    if(field.empty()) return 0;
    
    int size = (int)field.size();
    
    //insert a psuedo element at the beginning of the array to better handle corner cases
    //the first psuedo element has the value of the first orginal element to avoid creating extra castle
    
    field.insert(field.begin(), field[0]);
    
    int count = 0; //track the number of peaks and valleys
    
    int begin = 1; // track where the flat groud begin
    
    int idx = 1; //track the end of search
    
    while(idx < size){
        while(field[idx] == field[idx+1] && idx < size){
            idx++;
        }
        if (idx >= size) {
            
            //idx reach the end when the array ends with a flat ground
            //add the castle that is built at the end of the array
            return ++count;
        }
        
        //a valley is found, castle count plus one
        //pseudo element is added to ensure that field[begin-1] is always within bound
        if((field[begin-1] > field[begin] || begin == 1)&& field[idx+1] > field[idx]){
            count++;
        }
        //a peak is found, castle count plus one
        if((field[begin - 1] < field[begin]|| begin == 1) && field[idx+1] < field[idx]){
            count++;
        }
        
        idx++;
        begin = idx;
    }
    //search reaches the end when the array ends with at least two distinct elements
    return ++count;
}


int main(int argc, const char * argv[]) {
    //test cases for part 1
    vector<int> field1 = {};
    vector<int> field2 = {1};
    vector<int> field3 = {1, 1};
    vector<int> field4 = {3,2,1};
    vector<int> field5 = {1, 1, 2, 2};
    vector<int> field6 = {1, 2, 5, 4, 3, 6};
    vector<int> field7 = {1, 1, 2, 5, 4, 3, 6};
    vector<int> field8 = {1, 1, 2, 5, 4, 3, 6, 6};
    vector<int> field9 = {1, 2, 2, 2, 5, 4, 3, 6, 6};
    vector<int> field10 = {1, 2,2};
    cout << "test cases for part 1" << '\n';
    cout << "field1 has " << buildCastle(field1) << " castles" << '\n';
    cout << "field2 has " << buildCastle(field2) << " castles" << '\n';
    cout << "field3 has " << buildCastle(field3) << " castles" << '\n';
    cout << "field4 has " << buildCastle(field4) << " castles" << '\n';
    cout << "field5 has " << buildCastle(field5) << " castles" << '\n';
    cout << "field6 has " << buildCastle(field6) << " castles" << '\n';
    cout << "field7 has " << buildCastle(field7) << " castles" << '\n';
    cout << "field8 has " << buildCastle(field8) << " castles" << '\n';
    cout << "field9 has " << buildCastle(field9) << " castles" << '\n';
    cout << "field10 has " << buildCastle(field10) << " castles" << '\n';

    return 0;
}
