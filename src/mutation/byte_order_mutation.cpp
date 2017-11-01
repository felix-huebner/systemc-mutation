#include "byte_order_mutation.h"

#include <iostream>
#include <sstream>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "matchers.h"
#include "mutant_stream_provider.h"
#include "utils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;

static std::string replacement(const MatchFinder::MatchResult& result) {
	std::string intTypeName;

	const BinaryOperator* binop = result.Nodes.getNodeAs<BinaryOperator>("intAssignment");
	if(!binop || !result.SourceManager->isInMainFile(binop->getLocStart())) {
		return "";
	}

	const VarDecl* varDecl=result.Nodes.getNodeAs<VarDecl>("varDecl");
	if(varDecl) {
		intTypeName=varDecl->getType().getUnqualifiedType().getAsString();

	} else {
		const MemberExpr* memberExpr=result.Nodes.getNodeAs<MemberExpr>("member");
		if(memberExpr) {
			intTypeName=memberExpr->getType().getUnqualifiedType().getAsString();
		}
	}

	size_t intW=utils::intWidth(intTypeName);
	std::string htoCmd="";

	if(intW==16) {
		htoCmd="htobe16";
	} else if(intW==32) {
		htoCmd="htobe32";
	} else if(intW==64) {
		htoCmd="htobe64";
	} else {
		return "";
	}

	std::string lhsToReplace=utils::getSourceCodeLHS(*binop,*result.SourceManager);
	std::string rhsToReplace=utils::getSourceCodeRHS(*binop,*result.SourceManager);

	std::stringstream replacement;
	replacement<<lhsToReplace<<"="<<htoCmd<<"("<<rhsToReplace<<")";

	return replacement.str();
}

static std::string replacementOverloaded(const MatchFinder::MatchResult& result) {
	std::string intTypeName;

	const CXXOperatorCallExpr* opcall = result.Nodes.getNodeAs<CXXOperatorCallExpr>("intAssignmentOverloaded");
	if(!opcall || !result.SourceManager->isInMainFile(opcall->getLocStart())) {
		return "";
	}

	const VarDecl* varDecl=result.Nodes.getNodeAs<VarDecl>("varDecl");
	const Type* type=NULL;
	if(varDecl) {
		type=varDecl->getType().getUnqualifiedType().getTypePtrOrNull();

	} else {
		const MemberExpr* memberExpr=result.Nodes.getNodeAs<MemberExpr>("member");
		if(memberExpr) {
			type=memberExpr->getType().getUnqualifiedType().getTypePtrOrNull();
		}
	}

	if(type==NULL) {
		return "";
	}
	const ClassTemplateSpecializationDecl* templSpec = static_cast<const ClassTemplateSpecializationDecl*>(type->getAsCXXRecordDecl());
	if(templSpec==NULL) {
		return "";
	}

	intTypeName=templSpec->getTemplateArgs().get(0).getAsType().getUnqualifiedType().getAsString();

	size_t intW=utils::intWidth(intTypeName);
	std::string htoCmd="";

	if(intW==16) {
		htoCmd="htobe16";
	} else if(intW==32) {
		htoCmd="htobe32";
	} else if(intW==64) {
		htoCmd="htobe64";
	} else {
		return "";
	}

	std::string argToReplace=utils::getSourceCodeOfArgument(*opcall,1,*result.SourceManager);
	std::stringstream replacement;
	replacement<<htoCmd<<"("<<argToReplace<<")";

	return replacement.str();
}

class ByteOrderCallback : public MatchFinder::MatchCallback {
 public:
	ByteOrderCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
    const BinaryOperator* binop = result.Nodes.getNodeAs<BinaryOperator>("intAssignment");
    if(!binop || !result.SourceManager->isInMainFile(binop->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant (byte order) for binary operator: ";
    binop->dump();

    std::string repl=replacement(result);
    if(repl.empty()) {
    	return;
    }

    FileID fileID=result.SourceManager->getFileID(binop->getLocStart());

	Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.InsertTextBefore(result.SourceManager->getLocForStartOfFile(fileID),"#include <endian.h>\n");
	rewriter.ReplaceText(SourceRange(binop->getLocStart(),binop->getLocEnd()),repl);
	rewriter.getEditBuffer(fileID)
			.write(streamProvider.get_next_mutant_stream(
					result.SourceManager->getFilename(binop->getLocStart()),MutationType::byteOrder));
  }
};

class ByteOrderOverloadedCallback : public MatchFinder::MatchCallback {
 public:
  void run(const MatchFinder::MatchResult &result) override {
	const CXXOperatorCallExpr* opcall = result.Nodes.getNodeAs<CXXOperatorCallExpr>("intAssignmentOverloaded");

    if(!opcall || !result.SourceManager->isInMainFile(opcall->getLocStart())) {
    	return;
    }

    llvm::outs()<<"Generate mutant (byte order) for overloaded binary operator: ";
    opcall->dump();

    std::string repl=replacementOverloaded(result);
    if(repl.empty()) {
    	return;
    }

    FileID fileID=result.SourceManager->getFileID(opcall->getLocStart());

	Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.InsertTextBefore(result.SourceManager->getLocForStartOfFile(fileID),"#include <endian.h>\n");
	rewriter.ReplaceText(opcall->getArg(1)->getSourceRange(),repl);
	rewriter.getEditBuffer(fileID)
			.write(streamProvider.get_next_mutant_stream(
					result.SourceManager->getFilename(opcall->getLocStart()),MutationType::byteOrder));
  }
};



namespace ByteOrderMutations {

ByteOrderCallback byteOrderCallback;
ByteOrderOverloadedCallback byteOrderOverloadedCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(intAssignmentMatcherNonNull,&byteOrderCallback);
	finder.addMatcher(intAssignmentOverloadedMatcherNonNull,&byteOrderOverloadedCallback);
}

}
