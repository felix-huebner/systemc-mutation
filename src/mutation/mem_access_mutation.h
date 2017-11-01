#ifndef MEM_ACCESS_MUTATIONS_H
#define MEM_ACCESS_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace MemAccessMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //MEM_ACCESS_MUTATIONS_H
