/*
    Program template by Dr. Mark E. Lehr - May 11th, 2021
    AI by Stephanie Peacock December 12, 2024
    Mastermind AI v 2.3
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
//    code = "7852";
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
    
    //Exit the program
    return 0;
}

string AI(char rr, char rw) {
    // <editor-fold defaultstate="collapsed" desc="Print & Variables">
    // Define helper functions here
     void (*print)(string [], char [], char [], int, int) = [] (string g[], char r[], char w[], int nb, int ne) { 
             for (int i = nb; i <= ne; i++) {
                 cout << g[i] << " " << static_cast<int>(r[i]) << " " << static_cast<int>(w[i]) << endl;
             }
     };
    // Save the historical values of guesses and results
    static const int SIZE = 10000;      //How many guesses to save
    static string aGuess[SIZE];         //Save the guesses
    static char grr[SIZE];              //Save right guess in right spot
    static char grw[SIZE];              //Save right guess in wrong spot
    static int guess = 0;               //Increment the guess number
    static string digits = "";          //To store the identified digits
    static int found = 0;               //How many placements found
    static string sGuess = "0000";      //Current guess string
    static int pos, indx = 0;           //Digits then indexes to test
    static char placed[4] = {'x', 'x', 'x', 'x'}; //To hold the placement
    int num = rr + rw - found;          //Number of found digits to add
    // Store the results from the last guess
    grr[guess] = rr;
    grw[guess] = rw;
    
    // Test the helper function
    if (rr == 4) {      //We found all 4 placements
        //Print all the guesses, not just 10
        print(aGuess, grr, grw, 1, guess);
        return sGuess;  //We have our code, we can skip the rest of this
    }// </editor-fold>
    
    //Phase 1 - Identify the digits in the code
    if (digits.length() < 4) {
        //See if we need to add a found digit to our guess
        if(num > 0){
            char digit = ((pos-1) +'0');    //Assign the found digit
            for(int i = 0; i < num; i++){   //Add num times
                digits += digit;            //Add the found digit to digits sting                  
            }
            found += num;                   //Add new found to existing found
        }
        //Skip looking for 9, just fill the empty spots in digits with 9
        if(pos == 9 && digits.length() < 4){
            for (int i = digits.length(); i < 4; i++){
                digits += '9';              //Add a 9 to digits till it's 4 chars long
            }
        }
        //If we didn't hit 9 fill the rest
        if(digits.length() < 4){
            //Prep the next guess
            for(int i = found - num; i < digits.length(); i++){
                sGuess[i] = digits[i];  //Add the known digits to sGuess
            }
            //Add the next tester digit after the found digits 
            for (int i = digits.length(); i < 4; i++){
                sGuess[i] = '0' + pos;
            }
            //Increment pos so we test the next 0-9 digit
            pos++;
        }
        else {          //We have all 4 digits now - reset the counters
            pos = 0;    //Is now the position to test in sGuess
            indx = 0;   //Is the index to use from digits
            found = 0;  //Reset so we can use to find positions
        }
    }
    // Phase 2 - Determine the positions of each digit
    if (digits.length() == 4) {        
        //See if we have a match for current
        if(rr == 1 && rw == 0 && pos!= 0){
            //Add the current digit to the placed location
            placed[pos-1] = digits[indx];
            found++;        //Increment found
            //If no repeat reset pos otherwise we'll keep checking for next occurrence
            if(digits[indx] != digits[indx+1])pos = 0;
            indx++;         //Move to next index to find
        }
        //We've found 2 digits - put the last 2 in and check
        if(found == 2){
            static bool swap = false;
            for(int i = 0; i < 4; i++){
                if(placed[i] == 'x'){   //We found an empty place
                    sGuess[i] = swap ? digits[2] : digits[3];
                    swap = !swap;       //Fill the other index next
                }
                else {                  //Place is full copy it over
                    sGuess[i] = placed[i];
                }
            }
            // Swap digits[2] and digits[3] in case we need to try again
            char temp = digits[2];
            digits[2] = digits[3];
            digits[3] = temp;
        }
        //Still looking for first 2 placements
        else{
            if(pos == 4) {                      //All positions tested
                pos = 0;                        //Reset pos
                indx++;                         //Move to next digits index
            }   
            //Find an incorrect digit to use to test for placement
            char wrong = '0';                   //Start with 0
            //Look for the next possible wrong digit stop when found
            while(digits.find(wrong) != -1) wrong++;                        
            //Advance position if the current pos placement has been found
            while(pos < 4 && placed[pos] != 'x')pos++;
            //Fill the guess
            for(int i = 0; i < 4; i++){ 
                if(i != pos) sGuess[i] = wrong;     //Intentionally wrong
                else sGuess[pos] = digits[indx];    //Digit we're checking for placement
            }
            if(pos < 4) pos++;                      //Move to the next position 
            }
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
    //cout << guess << "\t" << rr+0 << "\t" << rw+0 << " code: " << code << endl;
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