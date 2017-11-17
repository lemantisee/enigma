#include "SubComponent.h"
#include "errors.h"

#include <iostream>
#include <fstream>

using namespace std;

int SubComponent::map(int input){
  for(int i = 0; i< PAIR_SIZE_; i++){
    if(input == input_pair1_[i]){
      return input_pair2_[i];
    }
    if(input == input_pair2_[i]){
      return input_pair1_[i];
    }
  }
  return input;
}

void SubComponent::setConfig(const char* path){
  // Do I need to get array length here??
  int array_length = getArrayLength(path);
  int num_array[array_length];
  mapInputToArray(path, num_array);

  if(isDuplicateInt(num_array, array_length)){
    //  How do you return this?
    returnConfigError();
  }
  for(int i = 0; i< array_length; i++){
    if(i%2 == 0){
      input_pair1_[i/2] = num_array[i];
    }
    else{
      input_pair2_[i/2] = num_array[i];
    }
  }
}
