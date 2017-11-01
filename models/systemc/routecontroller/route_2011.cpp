#include "route_2011.h"
#include "routecontrollergeneric.h"

SC_HAS_PROCESS(route_2011);
route_2011::route_2011(sc_module_name n) :
	route_controller_generic(n,route_info_t{
		{"t11","t10"},//path elements
		{{"t11",1}},	//std::map<std::string,int> path_points_requested_pos;
		{},	//std::map<std::string,int> protecting_points_requested_pos;
		{"mb10","mb12"},	//std::vector<std::string> protecting_signals;
		{"route1013","route1021","route1211"},	//std::vector<std::string> conflicting_routes
		"mb20",	//std::string start_signal;
		"mb11"	//std::string dest_signal;
	})
{
	SC_METHOD(update);
	sensitive<<mb10_act;
	sensitive<<mb12_act;
	sensitive<<route1013;
	sensitive<<route1021;
	sensitive<<route1211;
	sensitive<<sutCmd;
	sensitive<<t11;
	sensitive<<t11_occ;
	sensitive<<t10;
	sensitive<<t10_occ;
	sensitive<<t11_pos;
}

sc_in<signed int>& route_2011::in_sut_cmd() {
	return sutCmd;
}

sc_in<bool>& route_2011::in_locked(const std::string& e) {
	if(e=="t11") {
		return t11;
	} else if(e=="t10") {
		return t10;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_2011::in_stat_point(const std::string& e) {
	if(e=="t11") {
		return t11_pos;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_2011::in_stat_prot_mb(
		const std::string& e) {
	if(e=="mb10") {
		return mb10_act;
	} else if(e=="mb12") {
		return mb12_act;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<int>& route_2011::in_stat_conf_route(const std::string& r) {
	if(r=="route1013") {
		return route1013;
	} else if(r=="route1021") {
		return route1021;
	} else if(r=="route1211") {
		return route1211;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_in<bool>& route_2011::in_occupancy(const std::string& e) {
	if(e=="t11") {
		return t11_occ;
	} else if(e=="t10") {
		return t10_occ;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<bool>& route_2011::out_locked(const std::string& e) {
	if(e=="t11") {
		return t11_out;
	} else if(e=="t10") {
		return t10_out;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_2011::out_point_cmd(
		const std::string& e) {
	if(e=="t11") {
		return t11_cmd;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_2011::out_signal_cmd(
		const std::string& e) {
	if(e=="mb10") {
		return mb10_cmd;
	} else if(e=="mb12") {
		return mb12_cmd;
	} else if(e=="mb20") {
		return mb20_cmd;
	} else if(e=="mb30") {
		return mb11_cmd;
	} else if(e=="mb11") {
		return mb11_cmd;
	} else {
		throw std::runtime_error("failed");
	}
}

sc_out<int>& route_2011::out_loc() {
	return mylocation;
}

sc_out<bool>& route_2011::out_error() {
	return error;
}
