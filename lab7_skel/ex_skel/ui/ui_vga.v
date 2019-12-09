`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:02:04 11/12/2012 
// Design Name: 
// Module Name:    ui 
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
module ui_vga(
	CLK_50mhz,
	VGA_hsync,
	VGA_vsync,
	VGA_red,
	VGA_green,
	VGA_blue,
	ram_addr,
	ram_data,
	regs_addr,
	regs_data,
	state,
	cp,
	ind,
	am,
	aie,
	t1,
	t2,
	ri,
	mag);

parameter black	= 0;
parameter blue		= 1;
parameter green	= 2;
parameter cyan		= 3;
parameter red		= 4;
parameter magenta	= 5;
parameter yellow	= 6;
parameter white	= 7;

parameter word_width = 16;
parameter addr_width = 12;

input CLK_50mhz;
output VGA_hsync;
output VGA_vsync;
output VGA_red;
output VGA_green;
output VGA_blue;
output[addr_width-1 : 0] ram_addr;
input[word_width-1 : 0] ram_data;
output[3 : 0] regs_addr;
input[word_width-1 : 0] regs_data;
input[word_width-1 : 0] state;
input[word_width-1 : 0] cp;
input[word_width-1 : 0] ind;
input[word_width-1 : 0] am;
input[word_width-1 : 0] aie;
input[word_width-1 : 0] t1;
input[word_width-1 : 0] t2;
input[word_width-1 : 0] ri;
input[word_width-1 : 0] mag;

wire[2:0] rgb;

//synthesis attribute box_type char_rom "black_box"
wire[9:0] char_addr;
wire[7:0] char_data;
char_rom char_rom(
	.clka(CLK_50mhz),		// input clka
	.addra(char_addr),	// input [9 : 0] addra
	.douta(char_data)		// output [7 : 0] douta
);

wire active;
wire[9:0] x;
wire[9:0] y;
VGATimer #(800,64,120,56, 600,23,6,37, 1) timer (CLK_50mhz, VGA_hsync, VGA_vsync, active, x, y);

wire[word_width-1 : 0] res[8:0];
assign res[0] = state;
assign res[1] = ri;
assign res[2] = cp;
assign res[3] = am;
assign res[4] = aie;
assign res[5] = mag;
assign res[6] = t1;
assign res[7] = t2;
assign res[8] = ind;

wire[7:0] regs_title[7:0];
assign regs_title[0] = "R";
assign regs_title[1] = "e";
assign regs_title[2] = "g";
assign regs_title[3] = "i";
assign regs_title[4] = "s";
assign regs_title[5] = "t";
assign regs_title[6] = "r";
assign regs_title[7] = "e";

wire[7:0] res_title[7:0];
assign res_title[0] = "R";
assign res_title[1] = "e";
assign res_title[2] = "s";
assign res_title[3] = "u";
assign res_title[4] = "r";
assign res_title[5] = "s";
assign res_title[6] = "e";
assign res_title[7] = " ";

wire[7:0] stack_title[7:0];
assign stack_title[0] = "S";
assign stack_title[1] = "t";
assign stack_title[2] = "i";
assign stack_title[3] = "v";
assign stack_title[4] = "a";
assign stack_title[5] = " ";
assign stack_title[6] = " ";
assign stack_title[7] = " ";

wire[7:0] data_title[7:0];
assign data_title[0] = "M";
assign data_title[1] = "e";
assign data_title[2] = "m";
assign data_title[3] = "o";
assign data_title[4] = "r";
assign data_title[5] = "i";
assign data_title[6] = "a";
assign data_title[7] = " ";

wire[4*8-1 : 0] regs_name[7:0];
assign regs_name[0] = "RA  ";
assign regs_name[1] = "RB  ";
assign regs_name[2] = "RC  ";
assign regs_name[3] = "IS  ";
assign regs_name[4] = "XA  ";
assign regs_name[5] = "XB  ";
assign regs_name[6] = "BA  ";
assign regs_name[7] = "BB  ";

wire[4*8-1 : 0] res_name[8:0];
assign res_name[0] = "ST  ";
assign res_name[1] = "RI  ";
assign res_name[2] = "CP  ";
assign res_name[3] = "AM  ";
assign res_name[4] = "AIE ";
assign res_name[5] = "MAG ";
assign res_name[6] = "T1  ";
assign res_name[7] = "T2  ";
assign res_name[8] = "IND ";

wire valid_regs_title_addr;
wire[2:0] regs_title_addr;
wire[9:0] regs_title_char_addr;
wire[2:0] regs_title_rgb;
text_display #(
		.pos_x(30 - 1), .pos_y(19),
		.rows(1), .cols(8),
		.addr_width(3), .data_width(8),
		.data_delay(0),
		.char_delay(1))
	regs_title_display(x, y, valid_regs_title_addr, regs_title_addr, regs_title[regs_title_addr], regs_title_char_addr, char_data, regs_title_rgb);
	
wire valid_regs_name_addr;
wire[3:0] regs_name_addr;
wire[9:0] regs_name_char_addr;
wire[2:0] regs_name_rgb;
text_display #(
		.pos_x(30 - 1), .pos_y(58),
		.rows(15), .cols(1),
		.odd_row_even_col_fg(black),
		.addr_width(4), .data_width(4 * 8),
		.data_delay(0),
		.char_delay(1))
	regs_name_display(x, y, valid_regs_name_addr, regs_name_addr, regs_name[regs_name_addr / 2], regs_name_char_addr, char_data, regs_name_rgb);

wire valid_regs_addr;
wire[9:0] regs_char_addr;
wire[2:0] regs_rgb;
hex_display #(
		.pos_x(62 - 1), .pos_y(58),
		.rows(15), .cols(1),
		.even_row_even_col_bg(yellow),
		.odd_row_even_col_bg(yellow),
		.even_row_even_col_fg(black),
		.odd_row_even_col_fg(yellow),
		.addr_width(4),
		.data_delay(0),
		.char_delay(1))
	regs_display(x, y, valid_regs_addr, regs_addr, regs_data, regs_char_addr, char_data, regs_rgb);

wire valid_res_title_addr;
wire[2:0] res_title_addr;
wire[9:0] res_title_char_addr;
wire[2:0] res_title_rgb;
text_display #(
		.pos_x(34 - 1), .pos_y(395),
		.rows(1), .cols(8),
		.addr_width(3), .data_width(8),
		.data_delay(0),
		.char_delay(1))
	res_title_display(x, y, valid_res_title_addr, res_title_addr, res_title[res_title_addr], res_title_char_addr, char_data, res_title_rgb);
	
wire valid_res_name_addr;
wire[4:0] res_name_addr;
wire[9:0] res_name_char_addr;
wire[2:0] res_name_rgb;
text_display #(
		.pos_x(30 - 1), .pos_y(434),
		.rows(17), .cols(1),
		.odd_row_even_col_fg(black),
		.addr_width(5), .data_width(4 * 8),
		.data_delay(0),
		.char_delay(1))
	res_name_display(x, y, valid_res_name_addr, res_name_addr, res_name[res_name_addr / 2], res_name_char_addr, char_data, res_name_rgb);

wire valid_res_addr;
wire[4:0] res_addr;
wire[9:0] res_char_addr;
wire[2:0] res_rgb;
hex_display #(
		.pos_x(62 - 1), .pos_y(434),
		.rows(17), .cols(1),
		.even_row_even_col_bg(yellow),
		.odd_row_even_col_bg(yellow),
		.even_row_even_col_fg(black),
		.odd_row_even_col_fg(yellow),
		.addr_width(5),
		.data_delay(0),
		.char_delay(1))
	res_display(x, y, valid_res_addr, res_addr, res[res_addr / 2], res_char_addr, char_data, res_rgb);

wire valid_stack_title_addr;
wire[2:0] stack_title_addr;
wire[9:0] stack_title_char_addr;
wire[2:0] stack_title_rgb;
text_display #(
		.pos_x(138 - 1), .pos_y(19),
		.rows(1), .cols(8),
		.addr_width(3), .data_width(8),
		.data_delay(0),
		.char_delay(1))
	stack_title_display(x, y, valid_stack_title_addr, stack_title_addr, stack_title[stack_title_addr], stack_title_char_addr, char_data, stack_title_rgb);

wire valid_stack_row_addr;
wire[11:0] stack_row_addr;
wire[9:0] stack_row_char_addr;
wire[2:0] stack_row_rgb;
hex_display #(
		.pos_x(124 - 1), .pos_y(58),
		.rows(64), .cols(1),
		.data_delay(0),
		.char_delay(1))
	stack_row_display(x, y, valid_stack_row_addr, stack_row_addr, stack_row_addr + 60 * 16, stack_row_char_addr, char_data, stack_row_rgb);

wire valid_stack_addr;
wire[11:0] stack_addr;
wire[9:0] stack_char_addr;
wire[2:0] stack_rgb;
hex_display #(
		.pos_x(160 - 2), .pos_y(58),
		.rows(64), .cols(1),
		.even_row_even_col_bg(yellow),
		.odd_row_even_col_bg(yellow),
		.even_row_even_col_fg(black),
		.even_row_odd_col_fg(black),
		.odd_row_even_col_fg(blue),
		.odd_row_odd_col_fg(blue),
		.data_delay(1),
		.char_delay(1))
	stack_display(x, y, valid_stack_addr, stack_addr, ram_data, stack_char_addr, char_data, stack_rgb);

wire valid_data_title_addr;
wire[2:0] data_title_addr;
wire[9:0] data_title_char_addr;
wire[2:0] data_title_rgb;
text_display #(
		.pos_x(468 - 1), .pos_y(19),
		.rows(1), .cols(8),
		.addr_width(3), .data_width(8),
		.data_delay(0),
		.char_delay(1))
	data_title_display(x, y, valid_data_title_addr, data_title_addr, data_title[data_title_addr], data_title_char_addr, char_data, data_title_rgb);

wire valid_data_row_addr;
wire[11:0] data_row_addr;
wire[9:0] data_row_char_addr;
wire[2:0] data_row_rgb;
hex_display #(
		.pos_x(222 - 1), .pos_y(58),
		.rows(64), .cols(1),
		.data_delay(0),
		.char_delay(1))
	data_row_display(x, y, valid_data_row_addr, data_row_addr, data_row_addr * 16, data_row_char_addr, char_data, data_row_rgb);

wire valid_data_col_addr;
wire[11:0] data_col_addr;
wire[9:0] data_col_char_addr;
wire[2:0] data_col_rgb;
hex_display #(
		.pos_x(258 - 1), .pos_y(46),
		.rows(1), .cols(16),
		.even_row_even_col_bg(yellow),
		.odd_row_even_col_bg(yellow),
		.even_row_odd_col_bg(cyan),
		.odd_row_odd_col_bg(cyan),
		.even_row_even_col_fg(blue),
		.even_row_odd_col_fg(blue),
		.data_delay(0),
		.char_delay(1))
	data_col_display(x, y, valid_data_col_addr, data_col_addr, data_col_addr % 16, data_col_char_addr, char_data, data_col_rgb);

wire valid_data_addr;
wire[11:0] data_addr;
wire[9:0] data_char_addr;
wire[2:0] data_rgb;
hex_display #(
		.pos_x(258 - 2), .pos_y(58),
		.rows(64), .cols(16),
		.even_row_even_col_bg(yellow),
		.odd_row_even_col_bg(yellow),
		.even_row_odd_col_bg(cyan),
		.odd_row_odd_col_bg(cyan),
		.even_row_even_col_fg(black),
		.even_row_odd_col_fg(black),
		.odd_row_even_col_fg(blue),
		.odd_row_odd_col_fg(blue),
		.data_delay(1),
		.char_delay(1))
	data_display(x, y, valid_data_addr, data_addr, ram_data, data_char_addr, char_data, data_rgb);

assign ram_addr =		(valid_stack_addr ? (stack_addr + 60 * 16) : 0) |
							data_addr;

assign char_addr = 	regs_title_char_addr | regs_name_char_addr | regs_char_addr |
							res_title_char_addr | res_name_char_addr | res_char_addr |
							stack_title_char_addr | stack_char_addr | stack_row_char_addr |
							data_title_char_addr | data_char_addr | data_row_char_addr | data_col_char_addr;

assign rgb =			regs_title_rgb | regs_name_rgb | regs_rgb |
							res_title_rgb | res_name_rgb | res_rgb |
							stack_title_rgb | stack_rgb | stack_row_rgb |
							data_title_rgb | data_rgb | data_row_rgb | data_col_rgb;

assign {VGA_red, VGA_green, VGA_blue} = active ? rgb : 0;

endmodule
