#include "Rotor.h"
#include "errors.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Rotor::Rotor(const char* path, int start_position){
  checkInput(path);
  mapInput(path);
  current_position_ = start_position;
}

void Rotor::mapInput(const char* path){
  int array_length = getArrayLength(path);
  int num_array[array_length];
  mapInputToArray(path, num_array);

  if(array_length < ALPHABET_LENGTH_){
    cerr << "Not all inputs mapped in rotor file: rotor.rot" << endl;
    throw(INVALID_ROTOR_MAPPING);
  }
  // TODO Double check if this checking is enough
  if(isDuplicateInt(num_array, ALPHABET_LENGTH_)){
    cerr << "invalid rotor mapping" << endl;
    throw(INVALID_ROTOR_MAPPING);
  }
  // If not error, them map them to each attributes
  num_of_notches_ = array_length - ALPHABET_LENGTH_;
  // Why do I have to call mapInputToArray twice here?
  mapInputToArray(path, contacts_);

  for(int i = 0; i< num_of_notches_; i++){
    notches_.push_back(num_array[i+ALPHABET_LENGTH_]);
  }
}

void Rotor::rotateDown(){
  previous_position_ = current_position_;
  current_position_ = (current_position_ + 1) % ALPHABET_LENGTH_;
}

int Rotor::shuffleUp(int input_index){
  return (input_index - getCurrentPosition() + ALPHABET_LENGTH_) % ALPHABET_LENGTH_;
}

int Rotor::shuffleDown(int input_index){
  return (input_index + getCurrentPosition()) % ALPHABET_LENGTH_;
}

int Rotor::getPreviousPosition(){
  // cout << "previous position " << previous_position_ << endl;
  return previous_position_;
}

int Rotor::getCurrentPosition(){
  // cout << "current position " << current_position_ << endl;
  return current_position_;
}

bool Rotor::isCurrentPositionInNotch(){
  for(int i= 0; i< num_of_notches_; i++){
    if(current_position_ == notches_[i]){
      // cout << "yes notch at " << current_position_ << endl;
      return true;
    }
  }
  return false;
}

int Rotor::convertForward(int input_index){
  return contacts_[input_index];
}

int Rotor::convertBackward(int input_index){
  // cout << "starting position in backward " << input_index << endl;
  for(int i = 0; i < ALPHABET_LENGTH_; i++){
      // cout << "contacts " << contacts_[i] << endl;
      if(input_index == contacts_[i]){
        // cout << "contacts backward[input_index] " << i << endl;
          return i;
      }
  }
  // Double check this!!
  return input_index;
}

void Rotor::returnNonNumericCharacterError(){
  cerr << "Non-numeric character for mapping in rotor file rotor.rot"  << endl;
}
