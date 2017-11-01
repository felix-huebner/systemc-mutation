#ifndef STUCK_AT_MUTATIONS_H
#define STUCK_AT_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace StuckAtMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //STUCK_AT_MUTATIONS_H
