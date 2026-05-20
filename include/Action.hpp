#pragma once

#include <string>

enum class Action {
    Cooperate,
    Defect
};

std::string actionToString(Action action);
Action oppositeAction(Action action);
