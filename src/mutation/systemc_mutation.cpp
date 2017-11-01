#include "byte_order_mutation.h"

#include <iostream>
#include <sstream>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "matchers.h"
#include "mutant_stream_provider.h"
#include "utils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;




class SensitiveCallback : public MatchFinder::MatchCallback {
 public:
	SensitiveCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
    const CXXOperatorCallExpr* opcall = result.Nodes.getNodeAs<CXXOperatorCallExpr>("sensitive");
    if(!opcall || !result.SourceManager->isInMainFile(opcall->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant for sensitive operator<<: ";
    opcall->dump();


    std::string filename=result.SourceManager->getFilename(opcall->getLocStart());
    if(filename.empty()) {
    	std::cerr<<"WARNING: Sensitive call (CXXOperatorCallExpr) is not in ordinary file"<<std::endl;
    	return;
    }
    Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.RemoveText(opcall->getSourceRange());
	rewriter.getEditBuffer(result.SourceManager->getFileID(opcall->getLocStart()))
			.write(streamProvider.get_next_mutant_stream(
				result.SourceManager->getFilename(opcall->getLocStart()),MutationType::sensitivity));
  }
};



namespace SystemCMutations {

SensitiveCallback systemCCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(sensitiveMatcher,&systemCCallback);
}

}
