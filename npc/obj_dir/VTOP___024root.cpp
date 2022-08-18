// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VTOP.h for the primary calling header

#include "VTOP___024root.h"
#include "VTOP__Syms.h"

#include "verilated_dpi.h"

//==========

extern "C" svLogic helloFromCpp();

VL_INLINE_OPT void VTOP___024root____Vdpiimwrap_TOP__DOT__UseSimuControl0__DOT__control__DOT__helloFromCpp_TOP(const VerilatedScope* __Vscopep, const char* __Vfilenamep, IData/*31:0*/ __Vlineno, CData/*0:0*/ &helloFromCpp__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root____Vdpiimwrap_TOP__DOT__UseSimuControl0__DOT__control__DOT__helloFromCpp_TOP\n"); );
    // Body
    Verilated::dpiContext(__Vscopep, __Vfilenamep, __Vlineno);
    svLogic helloFromCpp__Vfuncrtn__Vcvt;
    helloFromCpp__Vfuncrtn__Vcvt = helloFromCpp();
    helloFromCpp__Vfuncrtn = (1U & helloFromCpp__Vfuncrtn__Vcvt);
}

void VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__Check_TOP(VTOP__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__Check_TOP\n"); );
    // Variables
    // Body
    VL_WRITEF("\nhere comes dpi-c\n\n");
}

void VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__check_TOP(VTOP__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root____Vdpiexp_TOP__DOT__UseSimuControl0__DOT__control__DOT__check_TOP\n"); );
    // Variables
    // Body
    VL_WRITEF("\nhere comes dpi-c\n\n");
}

VL_INLINE_OPT void VTOP___024root___sequent__TOP__1(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___sequent__TOP__1\n"); );
    // Body
    if (VL_UNLIKELY((((~ (IData)((0x13U == (0x707fU 
                                            & vlSelf->io_InstReadPort_data)))) 
                      & (0x100073U != vlSelf->io_InstReadPort_data)) 
                     & (~ (IData)(vlSelf->reset))))) {
        VL_FWRITEF(0x80000002U,"aaao%3#\n",7,(0x7fU 
                                              & vlSelf->io_InstReadPort_data));
    }
    vlSelf->TOP__DOT__IFU0__DOT__pc = ((IData)(vlSelf->reset)
                                        ? 0x80000000ULL
                                        : vlSelf->TOP__DOT__IFU0__DOT___pc_T_1);
    vlSelf->io_InstReadPort_addr = vlSelf->TOP__DOT__IFU0__DOT__pc;
    vlSelf->TOP__DOT__IFU0__DOT___pc_T_1 = (4ULL + vlSelf->TOP__DOT__IFU0__DOT__pc);
}

void VTOP___024root___eval(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___eval\n"); );
    // Body
    if (((IData)(vlSelf->clock) & (~ (IData)(vlSelf->__Vclklast__TOP__clock)))) {
        VTOP___024root___sequent__TOP__1(vlSelf);
    }
    // Final
    vlSelf->__Vclklast__TOP__clock = vlSelf->clock;
}

QData VTOP___024root___change_request_1(VTOP___024root* vlSelf);

VL_INLINE_OPT QData VTOP___024root___change_request(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___change_request\n"); );
    // Body
    return (VTOP___024root___change_request_1(vlSelf));
}

VL_INLINE_OPT QData VTOP___024root___change_request_1(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___change_request_1\n"); );
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}

#ifdef VL_DEBUG
void VTOP___024root___eval_debug_assertions(VTOP___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    VTOP__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    VTOP___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clock & 0xfeU))) {
        Verilated::overWidthError("clock");}
    if (VL_UNLIKELY((vlSelf->reset & 0xfeU))) {
        Verilated::overWidthError("reset");}
}
#endif  // VL_DEBUG
