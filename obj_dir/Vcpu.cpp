// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vcpu.h for the primary calling header

#include "Vcpu.h"              // For This
#include "Vcpu__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vcpu) {
    Vcpu__Syms* __restrict vlSymsp = __VlSymsp = new Vcpu__Syms(this, name());
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    A = VL_RAND_RESET_I(32);
    B = VL_RAND_RESET_I(32);
    v__DOT__state = VL_RAND_RESET_I(32);
}

void Vcpu::__Vconfigure(Vcpu__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vcpu::~Vcpu() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void Vcpu::eval() {
    Vcpu__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vcpu::eval\n"); );
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

void Vcpu::_eval_initial_loop(Vcpu__Syms* __restrict vlSymsp) {
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

VL_INLINE_OPT void Vcpu::_combo__TOP__1(Vcpu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::_combo__TOP__1\n"); );
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at cpu.v:8
    vlTOPp->v__DOT__state = (~ vlTOPp->A);
    vlTOPp->B = vlTOPp->v__DOT__state;
}

void Vcpu::_eval(Vcpu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::_eval\n"); );
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

void Vcpu::_eval_initial(Vcpu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::_eval_initial\n"); );
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vcpu::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::final\n"); );
    // Variables
    Vcpu__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vcpu::_eval_settle(Vcpu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::_eval_settle\n"); );
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_combo__TOP__1(vlSymsp);
}

VL_INLINE_OPT QData Vcpu::_change_request(Vcpu__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vcpu::_change_request\n"); );
    Vcpu* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}
