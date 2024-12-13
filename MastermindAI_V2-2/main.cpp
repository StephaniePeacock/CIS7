/*
    Program template by Dr. Mark E. Lehr - May 11th, 2021
    AI by Stephanie Peacock December 11, 2024
    Mastermind AI v 2.2
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
    //   cout << guess << "\t" << rr+0 << "\t" << rw+0 << endl;
       guess=AI(rr,rw);
    }while(eval(code,guess,rr,rw));
    //Check evaluation
    cout<<nGuess<<" "<<code<<" "<<guess<<endl;
    guess=AI(rr,rw);
    cout<<code<<endl;
    
    //Exit the program
    return 0;
}

string AI(char rr, char rw) {
    // <editor-fold defaultstate="collapsed" desc="Print & Variables">

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
    static const int SIZE = 10000; // How many guesses to save
    static string aGuess[SIZE]; // Save the guesses
    static char grr[SIZE]; // Save right guess in right spot
    static char grw[SIZE]; // Save right guess in wrong spot
    static int guess = 0; // Increment the guess number
    static string digits = ""; // To store the identified digits
    static int found = 0; // How many placements found
    static string sGuess = "0000"; // Current guess string
    static int pos, indx = 0; // Digiti then indexes to test
    static char placed[4] = {'x', 'x', 'x', 'x'}; //to hold the placement
    static int total = 0; //so we know how many digits we've found but not placed
    int num = rr + rw - total;  //number of found digits to add
    // Store the results from the last guess
    grr[guess] = rr;
    grw[guess] = rw;

    // Test the helper function
    if (rr == 4) {      //We found all 4 placements
        for(int i = 0; i < 4; i++){
            sGuess[i] = placed[i];
        }
        print(aGuess, grr, grw,guess - 10 > 0 ? guess - 10 : 0, guess);
        return sGuess;  //We have our code, we can skip the rest of this
    }// </editor-fold>
    
    //Phase 1 - Identify the digits in the code
    if (digits.length() < 4) {
        //See if we need to add a digit to our guess
        if(num > 0){
            char digit = ((pos-1) +'0');
            for(int i = 0; i < num; i++){ //If they are, add the current digit to digits that # times
                digits += digit;            //Sdd the current digit                  
            }
            total += num;        //Increment total digits found - existing total
        }
        if(digits.length() < 4){
            //Prep the next guess
            for(int i = total - num; i < digits.length(); i++){
                sGuess[i] = digits[i];  //add the known digits to guess
            }
            //Add the next tester digit after the found digits
            for (int i = digits.length(); i < 4; i++){
                sGuess[i] = '0' + pos;
            }
            //Increment pos so we test the next 0-9 digit
            pos++; //Move to the next spot
        }
        //We have all 4 digits now - reset the counters
        else if(digits.length() == 4){ 
            pos = 0;    //Is now the position to test in sGuess
            indx = 0;   //Is the index to use from digits
        }
    }
    // Phase 2 - Determine the positions of each digit
    if (digits.length() == 4) {
        //See if we have a match for current
        if(rr == 1 && rw == 0){
            //Add the current digit to the placed location
            placed[pos-1] = digits[indx];
            found++;
            indx++;  //Move to next index of digits
            pos = 0;        //Restart our search at the beginning
        }
        //We found all 4 let's skip the rest
        if(found == 4){
            for(int i = 0; i < 4; i++){
                sGuess[i] = placed[i];
            }
            return sGuess;
        }
        //Skip the position if we found it already
        while(pos < 4 && placed[pos] != 'x'){ pos++; } 
        //All positions tested, move to next index
        if(pos == 4) { 
            pos = 0; indx++; 
        }   
        //Find an incorrect digit to use to test for placement
        char wrong = '0';
        while(digits.find(wrong) != -1){    //Found a wrong digit stop looking
            wrong++;                        //Look for the next possible wrong digit
        }
        //Now check each digit's position
        for(int i = 0; i < 4; i++){
        //    if(placed[i] != 'x') pos++;         //Skip this one, we have a match already
            if(i != pos) sGuess[i] = wrong;     //Intentionally wrong
            else sGuess[pos] = digits[indx];    //Check the digit for placement
        }
        if(pos < 4) pos++;                      //Move to the next position     
    }
    //Save the guess and increment
    aGuess[++guess] = sGuess;
    //Return the guess
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