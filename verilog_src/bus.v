`timescale 1ns / 1ps

/**
 * Connected stuff to the bus. Note that in/outs are absolute 
 * and are not referred from bus' perspective
 **/
module bus(
        cpu_addr_in,
        cpu_data_in,
        t1_in,
        t2_in,
        t1_out,
        t2_out,
        dbg_in
    );

parameter word_width = 32;

input[word_width-1 : 0] cpu_data_in;
input[word_width-1 : 0] t1_out;
input[word_width-1 : 0] t2_out;

output[word_width-1 : 0] cpu_addr_in;
output[word_width-1 : 0] t1_in;
output[word_width-1 : 0] t2_in;
output[word_width-1 : 0] dbg_in;
// input [word_width-1 : 0] alu_out;
// input [word_width-1 : 0] regs_in;
// input [word_width-1 : 0] cp_in;
// input [word_width-1 : 0] ind_in;
// input [word_width-1 : 0] offset_in;
// output[word_width-1 : 0] am_out;
// output[word_width-1 : 0] aie_out;
// output[word_width-1 : 0] t1_out;
// output[word_width-1 : 0] t2_out;
// output[word_width-1 : 0] ri_out;
// output[word_width-1 : 0] regs_out;
// output[word_width-1 : 0] cp_out;
// output[word_width-1 : 0] ind_out;

wire [word_width-1 : 0]  bus;

assign bus = cpu_data_in | t1_out | t2_out | dbg_in;

assign cpu_addr_in = bus;
assign t1_in = bus;
assign t2_in = bus;
// assign am_out = bus;
// assign aie_out = bus;
// assign ram_out = bus;
// assign io_out = bus;
// assign regs_out = bus;
// assign cp_out = bus;
// assign ind_out = bus;
// assign t1_out = bus;
// assign t2_out = bus;
// assign ri_out = bus;
// assign disp_out = bus;

endmodule
