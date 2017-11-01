#include "standard_mutation.h"

#include <memory>
#include <vector>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/Refactoring.h"
#include "llvm/Support/MemoryBuffer.h"


#include "clang/Rewrite/Core/Rewriter.h"

#include "mutant_stream_provider.h"

#include "matchers.h"


using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;

StatementMatcher relOpMatcher =
		binaryOperator(
				anyOf(
						hasOperatorName("<"),
						hasOperatorName("<="),
						hasOperatorName(">"),
						hasOperatorName(">=")
				)
		).bind("binop");

StatementMatcher aritOpMatcher =
		binaryOperator(
						anyOf(
								hasOperatorName("+"),
								hasOperatorName("-"),
								hasOperatorName("*"),
								hasOperatorName("/"),
								hasOperatorName("%")
						)
				).bind("binop");

StatementMatcher logicalOpMatcher =
		binaryOperator(
						anyOf(
								hasOperatorName("&&"),
								hasOperatorName("||")
						)
				).bind("binop");

StatementMatcher binOpMatcher = anyOf(relOpMatcher, aritOpMatcher, logicalOpMatcher);

static void get_replacements_rel_op(BinaryOperator::Opcode op, std::vector<std::string>& replacements) {
	switch(op) {
	case BO_LT:
		replacements={"<=",">",">="};
		break;
	case BO_GT:
		replacements={">=","<=","<"};
		break;
	case BO_LE:
		replacements={"<",">",">="};
		break;
	case BO_GE:
		replacements={">","<","<="};
		break;
	default:
		break;
	}
}

static void get_replacements_arit_op(BinaryOperator::Opcode op, std::vector<std::string>& replacements) {
	switch(op) {
	case BO_Add:
		replacements={"-","*","/","%"};
		break;
	case BO_Sub:
		replacements={"+","*","/","%"};
		break;
	case BO_Mul:
		replacements={"+","-","/","%"};
		break;
	case BO_Div:
		replacements={"+","-","*","%"};
		break;
	case BO_Rem:
		replacements={"+","-","*","/"};
		break;
	default:
		break;
	}
}

static void get_replacements_logical_bin_op(BinaryOperator::Opcode op, std::vector<std::string>& replacements) {
	switch(op) {
	case BO_LAnd:
		replacements={"||"};
		break;
	case BO_LOr:
		replacements={"&&"};
		break;
	default:
		break;
	}
}

static void get_replacements(BinaryOperator::Opcode op, std::vector<std::string>& replacements) {
	get_replacements_rel_op(op,replacements);
	get_replacements_arit_op(op,replacements);
	get_replacements_logical_bin_op(op,replacements);
}


namespace {
class BinOpCallback : public MatchFinder::MatchCallback {
 public:
	BinOpCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
    const BinaryOperator* binop = result.Nodes.getNodeAs<BinaryOperator>("binop");
    if(!binop || !result.SourceManager->isInMainFile(binop->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant for binary operator: ";
    binop->dump();


    std::vector<std::string> replacements;

    get_replacements(binop->getOpcode(),replacements);

    for(auto& r : replacements) {
		Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
		rewriter.ReplaceText(binop->getOperatorLoc(),binop->getOpcodeStr().size(),r);
		rewriter.getEditBuffer(result.SourceManager->getFileID(binop->getLocStart()))
				.write(streamProvider.get_next_mutant_stream(
						result.SourceManager->getFilename(binop->getLocStart()),MutationType::ordinary));
    }
  }
};
} // end anonymous namespace

template<typename T>
class ConditionalStmtCallback : public MatchFinder::MatchCallback {
private:
	std::string mStatementType;
 public:
  ConditionalStmtCallback(const std::string& statementType) : mStatementType(statementType) {}

  void run(const MatchFinder::MatchResult &result) override {
    const T* conditionalStmt = result.Nodes.getNodeAs<T>(mStatementType);
    if(!conditionalStmt || !result.SourceManager->isInMainFile(conditionalStmt->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant for "<<mStatementType<<" statement: ";
    conditionalStmt->dump();


    for(int i=0;i<=1;i++) {
		Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
		SourceRange condRange=conditionalStmt->getCond()->getSourceRange();
		rewriter.ReplaceText(condRange,(i==0?"false":"true"));
		rewriter.getEditBuffer(result.SourceManager->getFileID(conditionalStmt->getLocStart()))
				.write(streamProvider.get_next_mutant_stream(
						result.SourceManager->getFilename(conditionalStmt->getLocStart()),MutationType::ordinary));
    }
  }
};

namespace StandardMutations {
BinOpCallback binOpCallback;
ConditionalStmtCallback<IfStmt> ifStmtCallback("if");
ConditionalStmtCallback<WhileStmt> whileStmtCallback("while");

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(binOpMatcher,&binOpCallback);
	finder.addMatcher(ifStmtMatcher,&ifStmtCallback);
	finder.addMatcher(whileStmtMatcher,&whileStmtCallback);
}
}
