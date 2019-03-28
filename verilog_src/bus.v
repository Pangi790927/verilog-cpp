`timescale 1ns / 1ps
module bus(
        alu_in,
        ram_in,
        ram_out,
        io_in,
        io_out,
        regs_in,
        regs_out,
        cp_in,
        cp_out,
        ind_in,
        ind_out,
        am_out,
        aie_out,
        t1_out,
        t2_out,
        offset_in,
        ri_out,
        disp_out
    );

parameter word_width = 32;

input [word_width-1 : 0] alu_in;
input [word_width-1 : 0] ram_in;
input [word_width-1 : 0] io_in;
input [word_width-1 : 0] regs_in;
input [word_width-1 : 0] cp_in;
input [word_width-1 : 0] ind_in;
input [word_width-1 : 0] offset_in;
output[word_width-1 : 0] am_out;
output[word_width-1 : 0] aie_out;
output[word_width-1 : 0] t1_out;
output[word_width-1 : 0] t2_out;
output[word_width-1 : 0] ri_out;
output[word_width-1 : 0] ram_out;
output[word_width-1 : 0] io_out;
output[word_width-1 : 0] regs_out;
output[word_width-1 : 0] cp_out;
output[word_width-1 : 0] ind_out;
output[word_width-1 : 0] disp_out;

wire [word_width-1 : 0]  bus;

assign bus = alu_in | ram_in | io_in | regs_in | cp_in | ind_in | offset_in;

assign am_out = bus;
assign aie_out = bus;
assign ram_out = bus;
assign io_out = bus;
assign regs_out = bus;
assign cp_out = bus;
assign ind_out = bus;
assign t1_out = bus;
assign t2_out = bus;
assign ri_out = bus;
assign disp_out = bus;

endmodule
