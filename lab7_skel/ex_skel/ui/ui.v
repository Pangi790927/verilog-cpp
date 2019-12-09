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
module ui(
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
	VGA_blue,
	clk,
	rst,
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

parameter lcd_ui     = 0;
parameter word_width = 16;
parameter addr_width = 12;

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
output VGA_red;
output VGA_green;
output VGA_blue;
output clk;
output rst;
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

wire[4:0] clk_div;
debouncer clk_div0_debounce(CLK_50mhz, BTN_clk_div0, clk_div[0]);
debouncer clk_div1_debounce(CLK_50mhz, BTN_clk_div1, clk_div[1]);
debouncer clk_div2_debounce(CLK_50mhz, BTN_clk_div2, clk_div[2]);
assign clk_div[3] = 0;
debouncer clk_div3_debounce(CLK_50mhz, BTN_clk_div3, clk_div[4]);

wire div_clk;
divider #(.f_width(5), .counter_width(23)) clk_divider(clk_div - 1, CLK_50mhz, div_clk);

wire clk_btn;
debouncer clk_btn_debounce(CLK_50mhz, BTN_clk_btn, clk_btn);

wire clk;
assign clk = clk_div ? div_clk : clk_btn;

wire rst;
debouncer rst_debounce(CLK_50mhz, BTN_rst, rst);

wire[addr_width-1 : 0]  vga_ram_addr,  lcd_ram_addr;
wire[3 : 0]             vga_regs_addr, lcd_regs_addr;

assign ram_addr  = lcd_ui ? lcd_ram_addr  : vga_ram_addr;
assign regs_addr = lcd_ui ? lcd_regs_addr : vga_regs_addr;

ui_vga vga(CLK_50mhz, VGA_hsync, VGA_vsync, VGA_red, VGA_green, VGA_blue,
    vga_ram_addr, ram_data, vga_regs_addr, regs_data,
    state, cp, ind, am, aie, t1, t2, ri, mag);

ui_lcd lcd(CLK_50mhz, BTN_next, BTN_prev, BTN_rot_A, BTN_rot_B,
	LCD_db7, LCD_db6, LCD_db5, LCD_db4, LCD_e, LCD_rs, LCD_rw, SF_ce0,
	lcd_ram_addr, ram_data, lcd_regs_addr, regs_data,
	state, cp, ind, am, aie, t1, t2, ri, mag);

endmodule
