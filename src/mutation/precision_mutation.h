#ifndef PRECISION_MUTATION_H
#define PRECISION_MUTATION_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace PrecisionMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //PRECISION_MUTATION_H
