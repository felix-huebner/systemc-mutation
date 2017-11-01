#ifndef SWITCH_PORTS_MUTATIONS_H
#define SWITCH_PORTS_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace SwitchPortMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

void complete();

}

#endif //SWITCH_PORTS_MUTATIONS_H
