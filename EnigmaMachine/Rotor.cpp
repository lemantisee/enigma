#include "Rotor.h"

namespace  {
const int ALPHABET_LENGTH = 26;
}

Rotor::Rotor(RotorNumber rotorNumber)
{
    auto rotorContacts = getRotorContacts(rotorNumber);
    if (rotorContacts.empty()) {
        return;
    }

    mContacts = rotorContacts;
    mContacts.pop_back();
    mNotches.push_back(rotorContacts.back());
}

void Rotor::setStartPosition(size_t position)
{
    mCurrentPosition = position;
}

size_t Rotor::getPreviousPosition()
{
    return mPrevPosition;
}

size_t Rotor::getCurrentPosition()
{
    return mCurrentPosition;
}

void Rotor::rotate()
{
    mPrevPosition = mCurrentPosition;
    mCurrentPosition = (mCurrentPosition + 1) % ALPHABET_LENGTH;
}

// Use moduler of ALPHABET_LENGTH (26) to make sure the output is always
// between 0 and 25
size_t Rotor::shiftUp(size_t input_index)
{
    return (input_index - getCurrentPosition() + ALPHABET_LENGTH) % ALPHABET_LENGTH;
}

size_t Rotor::shiftDown(size_t input_index)
{
    return (input_index + getCurrentPosition()) % ALPHABET_LENGTH;
}

size_t Rotor::mapForward(size_t input_index)
{
    return mContacts[input_index];
}

size_t Rotor::mapBackward(size_t contact)
{
    for (int i = 0; i < ALPHABET_LENGTH; i++) {
        if (contact == mContacts[i]) {
            return i;
        }
    }
    return contact;
}

bool Rotor::isCurrentPositionInNotch()
{
    int num_of_notches = mNotches.size();
    for (int i = 0; i < num_of_notches; i++) {
        if (mCurrentPosition == mNotches[i]) {
            return true;
        }
    }
    return false;
}

std::vector<size_t> Rotor::getRotorContacts(RotorNumber rotorNumber)
{
    switch (rotorNumber) {
    case Rotor_I: return {4, 10, 12, 5, 11, 6, 3, 16, 21, 25, 13, 19, 14, 22, 24, 7, 23, 20, 18, 15, 0, 8, 1, 17, 2, 9, 17};
    case Rotor_II: return {0, 9, 3, 10, 18, 8, 17, 20, 23, 1, 11, 7, 22, 19, 12, 2, 16, 6, 25, 13, 15, 24, 5, 21, 14, 4, 5};
    case Rotor_III: return {1, 3, 5, 7, 9, 11, 2, 15, 17, 19, 23, 21, 25, 13, 24, 4, 8, 22, 6, 0, 10, 12, 20, 18, 16, 14, 22};
    default: break;
    }
    return {};
}
