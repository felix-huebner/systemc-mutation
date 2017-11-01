#ifndef MATCHERS_H
#define MATCHERS_H

#include "clang/ASTMatchers/ASTMatchers.h"

extern clang::ast_matchers::StatementMatcher intAssignmentMatcher;

extern clang::ast_matchers::StatementMatcher intAssignmentOverloadedMatcher;

extern clang::ast_matchers::StatementMatcher intAssignmentMatcherNonNull;

extern clang::ast_matchers::StatementMatcher intAssignmentOverloadedMatcherNonNull;

extern clang::ast_matchers::StatementMatcher sensitiveMatcher;

extern clang::ast_matchers::StatementMatcher connectPortMatcher;

clang::ast_matchers::StatementMatcher getConnectPortMatcherWithSignalName(const std::string& signalName);

extern clang::ast_matchers::DeclarationMatcher signalVarDeclMatcher;

extern clang::ast_matchers::StatementMatcher ifStmtMatcher;

extern clang::ast_matchers::StatementMatcher whileStmtMatcher;

extern clang::ast_matchers::StatementMatcher floatLiteralMatcher;

extern clang::ast_matchers::DeclarationMatcher signalFieldDeclMatcher;

#endif //MATCHERS_H
