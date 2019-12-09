`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:11:30 11/28/2011 
// Design Name: 
// Module Name:    debouncer 
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
//////////////////////////////////////////////////////////////////////////////////
module debouncer(
		clk,
		in,
		out
	);

parameter counter_width = 20;
parameter counter_value = 1000000;

input			clk;
input			in;
output reg	out = 0;

reg prev_in = 0;
reg[counter_width-1 : 0] counter = 0;

always @(posedge clk) begin
	if(counter == counter_value)
		out <= prev_in;
		
	if(in != prev_in)
		counter <= 0;
	else
		counter <= counter + 1;
	
	prev_in <= in;
end

endmodule
