#ifndef CEILING_SPEED_MONITOR_H
#define CEILING_SPEED_MONITOR_H

#include <systemc.h>

SC_MODULE(output_ctrl) {
	sc_in<int> location;
	//outputs
	sc_out<int> DMICmd;
	sc_out<bool> DMIdisplaySBI;
	sc_out<int> TICmd;

	void update();

	SC_CTOR(output_ctrl);
};


SC_MODULE(ceiling_speed_monitor) {
	/*
	 * in and outputs copied from Java implementation:
	 *
	 * 	//inputs
	public float SimulatedTrainSpeed;
	public float V_mrsp;
	public boolean RevocationEmergencyBrake;
	public boolean SBAvailable;
	public int SUTCommand;
	public int Free_SpeedSupervisionStatus;
	public int TSM_RSM_PIM_M_SDMTYPE;
	//outputs
	public boolean displaySBI;
	public boolean displayPermittedSpeed = false;
	public boolean EmergencyBrakeCommand;
	public boolean ServiceBrakeCommand;
	public int M_SDMTYPE;
	public int SpeedSupervisionStatus;
	 */

	//inputs
	sc_in<float> V_est;
	sc_in<float> V_mrsp;

	//outputs
	sc_out<int> DMICmd;
	sc_out<bool> DMIdisplaySBI;
	sc_out<int> TICmd;

	//internals
	//int location;
	sc_signal<int> location;

	output_ctrl output_controller;

	SC_CTOR(ceiling_speed_monitor);

	void update();

	void reset();



	void updateL0();

	void updateL1();

	void updateL2();

	void updateL3();

	void updateL4();

	float dV_warning(float v);

	float dV_sbi(float v);

	float dV_ebi(float v);

};

#endif // CEILING_SPEED_MONITOR_H
