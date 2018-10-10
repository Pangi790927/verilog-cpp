// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _Vram_H_
#define _Vram_H_

#include "verilated.h"
class Vram__Syms;

//----------

VL_MODULE(Vram) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_IN16(ctrl,15,0);
    VL_IN(addr,31,0);
    VL_IN(data,31,0);
    VL_OUT(phy_addr,31,0);
    VL_OUT(phy_data,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vram__Syms*	__VlSymsp;		// Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    Vram& operator= (const Vram&);	///< Copying not allowed
    Vram(const Vram&);	///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible WRT DPI scope names.
    Vram(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vram();
    
    // USER METHODS
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vram__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vram__Syms* symsp, bool first);
  private:
    static QData	_change_request(Vram__Syms* __restrict vlSymsp);
  public:
    static void	_eval(Vram__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vram__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vram__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
