#ifndef ROUTE_2011_H
#define ROUTE_2011_H

#include <systemc.h>

#include "routecontrollergeneric.h"


class route_2011 : public route_controller_generic {
public:
	//inputs
	sc_in<signed int> mb10_act;
	sc_in<signed int> mb12_act;
	sc_in<signed int> route1013;
	sc_in<signed int> route1021;
	sc_in<signed int> route1211;
	sc_in<signed int> sutCmd;
	sc_in<bool> t11;
	sc_in<bool> t11_occ;
	sc_in<bool> t10;
	sc_in<bool> t10_occ;
	sc_in<int> t11_pos;

	//outputs
	sc_out<bool> error;
	sc_out<signed int> mb10_cmd;
	sc_out<signed int> mb12_cmd;
	sc_out<signed int> mb20_cmd;
	sc_out<signed int> mb11_cmd;
	sc_out<signed int> mylocation;
	sc_out<signed int> t11_cmd;
	sc_out<bool> t11_out;
	sc_out<bool> t10_out;

	route_2011(sc_module_name n);

	virtual sc_in<signed int>& in_sut_cmd();
	//locking status of sections (locked by others)
	virtual sc_in<bool>& in_locked(const std::string& e);
	//stati of points
	virtual sc_in<int >& in_stat_point(const std::string& e);
	//stati of protecting signals
	virtual sc_in<int >& in_stat_prot_mb(const std::string& e);
	//stati of conflicting routes
	virtual sc_in<int>& in_stat_conf_route(const std::string& r);
	//occupancy stati
	virtual sc_in<bool>& in_occupancy(const std::string& e);

	/*
	 * system outputs
	 */
	//locking stati of elements
	virtual sc_out<bool>& out_locked(const std::string& e);
	//commands to points
	virtual sc_out<int>& out_point_cmd(const std::string& e);
	//commands to signals
	virtual sc_out<int>& out_signal_cmd(const std::string& e);
	//state of this route
	virtual sc_out<int>& out_loc();
	//error flag
	virtual sc_out<bool>& out_error();
};

#endif // ROUTE_LYNGBY_R_12A_H
