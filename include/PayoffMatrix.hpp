#pragma once

#include "Action.hpp"
struct PayoffMatrix {
    int reward = 3;
    int temptation = 5;
    int sucker = 0;
    int punishment = 1;
    int scoreFor(Action myAction, Action opponentAction) const;
};
