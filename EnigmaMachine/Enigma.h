#ifndef ENIGMA_H
#define ENIGMA_H

#include <vector>
#include <string>
#include <map>

#include "Rotor.h"

class Component;

class Enigma
{
public:
    void setRotors(const std::vector<Rotor::RotorNumber> &rotors);
    void setRotorsPosition(const std::vector<size_t> &positions);
    void addPlugConnection(const std::string &from, const std::string &to);
    void clearPlugboard();

    std::string encrypt(const std::string &letter);
    void encryptMessage(char &letter);

private:
    size_t getConnectionsMap(size_t input, const std::map<size_t, size_t> &connections) const;
    size_t getReflectorMap(size_t input) const;

    std::vector<Rotor> mRotors;
    std::map<size_t, size_t> mPlugConnections;
    const std::map<size_t, size_t> mReflectoConnections = {
        {0, 4},
        {1, 9},
        {2, 12},
        {3, 25},
        {5, 11},
        {6, 24},
        {7, 23},
        {8, 21},
        {10, 22},
        {13, 17},
        {14, 16},
        {15, 20},
        {18, 19},
    };
};

#endif
