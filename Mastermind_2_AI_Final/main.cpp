#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>

using namespace std;

//Function Prototypes
string AI(char, char);

bool eval(string, string, char&, char&);

string set();

int main(int argc, char** argv) {
    //Set the random number seed
    srand(static_cast<unsigned int>(time(0)));

    //Declare variables
    string code, guess;  //code to break, and current guess
    char rr, rw;         //right digit in right place vs. wrong place
    int nGuess;         //number of guesses

    //Initialize Values
    nGuess = 0;
    code = set();
    rr = rw = 0;

    //Loop until solved and count to find solution
    do {
        nGuess++;
        guess = AI(rr, rw);
    } while (eval(code, guess, rr, rw));
    //Check evaluation
    //cout << "number of guesses: " << nGuess << endl;
    cout << "Number of guesses: " << nGuess << " Code: " << code << " AI Guess: " << guess << endl << endl;
    guess = AI(rr, rw);
    /*cout<<code<<endl;
    for(int i=0;i<10000;i++){
        guess=AI(0,0);
        eval(code,guess,rr,rw);
        cout<<setw(5)<<code<<setw(5)<<guess
                <<setw(2)<<static_cast<int>(rr)
                <<setw(2)<<static_cast<int>(rw)<<endl;
    }*/

    //Exit the program
    return 0;
}

bool eval(string code, string guess, char& rr, char& rw) {
    string check = "    ";
    rr = 0, rw = 0;
    //Check how many are right place
    for (int i = 0; i < code.length(); i++) {
        if (code[i] == guess[i]) {
            rr++;
            check[i] = 'x';
            guess[i] = 'x';
        }
    }
    //Check how many are wrong place
    for (int j = 0; j < code.length(); j++) {
        for (int i = 0; i < code.length(); i++) {
            if ((i != j) && (code[i] == guess[j]) && (check[i] == ' ')) {
                rw++;
                check[i] = 'x';
                break;
            }
        }
    }

    //Found or not
    if (rr == 4)return false;
    return true;
}

string set() {
    string code = "0000";
    for (int i = 0; i < code.length(); i++) {
        code[i] = rand() % 10 + '0';
    }
    return code;
}

string AI(char rr, char rw) {

    void (*print)(string[], char[], char[], int) =
        [](string g[], char r[], char w[], int sz) {

        for (int i = 1; i < sz; i++) {
            cout << "AI Guess #" << i << ": " << g[i] << " Right spot : " << to_string(r[i]) << "  Wrong spot : " << to_string(w[i]) << endl;

        }

    };

    // ********** Data ********** //

    // Static
    static int digitFrequencies[10] = { 0 };
    static int digitCounter = -1;
    static int digitIndexToTest = 0;
    static int digitIndexTested = 0;
    static int codeIndex = 0;
    static string code = "";

    // Const
    const int codeLength = 4;

    // For history
    static int hist_counter = 0;
    static string hist_code[32];
    static char hist_r[32];
    static char hist_w[32];

    hist_code[hist_counter] = code;
    hist_r[hist_counter] = rr;
    hist_w[hist_counter++] = rw;

    // ********** Base Case ********** //

    if (rr == codeLength) {
        print(hist_code, hist_r, hist_w, hist_counter);
        return code;
    }

    // ********** Finding Digits ********** //

    if (digitCounter < 8) {
        code = "";
        digitFrequencies[digitCounter] = rr + rw;

        digitCounter++;

        for (int i = 0; i < codeLength; i++)
            code += ('0' + digitCounter);


        return code;
    }
    else if (digitCounter == 8) {
        digitFrequencies[digitCounter] = rr + rw;
        int sumOfFoundDigitFreq = 0;
        for (int i = 0; i < 9; i++)
            sumOfFoundDigitFreq += digitFrequencies[i];
        digitFrequencies[9] = codeLength - sumOfFoundDigitFreq;
        code = "    ";
        digitCounter++;
        rr = 0;
    }

    // ********** Finding Places ********** //

    if (digitIndexTested == codeLength - 1 - codeIndex && rr <= codeIndex) {
        for (int i = digitIndexToTest; i < 10; i++) {
            if (digitFrequencies[i] > 0) {
                digitIndexToTest = i;
                code[codeIndex] = '0' + digitIndexToTest;
                digitFrequencies[i]--;
                digitIndexTested++;
                digitIndexToTest = 0;
                codeIndex++;
                break;
            }
        }
    }

    if (rr > codeIndex) {
        digitFrequencies[digitIndexToTest - 1]--;
        digitIndexToTest = 0;
        digitIndexTested = 0;
        codeIndex++;
    }

    for (int i = digitIndexToTest; i < 10; i++) {
        if (digitFrequencies[i] > 0) {
            digitIndexToTest = i;
            code[codeIndex] = '0' + digitIndexToTest;
            digitIndexToTest++;
            digitIndexTested++;
            return code;
        }
    }

    return code;
}