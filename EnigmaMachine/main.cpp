#include <iostream>
#include <cstring>
#include <string>

#include "Enigma.h"
#include "alphabet.h"
#include "errors.h"

using namespace std;

int main(int argc, char** argv)
{
    cout << "Hello World!" << endl;

    Enigma enigma(argc, argv);


    // You need to provide at least 3 command line arguments
    // (plugboard and reflector) or more than 5 if you include rotors
    // (and need a rotor position file)
    // Note that the value of argc is more than the actual number of command line
    // parameters given
    if(argc < 3 || argc == 4){
        // This cerr line exceeds 80 characters, but if I break it into two lines,
        // I cannnot pass LabTS auto-tests
        cerr << "usage: enigma plugboard-file reflector-file (<rotor-file>* rotor-positions)?" << endl;
        return INSUFFICIENT_NUMBER_OF_PARAMETERS;
    }

    char letter;
    // Within this while loop,
    // 1. it extract input letter by letter,
    // 2. check if it is within uppercase alphabet in ASCII table character code
    // 3. Encrypt a letter and overwrite it.
    while(!cin.eof()){
        cin >> letter;
        if(cin.fail()){
            break;
        }

        if(letter - 'A' < 0 || ALPHABET_LENGTH -1 < letter - 'A'){
            // This line exceeds 80 characters, but if I break it into two lines,
            // I cannnot pass LabTS auto-tests
            cerr << letter << " is not a valid input character (input characters must be upper case letters A-Z)!" << endl;
            return INVALID_INPUT_CHARACTER;
        }
        enigma.encryptMessage(letter);
        cout << letter;
    }
    return NO_ERROR;


    return 0;
}
