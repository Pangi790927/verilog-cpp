// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vvga.h for the primary calling header

#include "Vvga.h"              // For This
#include "Vvga__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vvga) {
    Vvga__Syms* __restrict vlSymsp = __VlSymsp = new Vvga__Syms(this, name());
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    clk = VL_RAND_RESET_I(1);
    rst = VL_RAND_RESET_I(1);
    addr = VL_RAND_RESET_I(32);
    data = VL_RAND_RESET_I(32);
    ctrl = VL_RAND_RESET_I(16);
    phy_addr = VL_RAND_RESET_I(32);
    phy_data = VL_RAND_RESET_I(32);
}

void Vvga::__Vconfigure(Vvga__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vvga::~Vvga() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void Vvga::eval() {
    Vvga__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vvga::eval\n"); );
    int __VclockLoop = 0;
    QData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	VL_DEBUG_IF(VL_PRINTF(" Clock loop\n"););
	vlSymsp->__Vm_activity = true;
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't converge");
    }
}

void Vvga::_eval_initial_loop(Vvga__Syms* __restrict vlSymsp) {
    vlSymsp->__Vm_didInit = true;
    _eval_initial(vlSymsp);
    vlSymsp->__Vm_activity = true;
    int __VclockLoop = 0;
    QData __Vchange=1;
    while (VL_LIKELY(__Vchange)) {
	_eval_settle(vlSymsp);
	_eval(vlSymsp);
	__Vchange = _change_request(vlSymsp);
	if (++__VclockLoop > 100) vl_fatal(__FILE__,__LINE__,__FILE__,"Verilated model didn't DC converge");
    }
}

//--------------------
// Internal Methods

void Vvga::_eval(Vvga__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vvga::_eval\n"); );
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vvga::_eval_initial(Vvga__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vvga::_eval_initial\n"); );
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vvga::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vvga::final\n"); );
    // Variables
    Vvga__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vvga::_eval_settle(Vvga__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vvga::_eval_settle\n"); );
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

VL_INLINE_OPT QData Vvga::_change_request(Vvga__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vvga::_change_request\n"); );
    Vvga* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}
