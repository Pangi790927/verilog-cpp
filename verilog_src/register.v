`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// * From AC Lab in UPB
//////////////////////////////////////////////////////////////////////////////////
module register(
        clk,
        rst,
        oe,
        we,
        in,
        out
    );

parameter word_width = 32;

input               clk;
input               rst;
input               oe;
input               we;
input [word_width-1 : 0] in;
output[word_width-1 : 0] out;

reg [word_width-1 : 0]   data;

always @(posedge clk) begin
    if(rst)
        data <= 0;
    else if(we)
        data <= in;
end

assign out = oe ? data : 0;
endmodule
