`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date:    19:24:24 11/26/2011
// Design Name:
// Module Name:    register
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
module register(
        clk,
        rst,
        oe,
        we,
        in,
        out
    );

parameter width = 32;

input               clk;
input               rst;
input               oe;
input               we;
input [width-1 : 0] in;
output[width-1 : 0] out;

reg [width-1 : 0]   data;

always @(posedge clk) begin
    if(rst)
        data <= 0;
    else if(we)
        data <= in;
end

assign out = oe ? data : 0;
endmodule
