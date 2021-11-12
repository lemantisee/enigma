#include "Enigma.h"

#include <iostream>
#include <fstream>

#include "Component.h"
#include "Rotor.h"
#include "alphabet.h"
#include "errors.h"

Enigma::Enigma(int argc, char** argv){
  vector<int> plugboard_contacts;
  checkPlugboardConfig(argv[1],plugboard_contacts);
  vector<int> reflector_contacts;
  checkReflectorConfig(argv[2], reflector_contacts);
  vector<vector<int>> rotor_contacts_array;

  vector<int> rotor_contacts;
  for(int i = 3; i < argc-1; i++){
    checkRotorConfig(argv[i], rotor_contacts);
    rotor_contacts_array.push_back(rotor_contacts);
    rotor_contacts.clear();
  }
  // 3 argc means no rotor is provided
  if(argc == 3){
    mRotorsNumber = 0;
  }
  else{
    mRotorsNumber = argc-4;
  }
  checkRotorPositionConfig(argv[argc-1]);

  // All the checking is done at this point, so instantiate each component

  mPlugboard = new Component(argv[1]);
  mReflecor = new Component(argv[2]);
  for(int i = 0; i < mRotorsNumber; i++){
    Rotor rotor(argv[3+i], mRotorPositions[i]);
    mRotors.push_back(rotor);
  }
}

Enigma::~Enigma(){
    delete mPlugboard;
    delete mReflecor;
}

bool Enigma::isPlugboardInputValid(const char* path, fstream& in_stream, \
  int& index_num){
  in_stream >> ws;
  int end_of_file = in_stream.peek();
  if(end_of_file == EOF){
    return false;
  }
  in_stream >> index_num;
  if(in_stream.fail()){
    cerr << "Non-numeric character in plugboard file " << path << endl;
    in_stream.close();
    throw(NON_NUMERIC_CHARACTER);
  }
  if(!isNumberRangeCorrect(index_num)){
    cerr << "The file " << path << \
    " contains a number that is not between 0 and 25" << endl;
    in_stream.close();
    throw(INVALID_INDEX);
  }
  return true;
}

void Enigma::checkPlugboardConfig(const char* path, vector<int>& contacts){
  int even_index_num, odd_index_num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }

  while(!in_stream.eof()){
    if(!isPlugboardInputValid(path, in_stream, even_index_num)){
      break;
    }
    if(!isPlugboardInputValid(path, in_stream, odd_index_num)){
      cerr << "Incorrect number of parameters in plugboard file " \
      << path << endl;
      in_stream.close();
      throw(INCORRECT_NUMBER_OF_PLUGBOARD_PARAMETERS);
    }
    contacts.push_back(even_index_num);
    if(checkAppearedBefore(contacts, even_index_num, counter) != -1){
      in_stream.close();
      throw(IMPOSSIBLE_PLUGBOARD_CONFIGURATION);
    }
    counter++;

    contacts.push_back(odd_index_num);
    if(checkAppearedBefore(contacts, odd_index_num, counter) != -1){
      in_stream.close();
      throw(IMPOSSIBLE_PLUGBOARD_CONFIGURATION);
    }
    counter++;
  }
  in_stream.close();
}

void Enigma::checkReflectorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }

  while(!in_stream.eof()){
    in_stream >> ws;
    int end_of_file = in_stream.peek();
    if(end_of_file == EOF){
      break;
    }
    in_stream >> num;
    if(in_stream.fail()){
      cerr << "Non-numeric character in reflector file " << path << endl;
      in_stream.close();
      throw(NON_NUMERIC_CHARACTER);
    }
    if(!isNumberRangeCorrect(num)){
      cerr << "The file " << path << \
      " contains a number that is not between 0 and 25" << endl;
      in_stream.close();
      throw(INVALID_INDEX);
    }
    contacts.push_back(num);
    if(counter < ALPHABET_LENGTH && \
      checkAppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
      throw(INVALID_REFLECTOR_MAPPING);
    }
    counter++;

  }
  in_stream.close();

  if(counter%2!=0){
      cerr << "Incorrect (odd) number of parameters in reflector file " \
      << path << endl;
      throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
  }
  if(counter != ALPHABET_LENGTH){
    cerr << "Insufficient number of mappings in reflector file: " \
    << path << endl;
    throw(INCORRECT_NUMBER_OF_REFLECTOR_PARAMETERS);
  }
}

void Enigma::checkRotorConfig(const char* path, vector<int>& contacts){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }

  while(!in_stream.eof()){
    in_stream >> ws;
    int end_of_file = in_stream.peek();
    if(end_of_file == EOF){
      break;
    }
    in_stream >> num;

    if(in_stream.fail()){
      cerr << "Non-numeric character for mapping in rotor file " \
      << path << endl;
      in_stream.close();
      throw(NON_NUMERIC_CHARACTER);
    }
    if(!isNumberRangeCorrect(num)){
      cerr << "The file " << path \
      << " contains a number that is not between 0 and 25" << endl;
      in_stream.close();
      throw(INVALID_INDEX);
    }
    contacts.push_back(num);

    if(counter < ALPHABET_LENGTH-1 && \
      checkAppearedBefore(contacts, num, counter) != -1){
      in_stream.close();
      throw(INVALID_ROTOR_MAPPING);
    }
    counter++;
  }
  in_stream.close();

  if(counter < ALPHABET_LENGTH){
    cerr << "Not all inputs mapped in rotor file: " << path << endl;
    throw(INVALID_ROTOR_MAPPING);
  }
}

void Enigma::checkRotorPositionConfig(const char* path){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail()){
    cerr << "Error opening or reading the configulation file " << path << endl;
    in_stream.close();
    throw(ERROR_OPENING_CONFIGURATION_FILE);
  }
  while(!in_stream.eof()){
    in_stream >> ws;
    int eof = in_stream.peek();
    if(eof == EOF){
      break;
    }
    in_stream >> num;
    if(in_stream.fail()){
      cerr << "Non-numeric character in rotor positions file " << path  << endl;
      in_stream.close();
      throw(NON_NUMERIC_CHARACTER);
    }

    if(!isNumberRangeCorrect(num)){
      cerr << "The file " << path \
      << " contains a number that is not between 0 and 25" << endl;
      in_stream.close();
      throw(INVALID_INDEX);
    }
    counter++;
    mRotorPositions.push_back(num);
  }

  int diff = counter - mRotorsNumber;
  if(diff < 0){
    cerr << "No starting position for rotor " << mRotorsNumber + diff \
    << " in rotor position file: " << path << endl;
    in_stream.close();
    throw(NO_ROTOR_STARTING_POSITION);
  }
  in_stream.close();
}

bool Enigma::isNumberRangeCorrect(int num){
  return (num < ALPHABET_LENGTH && num >= 0);
}

int Enigma::checkAppearedBefore(vector<int> contacts, int num, int position){
  for(int i = 0; i < position; i++ ){
    if(contacts[i] == num){
      cerr << "Invalid mapping of input " << position << " to output " << num \
      << " (output " << num << " is already mapped to from input " \
      << i << ")" << endl;
      return i;
    }
  }
  return -1;
}

void Enigma::encryptMessage(char& letter){
  int current_index = letter - 'A';
  current_index = mPlugboard->map(current_index);

  if(mRotorsNumber > 0){
    mRotors[mRotorsNumber-1].rotate();
  }

  if(mRotorsNumber > 0){
    for(int i = mRotorsNumber ; i > 0; i--){
      // TODO Needs explanation here
      current_index = mRotors[i-1].shiftDown(current_index);
      current_index = mRotors[i-1].mapForward(current_index);
      current_index = mRotors[i-1].shiftUp(current_index);
      if(mRotors[i-1].isCurrentPositionInNotch() && \
         mRotors[i-1].getPreviousPosition() != \
         mRotors[i-1].getCurrentPosition()){
        if(i-1 > 0){
          mRotors[i-2].rotate();
        }
      }
    }
  }
  current_index = mReflecor->map(current_index);
  if(mRotorsNumber > 0){
    for(int i = 0; i < mRotorsNumber; i++){
      current_index = mRotors[i].shiftDown(current_index);
      current_index = mRotors[i].mapBackward(current_index);
      current_index = mRotors[i].shiftUp(current_index);
    }
  }
  current_index = mPlugboard->map(current_index);
  letter = current_index + 'A';
}
