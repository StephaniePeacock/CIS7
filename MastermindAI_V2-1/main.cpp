/*
    Program template by Dr. Mark E. Lehr - May 11th, 2021
    AI by Stephanie Peacock December 8, 2024
    Mastermind AI v 2.1
 */

//System Libraries
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
using namespace std;

//Function Prototypes
string AI(char,char);
bool eval(string,string,char &,char &);
string set();

int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));
    
    //Declare variables
    string code,guess;  //code to break, and current guess
    char rr,rw;         //right digit in right place vs. wrong place
    int nGuess;         //number of guesses
    
    //Initialize Values
    nGuess=0;
    code=set();
    rr=rw=0;
    
    //Loop until solved and count to find solution
    do{
       nGuess++;
       guess=AI(rr,rw);
    }while(eval(code,guess,rr,rw));
    //Check evaluation
    cout<<nGuess<<" "<<code<<" "<<guess<<endl;
    guess=AI(rr,rw);
    cout<<code<<endl;
    /*for(int i=0;i<10000;i++){
        guess=AI(0,0);
        eval(code,guess,rr,rw);
        cout<<setw(5)<<code<<setw(5)<<guess
                <<setw(2)<<static_cast<int>(rr)
                <<setw(2)<<static_cast<int>(rw)<<endl;
    }*/
    
    //Exit the program
    return 0;
}

string AI(char rr, char rw) {
    // Define helper functions here
    void (*print)(string [], char [], char [], int, int) =
        [] (string g[], char r[], char w[], int nb, int ne) { 
            for (int i = nb; i <= ne; i++) {
                cout << g[i] << " "
                     << static_cast<int>(r[i]) << " "
                     << static_cast<int>(w[i]) << endl;
            }
    };

    // Save the historical values of guesses and results
    static const int SIZE = 10000;  // How many guesses to save
    static string aGuess[SIZE];     // Save the guesses
    static char grr[SIZE];          // Save right guess in right spot
    static char grw[SIZE];          // Save right guess in wrong spot
    static int guess = 0;           // Increment the guess number
    static string digits = "";      // To store the identified digits
    static int found = 0;           // How many placements found
    static string sGuess = "0000";  // Current guess string
    static int pos,indx = 0;        // Digiti then indexes to test
    static char placed[4] = {'x','x','x','x'};  //to hold the placement
    int total = (rr + 0)+(rw + 0);  //so we know how many copies of a digit
    // Store the results from the last guess
    grr[guess] = rr;
    grw[guess] = rw;

    // Test the helper function
    if (rr == 4) print(aGuess, grr, grw,
            guess - 10 > 0 ? guess - 10 : 0, guess);

    // Phase 0: Identify the digits in the secret code
    if (digits.length() < 4) {
        //see if we need to add a digit
        if(total > 0){
            char digit = ((pos-1) +'0');
            for(int i = 0; i < total; i++){ //if they are, add the current digit to digits that # times
                digits += digit;  // add the current digit
            }
        }
        //we have all 4 digits now - reset the counters & return the digits
        if(digits.length() == 4){ 
            pos = 0; 
            indx = 0; 
            aGuess[++guess] = digits;
            return digits;}
        // Still missing digits, test digits 0 to 9, check if they are in the code
        sGuess = string(4, '0' + pos++);  // Try each digit in all positions
        
    }
    // Phase 1: Determine the positions of each digit
    if (digits.length() == 4) {
        
        //see if we have a match for current
        if(rr == 1 && rw == 0){
            //add the current digit to the placed location
            placed[pos-1] = digits[indx];
            found++;
            //pos = 0; indx++;      //move to the next position to find
        }
        if(pos == 4){ pos = 0; indx++; } //reset pos to check, increment digits index
        // Find an incorrect digit to use to test for placement
        char wrong = '0';
        while(wrong <= '9'){
            if(digits.find(wrong) == -1) break; // Found a wrong digit stop looking
            else wrong++;
        }
        //Now check each digit's position
        for(int i = 0; i < 4; i++){
            if(i != pos) sGuess[i] = wrong; // Not testing this index
            else sGuess[pos] = digits[indx];
        }
        if(pos < 4) pos++;
        

        // We found all 4 placements
        if(found == 4){
            for(int i = 0; i < 4; i++){
                sGuess[i] = placed[i];
            }
        }
            
    }

    // Save the guess and increment
    aGuess[++guess] = sGuess;
    
    // Return the guess
    return sGuess;
}



bool eval(string code,string guess,char &rr,char &rw){
    string check="    ";
    rr=0,rw=0;
    //Check how many are right place
    for(int i=0;i<code.length();i++){
        if(code[i]==guess[i]){
            rr++;
            check[i]='x';
            guess[i]='x';
        }
    }
    //Check how many are wrong place
    for(int j=0;j<code.length();j++){
        for(int i=0;i<code.length();i++){
            if((i!=j)&&(code[i]==guess[j])&&(check[i]==' ')){
                rw++;
                check[i]='x';
                break;
            }
        }
    }
    //cout << guess << "\t" << rr+0 << "\t" << rw+0 << endl;
    //Found or not
    if(rr==4)return false;
    return true;
}

string set(){
    string code="0000";
    for(int i=0;i<code.length();i++){
        code[i]=rand()%10+'0';
    }
    return code;
}