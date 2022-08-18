// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VTOP.h for the primary calling header

#include "VTOP___024root.h"
#include "VTOP__Syms.h"

#include "verilated_dpi.h"

//==========


void VTOP___024root___ctor_var_reset(VTOP___024root* vlSelf);

VTOP___024root::VTOP___024root(const char* _vcname__)
    : VerilatedModule(_vcname__)
 {
    // Reset structure values
    VTOP___024root___ctor_var_reset(this);
}

void VTOP___024root::__Vconfigure(VTOP__Syms* _vlSymsp, bool first) {
    if (false && first) {}  // Prevent unused
    this->vlSymsp = _vlSymsp;
}

VTOP___024root::~VTOP___024root() {
}

void VTOP___024root___settle__TOP__2(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___settle__TOP__2\n"); );
    // Body
    vlSelf->io_InstReadPort_addr = vlSelf->TOP__DOT__IFU0__DOT__pc;
    vlSelf->TOP__DOT__IFU0__DOT___pc_T_1 = (4ULL + vlSelf->TOP__DOT__IFU0__DOT__pc);
}

void VTOP___024root___eval_initial(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___eval_initial\n"); );
    // Body
    vlSelf->__Vclklast__TOP__clock = vlSelf->clock;
}

void VTOP___024root___eval_settle(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___eval_settle\n"); );
    // Body
    VTOP___024root___settle__TOP__2(vlSelf);
}

void VTOP___024root___final(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___final\n"); );
}

void VTOP___024root___ctor_var_reset(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___ctor_var_reset\n"); );
    // Body
    vlSelf->clock = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->io_InstReadPort_addr = VL_RAND_RESET_Q(64);
    vlSelf->io_InstReadPort_data = VL_RAND_RESET_I(32);
    vlSelf->TOP__DOT__IFU0__DOT__pc = VL_RAND_RESET_Q(64);
    vlSelf->TOP__DOT__IFU0__DOT___pc_T_1 = VL_RAND_RESET_Q(64);
}
