#include "route_lyngby_r_12a.h"
#include "routecontrollergeneric.h"

//"Lyngby_r_12a,mb30,mb21,t30;t31;t20;t21,,t11:p;t13:p;t20:p;t31:m;t33:p,mb20;mb31;mb32,,r_01_;r_02a;r_02b;r_05_;r_06a;r_06b;r_07a;r_07b;r_08a;r_08b;r_09a;r_09b;r_10a;r_10b;r_11_;r_12b;r_13_;r_16a;r_16b"));
route_info_t lyngby_route_12a{
	{"t30","t31","t20","t21"},//path elements
	{{"t31",1},{"t20",0}},	//std::map<std::string,int> path_points_requested_pos;
	{{"t11",0},{"t13",0},{"t33",0}},	//std::map<std::string,int> protecting_points_requested_pos;
	{"mb20","mb31","mb32"},	//std::vector<std::string> protecting_signals;
	{"r_01_","r_02a","r_02b","r_05_","r_06a","r_06b","r_07a","r_07b","r_08a","r_08b","r_09a","r_09b","r_10a","r_10b","r_11_","r_12b","r_13_","r_16a","r_16b"},	//std::vector<std::string> conflicting_routes
	"mb30",	//std::string start_signal;
	"mb21"	//std::string dest_signal;
};

SC_HAS_PROCESS(route_lyngby_r_12a);
route_lyngby_r_12a::route_lyngby_r_12a(sc_module_name n) :
	route_controller_generic(n,route_info_t{
		{"t30","t31","t20","t21"},//path elements
		{{"t31",1},{"t20",0}},	//std::map<std::string,int> path_points_requested_pos;
		{{"t11",0},{"t13",0},{"t33",0}},	//std::map<std::string,int> protecting_points_requested_pos;
		{"mb20","mb31","mb32"},	//std::vector<std::string> protecting_signals;
		{"r_01_","r_02a","r_02b","r_05_","r_06a","r_06b","r_07a","r_07b","r_08a","r_08b","r_09a","r_09b","r_10a","r_10b","r_11_","r_12b","r_13_","r_16a","r_16b"},	//std::vector<std::string> conflicting_routes
		"mb30",	//std::string start_signal;
		"mb21"	//std::string dest_signal;
	})
{
	SC_METHOD(update);
		sensitive << mb20_act;
		sensitive << mb31_act;
		sensitive << mb32_act;
		sensitive << r_01_;
		sensitive << r_02a;
		sensitive << r_02b;
		sensitive << r_05_;
		sensitive << r_06a;
		sensitive << r_06b;
		sensitive << r_07a;
		sensitive << r_07b;
		sensitive << r_08a;
		sensitive << r_08b;
		sensitive << r_09a;
		sensitive << r_09b;
		sensitive << r_10a;
		sensitive << r_10b;
		sensitive << r_11_;
		sensitive << r_12b;
		sensitive << r_13_;
		sensitive << r_16a;
		sensitive << r_16b;
		sensitive << sutCmd;
		sensitive << t11;
		sensitive << t11_pos;
		sensitive << t13;
		sensitive << t13_pos;
		sensitive << t20;
		sensitive << t20_occ;
		sensitive << t20_pos;
		sensitive << t21;
		sensitive << t21_occ;
		sensitive << t30;
		sensitive << t30_occ;
		sensitive << t31;
		sensitive << t31_occ;
		sensitive << t31_pos;
		sensitive << t33;
		sensitive << t33_pos;
		;
}

sc_in<signed int>& route_lyngby_r_12a::in_sut_cmd() {
	return sutCmd;
}

sc_in<bool>& route_lyngby_r_12a::in_locked(const std::string& e) {
	if(e=="t30") {
		return t30;
	} else if(e=="t31") {
		return t31;
	} else if(e=="t20") {
		return t20;
	} else if(e=="t21") {
		return t21;
	} else if(e=="t11") {
		return t11;
	} else if(e=="t13") {
		return t13;
	} else if(e=="t33") {
		return t33;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_lyngby_r_12a::in_stat_point(const std::string& e) {
	if(e=="t31") {
		return t31_pos;
	} else if(e=="t20") {
		return t20_pos;
	} else if(e=="t11") {
		return t11_pos;
	} else if(e=="t13") {
		return t13_pos;
	} else if(e=="t33") {
		return t33_pos;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_lyngby_r_12a::in_stat_prot_mb(
		const std::string& e) {
	if(e=="mb20") {
		return mb20_act;
	} else if(e=="mb31") {
		return mb31_act;
	} else if(e=="mb32") {
		return mb32_act;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_lyngby_r_12a::in_stat_conf_route(const std::string& r) {
	if(r=="r_01_") {
		return r_01_;
	} else if(r=="r_02a") {
		return r_02a;
	} else if(r=="r_02b") {
		return r_02b;
	} else if(r=="r_05_") {
		return r_05_;
	} else if(r=="r_06a") {
		return r_06a;
	} else if(r=="r_06b") {
		return r_06b;
	} else if(r=="r_07a") {
		return r_07a;
	} else if(r=="r_07b") {
		return r_07b;
	} else if(r=="r_08a") {
		return r_08a;
	} else if(r=="r_08b") {
		return r_08b;
	} else if(r=="r_09a") {
		return r_09a;
	} else if(r=="r_09b") {
		return r_09b;
	} else if(r=="r_10a") {
		return r_10a;
	} else if(r=="r_10b") {
		return r_10b;
	} else if(r=="r_11_") {
		return r_11_;
	} else if(r=="r_12b") {
		return r_12b;
	} else if(r=="r_13_") {
		return r_13_;
	} else if(r=="r_16a") {
		return r_16a;
	} else if(r=="r_16b") {
		return r_16b;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<bool>& route_lyngby_r_12a::in_occupancy(const std::string& e) {
	if(e=="t30") {
		return t30_occ;
	} else if(e=="t31") {
		return t31_occ;
	} else if(e=="t20") {
		return t20_occ;
	} else if(e=="t21") {
		return t21_occ;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<bool>& route_lyngby_r_12a::out_locked(const std::string& e) {
	if(e=="t30") {
		return t30_out;
	} else if(e=="t31") {
		return t31_out;
	} else if(e=="t20") {
		return t20_out;
	} else if(e=="t21") {
		return t21_out;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_lyngby_r_12a::out_point_cmd(
		const std::string& e) {
	if(e=="t31") {
		return t31_cmd;
	} else if(e=="t20") {
		return t20_cmd;
	} else if(e=="t11") {
		return t11_cmd;
	} else if(e=="t13") {
		return t13_cmd;
	} else if(e=="t33") {
		return t33_cmd;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_lyngby_r_12a::out_signal_cmd(
		const std::string& e) {
	if(e=="mb20") {
		return mb20_cmd;
	} else if(e=="mb31") {
		return mb31_cmd;
	} else if(e=="mb32") {
		return mb32_cmd;
	} else if(e=="mb30") {
		return mb30_cmd;
	} else if(e=="mb21") {
		return mb21_cmd;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_lyngby_r_12a::out_loc() {
	return mylocation;
}

sc_out<bool>& route_lyngby_r_12a::out_error() {
	return error;
}
