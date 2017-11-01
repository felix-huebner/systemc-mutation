#ifndef SYSTEM_C_MUTATIONS_H
#define SYSTEM_C_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace SystemCMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //SYSTEM_C_MUTATIONS_H
