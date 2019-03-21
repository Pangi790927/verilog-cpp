`include "control_pins.v"
`include "mobo_states.v"

`include "verilog_include/register.v"

module mobo(
		// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input 		[31:0]	ram_stat,
		output 	reg [31:0]	ram_ctrl,

		// ctrl, statu
		input 		[31:0]	vga_stat,
		output 	reg [31:0]	vga_ctrl,

		// asta ara busul
		output	reg	[31:0]	addr,
		input		[31:0]	data_in,
		output	reg	[31:0]	data_out
	);
	
	reg [31:0] state 		= 0;
	reg [31:0] next_state 	= 0;
	reg [31:0] ram_state 	= 0;
	// var2 = 0;

	reg [31:0] vga_data		= 0;
	reg [15:0] vga_addr		= 0;
	reg vga_oe 				= 0;
	reg vga_we 				= 0;

	// instantiate AM register and related connections
	reg							am_oe;
	reg							am_we;
	reg[31 : 0]					am_in;
	reg[31 : 0]					am_out;
	register am(clk, rst, am_oe, am_we, am_in, am_out);
	// vga vga(clk, rst, );
	reg [31 : 0] idx = 0;

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// reset
			state <= 0;
		end
		else begin
			state <= next_state;

			if (ram_state == 1) begin
				idx += 1;
			end
		end
	end

	always @(*) begin
		//$display("(%d) CLC start", idx);
		ram_state = 0;

		case(state)
			`M_STATE_WRITE: begin
				// $display("(%d) write ram", idx);

				if (ram_stat[`RAM_ACK] == 0) begin //advance to the next state only when the physical chip is done doing its job
					ram_ctrl[`RAM_WRITE_PIN] = 1;
					addr = idx;
					data_out = idx;

					next_state = `M_STATE_WAIT_WRITE;
				end
			end
			`M_STATE_WAIT_WRITE: begin
				// $display("(%d) waiting for ram to write", idx);

				if (ram_stat[`RAM_ACK]) begin
					// here ram responded to us
					ram_ctrl = 0;
					// $display("(%d) ACK write, addr %x", idx, addr);

					next_state = `M_STATE_READ;
				end
			end
			`M_STATE_READ: begin
				// $display("(%d) ask ram", idx);

				if (ram_stat[`RAM_ACK] == 0) begin //advance to the next state only when the physical chip is done doing its job
					ram_ctrl[`RAM_READ_PIN] = 1;
					addr = idx;

					next_state = `M_STATE_WAIT_READ;
				end
			end

			`M_STATE_WAIT_READ: begin
				// $display("(%d) waiting for RAM respond", idx);

				if (ram_stat[`RAM_ACK]) begin
					// here ram responded to us
					ram_ctrl = 0;
					// $display("(%d) ACK read: %d", idx, data_from_hw);

					next_state = `M_STATE_VGA_WRITE;
					// ram_state = 1;
				end
			end
			`M_STATE_VGA_WRITE: begin
				// $display("(%d) write to VGA", idx);
				if (vga_stat[`VGA_ACK] == 0) begin
					vga_ctrl[`VGA_WRITE_PIN] = 1;

					addr = idx;
					data_out = 32'h000200 + data_in;

					next_state = `M_STATE_VGA_WAIT;
				end
			end
			`M_STATE_VGA_WAIT: begin
				// $display("(%d) waiting for VGA respond", idx);
				if (vga_stat[`VGA_ACK] && idx < 2000) begin
					vga_ctrl = 0;
					// $display("(%d) Done displaying current char", idx);

					next_state = `M_STATE_WRITE;
					ram_state = 1;
				end
			end
		endcase

		//$display("(%d) CLC end", idx);
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

// comm_add(state, next_state, comm_ctrl)
// comm_decode(state, next_state, comm_ctrl)
// comm_fetch(state, next_state, comm_ctrl)
// comm_inc_cp(state, next_state, comm_ctrl)
// comm_decode(state, next_state, comm_ctrl)
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
