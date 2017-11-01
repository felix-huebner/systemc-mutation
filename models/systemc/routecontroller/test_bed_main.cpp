#include <systemc.h>

#include "route_lyngby_r_12a.h"
#include "test_bed_main.h"
#include "fisignal.h"

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

route_lyngby_r_12a sut("route_lyngby_r_12a");

//create signals for inputs
fi_signal<signed int> mb20_act;
fi_signal<signed int> mb31_act;
fi_signal<signed int> mb32_act;
fi_signal<signed int> r_01_;
fi_signal<signed int> r_02a;
fi_signal<signed int> r_02b;
fi_signal<signed int> r_05_;
fi_signal<signed int> r_06a;
fi_signal<signed int> r_06b;
fi_signal<signed int> r_07a;
fi_signal<signed int> r_07b;
fi_signal<signed int> r_08a;
fi_signal<signed int> r_08b;
fi_signal<signed int> r_09a;
fi_signal<signed int> r_09b;
fi_signal<signed int> r_10a;
fi_signal<signed int> r_10b;
fi_signal<signed int> r_11_;
fi_signal<signed int> r_12b;
fi_signal<signed int> r_13_;
fi_signal<signed int> r_16a;
fi_signal<signed int> r_16b;
fi_signal<signed int> sutCmd;
fi_signal<bool> t11;
fi_signal<signed int> t11_pos;
fi_signal<bool> t13;
fi_signal<signed int> t13_pos;
fi_signal<bool> t20;
fi_signal<bool> t20_occ;
fi_signal<signed int> t20_pos;
fi_signal<bool> t21;
fi_signal<bool> t21_occ;
fi_signal<bool> t30;
fi_signal<bool> t30_occ;
fi_signal<bool> t31;
fi_signal<bool> t31_occ;
fi_signal<signed int> t31_pos;
fi_signal<bool> t33;
fi_signal<signed int> t33_pos;

//create signals for outputs
fi_signal<bool> error;
fi_signal<signed int> mb20_cmd;
fi_signal<signed int> mb21_cmd;
fi_signal<signed int> mb30_cmd;
fi_signal<signed int> mb31_cmd;
fi_signal<signed int> mb32_cmd;
fi_signal<signed int> mylocation;
fi_signal<signed int> t11_cmd;
fi_signal<signed int> t13_cmd;
fi_signal<signed int> t20_cmd;
fi_signal<bool> t20_out;
fi_signal<bool> t21_out;
fi_signal<bool> t30_out;
fi_signal<signed int> t31_cmd;
fi_signal<bool> t31_out;
fi_signal<signed int> t33_cmd;

int sc_main(int argc, char* argv[]) {

	//connect input signals to input channels
	sut.mb20_act(mb20_act);
	sut.mb31_act(mb31_act);
	sut.mb32_act(mb32_act);
	sut.r_01_(r_01_);
	sut.r_02a(r_02a);
	sut.r_02b(r_02b);
	sut.r_05_(r_05_);
	sut.r_06a(r_06a);
	sut.r_06b(r_06b);
	sut.r_07a(r_07a);
	sut.r_07b(r_07b);
	sut.r_08a(r_08a);
	sut.r_08b(r_08b);
	sut.r_09a(r_09a);
	sut.r_09b(r_09b);
	sut.r_10a(r_10a);
	sut.r_10b(r_10b);
	sut.r_11_(r_11_);
	sut.r_12b(r_12b);
	sut.r_13_(r_13_);
	sut.r_16a(r_16a);
	sut.r_16b(r_16b);
	sut.sutCmd(sutCmd);
	sut.t11(t11);
	sut.t11_pos(t11_pos);
	sut.t13(t13);
	sut.t13_pos(t13_pos);
	sut.t20(t20);
	sut.t20_occ(t20_occ);
	sut.t20_pos(t20_pos);
	sut.t21(t21);
	sut.t21_occ(t21_occ);
	sut.t30(t30);
	sut.t30_occ(t30_occ);
	sut.t31(t31);
	sut.t31_occ(t31_occ);
	sut.t31_pos(t31_pos);
	sut.t33(t33);
	sut.t33_pos(t33_pos);

	//connect output signals to output channels
	sut.error(error);
	sut.mb20_cmd(mb20_cmd);
	sut.mb21_cmd(mb21_cmd);
	sut.mb30_cmd(mb30_cmd);
	sut.mb31_cmd(mb31_cmd);
	sut.mb32_cmd(mb32_cmd);
	sut.mylocation(mylocation);
	sut.t11_cmd(t11_cmd);
	sut.t13_cmd(t13_cmd);
	sut.t20_cmd(t20_cmd);
	sut.t20_out(t20_out);
	sut.t21_out(t21_out);
	sut.t30_out(t30_out);
	sut.t31_cmd(t31_cmd);
	sut.t31_out(t31_out);
	sut.t33_cmd(t33_cmd);

	int result = Catch::Session().run(argc,argv);
	return result;
}

