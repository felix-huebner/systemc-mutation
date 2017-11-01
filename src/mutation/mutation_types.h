#ifndef MUTATION_TYPES_H
#define MUTATION_TYPES_H

namespace MutationType {

enum MutationType {
	byteOrder = 'b',
	memAccess = 'm',
	ordinary = 'o',
	switchPorts = 'p',
	stuckAt = 's',
	sensitivity = 't',
	precision = 'f'
};

}

#endif //MUTATION_TYPES_H
