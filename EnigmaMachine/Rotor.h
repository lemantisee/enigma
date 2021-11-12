#ifndef ROTOR_H
#define ROTOR_H

#include <vector>

class Rotor
{
public:
    enum RotorNumber {
        Rotor_I,
        Rotor_II,
        Rotor_III,
        Rotor_IV,
    };

    Rotor(RotorNumber rotorNumber);
    void setStartPosition(size_t position);

    size_t getCurrentPosition();
    size_t getPreviousPosition();
    void rotate();
    // shiftUp and shiftDown calculates absolute position of input_index
    // by adding/subtructing current position of rotor.
    size_t shiftUp(size_t input_index);
    size_t shiftDown(size_t input_index);
    size_t mapForward(size_t input_index);
    size_t mapBackward(size_t contact);
    bool isCurrentPositionInNotch();

private:
    static std::vector<size_t> getRotorContacts(RotorNumber rotorNumber);

    size_t mCurrentPosition = 0;
    // This attribute is used to compare with current_position to see
    // whether the next rotor should rotate, when a notche is triggered.
    size_t mPrevPosition = 0;
    // Store first 26 letter index
    //int contacts_[ALPHABET_LENGTH];
    // Store after 26th letter index as notches
    std::vector<size_t> mNotches;
    std::vector<size_t> mContacts;
};

#endif
