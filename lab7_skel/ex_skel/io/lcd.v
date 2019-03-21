`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    20:26:45 12/10/2014 
// Design Name: 
// Module Name:    lcd 
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
module lcd(
        clk,
        oe,
        we,
        port,
        in,
        out,
        lcd_clk,
        lcd_data,
        lcd_en,
        lcd_rs,
        lcd_rw,
        lcd_ce
    );

parameter word_width        = 16;
parameter port_width        = 8;

input                       clk;
input                       oe;
input                       we;
input [port_width-1 : 0]    port;
input [word_width-1 : 0]    in;
output[word_width-1 : 0]    out;

// display control connections
input                       lcd_clk;
output[3:0]                 lcd_data;
output                      lcd_en;
output                      lcd_rs;
output                      lcd_rw;
output                      lcd_ce;

reg [7 : 0] data[2*16 : 0];             // store display data

// auxialiary connection for converting display data to linearized format for display module
wire [2*8*16-1 : 0] display;

// convert to linearized format
assign display[ 0*8 + 7 :  0*8] = data[ 0];
assign display[ 1*8 + 7 :  1*8] = data[ 1];
assign display[ 2*8 + 7 :  2*8] = data[ 2];
assign display[ 3*8 + 7 :  3*8] = data[ 3];
assign display[ 4*8 + 7 :  4*8] = data[ 4];
assign display[ 5*8 + 7 :  5*8] = data[ 5];
assign display[ 6*8 + 7 :  6*8] = data[ 6];
assign display[ 7*8 + 7 :  7*8] = data[ 7];
assign display[ 8*8 + 7 :  8*8] = data[ 8];
assign display[ 9*8 + 7 :  9*8] = data[ 9];
assign display[10*8 + 7 : 10*8] = data[10];
assign display[11*8 + 7 : 11*8] = data[11];
assign display[12*8 + 7 : 12*8] = data[12];
assign display[13*8 + 7 : 13*8] = data[13];
assign display[14*8 + 7 : 14*8] = data[14];
assign display[15*8 + 7 : 15*8] = data[15];
assign display[16*8 + 7 : 16*8] = data[16];
assign display[17*8 + 7 : 17*8] = data[17];
assign display[18*8 + 7 : 18*8] = data[18];
assign display[19*8 + 7 : 19*8] = data[19];
assign display[20*8 + 7 : 20*8] = data[20];
assign display[21*8 + 7 : 21*8] = data[21];
assign display[22*8 + 7 : 22*8] = data[22];
assign display[23*8 + 7 : 23*8] = data[23];
assign display[24*8 + 7 : 24*8] = data[24];
assign display[25*8 + 7 : 25*8] = data[25];
assign display[26*8 + 7 : 26*8] = data[26];
assign display[27*8 + 7 : 27*8] = data[27];
assign display[28*8 + 7 : 28*8] = data[28];
assign display[29*8 + 7 : 29*8] = data[29];
assign display[30*8 + 7 : 30*8] = data[30];
assign display[31*8 + 7 : 31*8] = data[31];

// instantiate display module
lcd_display lcd_display(lcd_data, lcd_en, lcd_rs, lcd_rw, lcd_ce, display, lcd_clk);

endmodule

