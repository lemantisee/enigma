TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Component.cpp \
        Enigma.cpp \
        Rotor.cpp \
        main.cpp

HEADERS += \
    Component.h \
    Enigma.h \
    Rotor.h \
    alphabet.h \
    errors.h
