#include "mem_access_mutation.h"

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
	if(!binop) {
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
	std::string mutatedWidth="int64_t";

	if(intW==16) {
		mutatedWidth="int32_t";
	} else if(intW==32) {
		mutatedWidth="int64_t";
	} else {
		return "";
	}

	std::string lhsToReplace=utils::getSourceCodeLHS(*binop,*result.SourceManager);
	std::string rhsToReplace=utils::getSourceCodeRHS(*binop,*result.SourceManager);

	std::stringstream replacement;
	replacement<<"*(reinterpret_cast<"<<mutatedWidth<<"*>(&"<<lhsToReplace<<"))"<<"="<<rhsToReplace;

	return replacement.str();
}

class MemAccessCallback : public MatchFinder::MatchCallback {
 public:
	MemAccessCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
    const BinaryOperator* binop = result.Nodes.getNodeAs<BinaryOperator>("intAssignment");
    if(!binop || !result.SourceManager->isInMainFile(binop->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant (mem access violation) for binary operator: ";
    binop->dump();

    std::string repl=replacement(result);
    if(repl.empty()) {
    	return;
    }

    FileID fileID=result.SourceManager->getFileID(binop->getLocStart());

	Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.InsertTextBefore(result.SourceManager->getLocForStartOfFile(fileID),"#include <cstdint>\n");
	rewriter.ReplaceText(binop->getSourceRange(),repl);
	rewriter.getEditBuffer(fileID)
			.write(streamProvider.get_next_mutant_stream(
					result.SourceManager->getFilename(binop->getLocStart()),MutationType::memAccess));
  }
};

std::string memAccessViolationClassName(const std::string& originalType) {
	std::stringstream className;
	className<<"mem_access_violation_"<<originalType.substr(0,originalType.find('<'));
	return className.str();
}

std::string memAccessViolationClassDefinition(const std::string& originalType) {
	std::stringstream classDef;
	classDef<<"#ifndef MEM_ACCESS_VIOLATION_MUTATION_CLASS_H"<<std::endl;
	classDef<<"#define MEM_ACCESS_VIOLATION_MUTATION_CLASS_H"<<std::endl;
	classDef<<"#include <systemc.h>"<<std::endl;
	classDef<<"class "<<memAccessViolationClassName(originalType)<<" : public "<<originalType<< " {"<<std::endl;
	classDef<<"public:"<<std::endl;
	classDef<<"virtual void write( const int& value_ )"<<std::endl;
	classDef<<"{"<<std::endl;
	classDef<<"    bool value_changed = !( m_cur_val == value_ );"<<std::endl;
	classDef<<"    if ( !policy_type::check_write(this, value_changed) )"<<std::endl;
	classDef<<"        return;"<<std::endl;
	classDef<<"    *(reinterpret_cast<int64_t*>(&m_new_val))=value_;"<<std::endl;
	classDef<<"    if( value_changed ) {"<<std::endl;
	classDef<<"        request_update();"<<std::endl;
	classDef<<"    }"<<std::endl;
	classDef<<"}"<<std::endl;
	classDef<<memAccessViolationClassName(originalType)<<"& operator = ( const int& a )"<<std::endl;
	classDef<<"{ write( a ); return *this; }"<<std::endl;
	classDef<<"};"<<std::endl;
	classDef<<"#endif // MEM_ACCESS_VIOLATION_MUTATION_CLASS_H"<<std::endl;

	return classDef.str();
}

class SignalFieldDeclCallback : public MatchFinder::MatchCallback {
private:
	std::set<std::string> mHandledLocations;

 public:
  void run(const MatchFinder::MatchResult &result) override {
    const FieldDecl* fieldDecl = result.Nodes.getNodeAs<FieldDecl>("field");
    if(!fieldDecl || result.SourceManager->isInSystemHeader(fieldDecl->getLocStart())) {
    	return;
    }
    std::string locString=fieldDecl->getLocStart().printToString(*result.SourceManager);
    if(mHandledLocations.insert(locString).second==false) {
    	return;
    }
    llvm::outs()<<"Generate mutant (mem access violation) for signal field: ";
    fieldDecl->dump();

    FileID fileID=result.SourceManager->getFileID(fieldDecl->getLocStart());
    std::string typeStr = fieldDecl->getType().getUnqualifiedType().getAsString();
    std::string replacedTypename=memAccessViolationClassName(typeStr);



	Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.InsertTextBefore(result.SourceManager->getLocForStartOfFile(fileID),memAccessViolationClassDefinition(typeStr));
	SourceLocation locSpec=fieldDecl->getTypeSpecStartLoc();
	size_t length = fieldDecl->getType().getUnqualifiedType().getAsString().find(">");
	rewriter.ReplaceText(SourceRange(locSpec,locSpec.getLocWithOffset(length)),replacedTypename);
	rewriter.getEditBuffer(fileID)
			.write(streamProvider.get_next_mutant_stream(
					result.SourceManager->getFilename(fieldDecl->getLocStart()),MutationType::memAccess));
  }
};


namespace MemAccessMutations {

MemAccessCallback memAccessCallback;
SignalFieldDeclCallback signalFieldDeclCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(intAssignmentMatcher,&memAccessCallback);
	finder.addMatcher(signalFieldDeclMatcher,&signalFieldDeclCallback);
}

}
