#include "PayoffMatrix.hpp"

#include <string>

std::string actionToString(Action action) {
    return action == Action::Cooperate ? "Cooperate" : "Defect";
}

Action oppositeAction(Action action) {
    return action == Action::Cooperate ? Action::Defect : Action::Cooperate;
}

int PayoffMatrix::scoreFor(Action myAction, Action opponentAction) const {
    if (myAction == Action::Cooperate && opponentAction == Action::Cooperate) {
        return reward;
    }
    if (myAction == Action::Defect && opponentAction == Action::Cooperate) {
        return temptation;
    }
    if (myAction == Action::Cooperate && opponentAction == Action::Defect) {
        return sucker;
    }
    return punishment;
}
