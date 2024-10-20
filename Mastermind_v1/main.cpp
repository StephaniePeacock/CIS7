/* 
 * File:   main.cpp
 * Author: Stephanie Peacock
 * Created on: 10/19/24 at 1:41PM
 * Purpose:  Mastermind Game v1
 */

//System Libraries Here
#include <iostream>     //Input/Output Library
#include <ctime>        //Time Libary
using namespace std;

//User Libraries
//Global Constants, no Global Variables are allowed
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...
//Function Prototypes
void generate(int []);                  //Generate a code
void make(int []);                      //Make a guess
bool check(const int [], const int[]);  //Check if the guess is correct
void result(bool[], bool[]);            //Print results

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set Random Number Seed Here
    srand(static_cast<unsigned int>(time(0)));
    //Declare all Variables Here
    int code[4];                        //Code we have to Guess
    int guess[4];                       //Our Guesses
    bool match = false;                 //So we can end the game

    
    generate(code);                     //Create a code
    while(!match){                      //Loop runs until we get all 4
        make(guess);                    //Make a Guess
        match = check(code,guess);      //See if all 4 are correct
    }
    //Exit
    return 0;
}

//create a random code
void generate(int code[]){
    for(int i = 0; i < 4; i++){
        code[i] = rand()%10;        cout << code[i] << " ";     //for testing
    }                               cout << endl;               //for testing
}

//make a guess
void make(int guess[]) {
    string input;               
    bool invalid = true;    
    
    cout << "Enter a guess: ";
    
    while(invalid){                 //Run till we get exactly 4 digits
        getline(cin,input);
        if(input.length() == 4){    //Correct length
            invalid = false;        //Good so far 
            for(int i = 0; i < 4; i++){
                if(input[i] >= '0' && input[i] <= '9'){ //Current char is a number
                    guess[i] = (input[i] - '0');        //Set current guess to number
                }
                else {
                    cout << "Guess must contain only digits." << endl;
                    invalid = true; //Not a digit - Reenter Guess
                    i = 4;          //end the loop not digit
                }
            }
        }
        else {                      //Not the correct length
            cout << "Guess must be 4 digits long." << endl;
            invalid = true;         //Reenter Guess
            
        }
    }
}

bool check(const int code[], const int guess[]){
    bool exact[4]   = {false};      //Exact matches - default is false
    bool partial[4] = {false};      //Partial matches - default is false
    bool match = true;              //All 4 match - default true, change if not
    bool found = false;
    
    //Check if we have any exact matches
    for(int i = 0; i < 4; i++){      
        if(guess[i] == code[i]){
            exact[i] = true;        //Set current exact to true
        }
        else {
            match = false;          //No exact match, set match to false to prompt for retry
        }
    }
    //Check for partial matches
    for (int i = 0; i < 4; i++) {
        if (!exact[i]) { // Only check if it's not an exact match
            for (int n = 0; (n < 4 && !found); n++) {
                // Check for partial matches only for unmatched indices
                if (guess[i] == code[n] && !exact[n]) {
                    partial[i] = true;  // Mark partial match
                    found = true;       // Stop checking after a match is found
                }
            }
        }
        found = false;
    }
    
    for(int i = 0; i < 4; i++){
        cout << "Exact i is: " << exact[i] << "Partial i is: " << partial[i];
    }
    cout << endl;
    //and output the results
    result(exact,partial);
    return match;           //Return if all 4 match
}

void result(bool exact[4], bool partial[4]){
    //let's get pretty
    string dot      = "\xE2\x97\x8F";   //Dot
    string red      = "\033[31m";       
    string green    = "\033[32m";
    string yellow   = "\033[33m";
    string black    = "\033[0m";     

    //Print the number of exact matches
    for(int i = 0; i < 4; i++){
        if(exact[i]){
            cout << green << dot << " ";
        }
    }
    //Print the number of partial matches 
    for(int i = 0; i < 4; i++){
        if(partial[i] && !exact[i]){    //Don't print partial if this element is exact
                cout << yellow << dot << " ";
        }
    }
    
    //Print the number of no match or partial
    for(int i = 0; i < 4; i++){
        if(!exact[i] && !partial[i]){
                cout << red << dot << " ";
        }
    }
    cout << black << endl;  //Reset console color to black for next round
}