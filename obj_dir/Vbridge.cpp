// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vbridge.h for the primary calling header

#include "Vbridge.h"           // For This
#include "Vbridge__Syms.h"

//--------------------
// STATIC VARIABLES


//--------------------

VL_CTOR_IMP(Vbridge) {
    Vbridge__Syms* __restrict vlSymsp = __VlSymsp = new Vbridge__Syms(this, name());
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Reset internal values
    
    // Reset structure values
    clk = VL_RAND_RESET_I(1);
    rst = VL_RAND_RESET_I(1);
    cpu_iodata = VL_RAND_RESET_I(16);
    cpu_ioaddr = VL_RAND_RESET_I(16);
    cpu_addr = VL_RAND_RESET_I(32);
    cpu_data = VL_RAND_RESET_I(32);
    cpu_ctrl = VL_RAND_RESET_I(16);
    ram_addr = VL_RAND_RESET_I(32);
    ram_data = VL_RAND_RESET_I(32);
    ram_ctrl = VL_RAND_RESET_I(16);
    vga_addr = VL_RAND_RESET_I(32);
    vga_data = VL_RAND_RESET_I(32);
    vga_ctrl = VL_RAND_RESET_I(16);
    v__DOT__cpu_ctrl__out__out2 = VL_RAND_RESET_I(16);
    __Vclklast__TOP__clk = VL_RAND_RESET_I(1);
    __Vclklast__TOP__rst = VL_RAND_RESET_I(1);
}

void Vbridge::__Vconfigure(Vbridge__Syms* vlSymsp, bool first) {
    if (0 && first) {}  // Prevent unused
    this->__VlSymsp = vlSymsp;
}

Vbridge::~Vbridge() {
    delete __VlSymsp; __VlSymsp=NULL;
}

//--------------------


void Vbridge::eval() {
    Vbridge__Syms* __restrict vlSymsp = this->__VlSymsp; // Setup global symbol table
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Initialize
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) _eval_initial_loop(vlSymsp);
    // Evaluate till stable
    VL_DEBUG_IF(VL_PRINTF("\n----TOP Evaluate Vbridge::eval\n"); );
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

void Vbridge::_eval_initial_loop(Vbridge__Syms* __restrict vlSymsp) {
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

void Vbridge::_initial__TOP__1(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_initial__TOP__1\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // INITIAL at bridge.v:13
    vlTOPp->ram_data = 0U;
    // INITIAL at bridge.v:14
    vlTOPp->ram_ctrl = 0U;
    // INITIAL at bridge.v:18
    vlTOPp->vga_data = 0U;
    // INITIAL at bridge.v:19
    vlTOPp->vga_ctrl = 0U;
    // INITIAL at bridge.v:6
    vlTOPp->cpu_iodata = 0U;
    // INITIAL at bridge.v:9
    vlTOPp->cpu_data = 0U;
}

VL_INLINE_OPT void Vbridge::_sequent__TOP__2(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_sequent__TOP__2\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // ALWAYS at bridge.v:69
    if ((1U & (~ (IData)(vlTOPp->rst)))) {
	if (vlTOPp->clk) {
	    if ((1U & (IData)(vlTOPp->cpu_ctrl))) {
		vlTOPp->v__DOT__cpu_ctrl__out__out2 
		    = (0xfffeU & (IData)(vlTOPp->v__DOT__cpu_ctrl__out__out2));
	    }
	}
    }
    vlTOPp->cpu_ctrl = vlTOPp->v__DOT__cpu_ctrl__out__out2;
}

void Vbridge::_settle__TOP__3(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_settle__TOP__3\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->cpu_ctrl = vlTOPp->v__DOT__cpu_ctrl__out__out2;
}

void Vbridge::_eval(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_eval\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    if ((((IData)(vlTOPp->clk) & (~ (IData)(vlTOPp->__Vclklast__TOP__clk))) 
	 | ((IData)(vlTOPp->rst) & (~ (IData)(vlTOPp->__Vclklast__TOP__rst))))) {
	vlTOPp->_sequent__TOP__2(vlSymsp);
    }
    // Final
    vlTOPp->__Vclklast__TOP__clk = vlTOPp->clk;
    vlTOPp->__Vclklast__TOP__rst = vlTOPp->rst;
}

void Vbridge::_eval_initial(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_eval_initial\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_initial__TOP__1(vlSymsp);
}

void Vbridge::final() {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::final\n"); );
    // Variables
    Vbridge__Syms* __restrict vlSymsp = this->__VlSymsp;
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
}

void Vbridge::_eval_settle(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_eval_settle\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    vlTOPp->_settle__TOP__3(vlSymsp);
}

VL_INLINE_OPT QData Vbridge::_change_request(Vbridge__Syms* __restrict vlSymsp) {
    VL_DEBUG_IF(VL_PRINTF("    Vbridge::_change_request\n"); );
    Vbridge* __restrict vlTOPp VL_ATTR_UNUSED = vlSymsp->TOPp;
    // Body
    // Change detection
    QData __req = false;  // Logically a bool
    return __req;
}
