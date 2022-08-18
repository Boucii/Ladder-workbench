// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VTOP.h"
#include "VTOP__Syms.h"
#include "verilated_dpi.h"

//============================================================
// Constructors

VTOP::VTOP(VerilatedContext* _vcontextp__, const char* _vcname__)
    : vlSymsp{new VTOP__Syms(_vcontextp__, _vcname__, this)}
    , clock{vlSymsp->TOP.clock}
    , reset{vlSymsp->TOP.reset}
    , io_InstReadPort_addr{vlSymsp->TOP.io_InstReadPort_addr}
    , io_InstReadPort_data{vlSymsp->TOP.io_InstReadPort_data}
    , rootp{&(vlSymsp->TOP)}
{
}

VTOP::VTOP(const char* _vcname__)
    : VTOP(nullptr, _vcname__)
{
}

//============================================================
// Destructor

VTOP::~VTOP() {
    delete vlSymsp;
}

//============================================================
// Evaluation loop

void VTOP___024root___eval_initial(VTOP___024root* vlSelf);
void VTOP___024root___eval_settle(VTOP___024root* vlSelf);
void VTOP___024root___eval(VTOP___024root* vlSelf);
QData VTOP___024root___change_request(VTOP___024root* vlSelf);
#ifdef VL_DEBUG
void VTOP___024root___eval_debug_assertions(VTOP___024root* vlSelf);
#endif  // VL_DEBUG
void VTOP___024root___final(VTOP___024root* vlSelf);

static void _eval_initial_loop(VTOP__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    VTOP___024root___eval_initial(&(vlSymsp->TOP));
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial loop\n"););
        VTOP___024root___eval_settle(&(vlSymsp->TOP));
        VTOP___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VTOP___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("build/TOP.v", 501, "",
                "Verilated model didn't DC converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VTOP___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

void VTOP::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VTOP::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VTOP___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    int __VclockLoop = 0;
    QData __Vchange = 1;
    do {
        VL_DEBUG_IF(VL_DBG_MSGF("+ Clock loop\n"););
        VTOP___024root___eval(&(vlSymsp->TOP));
        if (VL_UNLIKELY(++__VclockLoop > 100)) {
            // About to fail, so enable debug to see what's not settling.
            // Note you must run make with OPT=-DVL_DEBUG for debug prints.
            int __Vsaved_debug = Verilated::debug();
            Verilated::debug(1);
            __Vchange = VTOP___024root___change_request(&(vlSymsp->TOP));
            Verilated::debug(__Vsaved_debug);
            VL_FATAL_MT("build/TOP.v", 501, "",
                "Verilated model didn't converge\n"
                "- See https://verilator.org/warn/DIDNOTCONVERGE");
        } else {
            __Vchange = VTOP___024root___change_request(&(vlSymsp->TOP));
        }
    } while (VL_UNLIKELY(__Vchange));
}

//============================================================
// Invoke final blocks

void VTOP::final() {
    VTOP___024root___final(&(vlSymsp->TOP));
}

//============================================================
// Utilities

VerilatedContext* VTOP::contextp() const {
    return vlSymsp->_vm_contextp__;
}

const char* VTOP::name() const {
    return vlSymsp->name();
}
