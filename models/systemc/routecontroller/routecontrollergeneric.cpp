/*
 * routecontrollergeneric.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: felixh
 */

#include "routecontrollergeneric.h"

const int route_controller_generic::BOTTOM=-1;
const int route_controller_generic::PLUS=0;
const int route_controller_generic::MINUS=1;
const int route_controller_generic::HALT=0;
const int route_controller_generic::PASS=1;
const int route_controller_generic::REQUEST=1;
const int route_controller_generic::CANCEL=-1;

route_controller_generic::route_controller_generic(sc_module_name n, const route_info_t& route) :
	sc_module(n),
	loc(FREE),
	front(-1),
	rear(-1),
	route_info(route)
{
}

void route_controller_generic::reset() {
	loc = FREE;
	enterFree();
}

void route_controller_generic::update() {
	switch (loc) {
	case FREE:
		updateFree();
		//			next_trigger(in_sut_cmd().value_changed_event());
		break;
	case MARKED:
		updateMarked();
		//			next_trigger(in_sut_cmd().value_changed_event());
		break;
	case ALLOCATING:
		updateAllocating();
		//			next_trigger(in_sut_cmd().value_changed_event());
		break;
	case LOCKED:
		updateLocked();
		//			next_trigger(in_sut_cmd().value_changed_event());
		break;
	case OCCUPIED:
		updateOccupied();
		//			next_trigger(in_sut_cmd().value_changed_event());
		break;
	case ERROR:
		break;
	}
	out_loc() = loc;
}

void route_controller_generic::reset_outputs_to_defaults() {
	resetPathElementLocksToDefaults();
	for (const auto& point : route_info.path_points_requested_pos) {
		out_point_cmd(point.first) = BOTTOM;
	}
	for (const auto& point : route_info.protecting_points_requested_pos) {
		out_point_cmd(point.first) = BOTTOM;
	}
	for (const auto& signal : route_info.protecting_signals) {
		out_signal_cmd(signal) = BOTTOM;
	}
	out_signal_cmd(route_info.start_signal) = BOTTOM;
	out_signal_cmd(route_info.dest_signal) = BOTTOM;
	//Bug Sysc1: reset of error was missing
	out_error() = false;
	//Bug Sysc2: reset of mylocaion was missing
	out_loc() = FREE;
}

void route_controller_generic::updateFree() {
	if (requested()) {
		loc = MARKED;
		updateMarked();
	}
}

void route_controller_generic::updateMarked() {
	if (cancel()) {
		loc = FREE;
		enterFree();
	} else if (no_conflicts()) {
		loc = ALLOCATING;
		enterAllocating();
		updateAllocating();
	}
}

void route_controller_generic::updateAllocating() {
	if (cancel()) {
		loc = FREE;
		enterFree();
	} else if (allElementsInCorrectPos()) {
		loc = LOCKED;
		enterLocked();
		updateLocked();
	}
}

void route_controller_generic::updateLocked() {
	if (cancel()) {
		//bug sysc3: loc=FREE was missing in marked, allocation and locked after cancel()
		loc = FREE;
		exitLocked();
		enterFree();
		updateFree();
	} else if (safetyViolation()) {
		loc = ERROR;
		//Bug 2.2: forgot to call exitLocked() here
		exitLocked();
		error();
	} else if (nextOccupied()) {
		//bug sysc4: loc=OCCUPIED; was missing
		loc = OCCUPIED;
		exitLocked();
		front = 0;
		rear = 0;
		enterOccupied();
		updateOccupied();
	}
}

bool route_controller_generic::no_conflicts() {
	for (const auto& pe : route_info.path_elements) {
		if (in_locked(pe)) {
			return false;
		}
		//Bug 1: forgot this for loop
		if (in_occupancy(pe)) {
			return false;
		}
	}
	for (const auto& confl_route : route_info.conflicting_routes) {
		if (in_stat_conf_route(confl_route) >= 2
				&& in_stat_conf_route(confl_route) <= 3) {
			return false;
		}
	}
	for (const auto& protecting_point : route_info.protecting_points_requested_pos) {
		//conflict if protecting point is locked and in wrong position
		if (in_locked(protecting_point.first)
				&& !pointIsInCorrectPosition(protecting_point.first)) {
			return false;
		}
	}
	return true;
}

bool route_controller_generic::allElementsInCorrectPos() {
	for (const auto point : route_info.path_points_requested_pos) {
		if (!pointIsInCorrectPosition(point.first)) {
			return false;
		}
	}
	for (const auto point : route_info.protecting_points_requested_pos) {
		if (!pointIsInCorrectPosition(point.first)) {
			return false;
		}
	}
	return allProtectingSignalsOff();
}

bool route_controller_generic::safetyViolation() {
	//Bug M3: in generated model instead of l (route.getTrackElements())
	//trackElement.length() (length of the name of every track element =3 was used)
	for (int i = front + 2; i <= route_info.path_elements.size() - 1; i++) {
		if (in_occupancy(route_info.path_elements.at(i)) == true) {
			return true;
		}
	}
	for (int i = rear + 1; i <= front; i++) {
		if (in_occupancy(route_info.path_elements.at(i)) == false) {
			return true;
		}
	}
	//Bug M1: in model "&& rear>=0 && rear<l-1" was missing, resulting in safety violations, when last element became vacant
	if (rear == front && rear >= 0 && rear < route_info.path_elements.size() - 1
			&& in_occupancy(route_info.path_elements.at(rear)) == false) {
		return true;
	}
	for (const auto& pathPoint : route_info.path_points_requested_pos) {
		if (released_elements.count(pathPoint.first) == 0
				&& in_stat_point(pathPoint.first) != pathPoint.second) {
			return true;
		}
	}
	return false;
}

void route_controller_generic::error() {
	failSafe();
}

void route_controller_generic::enterFree() {
	released_elements.clear();
	resetPathElementLocksToDefaults();
	//Bug 3.1: call to resetOutputsToDefaults(); missing
	reset_outputs_to_defaults();
}

void route_controller_generic::resetPathElementLocksToDefaults() {
	for (unsigned i = 0; i < route_info.path_elements.size(); i++) {
		out_locked(route_info.path_elements.at(i)) = false;
	}
}

void route_controller_generic::enterAllocating() {
	for (const auto& elem : route_info.path_elements) {
		out_locked(elem) = true;
	}
	for (const auto& point : route_info.path_points_requested_pos) {
		out_point_cmd(point.first) = point.second;
	}
	for (const auto& point : route_info.protecting_points_requested_pos) {
		out_point_cmd(point.first) = point.second;
	}
	for (const auto& signal : route_info.protecting_signals) {
		out_signal_cmd(signal) = HALT;
	}
}

void route_controller_generic::enterLocked() {
	out_signal_cmd(route_info.start_signal) = PASS;
	front = -1;
	rear = -1;
}

void route_controller_generic::enterOccupied() {
}

bool route_controller_generic::nextOccupied() {
	//Bug 6: this check was missing
	if (front + 1 >= route_info.path_elements.size()) {
		return false;
	}
	auto nextTe = route_info.path_elements.at(front + 1);
	return in_occupancy(nextTe);
}

bool route_controller_generic::nextVacant() {
	auto nextTe = route_info.path_elements.at(rear);
	return in_occupancy(nextTe) == false;
}

bool route_controller_generic::pointIsInCorrectPosition(
		const std::string& point) {
	if (route_info.path_points_requested_pos.count(point)) {
		return in_stat_point(point)
				== route_info.path_points_requested_pos.at(point);
	}
	return in_stat_point(point)
			== route_info.protecting_points_requested_pos.at(point);
}

bool route_controller_generic::allProtectingSignalsOff() {
	for (const auto& signal : route_info.protecting_signals) {
		if (in_stat_prot_mb(signal) != HALT) {
			return false;
		}
	}
	return true;
}

void route_controller_generic::exitLocked() {
	out_signal_cmd(route_info.start_signal) = HALT;
	//Bug 2.1: forgot to implement releaseProtection in this exit action
	releaseProtection();
}

bool route_controller_generic::requested() {
	return in_sut_cmd() == 1;
}

bool route_controller_generic::cancel() {
	return in_sut_cmd() == -1;
}

void route_controller_generic::releaseProtection() {
	for (const auto& point : route_info.protecting_points_requested_pos) {
		out_point_cmd(point.first) = BOTTOM;
	}
	for (const auto& signal : route_info.protecting_signals) {
		out_signal_cmd(signal) = BOTTOM;
	}
}

void route_controller_generic::failSafe() {
	for (const auto& signal : route_info.protecting_signals) {
		out_signal_cmd(signal) = HALT;
	}
	out_signal_cmd(route_info.start_signal) = HALT;
	out_signal_cmd(route_info.dest_signal) = HALT;
	out_error() = true;
}

void route_controller_generic::sequentialRelease() {
	auto to_release = route_info.path_elements.at(rear);
	released_elements.insert(to_release);
	out_locked(to_release) = false;
	//bug 5: reset of point command was not implemented
	if (route_info.path_points_requested_pos.count(to_release)) {
		out_point_cmd(to_release) = BOTTOM;
	}
}

void route_controller_generic::updateOccupied() {
	if (safetyViolation()) {
		loc = ERROR;
		error();
		return;
	} else if (nextOccupied()) {
		front = front + 1;
	}

	if (rear < route_info.path_elements.size() - 1 && nextVacant()) {
		//bug 4: used if else if structure, nextVacant was not checked if nextOccupied was true
		sequentialRelease();
		rear = rear + 1;
	} else if (rear >= route_info.path_elements.size() - 1 && nextVacant()) {
		sequentialRelease();
		loc = FREE;
		enterFree();
		//Bug 7: updateFree() was missing
		updateFree();
		//Bug 3.2: resetOutputsToDefaults(); was called here (additionally to enterFree()), before it moved inside enterFree()
	}
}
