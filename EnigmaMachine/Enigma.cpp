#include "Enigma.h"

#include "Rotor.h"

void Enigma::setRotors(const std::vector<Rotor::RotorNumber> &rotors)
{
    mRotors.clear();
    for (const auto &rt : rotors) {
        mRotors.push_back(Rotor(rt));
    }
}

void Enigma::setRotorsPosition(const std::vector<size_t> &positions)
{
    if (mRotors.size() != positions.size()) {
        return;
    }

    for (size_t i = 0; i < positions.size(); ++i) {
        size_t pos = positions[i];
        auto &rotor = mRotors[i];
        rotor.setStartPosition(pos);
    }
}

void Enigma::addPlugConnection(const std::string &from, const std::string &to)
{
    int fromChar = std::toupper(from.front());
    int toChar = std::toupper(to.front());

    mPlugConnections[size_t(fromChar - 'A')] = size_t(toChar - 'A');
}

void Enigma::clearPlugboard()
{
    mPlugConnections.clear();
}

std::string Enigma::encrypt(const std::string &letter)
{
    if (mRotors.empty()) {
        return letter;
    }

    if (letter.empty()) {
        return letter;
    }

    size_t current_index = size_t(letter.front() - 'A');
    current_index = getConnectionsMap(current_index, mPlugConnections);

    mRotors.back().rotate();

    for (size_t i = mRotors.size(); i > 0; i--) {
        // TODO Needs explanation here
        current_index = mRotors[i - 1].shiftDown(current_index);
        current_index = mRotors[i - 1].mapForward(current_index);
        current_index = mRotors[i - 1].shiftUp(current_index);
        if (mRotors[i - 1].isCurrentPositionInNotch()
            && mRotors[i - 1].getPreviousPosition() != mRotors[i - 1].getCurrentPosition()) {
            if (i - 1 > 0) {
                mRotors[i - 2].rotate();
            }
        }
    }

    current_index = getConnectionsMap(current_index, mReflectoConnections);

    for (auto &rotor : mRotors) {
        current_index = rotor.shiftDown(current_index);
        current_index = rotor.mapBackward(current_index);
        current_index = rotor.shiftUp(current_index);
    }

    current_index = getConnectionsMap(current_index, mPlugConnections);
    std::string res;
    res.push_back(char(current_index + 'A'));
    return res;
}

size_t Enigma::getConnectionsMap(size_t input, const std::map<size_t, size_t> &connections) const
{
    auto it = connections.find(input);
    if (it != connections.end()) {
        return it->second;
    }

    auto it_val = std::find_if(connections.begin(), connections.end(), [input](const auto &p) { return p.second == input; });

    if (it_val != connections.end()) {
        return it_val->first;
    }

    return input;
}
