#include <iostream>
#include <fstream>
#include <cstring>
#include "Enigma.h"
#include "Rotor.h"
#include "Plugboard.h"
#include "Reflector.h"

using namespace std;

Enigma::Enigma(int argc, char** argv){
  // Does not have to use new here
  // if used new, put delete in destructor
  plugboard = new Plugboard(argv[1]);
  reflector = new Reflector(argv[2]);

  num_of_rotors = argc-4;
  rotors = new Rotor*[num_of_rotors];
  int starting_position;
  for(int i = 0; i < num_of_rotors; i++){
    starting_position = get_rotor_position(argv[argc-1], i);
    rotors[i] = new Rotor(argv[i+3], starting_position);
  }
}

int Enigma::get_rotor_position(const char* path, int position){
  int num;
  int counter = 0;
  fstream in_stream;
  in_stream.open(path);
  if(in_stream.fail() && in_stream >> num){
    cout << "error opening file for rotor position" << endl;
    // TODO error message
  }
  while(!in_stream.eof() && in_stream >> num){
    if(position == counter){
      return num;
    }
    counter++;
  }
  // Does this always close input stream?
  in_stream.close();
  return -1;
}

void Enigma::encrypt_message(const char* message, char* encrypted_message){
  int message_length = strlen(message);
  int current_index;
    // TODO Can I use recursion here??
  for(int i = 0; i < message_length; i++){
    // TODO not put integer here! Use const int
    current_index = message[i] -65;

    rotor_process(current_index);
    // cout << "ascii index " << current_index << endl;
    encrypted_message[i] = current_index + 65;

  }
  encrypted_message[message_length] = '\0';

}

void Enigma::rotor_process(int& current_index){
  // TODO Can I use recursion here??

  // TODO pass reference instead
  // cout << "current index before plugboard " << current_index << endl;
  current_index = plugboard->convert_forward(current_index);
  // cout << "current index after plugboard " << current_index << endl;

  // First rotate the right most rotor by one
  rotors[num_of_rotors-1]->rotate_forward();
  for(int i = num_of_rotors; i > 0; i--){
    current_index = rotors[i-1]->convert_forward(current_index);
    // cout << "current_index i " << i << " and index " << current_index << endl;
    if(rotors[i-1]->is_current_position_in_notch()){
      if(i-1 > 0){
        // cout << "notch detected! " << endl;
        rotors[i-2]->rotate_forward();
      }
    }
  }
  current_index = reflector->convert_forward(current_index);
  // cout << "reversing from here... " << current_index << endl;
  for(int i = 0; i < num_of_rotors; i++){
    current_index = rotors[i]->convert_backward(current_index);
    // cout << "current_index " << current_index << endl;
  }
  current_index = plugboard->convert_forward(current_index);
}
