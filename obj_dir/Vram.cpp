// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vram.h for the primary calling header

#include "Vram.h"              // For This
#include "Vram__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vram) {
    Vram__Syms* __restrict vlSymsp = __VlSymsp = new Vram__Syms(this, name());
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
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

void Vram::__Vconfigure(Vram__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vram::~Vram() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void Vram::eval() {
    Vram__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vram::eval\n"); );
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

void Vram::_eval_initial_loop(Vram__Syms* __restrict vlSymsp) {
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

void Vram::_eval(Vram__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vram::_eval\n"); );
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vram::_eval_initial(Vram__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vram::_eval_initial\n"); );
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vram::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vram::final\n"); );
    // Variables
    Vram__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vram::_eval_settle(Vram__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vram::_eval_settle\n"); );
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

VL_INLINE_OPT QData Vram::_change_request(Vram__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vram::_change_request\n"); );
    Vram* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}
