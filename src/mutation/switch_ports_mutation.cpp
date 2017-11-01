#include "switch_ports_mutation.h"

#include <iostream>
#include <sstream>
#include <utility>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "matchers.h"
#include "mutant_stream_provider.h"
#include "utils.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace llvm;

typedef std::set<std::string> Signals;
typedef std::map<std::string, Signals> SignalTypeToSignalName;
typedef std::pair<std::string,std::string> SignalPair;
typedef std::vector<SignalPair> SignalPairs;

static void getSignalPairs(SignalPairs& pairs, const SignalTypeToSignalName& signals) {
	for(const auto& sameTypedSignals : signals) {
		for(Signals::const_iterator itOuter=sameTypedSignals.second.begin();
				itOuter!=sameTypedSignals.second.end(); itOuter++) {
			for(Signals::const_iterator itInner=itOuter;
					itInner!=sameTypedSignals.second.end(); itInner++) {
				if(itInner!=itOuter) {
					pairs.push_back(std::make_pair(*itOuter,*itInner));
				}
			}
		}
	}
}

class SwitchPortsCallback : public MatchFinder::MatchCallback {
 public:
	SwitchPortsCallback() {}

  void run(const MatchFinder::MatchResult &result) override {
    const CXXOperatorCallExpr* opcall = result.Nodes.getNodeAs<CXXOperatorCallExpr>("connectPorts");
    if(!opcall || !result.SourceManager->isInMainFile(opcall->getLocStart())) {
    	return;
    }
    llvm::outs()<<"Generate mutant for port connection: ";
    opcall->dump();

    const VarDecl* varDecl=result.Nodes.getNodeAs<VarDecl>("varDecl");
    if(!varDecl) {
    	return;
    }
    std::string signalType=varDecl->getType().getUnqualifiedType().getAsString();
    std::string signalName=varDecl->getQualifiedNameAsString();
    mSignals[signalType].insert(signalName);
    mSignalMatchResults.insert(std::make_pair(signalName,result));
    mSignalMatchResults.at(signalName).Context->Retain(); //TODO needed!!!: otherwise segfault in complete(), probably because of dangling references; no guarantuee that this really is the correct solution
    mSignalMatchResults.at(signalName).SourceManager->Retain();
  }

  void printSignals() {
	  for(const auto& ss : mSignals) {
		  std::cout<<"signal type "<<ss.first<<":"<<std::endl;
		  for(const auto& s : ss.second) {
			  std::cout<<s<<std::endl;
		  }
	  }

	  SignalPairs signalsPairs;
	  getSignalPairs(signalsPairs,mSignals);
	  std::cout<<"All signal pairs are:"<<std::endl;
	  for(const auto& sp : signalsPairs) {
		  std::cout<<"("<<sp.first<<","<<sp.second<<")"<<std::endl;
	  }
  }

  const SignalTypeToSignalName& getSameTypedSignals() {
	  return mSignals;
  }

  void complete() {
  	printSignals();

  	const SignalTypeToSignalName& signals =
  			getSameTypedSignals();
  	SignalPairs signalsPairs;
  	getSignalPairs(signalsPairs, signals);
  	std::cout << "All signal pairs are:" << std::endl;
  	for (const auto& sp : signalsPairs) {
  		std::cout << "(" << sp.first << "," << sp.second << ")" << std::endl;
  		switchSignals(sp);
  	}
  }
 private:
  void switchSignals(const SignalPair& signalPair) {
  	const CXXOperatorCallExpr* opcall1=mSignalMatchResults.at(signalPair.first).Nodes.getNodeAs<CXXOperatorCallExpr>("connectPorts");
  	const CXXOperatorCallExpr* opcall2=mSignalMatchResults.at(signalPair.second).Nodes.getNodeAs<CXXOperatorCallExpr>("connectPorts");

  	assert(opcall1 && opcall2);

  	SourceManager& sourceManager=*mSignalMatchResults.at(signalPair.second).SourceManager;

  	Rewriter rewriter(sourceManager, LangOptions());
  	rewriter.ReplaceText(opcall1->getArg(1)->getSourceRange(),
  			signalPair.second);
  	rewriter.ReplaceText(opcall2->getArg(1)->getSourceRange(),
  			signalPair.first);
  	rewriter.getEditBuffer(sourceManager.getFileID(opcall1->getLocStart()))
  			.write(streamProvider.get_next_mutant_stream(
  					sourceManager.getFilename(opcall1->getLocStart()),MutationType::switchPorts));
  }

  SignalTypeToSignalName mSignals;
  typedef std::map<std::string, MatchFinder::MatchResult> SignalToMatchResult;
  SignalToMatchResult mSignalMatchResults;
};



namespace SwitchPortMutations {

SwitchPortsCallback switchPortsCallback;

void addMatcher(clang::ast_matchers::MatchFinder& finder) {
	finder.addMatcher(connectPortMatcher,&switchPortsCallback);
}

void complete() {
	switchPortsCallback.complete();
}

}
