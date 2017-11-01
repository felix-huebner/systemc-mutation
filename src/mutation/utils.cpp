#include "utils.h"

#include "clang/Lex/Lexer.h"

#include <iostream>

using namespace clang;

namespace utils {

std::string getSourceCode(const clang::BinaryOperator& binop, const clang::SourceManager& sm) {
	return Lexer::getSourceText(CharSourceRange::getTokenRange(binop.getLocStart(),binop.getLocEnd()),sm,LangOptions());//see http://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
}

std::string getSourceCodeLHS(const clang::BinaryOperator& binop, const clang::SourceManager& sm) {
	return Lexer::getSourceText(CharSourceRange::getTokenRange(binop.getLocStart(),binop.getOperatorLoc().getLocWithOffset(-1)),sm,LangOptions());//see http://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
}

std::string getSourceCodeRHS(const clang::BinaryOperator& binop, const clang::SourceManager& sm) {
	return Lexer::getSourceText(CharSourceRange::getTokenRange(binop.getOperatorLoc().getLocWithOffset(1),binop.getLocEnd()),sm,LangOptions());//see http://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
}

std::string getSourceCode(const clang::Expr& expr, const clang::SourceManager& sm) {
	return Lexer::getSourceText(CharSourceRange::getTokenRange(expr.getExprLoc()),sm,LangOptions());//see http://stackoverflow.com/questions/11083066/getting-the-source-behind-clangs-ast
}

std::string getSourceCodeOfArgument(const clang::CXXOperatorCallExpr& opcall, unsigned int arg, const clang::SourceManager& sm) {
	return Lexer::getSourceText(CharSourceRange::getTokenRange(opcall.getArg(arg)->getExprLoc()),sm,LangOptions());
}

size_t intWidth(const std::string& intType) {
	std::cout<<"intType: "<<intType<<std::endl;
	if(intType=="int") {
		return sizeof(int)*8;
	} else if(intType=="unsigned int") {
		return sizeof(unsigned int)*8;
	} else if(intType=="unsigned") {
		return sizeof(unsigned)*8;
	} else if(intType=="short") {
		return sizeof(short)*8;
	} else if(intType=="long") {
		return sizeof(long)*8;
	}
	return 0;
}

size_t podWidth(const std::string& podType) {
	if(intWidth(podType)>0) {
		return intWidth(podType);
	} else if(podType=="float") {
		return 32;
	} else if(podType=="double") {
		return 64;
	} else if(podType=="bool" || podType=="_Bool") { //_Bool is the template argument of sc_signal
		return 1;
	}
	return 0;
}

}
