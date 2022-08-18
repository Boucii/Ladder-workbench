// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI EXPORTS
    // DPI export at sim//SimuControl.v:13:15
    extern void Check();
    // DPI export at sim//SimuControl.v:19:6
    extern void check();

    // DPI IMPORTS
    // DPI import at sim//SimuControl.v:5:33
    extern svLogic helloFromCpp();

#ifdef __cplusplus
}
#endif
