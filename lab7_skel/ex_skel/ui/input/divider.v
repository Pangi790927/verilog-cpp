`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:56:25 11/28/2011 
// Design Name: 
// Module Name:    divider 
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
module divider(
		f,
		in,
		out
	);

parameter f_width = 4;
parameter counter_width = 16;

input [f_width-1 : 0] f;
input in;
output out;

reg[counter_width-1 : 0] counter = 0;

always @(posedge in) begin
	counter <= counter + 1;
end

assign out = counter[f];

endmodule
