#ifndef UTILS_H
#define UTILS_H

#include "clang/AST/AST.h"

#include <string>

namespace utils {

std::string getSourceCode(const clang::BinaryOperator& binop, const clang::SourceManager& sm);
std::string getSourceCodeLHS(const clang::BinaryOperator& binop, const clang::SourceManager& sm);
std::string getSourceCodeRHS(const clang::BinaryOperator& binop, const clang::SourceManager& sm);
std::string getSourceCode(const clang::Expr& expr, const clang::SourceManager& sm);
std::string getSourceCodeOfArgument(const clang::CXXOperatorCallExpr& opcall, unsigned int arg, const clang::SourceManager& sm);

size_t intWidth(const std::string& intType);
size_t podWidth(const std::string& podType);

}

#endif //UTILS_H
