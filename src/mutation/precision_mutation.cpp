#include "precision_mutation.h"

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


class FloatLiteralCallback : public MatchFinder::MatchCallback {
 public:
  void run(const MatchFinder::MatchResult &result) override {
    const FloatingLiteral* floatLiteral = result.Nodes.getNodeAs<FloatingLiteral>("float");
    if(!floatLiteral || !result.SourceManager->isInMainFile(floatLiteral->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate precision mutation: ";
    floatLiteral->dump();

    std::string replacement;
    APFloat val = floatLiteral->getValue();
    if(val.getSizeInBits(val.getSemantics())==32) {
    	replacement=Lexer::getSourceText(CharSourceRange::getTokenRange(floatLiteral->getSourceRange()),*result.SourceManager,result.Context->getLangOpts());
    	replacement=replacement.substr(0,replacement.length()-1);
    } else if(val.getSizeInBits(val.getSemantics())==64) {
    	replacement=Lexer::getSourceText(CharSourceRange::getTokenRange(floatLiteral->getSourceRange()),*result.SourceManager,result.Context->getLangOpts());
    	replacement+="f";
    } else {
    	return;
    }

    if(replacement.empty()) {
    	return;
    }

    FileID fileID=result.SourceManager->getFileID(floatLiteral->getLocStart());

	Rewriter rewriter(result.Context->getSourceManager(), result.Context->getLangOpts());
	rewriter.InsertTextBefore(result.SourceManager->getLocForStartOfFile(fileID),"#include <endian.h>\n");
	rewriter.ReplaceText(floatLiteral->getSourceRange(),replacement);
	rewriter.getEditBuffer(fileID)
			.write(streamProvider.get_next_mutant_stream(
					result.SourceManager->getFilename(floatLiteral->getLocStart()),MutationType::precision));
  }
};

namespace PrecisionMutations {

FloatLiteralCallback floatLiteralCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(floatLiteralMatcher,&floatLiteralCallback);
}

}
