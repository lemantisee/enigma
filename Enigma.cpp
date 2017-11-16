#include "Enigma.h"
#include "Rotor.h"
#include "Plugboard.h"
#include "Reflector.h"
#include "errors.h"

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

Enigma::Enigma(int argc, char** argv){
    plugboard_ = new Plugboard(argv[1]);
    // Plugboard plugboard_(argv[1]);
    is_plugboard_initialized = true;
    // cout << "newed plugboard" << endl;
    reflector_ = new Reflector(argv[2]);
    // Reflector reflector_(argv[2]);
    // cout << "newed reflector" << endl;
    is_reflector_initialized = true;
    // cout << "argc " << argc << endl;
    if(argc == 3){
      num_of_rotors_ = 0;
    }
    else{
      num_of_rotors_ = argc-4;
    }

    // Is this even correct??
    if(num_of_rotors_ < 0){
      cerr << "No starting position for rotor 0 in rotor position file: rotor.pos";
      throw(NO_ROTOR_STARTING_POSITION);
    }

  initialiseRotorPosition(argv[argc-1]);
  checkRotorAndRotorPosition();
  // int starting_position;
  for(int i = 0; i < num_of_rotors_; i++){
    // starting_position = getRotorPosition(argv[argc-1], i);
    // if starting_position is -1, do something!!

    // rotors_[i] = new Rotor(argv[i+3], starting_position);
    Rotor rotor(argv[i+3], rotor_positions_[i]);
    rotors_.push_back(rotor);
    // cout << "newed rotor " << i << endl;
  }
}

Enigma::~Enigma(){
  if(is_plugboard_initialized){
      delete plugboard_;
  }
  if(is_plugboard_initialized){
      delete reflector_;
  }

  // rotors_.~Rotor();

  // for(int i = 0; i < num_of_rotors_; i++){
  //   delete rotors_[i];
  // }
  // // Is this corrent?
  // if(num_of_rotors_ > 0){
  //   delete[] rotors_;
  // }
}

void Enigma::checkRotorAndRotorPosition(){
  int rotor_position_length = rotor_positions_.size();
  int diff = rotor_position_length - num_of_rotors_;
  cout << "num_of_rotors_ " << num_of_rotors_ << endl;

  if(diff < 0){
    cerr << "No starting position for rotor " << num_of_rotors_ - diff - 1<< " in rotor position file: rotor.pos" << endl;
    throw(NO_ROTOR_STARTING_POSITION);
  }
  if(diff > 0){
    // This should not happen, I guess.
  }
}

void Enigma::initialiseRotorPosition(const char* path){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  // cout << "path " << path << endl;
  // if(in_stream.fail()){
  //   cerr << "error opening file for rotor position" << endl;
  //   throw INVALID_ROTOR_MAPPING;


  // char line[80];
  // in_stream.getline(line, 80);
  // cout << "line " << line << endl;

  while(in_stream >> num ){
    // Can I abstruct this away?
    // if(num > 25 || num < 0){
    //   // What is the message here?
    //   throw(INVALID_INDEX);
    // }
    rotor_positions_.push_back(num);
    counter++;
  }
  if(in_stream.eof()){
    return;
  }
  if(in_stream.fail()){
  // if(in_stream.fail() && !in_stream.eof()){
    cerr << "Non-numeric character in rotor positions file rotor.pos" << endl;
    throw(NON_NUMERIC_CHARACTER);
  }
  // Does this always close input stream?
  in_stream.close();
  // TODO have to do something about it
  // cerr << "No starting position for rotor " << counter << " in rotor position file: rotor.pos" << endl;
  throw(NO_ROTOR_STARTING_POSITION);
}

void Enigma::encryptMessage(char& message){
    // TODO Can I use recursion here??

    // TODO not put integer here! Use const int
  int current_index = message - 'A';

  rotorProcess(current_index);
  // cout << "ascii index " << current_index << endl;
  message = current_index + 'A';
  // cout << "encrypted_message is " << message << endl;
}

// TODO change function name
void Enigma::rotorProcess(int& current_index){
  // cout << "current index before plugboard " << current_index << endl;
  current_index = plugboard_->convertForward(current_index);
  // current_index = plugboard_.convertForward(current_index);

  // cout << "current index after plugboard " << current_index << endl;
  // First rotate the right most rotor by one
  if(num_of_rotors_ > 0){
    rotors_[num_of_rotors_-1].rotateDown();
    // rotors_[num_of_rotors_-1].rotateDown();
  }

  if(num_of_rotors_ > 0){
    for(int i = num_of_rotors_ ; i > 0; i--){
      // cout << "rotors_[i-1] " << rotors_[i]->getCurrentPosition() << endl;
      current_index = rotors_[i-1].shuffleDown(current_index);
      // cout << "current_index1 " << current_index << endl;
      current_index = rotors_[i-1].convertForward(current_index);
      // cout << "current_index2 " << current_index << endl;
      current_index = rotors_[i-1].shuffleUp(current_index);
      // cout << "current_index3 " << current_index << endl;
      // cout << "current_index i " << i << " and index " << current_index << endl;
      // TODO need to fix!!
      if(rotors_[i-1].isCurrentPositionInNotch() && \
         rotors_[i-1].getPreviousPosition() != \
         rotors_[i-1].getCurrentPosition()){
        // cout << "yes, notch at " << rotors_[i-1]->getCurrentPosition() << endl;
        if(i-1 > 0){
          rotors_[i-2].rotateDown();
        }
      }
    }
  }

  current_index = reflector_->convertForward(current_index);
  // current_index = reflector_.convertForward(current_index);
  // cout << "reversing from here... " << current_index << endl;
  if(num_of_rotors_ > 0){
    for(int i = 0; i < num_of_rotors_; i++){
      current_index = rotors_[i].shuffleDown(current_index);
      // cout << "current_index1 " << current_index << endl;
      current_index = rotors_[i].convertBackward(current_index);
      // cout << "current_index2 " << current_index << endl;

      current_index = rotors_[i].shuffleUp(current_index);
      // cout << "current_index3 " << current_index << endl;
    }
  }
  current_index = plugboard_->convertForward(current_index);
  // current_index = plugboard_.convertForward(current_index);
}
