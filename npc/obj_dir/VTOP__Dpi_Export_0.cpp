// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Implementation of DPI export functions.
//
#include "VTOP.h"
#include "VTOP__Syms.h"
#include "verilated_dpi.h"


void VTOP::Check() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root::Check\n"); );
    // Body
    static int __Vfuncnum = -1;
    if (VL_UNLIKELY(__Vfuncnum == -1)) __Vfuncnum = Verilated::exportFuncNum("Check");
    const VerilatedScope* __Vscopep = Verilated::dpiScope();
    VTOP__Vcb_Check_t __Vcb = (VTOP__Vcb_Check_t)(VerilatedScope::exportFind(__Vscopep, __Vfuncnum));
    (*__Vcb)((VTOP__Syms*)(__Vscopep->symsp()));
}

void VTOP::check() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root::check\n"); );
    // Body
    static int __Vfuncnum = -1;
    if (VL_UNLIKELY(__Vfuncnum == -1)) __Vfuncnum = Verilated::exportFuncNum("check");
    const VerilatedScope* __Vscopep = Verilated::dpiScope();
    VTOP__Vcb_check_t __Vcb = (VTOP__Vcb_check_t)(VerilatedScope::exportFind(__Vscopep, __Vfuncnum));
    (*__Vcb)((VTOP__Syms*)(__Vscopep->symsp()));
}
