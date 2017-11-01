#include <systemc.h>

#include "airbag.h"
#include "test_bed_main.h"
#include "fisignal.h"

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

airbag sut("airbag");

//create signals for inputs
fi_signal<float> s1;
fi_signal<float> s2;
fi_signal<signed int> t;

//create signals for outputs
fi_signal<signed int> defect;
fi_signal<signed int> fire;

int sc_main(int argc, char* argv[]) {

	//connect input signals to input channels
	sut.s1(s1);
	sut.s2(s2);
	sut.t(t);

	//connect output signals to output channels
	sut.defect(defect);
	sut.fire(fire);

	int result = Catch::Session().run(argc,argv);
	return result;
}

