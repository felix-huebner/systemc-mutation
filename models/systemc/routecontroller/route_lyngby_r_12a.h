#ifndef ROUTE_LYNGBY_R_12A_H
#define ROUTE_LYNGBY_R_12A_H

#include <systemc.h>

#include "routecontrollergeneric.h"


class route_lyngby_r_12a : public route_controller_generic {
public:
	//inputs
	sc_in<signed int> mb20_act;
	sc_in<signed int> mb31_act;
	sc_in<signed int> mb32_act;
	sc_in<signed int> r_01_;
	sc_in<signed int> r_02a;
	sc_in<signed int> r_02b;
	sc_in<signed int> r_05_;
	sc_in<signed int> r_06a;
	sc_in<signed int> r_06b;
	sc_in<signed int> r_07a;
	sc_in<signed int> r_07b;
	sc_in<signed int> r_08a;
	sc_in<signed int> r_08b;
	sc_in<signed int> r_09a;
	sc_in<signed int> r_09b;
	sc_in<signed int> r_10a;
	sc_in<signed int> r_10b;
	sc_in<signed int> r_11_;
	sc_in<signed int> r_12b;
	sc_in<signed int> r_13_;
	sc_in<signed int> r_16a;
	sc_in<signed int> r_16b;
	sc_in<signed int> sutCmd;
	sc_in<bool> t11;
	sc_in<signed int> t11_pos;
	sc_in<bool> t13;
	sc_in<signed int> t13_pos;
	sc_in<bool> t20;
	sc_in<bool> t20_occ;
	sc_in<signed int> t20_pos;
	sc_in<bool> t21;
	sc_in<bool> t21_occ;
	sc_in<bool> t30;
	sc_in<bool> t30_occ;
	sc_in<bool> t31;
	sc_in<bool> t31_occ;
	sc_in<signed int> t31_pos;
	sc_in<bool> t33;
	sc_in<signed int> t33_pos;

	//outputs
	sc_out<bool> error;
	sc_out<signed int> mb20_cmd;
	sc_out<signed int> mb21_cmd;
	sc_out<signed int> mb30_cmd;
	sc_out<signed int> mb31_cmd;
	sc_out<signed int> mb32_cmd;
	sc_out<signed int> mylocation;
	sc_out<signed int> t11_cmd;
	sc_out<signed int> t13_cmd;
	sc_out<signed int> t20_cmd;
	sc_out<bool> t20_out;
	sc_out<bool> t21_out;
	sc_out<bool> t30_out;
	sc_out<signed int> t31_cmd;
	sc_out<bool> t31_out;
	sc_out<signed int> t33_cmd;

	route_lyngby_r_12a(sc_module_name n);

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
