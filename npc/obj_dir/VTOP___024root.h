// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VTOP.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated_heavy.h"

//==========

class VTOP__Syms;

//----------

VL_MODULE(VTOP___024root) {
  public:

    // PORTS
    VL_IN8(clock,0,0);
    VL_IN8(reset,0,0);
    VL_IN(io_InstReadPort_data,31,0);
    VL_OUT64(io_InstReadPort_addr,63,0);

    // LOCAL SIGNALS
    QData/*63:0*/ TOP__DOT__IFU0__DOT__pc;
    QData/*63:0*/ TOP__DOT__IFU0__DOT___pc_T_1;

    // LOCAL VARIABLES
    CData/*0:0*/ __Vclklast__TOP__clock;

    // INTERNAL VARIABLES
    VTOP__Syms* vlSymsp;  // Symbol table

    // CONSTRUCTORS
  private:
    VL_UNCOPYABLE(VTOP___024root);  ///< Copying not allowed
  public:
    VTOP___024root(const char* name);
    ~VTOP___024root();

    // INTERNAL METHODS
    void __Vconfigure(VTOP__Syms* symsp, bool first);
} VL_ATTR_ALIGNED(VL_CACHE_LINE_BYTES);

//----------


#endif  // guard
