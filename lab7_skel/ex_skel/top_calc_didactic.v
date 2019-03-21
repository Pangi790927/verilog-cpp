`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    11:15:42 10/22/2011 
// Design Name: 
// Module Name:    top_calc_didactic 
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
module top_calc_didactic(
    CLK_50mhz,
    BTN_clk_div0,
    BTN_clk_div1,
    BTN_clk_div2,
    BTN_clk_div3,
    BTN_clk_btn,
    BTN_rst,
    BTN_next,
	BTN_prev,
	BTN_rot_A,
	BTN_rot_B,
	LCD_db7,
	LCD_db6,
	LCD_db5,
	LCD_db4,
	LCD_e,
    LCD_rs,
    LCD_rw,
    SF_ce0,
    VGA_hsync,
    VGA_vsync,
    VGA_red,
    VGA_green,
    VGA_blue);

parameter lcd_ui     = 0;
parameter word_width = 16;
parameter addr_width = 12;
parameter port_width = 8;
			
input CLK_50mhz;
input BTN_clk_div0;
input BTN_clk_div1;
input BTN_clk_div2;
input BTN_clk_div3;
input BTN_clk_btn;
input BTN_rst;
input BTN_next;
input BTN_prev;
input BTN_rot_A;
input BTN_rot_B;
output LCD_db7;
output LCD_db6;
output LCD_db5;
output LCD_db4;
output LCD_e;
output LCD_rs;
output LCD_rw;
output SF_ce0;
output VGA_hsync;
output VGA_vsync;
output VGA_red, VGA_green, VGA_blue;

wire clk;
wire rst;
wire io_oe;
wire io_we;
wire[port_width-1 : 0]	io_port;
wire[word_width-1 : 0]	io_in;
wire[word_width-1 : 0]	io_out;
wire[addr_width-1 : 0]	ram_addr;
wire[word_width-1 : 0]	ram_data;
wire[3 : 0]					regs_addr;
wire[word_width-1 : 0]	regs_data;
wire[word_width-1 : 0]	state;
wire[word_width-1 : 0]	cp;
wire[word_width-1 : 0]	ind;
wire[word_width-1 : 0]	am, aie;
wire[word_width-1 : 0]	t1, t2;
wire[word_width-1 : 0]	ri;
wire[word_width-1 : 0]	mag;

wire[3:0]               ui_sf_d, io_sf_d;
wire                    ui_lcd_e, io_lcd_e;
wire                    ui_lcd_rs, io_lcd_rs;
wire                    ui_lcd_rw, io_lcd_rw;
wire                    ui_sf_ce0, io_sf_ce0;

assign {LCD_db7, LCD_db6, LCD_db5, LCD_db4} = lcd_ui ? ui_sf_d : io_sf_d;
assign LCD_e = lcd_ui ? ui_lcd_e : io_lcd_e;
assign LCD_rs = lcd_ui ? ui_lcd_rs : io_lcd_rs;
assign LCD_rw = lcd_ui ? ui_lcd_rw : io_lcd_rw;
assign SF_ce0 = lcd_ui ? ui_sf_ce0 : io_sf_ce0;

ui #(.lcd_ui(lcd_ui)) ui(CLK_50mhz, BTN_clk_div0, BTN_clk_div1, BTN_clk_div2, BTN_clk_div3, BTN_clk_btn, BTN_rst,     BTN_next,
	BTN_prev,
	BTN_rot_A,
	BTN_rot_B,
	ui_sf_d[3],
	ui_sf_d[2],
	ui_sf_d[1],
	ui_sf_d[0],
	ui_lcd_e,
    ui_lcd_rs,
    ui_lcd_rw,
    ui_sf_ce0, VGA_hsync, VGA_vsync, VGA_red, VGA_green, VGA_blue, clk, rst, ram_addr, ram_data, regs_addr, regs_data, state, cp, ind, am, aie, t1, t2, ri, mag);

lcd lcd(clk, io_oe, io_we, io_port, io_in, io_out, CLK_50mhz, io_sf_d, io_lcd_e, io_lcd_rs, io_lcd_rw, io_sf_ce0);

calc_didactic calc_didactic(clk, rst, io_oe, io_we, io_port, io_in, io_out, CLK_50mhz, ram_addr, ram_data, regs_addr / 2, regs_data, state, cp, ind, am, aie, t1, t2, ri, mag);

endmodule
