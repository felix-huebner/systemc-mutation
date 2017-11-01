#include <systemc.h>

#include "ceiling_speed_monitor.h"
#include "test_bed_main.h"
#include "fisignal.h"

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

ceiling_speed_monitor sut("ceiling_speed_monitor");

//create signals for inputs
fi_signal<float> V_est;
fi_signal<float> V_mrsp;

//create signals for outputs
fi_signal<signed int> DMICmd;
fi_signal<bool> DMIdisplaySBI;
fi_signal<signed int> TICmd;

int sc_main(int argc, char* argv[]) {

	//connect input signals to input channels
	sut.V_est(V_est);
	sut.V_mrsp(V_mrsp);

	//connect output signals to output channels
	sut.DMICmd(DMICmd);
	sut.DMIdisplaySBI(DMIdisplaySBI);
	sut.TICmd(TICmd);

	int result = Catch::Session().run(argc,argv);
	return result;
}

