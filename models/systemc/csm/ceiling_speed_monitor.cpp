#include "ceiling_speed_monitor.h"


void output_ctrl::update() {
	switch (location) {
	case 0:
		DMICmd = 0;
		DMIdisplaySBI = 0;
		TICmd = 0;
		break;
	case 1:
		DMICmd = 2;
		DMIdisplaySBI = 1;
		TICmd = 0;
		break;
	case 2:
		DMICmd = 3;
		DMIdisplaySBI = 1;
		TICmd = 0;
		break;
	case 3:
		DMICmd = 4;
		DMIdisplaySBI = 1;
		TICmd = 1;
		break;
	case 4:
		DMICmd = 4;
		DMIdisplaySBI = 1;
		TICmd = 2;
		break;
	}
}

output_ctrl::output_ctrl(::sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<location;
}

ceiling_speed_monitor::ceiling_speed_monitor( ::sc_core::sc_module_name ) :
	output_controller("output_controller")
{
	location=0;
	SC_METHOD(update);
	sensitive<<V_est;
	sensitive<<V_mrsp;

	output_controller.location(location);
	output_controller.DMICmd(DMICmd);
	output_controller.DMIdisplaySBI(DMIdisplaySBI);
	output_controller.TICmd(TICmd);
}

void ceiling_speed_monitor::update() {
	switch (location) {
	case 0:
		updateL0();
		break;
	case 1:
		updateL1();
		break;
	case 2:
		updateL2();
		break;
	case 3:
		updateL3();
		break;
	case 4:
		updateL4();
		break;
	}
}

void ceiling_speed_monitor::reset() {
	location=0;
	//reset outputs to default values
	DMICmd=0;
	DMIdisplaySBI=0;
	TICmd=0;
}

void ceiling_speed_monitor::updateL0() {
	if (V_est > V_mrsp) {
		location = 1;
		updateL1();
	}
}

void ceiling_speed_monitor::updateL1() {
	if (V_est <= V_mrsp) {
		location = 0;
		updateL0();
	}

	if (V_est > V_mrsp + dV_warning(V_mrsp)) {
		location = 2;
		updateL2();
	}
}

void ceiling_speed_monitor::updateL2() {
	if (V_est <= V_mrsp) {
		location = 0;
		updateL0();
	}

	if (V_est > V_mrsp + dV_sbi(V_mrsp)) {
		location = 3;
		updateL3();
	}
}

void ceiling_speed_monitor::updateL3() {
	if (V_est <= V_mrsp) {
		location = 0;
		updateL0();
	}

	if (V_est > V_mrsp + dV_ebi(V_mrsp)) {
		location = 4;
		updateL4();
	}
}

void ceiling_speed_monitor::updateL4() {
	if (V_est == 0) {
		location = 0;
		updateL0();
	}
}

float ceiling_speed_monitor::dV_warning(float v) {
	if (v <= 110) {
		return 4.0f;
	} else if (v <= 140) {
		return v / 30.0f + 1.0f / 3.0f;
	} else {
		return 5;
	}
}

float ceiling_speed_monitor::dV_sbi(float v) {
	if (v <= 110) {
		return 5.5f;
	} else if (v <= 210) {
		return 0.045f * v + 0.55f;
	} else {
		return 10.0f;
	}
}

float ceiling_speed_monitor::dV_ebi(float v) {
	if (v <= 110) {
		return 7.5f;
	} else if (v <= 210) {
		return 0.075f * v - 0.75f;
	} else {
		return 15.0f;
	}
}
