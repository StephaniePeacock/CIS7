/* 
 * File:   main.cpp
 * Author: Stephanie Peacock
 * Created on: 10/20/2024 at 12:15PM
 * Purpose:  Mastermind Game v1.1
 */

//System Libraries Here
#include <iostream>     //Input/Output Library
#include <ctime>        //Time Libary
using namespace std;

//User Libraries
//Global Constants, no Global Variables are allowed
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...
//Function Prototypes
void generate(int [], int, int);                //Generate a code
void make(int [], int, int);                    //Make a guess
bool check(const int [], const int[], int);     //Check if the guess is correct
void result(bool[], bool[], int);               //Print results
void prompt(int&, int&, int&);                  //Get the game settings

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set Random Number Seed Here
    srand(static_cast<unsigned int>(time(0)));
    //Game settings variables
    int size;       //Determine size of code
    int choices;    //Determine digits in code
    int tries;      //Determine attempts remaining
    int total = 0;  //number of attempts made
    
    //Prompt for game settings
    prompt(size,choices,tries);
    
    //Assign array variables
    int code[size];                         //Code we have to Guess
    int guess[size];                        //Our Guesses
    bool match = false;                     //So we can end the game
   
    generate(code, size, choices);          //Create a code
    
    while(tries > 0){             //Loop runs until we get all 4 or run out of tries
        make(guess,size,choices);           //Make a Guess
        match = check(code,guess,size);     //See if all 4 are correct
        tries--;                            //Remove an attempt
        total++;                            //Add to the total
        if(match) tries = 0;                //We got it, end game
    }
    if(!match) {
        cout << "This code was unbroken. Better luck next time!" << endl;
    }
    else {
        cout << "You broke the code in " << total << " attempts. Good job, Mastermind!" << endl;
    }
    
    //Exit
    return 0;
}

//Get the game settings - size of code, valid digits, and number of attempts
void prompt(int& size, int& choices, int& tries){
    cout << "Welcome to Mastermind. Can you break the code?" << endl
         << "Enter the length of the code you want to break: ";
    cin >> size;
    cout << "The count begins at 0. If you choose 5, then the possible options are 0 1 2 3 4." << endl
         << "Enter the number of choices: ";
    while(choices < 1 || choices > 10){
        cin >> choices;
        if(choices < 1 || choices > 10){
            cout << "Codes must have between 1 and 10 digits as choices." << endl
                 << "Enter the number of choices: ";
        }
    }
    cout << "How many attempts would you like to break the code: ";
    cin >> tries;
    cin.ignore();
    cout << endl;
}

//Create a random code
void generate(int code[], int size, int choices){
    for(int i = 0; i < size; i++){
        code[i] = rand()%choices;       //   cout << code[i] << " ";     //for testing
    }                                   //   cout << endl;               //for testing
}

//Make a guess
void make(int guess[], int size, int choices) {
    string input;               
    bool invalid = true;    
    
    cout << "Enter a guess: ";
    
    while(invalid){                 //Run till we get exactly 4 digits
        getline(cin,input);
        if(input.length() == size){    //Correct length
            invalid = false;        //Good so far 
            for(int i = 0; i < size; i++){
                if(input[i] >= '0' && input[i] <= (choices + '0')){  //Current char is a number & applicable
                    guess[i] = (input[i] - '0');                    //Set current guess to number
                }
                else {
                    cout << "Guess must contain only digits, and be within the range of choices." << endl;
                    invalid = true;     //Not a digit - Reenter Guess
                    i = size;           //end the loop not digit
                }
            }
        }
        else {                      //Not the correct length
            cout << "Guess must be " << size << " digits long." << endl;
            invalid = true;         //Reenter Guess
            
        }
    }
}

//Check if the guess matches the code
bool check(const int code[], const int guess[], int size) {
    bool exact[size] = {false};         // Exact matches - default is false
    bool partial[size] = {false};       // Partial matches - default is false
    int count[size] = {0};              // To track how many times a number is present in the code
    bool match = true;                  // All 4 match - default true, change if not
    
    // Check if we have any exact matches
    for (int i = 0; i < size; i++) {
        if (guess[i] == code[i]) {
            exact[i] = true;        // Set current exact to true
        } else {
            match = false;          // No exact match, set match to false to prompt for retry
        }
    }

    // Count occurrences of each number in the code
    for (int i = 0; i < size; i++) {
        for (int n = 0; n < size; n++) {
            if (code[i] == code[n]) {
                count[i]++;         // Count the number of occurrences of each digit
            }
        }
    }

    // Check for partial matches
    for (int i = 0; i < size; i++) {
        if (!exact[i]) {                    // Only check if it's not an exact match
            for (int n = 0; n < size; n++) {
                if (count[n] < 1) continue; // Skip if no more duplicates available
                if (guess[i] == code[n] && !exact[n]) {  // Current guess isn't an exact match
                    partial[i] = true;      // Mark partial match
                    count[n]--;             // Reduce count to avoid double-counting
                    n = size;               // Stop checking after a partial is found
                }
            }
        }
    }

    // Output the results
    result(exact, partial, size);
    return match;  // Return if all 4 match
}

//Print out the result
void result(bool exact[], bool partial[], int size){
    //let's get pretty
    string dot      = "\xE2\x97\x8F";   //Dot
    string red      = "\033[31m";       
    string green    = "\033[32m";
    string yellow   = "\033[33m";
    string black    = "\033[0m";     

    //Print the number of exact matches
    for(int i = 0; i < size; i++){
        if(exact[i]){
            cout << green << dot << " ";
        }
    }
    //Print the number of partial matches 
    for(int i = 0; i < size; i++){
        if(partial[i] && !exact[i]){    //Don't print partial if this element is exact
                cout << yellow << dot << " ";
        }
    }
    
    //Print the number of no match or partial
    for(int i = 0; i < size; i++){
        if(!exact[i] && !partial[i]){
                cout << red << dot << " ";
        }
    }
    cout << black << endl;  //Reset console color to black for next round
}