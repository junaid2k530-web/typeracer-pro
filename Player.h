#pragma once
#include "Typist.h"

// Human player - reads real keyboard input
class Player : public Typist {
public:
    Player(const std::string& name);

    // Overrides Typist::type() with real keyboard input + timing
    bool type(const std::string& targetText) override;
};
