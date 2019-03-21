`include "alu.v"

module cpu(
		input	clk,
		input	rst,
		output	[width-1 : 0] mobo_ctrl,
		input	[width-1 : 0] mobo_stat,
		output	[width-1 : 0] addr,
		output	[width-1 : 0] data_out,
		input	[width-1 : 0] data_in
	);

	parameter width = 32;

	// instantiate T1 register and related connections
	wire                        t1_oe;
	wire                        t1_we;
	wire[word_width-1 : 0]      t1_in;
	wire[word_width-1 : 0]      t1_out;
	register #(word_width) t1(clk, rst, t1_oe, t1_we, t1_in, t1_out);

	// instantiate T2 register and related connections
	wire                        t2_oe;
	wire                        t2_we;
	wire[word_width-1 : 0]      t2_in;
	wire[word_width-1 : 0]      t2_out;
	register #(word_width) t2(clk, rst, t2_oe, t2_we, t2_in, t2_out);

	// instantiate ALU and related connections
	wire                        alu_oe;
	wire[3 : 0]                 alu_opcode;
	wire                        alu_carry;
	wire[word_width-1 : 0]      alu_out;
	wire[4 : 0]                 alu_flags;
	alu #(word_width, 5) alu(alu_oe, alu_opcode, t1_out, t2_out, alu_carry, alu_out, alu_flags);
endmodule