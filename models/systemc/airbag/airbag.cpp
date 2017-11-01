#include "airbag.h"
#include <limits>

//reset event
sc_event reset_ev;
sc_event plausible_check_completed;

static int ERROR_THRESHOLD = 3;
static int PLAUSIBLE_THRESHOLD = 3;
static int CRASH_THRESHOLD = 3;

plausibility_checker::plausibility_checker(sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<t;
	SC_METHOD(reset);
	sensitive<<reset_ev;
	t_old=0;
}

void plausibility_checker::update() {
	if(t==t_old) {
		return;
	}
	t_old=t;
	if(s1 > s2*1.05) {
		plausible=false;
		plausible_ctr=0;
	} else if(s1<s2*0.95) {
		plausible=false;
		plausible_ctr=0;
	} else {
		plausible=true;
		if(plausible_ctr<std::numeric_limits<int>::max()) {//prevent overflow
			plausible_ctr.write(plausible_ctr.read()+1);
		}
	}
	plausible_check_completed.notify(1,SC_NS);
}

void plausibility_checker::reset() {
	plausible=true;
	plausible_ctr=0;
	t_old=0;
}

error_detection::error_detection(sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<plausible_check_completed;
	SC_METHOD(reset);
	sensitive<<reset_ev;
}

void error_detection::update() {
	if(plausible) {
		if(plausible_ctr>=PLAUSIBLE_THRESHOLD) {
			error_ctr=0;
		}
	} else {
		error_ctr.write(error_ctr.read()+1);
	}
}

void error_detection::reset() {
	error_ctr=0;
}

crash_detection::crash_detection(sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<plausible_check_completed;
	SC_METHOD(reset);
	sensitive<<reset_ev;
}

void crash_detection::update() {
	if(!plausible) {
		return;
	}
	if(s1<=3 || s2<=3) {
		crash_ctr=0;
	} else {
		crash_ctr.write(crash_ctr.read()+1);
	}
}

void crash_detection::reset() {
	crash_ctr=0;
}

defect_indication::defect_indication(sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<error_ctr;
	SC_METHOD(reset);
	sensitive<<reset_ev;
}

void defect_indication::update() {
	if(defect || fire) {
		return;
	} else if(error_ctr>=ERROR_THRESHOLD) {
		defect=1;
	}
}

void defect_indication::reset() {
	defect=0;
}

crash_indication::crash_indication(sc_core::sc_module_name) {
	SC_METHOD(update);
	sensitive<<crash_ctr;
	SC_METHOD(reset);
	sensitive<<reset_ev;
}

void crash_indication::update() {
	if(fire || defect) {
		return;
	}
	if(crash_ctr>=CRASH_THRESHOLD) {
		fire=1;
	}
}

void crash_indication::reset() {
	fire=0;
}

airbag::airbag( ::sc_core::sc_module_name ) :
	plausible_check("plausible_check"),
	error_det("error_det"),
	crash_det("crash_det"),
	defect_ind("defect_ind"),
	crash_ind("crash_ind")
{
	plausible_check.s1(s1);
	plausible_check.s2(s2);
	plausible_check.t(t);
	plausible_check.plausible(plausible);
	plausible_check.plausible_ctr(plausible_ctr);

	error_det.plausible(plausible);
	error_det.plausible_ctr(plausible_ctr);
	error_det.t(t);
	error_det.error_ctr(error_ctr);

	crash_det.plausible(plausible);
	crash_det.t(t);
	crash_det.s1(s1);
	crash_det.s2(s2);
	crash_det.crash_ctr(crash_ctr);

	defect_ind.error_ctr(error_ctr);
	defect_ind.fire(fire);
	defect_ind.defect(defect);

	crash_ind.crash_ctr(crash_ctr);
	crash_ind.defect(defect);
	crash_ind.fire(fire);

	SC_METHOD(update);
	sensitive<<s1;
	sensitive<<s2;
	sensitive<<t;

	plausible=true;
	plausible_ctr=0;
	error_ctr=0;
	crash_ctr=0;

}

void airbag::update() {
	//nothing to do, everything is handled in sub-modules
}

void airbag::reset() {
	sc_start(1,SC_NS);
	reset_ev.notify(1,SC_NS);
	sc_start(1,SC_NS);
}
