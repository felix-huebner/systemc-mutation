#include "mutant_stream_provider.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>


MutantStreamProvider streamProvider;

std::string MutantStreamProvider::sMutationDir="";

void MutantStreamProvider::set_mutation_dir(const std::string& dir) {
	sMutationDir=dir;
}

llvm::raw_ostream& MutantStreamProvider::get_next_mutant_stream(const std::string& filename, MutationType::MutationType type) {
	std::string name=filename;
	if(filename.find_last_of("/")!=std::string::npos) {
		name=filename.substr(filename.find_last_of("/")+1);
	}

	std::stringstream mutant_name;
	if(!sMutationDir.empty()) {
		mutant_name<<sMutationDir<<"/";
	}
	mutant_name<<name<<".mu"<<static_cast<char>(type)<<std::setfill('0')<<std::setw(4)<<mutant_no++;
	mRawStream.reset();
	mStream.reset(new std::ofstream(mutant_name.str().c_str()));
	mRawStream.reset(new llvm::raw_os_ostream(*mStream));
	if(!mStream->good()) {
		std::cerr<<"Cannot write to file "<<mutant_name.str()<<std::endl;
		abort();
	}
	return *mRawStream;
}
