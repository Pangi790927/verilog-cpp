// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary design header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef _Vcpu_H_
#define _Vcpu_H_

#include "verilated.h"
class Vcpu__Syms;

//----------

VL_MODULE(Vcpu) {
  public:
    // CELLS
    // Public to allow access to /*verilator_public*/ items;
    // otherwise the application code can consider these internals.
    
    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN(A,31,0);
    VL_OUT(B,31,0);
    
    // LOCAL SIGNALS
    // Internals; generally not touched by application code
    VL_SIG(v__DOT__state,31,0);
    
    // LOCAL VARIABLES
    // Internals; generally not touched by application code
    
    // INTERNAL VARIABLES
    // Internals; generally not touched by application code
    Vcpu__Syms*	__VlSymsp;		// Symbol table
    
    // PARAMETERS
    // Parameters marked /*verilator public*/ for use by application code
    
    // CONSTRUCTORS
  private:
    Vcpu& operator= (const Vcpu&);	///< Copying not allowed
    Vcpu(const Vcpu&);	///< Copying not allowed
  public:
    /// Construct the model; called by application code
    /// The special name  may be used to make a wrapper with a
    /// single model invisible WRT DPI scope names.
    Vcpu(const char* name="TOP");
    /// Destroy the model; called (often implicitly) by application code
    ~Vcpu();
    
    // USER METHODS
    
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval();
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    
    // INTERNAL METHODS
  private:
    static void _eval_initial_loop(Vcpu__Syms* __restrict vlSymsp);
  public:
    void __Vconfigure(Vcpu__Syms* symsp, bool first);
  private:
    static QData	_change_request(Vcpu__Syms* __restrict vlSymsp);
  public:
    static void	_combo__TOP__1(Vcpu__Syms* __restrict vlSymsp);
    static void	_eval(Vcpu__Syms* __restrict vlSymsp);
    static void	_eval_initial(Vcpu__Syms* __restrict vlSymsp);
    static void	_eval_settle(Vcpu__Syms* __restrict vlSymsp);
} VL_ATTR_ALIGNED(128);

#endif  /*guard*/
