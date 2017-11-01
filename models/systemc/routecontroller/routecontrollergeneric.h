/*
 * routecontrollergeneric.h
 *
 *  Created on: Mar 14, 2017
 *      Author: felixh
 */

#ifndef ROUTECONTROLLERGENERIC_H_
#define ROUTECONTROLLERGENERIC_H_

#include <map>
#include <set>

#include <systemc.h>

struct route_info_t {
	std::vector<std::string> path_elements;
	std::map<std::string,int> path_points_requested_pos;
	std::map<std::string,int> protecting_points_requested_pos;
	std::vector<std::string> protecting_signals;
	std::vector<std::string> conflicting_routes;
	std::string start_signal;
	std::string dest_signal;
};

class route_controller_generic : public sc_module {
public:
	typedef int signal_aspect_t;
	typedef int point_aspect_t;
	enum location_t {
		FREE=0,
		MARKED,
		ALLOCATING,
		LOCKED,
		OCCUPIED,
		ERROR
	} ;

	static const int BOTTOM;
	static const int PLUS;
	static const int MINUS;
	static const int HALT;
	static const int PASS;
	static const int REQUEST;
	static const int CANCEL;

	/*
	 * system inputs
	 */
	//sut command
	virtual sc_in<signed int>& in_sut_cmd()=0;
	//locking status of sections (locked by others)
	virtual sc_in<bool>& in_locked(const std::string& e)=0;
	//stati of points
	virtual sc_in<point_aspect_t >& in_stat_point(const std::string& e)=0;
	//stati of protecting signals
	virtual sc_in<signal_aspect_t >& in_stat_prot_mb(const std::string& e)=0;
	//stati of conflicting routes
	virtual sc_in<int>& in_stat_conf_route(const std::string& r)=0;
	//occupancy stati
	virtual sc_in<bool>& in_occupancy(const std::string& e)=0;

	/*
	 * system outputs
	 */
	//locking stati of elements
	virtual sc_out<bool>& out_locked(const std::string& e)=0;
	//commands to points
	virtual sc_out<point_aspect_t>& out_point_cmd(const std::string& e)=0;
	//commands to signals
	virtual sc_out<point_aspect_t>& out_signal_cmd(const std::string& e)=0;
	//state of this route
	virtual sc_out<int>& out_loc()=0;
	//error flag
	virtual sc_out<bool>& out_error()=0;

	/*
	 * internal: location
	 */
	location_t loc;
	int front,rear;
	route_info_t route_info;
	std::set<std::string> released_elements;


	route_controller_generic(sc_module_name n, const route_info_t& route);

	void reset();
	void update();

protected:
	void reset_outputs_to_defaults();

	void updateFree();

	void updateMarked();

	void updateAllocating();

	void updateLocked();

	void updateOccupied();

	void sequentialRelease();

	bool no_conflicts();

	bool allElementsInCorrectPos();

	bool safetyViolation();

	void error();

	void enterFree();

	void resetPathElementLocksToDefaults();

	void enterAllocating();

	void enterLocked();

	void enterOccupied();

	bool nextOccupied();

	bool nextVacant();

	bool pointIsInCorrectPosition(const std::string& point);

	bool allProtectingSignalsOff();

	void exitLocked();

	bool requested();

	bool cancel();

	void releaseProtection();

	void failSafe();

};

#endif /* ROUTECONTROLLERGENERIC_H_ */
