`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    18:02:00 11/21/2011 
// Design Name: 
// Module Name:    hex_display 
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
module hex_display(
		x, y,
		valid_addr, addr, data,
		char_addr, char_data,
		rgb
	);

// screen position
parameter pos_x = 0;
parameter pos_y = 0;
// log2 of max position
parameter x_width = 10;
parameter y_width = 10;

// display size
parameter rows = 1;
parameter cols = 1;

// foreground color
parameter even_row_even_col_fg	= 7;		// white
parameter even_row_odd_col_fg		= 7;		// white
parameter odd_row_even_col_fg		= 7;		// white
parameter odd_row_odd_col_fg		= 7;		// white

// background color
parameter even_row_even_col_bg	= 0;		// black
parameter even_row_odd_col_bg		= 0;		// black
parameter odd_row_even_col_bg		= 0;		// black
parameter odd_row_odd_col_bg		= 0;		// black

// memory address width
parameter addr_width = 16;
// memory data width
parameter data_width = 16;
// memory address to data delay
parameter data_delay = 1;

// character size on screen
parameter char_rows = 8;
parameter char_cols = 8;
// log2 of character size
parameter char_row_width = 3;
parameter char_col_width = 3;
// character ROM address to data delay
parameter char_delay = 1;

// no need to modify these parameters
// word size on screen
parameter word_rows = char_rows;
parameter word_cols = (data_width / 4) * char_cols;
// character ROM address width
parameter char_addr_width = char_row_width + 8;


input	[x_width-1 : 0]			x;
input	[y_width-1 : 0]			y;
output								valid_addr;
output[addr_width-1 : 0]		addr;
input	[data_width-1 : 0]		data;
output[char_addr_width-1 : 0]	char_addr;
input	[0 : char_cols-1]			char_data;
output[2 : 0]						rgb;

wire[x_width-1 : 0]				area_x_data, area_x_char, area_x_rgb, col_data, col_char, col_rgb;
wire[y_width-1 : 0]				area_y, row;
wire									active_data, active_char, active_rgb;
wire[(data_width / 4)-1 : 0]	nibble_idx;
wire[3 : 0]							nibble;
wire[7 : 0]							char;
wire[char_row_width-1 : 0]		char_row;
wire[char_col_width-1 : 0]		char_col;
wire[char_addr_width-1 : 0]	char_addr;
wire[2:0]							foreground[3:0];
wire[2:0]							background[3:0];

assign foreground[0] = even_row_even_col_fg;
assign foreground[1] = even_row_odd_col_fg;
assign foreground[2] = odd_row_even_col_fg;
assign foreground[3] = odd_row_odd_col_fg;

assign background[0] = even_row_even_col_bg;
assign background[1] = even_row_odd_col_bg;
assign background[2] = odd_row_even_col_bg;
assign background[3] = odd_row_odd_col_bg;

assign area_x_data = x - pos_x;
assign area_x_char = area_x_data - data_delay;
assign area_x_rgb = area_x_char - char_delay;
assign area_y = y - pos_y;

assign col_data = area_x_data / word_cols;
assign col_char = area_x_char / word_cols;
assign col_rgb = area_x_rgb / word_cols;
assign row = area_y / word_rows;

assign active_data = (col_data >= 0) && (col_data < cols) && (row >= 0) && (row < rows);
assign active_char = (col_char >= 0) && (col_char < cols) && (row >= 0) && (row < rows);
assign active_rgb = (col_rgb >= 0) && (col_rgb < cols) && (row >= 0) && (row < rows);

assign valid_addr = active_data;
assign addr = active_data ? (row * cols + col_data) : 0;

assign nibble_idx = ((data_width / 4) - 1) - (area_x_char % word_cols) / char_cols;
assign nibble = {	data[{nibble_idx, 2'h3}],
						data[{nibble_idx, 2'h2}],
						data[{nibble_idx, 2'h1}],
						data[{nibble_idx, 2'h0}]};
assign char = nibble < 10 ? (nibble + 8'h30) : (nibble - 10 + 8'h61);
assign char_col = area_x_rgb % char_cols;
assign char_row = area_y % char_rows;
assign char_addr = active_char ? (char * char_cols + char_row) : 0;

assign rgb = active_rgb ? (char_data[char_col] ? foreground[{row[0], col_rgb[0]}] : background[{row[0], col_rgb[0]}]) : 0;

endmodule
