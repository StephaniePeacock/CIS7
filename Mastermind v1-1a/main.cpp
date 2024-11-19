/* 
 * File:   main.cpp
 * Author: Stephanie Peacock
 * Created on: 11/17/2024 at 6:15PM
 * Purpose:  Mastermind Game v1.1a - brute force ai guess
 */

//System Libraries Here
#include <iostream>     //Input/Output Library
#include <ctime>        //Time Libary
#include <iomanip>      //Formatting Library
using namespace std;

//User Libraries
//Global Constants, no Global Variables are allowed
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...
//Function Prototypes
void humans();                                                  //Classic - user breaks code
void robots();                                                  //AI systematically tries to break a random code
char* generate(int, int, bool);                                 //Generate a random code
char* make(int, int, bool);                                     //Human makes a guess, or creates a code
bool check(const char*, const char*, int, bool,int[][5]);       //Check if the guess is correct - Calls Result
void result(bool[], bool[], int);                               //Print results for Human Codebreaker
void aiResult(const char*,const char*,bool[],bool[],int,int[][5]); //Print results for AI Codebreaker
void prompt(int&, int&, int&);                                  //Get the game settings for Human

//Execution Begins Here!
int main(int argc, char** argv) {
    //Set Random Number Seed Here
    srand(static_cast<unsigned int>(time(0)));
    //Variables
    char choice;        //What kind of game are we playing
    //Prompt for game settings
    do{
        cout << "Welcome to Mastermind. Who is the Code Breaker?\n"
                "1. A Human\n"
                "2. The Robots\n"
                "Anything else to exit.";
        cin >> choice;
        switch(choice){
            case '1' : 
                humans();
                break;
            case '2' : 
                robots();
                break;
            default  : break;
        }
    }while(choice == '1' || choice == '2' || choice == '3');
    
    //Exit
    return 0;
}

//Original 1.1 version - Human plays
void humans(){
    //Game settings variables
    int size;       //Determine size of code
    int choices;    //Determine digits in code
    int tries;      //Determine attempts remaining
    int total = 0;  //number of attempts made
    bool human = true;
    int totals[3][5];   //not used in humans - needed by check for ai
    
    //Prompt for game settings
    prompt(size,choices,tries);
    
    //Assign array variables
    char *code = nullptr;                       //Code we have to Guess
    char *guess = nullptr;                      //Our Guesses
    bool match = false;                         //So we can end the game
   
    code = generate(size, choices, human);      //Create a code
    
    while(tries > 0){             //Loop runs until we get all 4 or run out of tries
        if (guess != nullptr) {                 //only delete if it is allocated
            delete [] guess;
            guess = nullptr;                    //prevent double deletion
        }
        guess = make(size,choices,human);       //Make a Guess
        match = check(code,guess,size,human,totals);   //See if all 4 are correct
        tries--;                                //Remove an attempt
        total++;                                //Add to the total
        if(match) tries = 0;                    //We got it, end game
    }
    if(!match) {
        cout << "This code was unbroken. Better luck next time!" << endl;
    }
    else {
        cout << "You broke the code in " << total << " attempts. Good job, Mastermind!" << endl;
    }
    //cleanup
    delete []code;
    delete []guess;
}

//AI systematically tries to break a random code
void robots(){
    int size = 4,               //Size of the Arrays
        choices = 10;           //All 10 digits used
    char *code = nullptr;       //Code to break
    char *guess = nullptr;      //Guesses to be incremented - start all at '0'
    bool human = false;         //To print AI results
    bool match;                 //Holds check results - not needed in this version - to end game
    int totals[3][5] = {0};      //Hold the results - first row exact, second row partials, third row sums
    char who;                   //Who will make the code, user or rand()
    
    //Create the code
    cout << "\nCreate random code (y) or user generated code (n)?";
    do{
        cin >> who;
        if(who != 'y' && who != 'Y' && who != 'n' && who != 'N'){
            cout << "I asked a yes or no question...";
        }
    }while(who != 'y' && who != 'Y' && who != 'n' && who != 'N');
    if(who == 'y' || who == 'Y'){
        code = generate(size,choices,human); //randomly create a code
    } else {
        code = make(size,choices,human);     //human makes the code
    }
    //make sure guesses start at 0000
    guess = new char[size];
    for(int i = 0; i < size; i++){
        guess[i] = '0';
    }
    //Brute force guess list
    cout << "\n\nCode\tGuess\t#Exact\t#Partial   Sum\n";
    do{
        //check the guess & output results
        match = check(code,guess,size,human,totals);
        //increment the guess
        for (int i = size - 1; i >= 0; i--) {   //reverse! so we start at the last element and work our way up
            if (guess[i] < '9') {
                guess[i]++;
                i = -1;         //break the loop on increment
            } else {
                guess[i] = '0'; //carry over to the next digit
            }
        }
        if (guess[0] == '9' && guess[1] == '9' && guess[2] == '9' && guess[3] == '9') {
            match = check(code,guess,size,human,totals); //one last time for the final round!
        }
    }while(!(guess[0] == '9' && guess[1] == '9' && guess[2] == '9' && guess[3] == '9'));
    
    //output totals
    cout << "\n\nTotals     4       3       2       1       0"
            "\n-----------------------------------------------" << endl;
    for(int i = 0; i < 3; i++){
        i == 0 ? cout << "Exact  :   " :   //first row
        i == 1 ? cout << "Partial:   " :   //second row
                 cout << "Sum    :   ";    //third row
        for(int j = 0; j < 5; j++){
            cout << left << setw(8) << setfill(' ') << totals[i][j];
        }
        cout << endl;
    }
    cout << endl << endl;
    
    delete []guess;
    delete []code;
}                         

//Create a random code
char* generate(int size, int choices, bool human){
    char* code = new char[size];
    for(int i = 0; i < size; i++){
        code[i] = '0' + rand()%choices;     //   cout << code[i] << " ";     //for testing
    }                                       //   cout << endl;               //for testing
    return code;
}

//Make a guess
char* make(int size, int choices, bool human) {
    char* guess = new char[size];
    string input;               
    bool invalid = true;    
    
    human ?  cout << "Enter a guess: " : cout << "Enter a code: ";
    
    while(invalid){                 //Run till we get exactly 4 digits
        getline(cin,input);
        if(input.length() == size){ //Correct length
            invalid = false;        //Good so far 
            for(int i = 0; i < size; i++){
                if(input[i] >= '0' && input[i] <= ((choices - 1) + '0')){  //Current char is a number & applicable
                    guess[i] = input[i];                    //Set current guess to number
                }
                else {
                    human ? cout << "Guess must contain only digits, and be within the range of choices." << endl : 
                            cout << "Code must contain only digits." << endl;
                    invalid = true;     //Not a digit - Reenter Guess
                    i = size;           //end the loop not digit
                }
            }
        }
        else {                      //Not the correct length
            human ? cout << "Guess must be " << size << " digits long." << endl :
                    cout << "Code must be " << size << " digits long." << endl;
            invalid = true;         //Reenter Guess
        }
    }
    return guess;
}

//Check if the guess matches the code
bool check(const char* code, const char* guess, int size, bool human, int totals[3][5]) {
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
    if(human){
        result(exact, partial, size);   //player results
    } else {
        aiResult(code, guess,exact, partial, size, totals); //ai results
    }
    
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

//Print results for AI incremented guesses
void aiResult(const char* code, const char* guess, bool exact[], bool partial[], int size, int totals[3][5]){
    int e = 0,      //Number of exact matches  
        p = 0,      //Number of partial matches
        sum = 0;    //Sum of partial & exact
    
    //Get the number of exact matches
    for(int i = 0; i < size; i++){
        if(exact[i]){
            e++;
        }
    }
    //plug number of exact into totals array
    e == 4 ? totals[0][0]++ :   //add 1 to all 4 exact
    e == 3 ? totals[0][1]++ :   //add 1 to 3 exact
    e == 2 ? totals[0][2]++ :   //add 1 to 2 exact
    e == 1 ? totals[0][3]++ :   //add 1 to 1 exact
             totals[0][4]++;    //add 1 to 0 exact
    //Print the number of partial matches 
    for(int i = 0; i < size; i++){
        if(partial[i] && !exact[i]){    //Don't count partial if this element is exact
                p++;
        }
    }
    //plug number of partial into totals array
    p == 4 ? totals[1][0]++ :   //add 1 to all 4 partial
    p == 3 ? totals[1][1]++ :   //add 1 to 3 partial
    p == 2 ? totals[1][2]++ :   //add 1 to 2 partial
    p == 1 ? totals[1][3]++ :   //add 1 to 1 partial
             totals[1][4]++;    //add 1 to 0 partial
    //Get the sum of exact and partial
    sum = e+p;
    //plug sum into totals array
    sum == 4 ? totals[2][0]++ :   //add 1 to all 4 sum
    sum == 3 ? totals[2][1]++ :   //add 1 to 3 sum
    sum == 2 ? totals[2][2]++ :   //add 1 to 2 sum
    sum == 1 ? totals[2][3]++ :   //add 1 to 1 sum
               totals[2][4]++;    //add 1 to 0 sum
    //Print all the things
    for(int i = 0; i < size; i++){
        cout << code[i];
    }
    cout <<  "\t";
    for(int i = 0; i < size; i++){
        cout << guess[i];
    }
    cout << "       " << e << "       " << p << "       " << sum << endl;
}      

//Get the game settings - size of code, valid digits, and number of attempts
void prompt(int& size, int& choices, int& tries){
    cout << "Welcome to Mastermind. Can you break the code?" << endl
         << "Enter the length of the code you want to break: ";
    
    do{
        cin >> size;
        if(size < 1) cout << "Codes must be at least 1 digit long.";
    }while(size < 1);
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
    do{
        cin >> tries;
        if(tries < 1) cout << "I'm not a mind reader, you must make at least 1 try.";
    }while(tries < 1);
    cin.ignore();
    cout << endl;
}        