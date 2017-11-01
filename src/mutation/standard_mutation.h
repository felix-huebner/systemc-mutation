#ifndef STANDARD_MUTATIONS_H
#define STANDARD_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace StandardMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //STANDARD_MUTATIONS_H
