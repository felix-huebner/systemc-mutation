#ifndef AIRBAG_H
#define AIRBAG_H

#include <systemc.h>

SC_MODULE(plausibility_checker) {
	sc_in<float> s1;
	sc_in<float> s2;
	sc_in<int> t;

	sc_out<bool> plausible;
	sc_out<int> plausible_ctr;

	int t_old;

	SC_CTOR(plausibility_checker);

	void update();

	void reset();
};

SC_MODULE(error_detection) {
	sc_in<bool> plausible;
	sc_in<int> plausible_ctr;
	sc_in<int> t;

	sc_out<int> error_ctr;

	SC_CTOR(error_detection);

	void update();

	void reset();
};

SC_MODULE(crash_detection) {
	sc_in<bool> plausible;
	sc_in<int> t;
	sc_in<float> s1;
	sc_in<float> s2;

	sc_out<int> crash_ctr;

	SC_CTOR(crash_detection);

	void update();

	void reset();
};

SC_MODULE(defect_indication) {
	sc_in<int> error_ctr;
	sc_in<int> fire;

	sc_out<int> defect;

	SC_CTOR(defect_indication);

	void update();

	void reset();
};

SC_MODULE(crash_indication) {
	sc_in<int> crash_ctr;
	sc_in<int> defect;
	sc_out<int> fire;

	SC_CTOR(crash_indication);

	void update();

	void reset();
};

SC_MODULE(airbag) {
	//inputs
	sc_in<float> s1;
	sc_in<float> s2;
	sc_in<int> t;

	//outputs
	sc_out<int> defect;
	sc_out<int> fire;

	//internals
	//int location;
	sc_signal<bool,sc_core::SC_MANY_WRITERS> plausible;
	sc_signal<int,sc_core::SC_MANY_WRITERS> plausible_ctr;
	sc_signal<int,sc_core::SC_MANY_WRITERS> error_ctr;
	sc_signal<int,sc_core::SC_MANY_WRITERS> crash_ctr;

	plausibility_checker plausible_check;
	error_detection error_det;
	crash_detection crash_det;
	defect_indication defect_ind;
	crash_indication crash_ind;

	SC_CTOR(airbag);

	void update();

	void reset();

};

#endif // AIRBAG_H
