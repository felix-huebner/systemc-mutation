#include <memory>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/Lexer.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Refactoring.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/Signals.h"


#include "clang/Rewrite/Core/Rewriter.h"

#include "byte_order_mutation.h"
#include "mem_access_mutation.h"
#include "mutant_stream_provider.h"
#include "precision_mutation.h"
#include "standard_mutation.h"
#include "systemc_mutation.h"
#include "switch_ports_mutation.h"
#include "stuck_at_mutation.h"

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::tooling;
using namespace llvm;


// Set up the command line options
static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
static cl::OptionCategory mutateCategory("mutate++ options");
static cl::opt<std::string> mutationDir("mutation-dir",cl::cat(mutateCategory),cl::desc("defines the directory, where all mutations are stored to"));

int main(int argc, const char **argv) {

	mutationDir.Default="./";

  llvm::sys::PrintStackTraceOnErrorSignal(argv[0]);
  CommonOptionsParser optionsParser(argc, argv, mutateCategory);
  RefactoringTool tool(optionsParser.getCompilations(),
                       optionsParser.getSourcePathList());
  ast_matchers::MatchFinder finder;
  std::cout<<"mutation-dir: "<<mutationDir.getValue()<<std::endl;
  MutantStreamProvider::set_mutation_dir(mutationDir.getValue());

  StandardMutations::addMatcher(finder);
  ByteOrderMutations::addMatcher(finder);
  //MemAccessMutations::addMatcher(finder);
  PrecisionMutations::addMatcher(finder);
  SystemCMutations::addMatcher(finder);
  SwitchPortMutations::addMatcher(finder);
  StuckAtMutations::addMatcher(finder);

  int result = tool.run(newFrontendActionFactory(&finder).get());

  SwitchPortMutations::complete();

  return result;
}

