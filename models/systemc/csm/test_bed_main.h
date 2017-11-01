#include "ceiling_speed_monitor.h"
#include "fisignal.h"

extern ceiling_speed_monitor sut;

//create signals for inputs
extern fi_signal<float> V_est;
extern fi_signal<float> V_mrsp;

//create signals for outputs
extern fi_signal<signed int> DMICmd;
extern fi_signal<bool> DMIdisplaySBI;
extern fi_signal<signed int> TICmd;
