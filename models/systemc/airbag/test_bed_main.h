#include "airbag.h"
#include "fisignal.h"

extern airbag sut;

//create signals for inputs
extern fi_signal<float> s1;
extern fi_signal<float> s2;
extern fi_signal<signed int> t;

//create signals for outputs
extern fi_signal<signed int> defect;
extern fi_signal<signed int> fire;
