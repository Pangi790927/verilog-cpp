`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Create Date:    16:56:49 11/01/2013
// Module Name:    lcd
// Description: Arhitectura Calculatoarelor - Calculatorul Didactic
//              Interfata LCD
//////////////////////////////////////////////////////////////////////////////////
module lcd_display(
    output reg [3:0] sf_d,  // data bits
    output reg lcd_e,       // read/write enable
    output reg lcd_rs,      // register select
    output reg lcd_rw,      // read/write control
    output reg sf_ce0,      // enable/disable StrataFlash (High -> disabled)
    input [2*8*16-1 : 0] display_in,
    input clk);

`define INIT_STEP_1         0
`define INIT_STEP_2         1
`define INIT_STEP_3         2
`define INIT_STEP_4         3
`define INIT_STEP_5         4
`define FUNCTION_SET        5
`define ENTRY_MODE_SET      6
`define DISPLAY_ON          7
`define CLEAR_DISPLAY       8
`define CURSOR_ROW1         9
`define ROW1                10
`define CURSOR_ROW2         11
`define ROW2                12
`define WRITE_SETDATA1      13
`define WRITE_SETDATA2      14
`define WRITE_SETUP1        15
`define WRITE_SETUP2        16
`define WRITE_ENABLE1       17
`define WRITE_ENABLE2       18
`define WRITE_WAIT          19

reg[3:0] column = 0;
wire[7:0] display[31:0];

reg[7:0] state = `INIT_STEP_1;
reg[7:0] after_write_state = 0;

reg[19:0] counter = 0; 	    // used for counting the clock cycles
reg reset_counter = 0;      // 1 - reset the counter

reg[7:0] data;              // the byte that needs to be sent to the lcd controller
reg[17:0] no_cycles;        // number of cycles to wait between commands

assign display[ 0] = display_in[ 0*8+7 :  0*8];
assign display[ 1] = display_in[ 1*8+7 :  1*8];
assign display[ 2] = display_in[ 2*8+7 :  2*8];
assign display[ 3] = display_in[ 3*8+7 :  3*8];
assign display[ 4] = display_in[ 4*8+7 :  4*8];
assign display[ 5] = display_in[ 5*8+7 :  5*8];
assign display[ 6] = display_in[ 6*8+7 :  6*8];
assign display[ 7] = display_in[ 7*8+7 :  7*8];
assign display[ 8] = display_in[ 8*8+7 :  8*8];
assign display[ 9] = display_in[ 9*8+7 :  9*8];
assign display[10] = display_in[10*8+7 : 10*8];
assign display[11] = display_in[11*8+7 : 11*8];
assign display[12] = display_in[12*8+7 : 12*8];
assign display[13] = display_in[13*8+7 : 13*8];
assign display[14] = display_in[14*8+7 : 14*8];
assign display[15] = display_in[15*8+7 : 15*8];
assign display[16] = display_in[16*8+7 : 16*8];
assign display[17] = display_in[17*8+7 : 17*8];
assign display[18] = display_in[18*8+7 : 18*8];
assign display[19] = display_in[19*8+7 : 19*8];
assign display[20] = display_in[20*8+7 : 20*8];
assign display[21] = display_in[21*8+7 : 21*8];
assign display[22] = display_in[22*8+7 : 22*8];
assign display[23] = display_in[23*8+7 : 23*8];
assign display[24] = display_in[24*8+7 : 24*8];
assign display[25] = display_in[25*8+7 : 25*8];
assign display[26] = display_in[26*8+7 : 26*8];
assign display[27] = display_in[27*8+7 : 27*8];
assign display[28] = display_in[28*8+7 : 28*8];
assign display[29] = display_in[29*8+7 : 29*8];
assign display[30] = display_in[30*8+7 : 30*8];
assign display[31] = display_in[31*8+7 : 31*8];

always @(posedge clk) begin
    sf_ce0 <= 1;

    if (reset_counter) begin
        counter <= 0;
        reset_counter <= 0;
    end
    else
        counter <= counter + 1;

    case (state)
        /* Power-on INIT */
        `INIT_STEP_1: begin 	// wait 15 ms
            lcd_rs <= 0;
            lcd_e <= 0;
            lcd_rw <= 0;
            if (counter == 750000) begin
                    state <= `INIT_STEP_2;
                    reset_counter <= 1;
            end
            else
                state <= `INIT_STEP_1;
        end

        `INIT_STEP_2: begin 	// write SF_D<11:8> <= 0x3, pulse LCD_E High for 12 clock cycles.
                                // wait 4.1 ms or longer, which is 205,000 clock cycles at 50 MHz.
            sf_d <= 4'h3;
            if (counter < 12) lcd_e <= 1;
            else	lcd_e <= 0;

            if (counter == 220_000)	begin
                    state <= `INIT_STEP_3;
                    reset_counter <= 1;
            end
            else  state <= `INIT_STEP_2;
        end

        `INIT_STEP_3: begin     // write SF_D<11:8> <= 0x3, pulse LCD_E High for 12 clock cycles.
                                // wait 100 us or longer, which is 5,000 clock cycles at 50 MHz.
            sf_d <= 4'h3;
            if (counter < 12) lcd_e <= 1;
            else	lcd_e <= 0;

            if (counter == 5020) begin
                state <= `INIT_STEP_4;
                reset_counter <= 1;
            end
            else state <= `INIT_STEP_3;
        end

        `INIT_STEP_4: begin     // write SF_D<11:8> <= 0x3, pulse LCD_E High for 12 clock cycles.
                                // wait 40 us or longer, which is 2,000 clock cycles at 50 MHz.
            sf_d <= 4'h3;
            if (counter < 12) lcd_e <= 1;
            else	lcd_e <= 0;

            if (counter == 2020) begin
                state <= `INIT_STEP_5;
                reset_counter <= 1;
            end
            else state <= `INIT_STEP_4;
        end

        `INIT_STEP_5: begin     // write SF_D<11:8> <= 0x2, pulse LCD_E High for 12 clock cycles.
                                // wait 40 us or longer, which is 2,000 clock cycles at 50 MHz.
            sf_d <= 4'h2;
            if (counter < 12) lcd_e <= 1;
            else	lcd_e <= 0;

            if (counter == 2020) begin
                state <= `FUNCTION_SET;
                reset_counter <= 1;
            end
            else state <= `INIT_STEP_5;
        end

            /* Display configuration */

        `FUNCTION_SET: begin
            data <= 8'h28;      // Function Set command code
            no_cycles <= 2000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            after_write_state <= `ENTRY_MODE_SET;
        end

        `ENTRY_MODE_SET: begin
            data <= 8'h06;      // Entry Mode Set command code, enable cursor auto-increment
            no_cycles <= 2000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            after_write_state <= `DISPLAY_ON;
        end

        `DISPLAY_ON: begin
            //		- Display characters stored in DD RAM
            //		- No cursor
            //		- No cursor blinking
            data <= 8'h0c;      // Display On command code
            no_cycles <= 2000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            after_write_state <= `CLEAR_DISPLAY;
        end

        `CLEAR_DISPLAY: begin
            data <= 8'h01;      // Clear Display command code
            no_cycles <= 100_000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            after_write_state <= `CURSOR_ROW1;
        end

        /* Display write */

        `CURSOR_ROW1: begin
            data <= {1'b1, 7'h00};
            no_cycles <= 2_000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            column <= 0;
            after_write_state <= `ROW1;
        end

        `ROW1: begin
            data <= display[0 + column];
            no_cycles <= 2_000;
            lcd_rs <= 1;
            state <= `WRITE_SETDATA1;
            column <= column + 1;
            if(column == 15)
                after_write_state <= `CURSOR_ROW2;
            else
                after_write_state <= `ROW1;
        end

        `CURSOR_ROW2: begin
            data <= {1'b1, 7'h40};
            no_cycles <= 2_000;
            lcd_rs <= 0;
            state <= `WRITE_SETDATA1;
            column <= 0;
            after_write_state <= `ROW2;
        end

        `ROW2: begin
            data <= display[16 + column];
            no_cycles <= 2_000;
            lcd_rs <= 1;
            state <= `WRITE_SETDATA1;
            column <= column + 1;
            if(column == 15)
                after_write_state <= `CURSOR_ROW1;
            else
                after_write_state <= `ROW2;
        end

        /* Data transfer */

        `WRITE_SETDATA1: begin
            lcd_e <= 0;
            // put on the data lines (sf_d) the first half of the data that needs to be sent
            sf_d <= data[7:4];
            state <= `WRITE_SETUP1;
            reset_counter <= 1;
        end

        `WRITE_SETUP1: begin
            lcd_e <= 0;
            // control signals must be set up and stable at least 40 ns
            if (counter == 2) begin
                state <= `WRITE_ENABLE1;
                reset_counter <= 1;
            end
            else state <= `WRITE_SETUP1;
        end

        `WRITE_ENABLE1: begin
            lcd_e <= 1;
            // enable signal must remain High for 230 ns or longer
            if (counter == 12) begin
                state <= `WRITE_SETDATA2;
                reset_counter <= 1;
            end
            else state <= `WRITE_ENABLE1;
        end

        `WRITE_SETDATA2: begin
            // put on the data lines (sf_d) the second half of the data that needs to be sent
            sf_d <= data[3:0];
            lcd_e <= 0;
            // wait at least 1us before transferring the data
            if (counter == 51) begin
                state <= `WRITE_SETUP2;
                reset_counter <= 1;
            end
            else state <= `WRITE_SETDATA2;
        end

        `WRITE_SETUP2: begin
            lcd_e <= 0;
            // control signals must be set up and stable at least 40 ns
            if (counter == 2) begin
                state <= `WRITE_ENABLE2;
                reset_counter <= 1;
            end
            else state <= `WRITE_SETUP2;
        end

        `WRITE_ENABLE2: begin
            lcd_e <= 1;
            // enable signal must remain High for 230 ns or longer
            if (counter == 12) begin
                state <= `WRITE_WAIT;
                reset_counter <= 1;
            end
            else state <= `WRITE_ENABLE2;
        end

        `WRITE_WAIT: begin
            lcd_e <= 0;
            // 8-bit write operations must be have 40 us between them
            if (counter == (1 + no_cycles)) begin
                state <= after_write_state;
                reset_counter <= 1;
            end
            else state <= `WRITE_WAIT;
        end

        default: begin                  // should not enter here
            after_write_state <= 0;
            state <= 0;
            data <= 0;
            no_cycles <= 0;
        end
    endcase
end

endmodule
