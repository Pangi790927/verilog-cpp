// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _Vvga_H_
#define _Vvga_H_

#include "verilated.h"
class Vvga__Syms;

//----------

VL_MODULE(Vvga) {
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
    Vvga__Syms*	__VlSymsp;		// Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    Vvga& operator= (const Vvga&);	///< Copying not allowed
    Vvga(const Vvga&);	///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible WRT DPI scope names.
    Vvga(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vvga();
    
    // USER METHODS
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vvga__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vvga__Syms* symsp, bool first);
  private:
    static QData	_change_request(Vvga__Syms* __restrict vlSymsp);
  public:
    static void	_eval(Vvga__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vvga__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vvga__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
