#ifndef MutantStreamProvider_H
#define MutantStreamProvider_H

#include <memory>

#include "llvm/Support/raw_os_ostream.h"

#include "mutation_types.h"


class MutantStreamProvider {
public:
	MutantStreamProvider() : mutant_no(0) {}

	llvm::raw_ostream& get_next_mutant_stream(const std::string& filename, MutationType::MutationType type);

	static void set_mutation_dir(const std::string& dir);
private:
	static std::string sMutationDir;
	unsigned int mutant_no=0;
	std::unique_ptr<std::ostream> mStream;
	std::unique_ptr<llvm::raw_os_ostream> mRawStream;
};

extern MutantStreamProvider streamProvider;

#endif //MutantStreamProvider_H
