#include <iostream>
#include <cstring>
#include <string>

#include "Enigma.h"

using namespace std;

int main()
{
    cout << "Encrypting!" << endl;

    Enigma enigma;
    enigma.setRotors({Rotor::Rotor_I, Rotor::Rotor_II, Rotor::Rotor_III});
    enigma.setRotorsPosition({1, 2, 3});

    std::vector<std::string> encrypted;

    std::string key = "A";
    auto res = enigma.encrypt(key);
    encrypted.push_back(res);

    printf("Encrypt result: %s -> %s\n", key.c_str(), res.c_str());

    key = "B";
    res = enigma.encrypt(key);
    encrypted.push_back(res);
    printf("Encrypt result: %s -> %s\n", key.c_str(), res.c_str());

    key = "C";
    res = enigma.encrypt(key);
    encrypted.push_back(res);
    printf("Encrypt result: %s -> %s\n", key.c_str(), res.c_str());

    cout << "Decrypting!" << endl;

    enigma.setRotorsPosition({1, 2, 3});

    for(const auto &l: encrypted) {
        auto res = enigma.encrypt(l);
        printf("Decrypt result: %s -> %s\n", l.c_str(), res.c_str());
    }

    return 0;
}
