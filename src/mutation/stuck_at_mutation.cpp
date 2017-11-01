#include "switch_ports_mutation.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

#include "matchers.h"
#include "mutant_stream_provider.h"
#include "utils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;

class StuckAtCallback : public MatchFinder::MatchCallback {
 public:
	StuckAtCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
	const CXXOperatorCallExpr* opcall = result.Nodes.getNodeAs<CXXOperatorCallExpr>("connectPorts");
    const VarDecl* varDecl = result.Nodes.getNodeAs<VarDecl>("varDecl");
    if(!varDecl || !result.SourceManager->isInMainFile(varDecl->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate stuck at mutant for: ";
    varDecl->dump();

    std::string signalType=varDecl->getType().getUnqualifiedType().getAsString();
    ClassTemplateSpecializationDecl* templDecl=static_cast<ClassTemplateSpecializationDecl*>(varDecl->getType()->getAsCXXRecordDecl());
    if(templDecl==NULL) {
    	std::cerr<<"cannot get ClassTemplateSpecializationDecl"<<std::endl;
    	abort();
    }
    std::string signalInnerType=templDecl->getTemplateArgs().get(0).getAsType().getAsString();
    std::cout<<"stuck at for type: "<<signalInnerType<<std::endl;

    size_t podWidth=utils::podWidth(signalInnerType);
    if(podWidth<1) {
    	return;
    }

    for(int val=0;val<=1;val++) {
		for(size_t bit = 0; bit<podWidth; bit+=4) {
			std::stringstream setMutationLine;
			setMutationLine<<varDecl->getQualifiedNameAsString()<<".set_mutation("<<bit<<","<<(val?"true":"false")<<");"<<std::endl;

			Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
			rewriter.InsertTextAfter(opcall->getLocEnd().getLocWithOffset(2),setMutationLine.str());
			rewriter.getEditBuffer(result.SourceManager->getFileID(varDecl->getLocEnd()))
					.write(streamProvider.get_next_mutant_stream(
							result.SourceManager->getFilename(varDecl->getLocEnd()),MutationType::stuckAt));

		}
    }
  }

};



namespace StuckAtMutations {

StuckAtCallback stuckAtCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(connectPortMatcher,&stuckAtCallback);
}

}
