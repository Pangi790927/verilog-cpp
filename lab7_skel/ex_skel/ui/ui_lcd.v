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
module ui_lcd(
	CLK_50mhz,
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

parameter word_width = 16;
parameter addr_width = 12;

input CLK_50mhz;
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


wire next;
debouncer next_debounce(CLK_50mhz, BTN_next, next);
wire prev;
debouncer prev_debounce(CLK_50mhz, BTN_prev, prev);
wire rot_A;
debouncer #(.counter_value(50000)) rot_A_debounce(CLK_50mhz, BTN_rot_A, rot_A);
wire rot_B;
debouncer #(.counter_value(50000)) rot_B_debounce(CLK_50mhz, BTN_rot_B, rot_B);

wire[3*8-1 : 0] regs_name[7:0];
assign regs_name[0] = "RA ";
assign regs_name[1] = "RB ";
assign regs_name[2] = "RC ";
assign regs_name[3] = "IS ";
assign regs_name[4] = "XA ";
assign regs_name[5] = "XB ";
assign regs_name[6] = "BA ";
assign regs_name[7] = "BB ";

wire[3*8-1 : 0] res_name[7:0];
assign res_name[0] = " ST";
assign res_name[1] = " AM";
assign res_name[2] = " RI";
assign res_name[3] = " CP";
assign res_name[4] = " T1";
assign res_name[5] = "MAG";
assign res_name[6] = " T2";
assign res_name[7] = "IND";

wire[15:0] res[7:0];
assign res[0] = state;
assign res[1] = am;
assign res[2] = ri;
assign res[3] = cp;
assign res[4] = t1;
assign res[5] = mag;
assign res[6] = t2;
assign res[7] = ind;

function[7:0] to_hex;
input[3:0] nibble;
begin
    to_hex = nibble < 10 ? (nibble + "0") : (nibble - 10 + "a");
end
endfunction

`define RES     0
`define REGS    1
`define RAM     2
`define STACK   3

reg[1:0] section = `RES;

reg[2:0] res_offset = 0;
reg[2:0] regs_offset = 0;
reg[15:0] regs[3:0];
reg[9:0] ram_offset = 0;
reg[15:0] ram[3:0];

reg[7:0] display[31:0];

reg rot_A_prev = 1'b0;
reg rot_B_prev = 1'b0;
reg next_prev = 1'b0;
reg prev_prev = 1'b0;

always @(posedge CLK_50mhz) begin
    rot_A_prev <= rot_A;
    rot_B_prev <= rot_B;
    
    if(next_prev == 0 && next == 1)
        if(section == 2)
            section <= 0;
        else
            section <= section + 1;
    if(prev_prev == 0 && prev == 1)
        if(section == 0)
            section <= 2;
        else
            section <= section - 1;
    
    next_prev <= next;
    prev_prev <= prev;
        
    case(section)
        `REGS: begin
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 1 && rot_B == 0)
                regs_offset <= regs_offset + 2;
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 0 && rot_B == 1)
                regs_offset <= regs_offset - 2;
            
            display[ 0] <= regs_name[regs_offset + 0][23:16];
            display[ 1] <= regs_name[regs_offset + 0][15: 8];
            display[ 2] <= regs_name[regs_offset + 0][ 7: 0];
            display[ 3] <= to_hex(regs[0][15:12]);
            display[ 4] <= to_hex(regs[0][11: 8]);
            display[ 5] <= to_hex(regs[0][ 7: 4]);
            display[ 6] <= to_hex(regs[0][ 3: 0]);
            display[ 7] <= " ";
            display[ 8] <= " ";
            display[ 9] <= regs_name[regs_offset + 1][23:16];
            display[10] <= regs_name[regs_offset + 1][15: 8];
            display[11] <= regs_name[regs_offset + 1][ 7: 0];
            display[12] <= to_hex(regs[1][15:12]);
            display[13] <= to_hex(regs[1][11: 8]);
            display[14] <= to_hex(regs[1][ 7: 4]);
            display[15] <= to_hex(regs[1][ 3: 0]);
            display[16] <= regs_name[regs_offset + 2][23:16];
            display[17] <= regs_name[regs_offset + 2][15: 8];
            display[18] <= regs_name[regs_offset + 2][ 7: 0];
            display[19] <= to_hex(regs[2][15:12]);
            display[20] <= to_hex(regs[2][11: 8]);
            display[21] <= to_hex(regs[2][ 7: 4]);
            display[22] <= to_hex(regs[2][ 3: 0]);
            display[23] <= " ";
            display[24] <= " ";
            display[25] <= regs_name[regs_offset + 3][23:16];
            display[26] <= regs_name[regs_offset + 3][15: 8];
            display[27] <= regs_name[regs_offset + 3][ 7: 0];
            display[28] <= to_hex(regs[3][15:12]);
            display[29] <= to_hex(regs[3][11: 8]);
            display[30] <= to_hex(regs[3][ 7: 4]);
            display[31] <= to_hex(regs[3][ 3: 0]);
        end
        
        `RAM: begin
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 1 && rot_B == 0)
                ram_offset <= ram_offset + 2;
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 0 && rot_B == 1)
                ram_offset <= ram_offset - 2;
            
            display[ 0] <= to_hex(0);
            display[ 1] <= to_hex(ram_offset[ 9: 8]);
            display[ 2] <= to_hex(ram_offset[ 7: 4]);
            display[ 3] <= to_hex(ram_offset[ 3: 0]);
            display[ 4] <= ":";
            display[ 5] <= " ";
            display[ 6] <= " ";
            display[ 7] <= to_hex(ram[0][15:12]);
            display[ 8] <= to_hex(ram[0][11: 8]);
            display[ 9] <= to_hex(ram[0][ 7: 4]);
            display[10] <= to_hex(ram[0][ 3: 0]);
            display[11] <= " ";
            display[12] <= to_hex(ram[1][15:12]);
            display[13] <= to_hex(ram[1][11: 8]);
            display[14] <= to_hex(ram[1][ 7: 4]);
            display[15] <= to_hex(ram[1][ 3: 0]);
            display[16] <= to_hex(0);
            display[17] <= to_hex(((ram_offset + 2) >>  8) & 2'b11);
            display[18] <= to_hex((ram_offset + 2) >>  4);
            display[19] <= to_hex((ram_offset + 2) >>  0);
            display[20] <= ":";
            display[21] <= " ";
            display[22] <= " ";
            display[23] <= to_hex(ram[2][15:12]);
            display[24] <= to_hex(ram[2][11: 8]);
            display[25] <= to_hex(ram[2][ 7: 4]);
            display[26] <= to_hex(ram[2][ 3: 0]);
            display[27] <= " ";
            display[28] <= to_hex(ram[3][15:12]);
            display[29] <= to_hex(ram[3][11: 8]);
            display[30] <= to_hex(ram[3][ 7: 4]);
            display[31] <= to_hex(ram[3][ 3: 0]);
        end
        
        `RES: begin
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 1 && rot_B == 0)
                res_offset <= res_offset + 4;
            if(rot_A_prev == 0 && rot_B_prev == 0 && rot_A == 0 && rot_B == 1)
                res_offset <= res_offset - 4;
            
            display[ 0] <= res_name[res_offset + 0][15: 8];
            display[ 1] <= res_name[res_offset + 0][ 7: 0];
            display[ 2] <= " ";
            display[ 3] <= to_hex(res[res_offset + 0][15:12]);
            display[ 4] <= to_hex(res[res_offset + 0][11: 8]);
            display[ 5] <= to_hex(res[res_offset + 0][ 7: 4]);
            display[ 6] <= to_hex(res[res_offset + 0][ 3: 0]);
            display[ 7] <= " ";
            display[ 8] <= res_name[res_offset + 1][23:16];
            display[ 9] <= res_name[res_offset + 1][15: 8];
            display[10] <= res_name[res_offset + 1][ 7: 0];
            display[11] <= " ";
            display[12] <= to_hex(res[res_offset + 1][15:12]);
            display[13] <= to_hex(res[res_offset + 1][11: 8]);
            display[14] <= to_hex(res[res_offset + 1][ 7: 4]);
            display[15] <= to_hex(res[res_offset + 1][ 3: 0]);
            display[16] <= res_name[res_offset + 2][15: 8];
            display[17] <= res_name[res_offset + 2][ 7: 0];
            display[18] <= " ";
            display[19] <= to_hex(res[res_offset + 2][15:12]);
            display[20] <= to_hex(res[res_offset + 2][11: 8]);
            display[21] <= to_hex(res[res_offset + 2][ 7: 4]);
            display[22] <= to_hex(res[res_offset + 2][ 3: 0]);
            display[23] <= " ";
            display[24] <= res_name[res_offset + 3][23:16];
            display[25] <= res_name[res_offset + 3][15: 8];
            display[26] <= res_name[res_offset + 3][ 7: 0];
            display[27] <= " ";
            display[28] <= to_hex(res[res_offset + 3][15:12]);
            display[29] <= to_hex(res[res_offset + 3][11: 8]);
            display[30] <= to_hex(res[res_offset + 3][ 7: 4]);
            display[31] <= to_hex(res[res_offset + 3][ 3: 0]);
        end
    endcase
end

reg[1:0] regs_index = 0;
assign regs_addr[3:1] = regs_offset + regs_index;
assign regs_addr[0] = 0;
always @(posedge CLK_50mhz) begin
    regs[regs_index] <= regs_data;
    regs_index <= regs_index + 1;
end

reg[1:0] ram_index = 0;
assign ram_addr = ram_offset + ram_index;
always @(posedge CLK_50mhz) begin
    ram[ram_index - 1] <= ram_data;
    ram_index <= ram_index + 1;
end

wire[2*16*8-1 : 0] display_out;
assign display_out = {
    display[31], display[30], display[29], display[28],
    display[27], display[26], display[25], display[24],
    display[23], display[22], display[21], display[20],
    display[19], display[18], display[17], display[16],
    display[15], display[14], display[13], display[12],
    display[11], display[10], display[ 9], display[ 8],
    display[ 7], display[ 6], display[ 5], display[ 4],
    display[ 3], display[ 2], display[ 1], display[ 0]};
lcd_display lcd({LCD_db7, LCD_db6, LCD_db5, LCD_db4}, LCD_e, LCD_rs, LCD_rw, SF_ce0, display_out, CLK_50mhz);


endmodule
