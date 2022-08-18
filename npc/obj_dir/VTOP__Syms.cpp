// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table implementation internals

#include "VTOP__Syms.h"
#include "VTOP.h"
#include "VTOP___024root.h"

void VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__Check_TOP(VTOP__Syms* __restrict vlSymsp);
void VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__check_TOP(VTOP__Syms* __restrict vlSymsp);

// FUNCTIONS
VTOP__Syms::~VTOP__Syms()
{
}

VTOP__Syms::VTOP__Syms(VerilatedContext* contextp, const char* namep,VTOP* modelp)
    : VerilatedSyms{contextp}
    // Setup internal state of the Syms class
    , __Vm_modelp(modelp)
    // Setup module instances
    , TOP(namep)
{
    // Configure time unit / time precision
    _vm_contextp__->timeunit(-12);
    _vm_contextp__->timeprecision(-12);
    // Setup each module's pointers to their submodules
    // Setup each module's pointer back to symbol table (for public functions)
    TOP.__Vconfigure(this, true);
    // Setup scopes
    __Vscope_TOP__UseSimuControl0__control.configure(this, name(), "UseSimuControl0.control", "control", -12, VerilatedScope::SCOPE_OTHER);
    // Setup export functions
    for (int __Vfinal=0; __Vfinal<2; __Vfinal++) {
        __Vscope_TOP__UseSimuControl0__control.exportInsert(__Vfinal, "Check", (void*)(&VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__Check_TOP));
        __Vscope_TOP__UseSimuControl0__control.exportInsert(__Vfinal, "check", (void*)(&VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__check_TOP));
    }
}
