#include "route_lyngby_r_12a.h"
#include "fisignal.h"

extern route_lyngby_r_12a sut;

//create signals for inputs
extern fi_signal<signed int> mb20_act;
extern fi_signal<signed int> mb31_act;
extern fi_signal<signed int> mb32_act;
extern fi_signal<signed int> r_01_;
extern fi_signal<signed int> r_02a;
extern fi_signal<signed int> r_02b;
extern fi_signal<signed int> r_05_;
extern fi_signal<signed int> r_06a;
extern fi_signal<signed int> r_06b;
extern fi_signal<signed int> r_07a;
extern fi_signal<signed int> r_07b;
extern fi_signal<signed int> r_08a;
extern fi_signal<signed int> r_08b;
extern fi_signal<signed int> r_09a;
extern fi_signal<signed int> r_09b;
extern fi_signal<signed int> r_10a;
extern fi_signal<signed int> r_10b;
extern fi_signal<signed int> r_11_;
extern fi_signal<signed int> r_12b;
extern fi_signal<signed int> r_13_;
extern fi_signal<signed int> r_16a;
extern fi_signal<signed int> r_16b;
extern fi_signal<signed int> sutCmd;
extern fi_signal<bool> t11;
extern fi_signal<signed int> t11_pos;
extern fi_signal<bool> t13;
extern fi_signal<signed int> t13_pos;
extern fi_signal<bool> t20;
extern fi_signal<bool> t20_occ;
extern fi_signal<signed int> t20_pos;
extern fi_signal<bool> t21;
extern fi_signal<bool> t21_occ;
extern fi_signal<bool> t30;
extern fi_signal<bool> t30_occ;
extern fi_signal<bool> t31;
extern fi_signal<bool> t31_occ;
extern fi_signal<signed int> t31_pos;
extern fi_signal<bool> t33;
extern fi_signal<signed int> t33_pos;

//create signals for outputs
extern fi_signal<bool> error;
extern fi_signal<signed int> mb20_cmd;
extern fi_signal<signed int> mb21_cmd;
extern fi_signal<signed int> mb30_cmd;
extern fi_signal<signed int> mb31_cmd;
extern fi_signal<signed int> mb32_cmd;
extern fi_signal<signed int> mylocation;
extern fi_signal<signed int> t11_cmd;
extern fi_signal<signed int> t13_cmd;
extern fi_signal<signed int> t20_cmd;
extern fi_signal<bool> t20_out;
extern fi_signal<bool> t21_out;
extern fi_signal<bool> t30_out;
extern fi_signal<signed int> t31_cmd;
extern fi_signal<bool> t31_out;
extern fi_signal<signed int> t33_cmd;
