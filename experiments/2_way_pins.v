//==============================================================================
//            en --                             
//                |                             
//                |                             
//              |\|  <-- bi-directional port -->
// sig_out -->__| \_____________________________
//              | /           |     sig_io      
//              |/            |                 
//                            |                 
//   sig_in   <-- ____________|                 
//  
//   VERILOG CODE:
// 
// 
// assign sig_io = (en) ? sig_out : {DATAWIDTH{1'bz}}; //DATAWIDTH = size of bus
// assign sig_in = sig_io ;  
//
//==============================================================================

module master_chip(
	inout [3:0] io
);
	
endmodule

module slave_chip(
	inout [3:0] io
);
	
endmodule
