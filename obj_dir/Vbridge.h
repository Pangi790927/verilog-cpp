// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _Vbridge_H_
#define _Vbridge_H_

#include "verilated.h"
class Vbridge__Syms;

//----------

VL_MODULE(Vbridge) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_INOUT16(cpu_iodata,15,0);
    VL_IN16(cpu_ioaddr,15,0);
    VL_INOUT16(cpu_ctrl,15,0);
    VL_INOUT16(ram_ctrl,15,0);
    VL_INOUT16(vga_ctrl,15,0);
    VL_IN(cpu_addr,31,0);
    VL_INOUT(cpu_data,31,0);
    VL_IN(ram_addr,31,0);
    VL_INOUT(ram_data,31,0);
    VL_IN(vga_addr,31,0);
    VL_INOUT(vga_data,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    VL_SIG8(__Vclklast__TOP__clk,0,0);
    VL_SIG8(__Vclklast__TOP__rst,0,0);
    VL_SIG16(v__DOT__cpu_ctrl__out__out2,15,0);
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    //char	__VpadToAlign52[4];
    Vbridge__Syms*	__VlSymsp;		// Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    Vbridge& operator= (const Vbridge&);	///< Copying not allowed
    Vbridge(const Vbridge&);	///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible WRT DPI scope names.
    Vbridge(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vbridge();
    
    // USER METHODS
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vbridge__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vbridge__Syms* symsp, bool first);
  private:
    static QData	_change_request(Vbridge__Syms* __restrict vlSymsp);
  public:
    static void	_eval(Vbridge__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vbridge__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vbridge__Syms* __restrict vlSymsp);
    static void	_initial__TOP__1(Vbridge__Syms* __restrict vlSymsp);
    static void	_sequent__TOP__2(Vbridge__Syms* __restrict vlSymsp);
    static void	_settle__TOP__3(Vbridge__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
