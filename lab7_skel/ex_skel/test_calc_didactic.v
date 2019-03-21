`timescale 1ns / 1ps
////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:    11:11:42 11/10/2014
// Design Name: 
// Module Name:    test_calc_didactic
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////
module test_calc_didactic;

	// Inputs
	reg clk;
	reg rst;
	reg [15:0] io_out;
	reg disp_clk;
	reg [11:0] disp_addr;
	reg [11:0] disp_reg_addr;

	// Outputs
	wire io_oe;
	wire io_we;
	wire [7:0] io_port;
	wire [15:0] io_in;
	wire [15:0] disp_data;
	wire [15:0] disp_reg_data;
	wire [15:0] disp_state;
	wire [15:0] disp_cp;
	wire [15:0] disp_ind;
	wire [15:0] disp_am;
	wire [15:0] disp_aie;
	wire [15:0] disp_t1;
	wire [15:0] disp_t2;
	wire [15:0] disp_ri;
	wire [15:0] disp_mag;

	// Instantiate the Unit Under Test (UUT)
	calc_didactic uut (
		.clk(clk), 
		.rst(rst), 
		.io_oe(io_oe), 
		.io_we(io_we), 
		.io_port(io_port), 
		.io_in(io_in), 
		.io_out(io_out), 
		.disp_clk(disp_clk), 
		.disp_addr(disp_addr), 
		.disp_data(disp_data), 
		.disp_reg_addr(disp_reg_addr), 
		.disp_reg_data(disp_reg_data), 
		.disp_state(disp_state), 
		.disp_cp(disp_cp), 
		.disp_ind(disp_ind), 
		.disp_am(disp_am), 
		.disp_aie(disp_aie), 
		.disp_t1(disp_t1), 
		.disp_t2(disp_t2), 
		.disp_ri(disp_ri), 
		.disp_mag(disp_mag)
	);

	initial begin
		// Initialize Inputs
		clk = 0;
		rst = 0;
		io_out = 0;
		disp_clk = 0;
		disp_addr = 0;
		disp_reg_addr = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		rst = 1;
		#30;
		rst = 0;
	end
	
	always #10 clk = !clk;
	always #10 disp_clk = !disp_clk;

	always @(posedge disp_clk)
	begin
		disp_addr = disp_addr + 1;
		disp_reg_addr = disp_reg_addr + 1;
	end
endmodule

