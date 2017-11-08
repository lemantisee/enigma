#ifndef ROTOR_H
#define ROTOR_H

const int ALPHABET_LENGTH = 26;
class Rotor{
private:
  int contacts[ALPHABET_LENGTH];
  int num_of_notches;
  int* notches;
public:
  Rotor(const char* path_rot);
  int check_input(const char* path_rot);
  void rotate();
  bool is_key_pressed();
  void rotate_forward(int* array);
  void rotate_backward(int* array);
};

#endif
