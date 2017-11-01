#ifndef BYTE_ORDER_MUTATIONS_H
#define BYTE_ORDER_MUTATIONS_H

#include "clang/ASTMatchers/ASTMatchFinder.h"

namespace ByteOrderMutations {

void addMatcher(clang::ast_matchers::MatchFinder& finder);

}

#endif //BYTE_ORDER_MUTATIONS_H
