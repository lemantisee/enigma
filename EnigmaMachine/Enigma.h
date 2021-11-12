#ifndef ENIGMA_H
#define ENIGMA_H

#include <fstream>
#include <vector>

#include "Rotor.h"

class Component;

class Enigma{
public:
  // Within this constructor, check all input files are valid by calling
  // each "checkXXXConfig" method.
  // Initialise all the components after all the check is done, so that, if
  // exception is thrown while checking config, it throws an error to main.cpp
  // before a new heap is created, thus avoiding memory leak even if
  //  constructor fails in the middle of construction.
  Enigma(int argc, char** argv);
  ~Enigma();

  void encryptMessage(char& letter);

private:
  // These 4 checkConfig methods check each input file by extracting input
  // and check if each input is valid (and throws exception error
  // as defined in the spec).
  // They also assign each valid integer to corresponding vectors.
  void checkPlugboardConfig(const char* path, std::vector<int>& contacts);
  void checkReflectorConfig(const char* path, std::vector<int>& contacts);
  void checkRotorConfig(const char* path, std::vector<int>& contacts);
  void checkRotorPositionConfig(const char* path);

  // This is a helper method for checking if inputs are valid for plugboard
  // The reason this is in a separate method is that the numbers in plugboard
  // have to be read off in pairs (as defined in the spec). Thus, unlike other
  // checkConfig methods, I call istream twice in every while loop iteration.
  bool isPlugboardInputValid(const char* path, fstream& in_stream, int& index_num);

  // Check if num is within the range of 0-25
  bool isNumberRangeCorrect(int num);

  // Check if num is already appeared in contacts.
  // If yes, return the index of the previous contact. Otherwise return -1
  int checkAppearedBefore(std::vector<int> contacts, int num, int position);

  std::vector<Rotor> mRotors;
  std::vector<int> mRotorPositions;
  // Even though I could easily get the number of rotors from rotors_,
  // I store this as an attribute, because I use it frequently
  // (e.g if statement, for loop etc).
  int mRotorsNumber = 0;
  Component *mPlugboard = nullptr;
  Component *mReflecor = nullptr;
};

#endif
