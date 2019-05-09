`include "verilog_src/control_pins.v"
`include "verilog_src/mobo_ctrl.v"
`include "verilog_src/address_translate.v"
`include "verilog_src/cpu.v"

`include "verilog_src/mobo_vga.v"
`include "verilog_src/mobo_ram.v"
`include "verilog_src/mobo_states.v"

`include "test.v"

module mobo(
		/* clk, rst - those are required on all devices */
		input				clk,
		input				rst,

		/* The connected ram */
		input 		[word_width-1:0]	ram_stat,
		output 	reg [word_width-1:0]	ram_ctrl,

		/* The connected vga */
		input 		[word_width-1:0]	vga_stat,
		output 	reg [word_width-1:0]	vga_ctrl,

		/* Data buss for devices */
		output	reg	[word_width-1:0]	addr,
		input		[word_width-1:0]	data_in,
		output	reg	[word_width-1:0]	data_out
	);

	parameter word_width = 32;
	
	/* Intern state, usefull for waiting for device response */
	reg [word_width-1:0] state 		= 0;
	reg [word_width-1:0] next_state = 0;
	reg [word_width-1:0] ram_state	= 0;

	/* Those are from cpu perspective */
	wire [word_width-1 : 0] mobo_ctrl;
	reg [word_width-1 : 0] mobo_stat = 0;
	reg [word_width-1 : 0] mobo_stat_next = `STAT_IDLE;
	wire [word_width-1 : 0] cpu_addr;
	wire [word_width-1 : 0] cpu_data_out;
	reg [word_width-1 : 0] cpu_data_in = 0;

	/* The connected cpu */
	cpu cpu(clk, rst, mobo_ctrl, mobo_stat,
			cpu_addr, cpu_data_out, cpu_data_in);

	wire [word_width-1 : 0] transl_addr;
	wire [word_width-1 : 0] transl_state;
	addr_translate addr_translate(cpu_addr, transl_addr, transl_state);

	mobo_vga mobo_vga(state, next_state, transl_addr, cpu_data_in,
			cpu_data_out, ram_stat, ram_ctrl, addr, data_in, data_out);
	mobo_ram mobo_ram(state, next_state, transl_addr, cpu_data_in,
			cpu_data_out, vga_stat, vga_ctrl, addr, data_in, data_out);

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			state <= `MOBO_IDLE;
		end
		else begin
			state <= next_state;
			mobo_stat <= mobo_stat_next;
		end
	end

	always @(*) begin
		case (state)
			`MOBO_IDLE: begin
				if (mobo_ctrl != 0) begin
					case (transl_state)
						`VGA_STATE: begin
							if ((mobo_ctrl & `CTRL_WRITE) != 0)
								next_state = `MOBO_VGA_WRITE;
							if ((mobo_ctrl & `CTRL_READ) != 0)
								next_state = `MOBO_VGA_READ;
						end
						`RAM_STATE: begin
							if ((mobo_ctrl & `CTRL_WRITE) != 0)
								next_state = `MOBO_RAM_WRITE;
							if ((mobo_ctrl & `CTRL_READ) != 0)
								next_state = `MOBO_RAM_READ;
						end
						`ERR_STATE:
							next_state = `MOBO_ERR_ADDR;
					endcase
				end
			end

			/* MOBO VGA READ/WRITE present in mobo_vga */
			/* MOBO RAM READ/WRITE present in mobo_ram */

			`MOBO_ERR_ADDR:
				next_state = `MOBO_DONE;

			`MOBO_DONE: begin
				$display("mobo_ctrl %d", mobo_ctrl);
				mobo_stat_next = `STAT_DONE;
				if (mobo_ctrl != 0)
					next_state = `MOBO_DONE;
				else begin
					mobo_stat_next = `STAT_IDLE;
					next_state = `MOBO_IDLE;
				end
			end
		endcase
	end
endmodule

// struct comm_flags {
// 	ram_oe
// 	ram_we
// 	regs_oe
// }
// // -----------------------------------------

// always @(posedge clk) begin
// 	if (rst) begin
// 		// reset
// 	end
// 	else if (clk) begin
// 		state = next_state;
// 		decode_state = next_decode_state;
// 	end
// end

// `define fetch_state			16'h000
// `define decode_state		16'h010
// `define add_state			16'h020

// state <> next_state
// alu_op <> next_alu_op

// cpu_add(state, next_state, comm_ctrl)
// cpu_decode(state, next_state, comm_ctrl)
// cpu_fetch(state, next_state, comm_ctrl)
// cpu_inc_cp(state, next_state, comm_ctrl)
// cpu_decode(state, next_state, comm_ctrl)
// // -----------------------------------------
// module comm_fetch(state, )
// 	always @(*) begin
// 		case (state)
// 			`fetch_state: begin
// 				next_state = next_state + 1;
// 				next_state = `fetch_state + 1;
// 			end
// 			`fetch_state + 1: begin
// 				next_state = `fetch_state + 2;
// 			end
// 			`fetch_state + 2: begin
// 				next_state = `decode_state;
// 			end
// 		endcase
// 	end
// endmodule
// // -----------------------------------------
// add ra, [ba + addr]
// module comm_fetch(...)
// 	always @(*) begin
// 		case (state)
// 			`comm_decode: begin
// 				// citeste din ram ri
// 				next_state = `comm_decode + 1;
// 			end
// 			`comm_decode + 1: begin
// 				// decizi ce faci cu comanda
// 				next_state = `comm_decode + 2;
// 			end
// 			`comm_decode + 2: begin
// 				next_state = add;
// 			end
// 		endcase
// 	end
// endmodule
// // -----------------------------------------

// add ra, [ba + depl]
// add [ba + addr], operand

// [    ri    ][   addr   ][   depl   ]
// add [ba + 0xff23], 100 // ba = 16
// // fetch 
// // decode
// // 

// t1, t2
// mov ra, 1
// add ba, bb
// call func // foloseste ra
