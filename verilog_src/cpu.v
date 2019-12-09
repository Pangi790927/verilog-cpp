`include "verilog_src/global_const.v"
`include "verilog_src/alu.v"
`include "verilog_src/register.v"
`include "verilog_src/cpu_states.v"
`include "verilog_src/bus.v"
`include "verilog_src/mobo_ctrl.v"
`include "verilog_src/mobo_states.v"
`include "verilog_src/cpu_stack_def.v"
`include "verilog_src/cpu_fcn_rw.v"

module cpu(
		input		clk,
		input		rst,
		output	reg	[word_width-1 : 0] mobo_ctrl,
		input		[word_width-1 : 0] mobo_stat,
		output	reg	[word_width-1 : 0] addr_out,
		output	reg	[word_width-1 : 0] mobodat_out,
		input		[word_width-1 : 0] mobodat_in
	);
	`include "verilog_src/cpp_instr_debug.v"
	`include "verilog_src/cpu_stack_fcn.v"

	parameter word_width = `WORD_WIDTH;

	state_t state 		= '{0, 0, 0};
	state_t next_state	= '{0, 0, 0};

	// instantiate T1 register and related connections
	logic                        t1_oe;
	logic                        t1_we;
	wire[word_width-1 : 0]      t1_in;
	wire[word_width-1 : 0]      t1_out;
	register #(word_width) t1(clk, rst, t1_oe, t1_we, t1_in, t1_out);

	// instantiate T2 register and related connections
	logic                        t2_oe;
	logic                        t2_we;
	wire[word_width-1 : 0]      t2_in;
	wire[word_width-1 : 0]      t2_out;
	register #(word_width) t2(clk, rst, t2_oe, t2_we, t2_in, t2_out);

	// instantiate ALU and related connections
	wire                        alu_oe;
	wire[3 : 0]                 alu_opcode;
	wire                        alu_carry;
	wire[word_width-1 : 0]      alu_out;
	wire[4 : 0]                 alu_flags;
	alu #(word_width, 5) alu(alu_oe, alu_opcode, t1_out, t2_out, alu_carry, alu_out, alu_flags);

	logic 						addr_oe;
	logic 						addr_we;
	wire [word_width-1 : 0]		addr_in;
	register addr_reg(clk, rst, addr_oe, addr_we, addr_in, addr_out);
	/* cpu -> we -> addr -> oe -> mobo */

	logic 						mobodat_out_oe;
	logic 						mobodat_out_we;
	wire [word_width-1 : 0]		mobodat_out_in;
	register mobodata_out_reg(clk, rst, mobodat_out_oe, mobodat_out_we,
			mobodat_out_in, mobodat_out);
	/* cpu -> we -> mobodat_out -> oe -> mobo */

	logic 						mobodat_in_oe;
	logic 						mobodat_in_we;
	wire [word_width-1 : 0]		mobodat_in_out;
	register mobodat_in_reg(clk, rst, mobodat_in_oe, mobodat_in_we,
				mobodat_in, mobodat_in_out);
	/* cpu <- oe <- mobodat_in <- we <- mobo */
	
	logic [word_width-1 : 0]		dbg_in;
	bus bus(addr_out, mobodat_in_out, t1_in, t2_in, t1_out, t2_out, dbg_in);

	logic dbg_enable;

	/* fcn modules */
	cpu_fcn_rw cpu_fcn_rw(state, next_state, mobo_stat, mobo_ctrl, dbg_enable);

	// FSM - sequential part
	always @(posedge clk) begin
		state <= '{0, 0, 0};

		if(!rst) begin
			// $display("astate      %x, %d %d", state.ret_state,
			// 		state.fcn_state, state.stack_level);

			// $display("anext_state %x, %d %d", next_state.ret_state,
			// 		next_state.fcn_state, next_state.stack_level);
			state <= next_state;
		end

		if (dbg_enable) begin
			$display("------------------ DEBUG ------------------");
			$display("state: %x %s + %d", state.fcn_state,
					cpu_str_state(state.fcn_state), (state.fcn_state & 'hf));
			$display("stack_level: %d", state.stack_level);
			$display("t1 content: %d", t1_out);
			$display("t2 content: %d", t2_out);
			$display("-------------------------------------------");
			$c32("std::cout << disas(",state.fcn_state,",",0,") << std::endl;");
		end
	end

	// FSM - combinational part
	always @(*) begin
		t1_we = 0;
		t1_oe = 0;

		t2_we = 0;
		t2_oe = 0;

		addr_we = 0;
		mobodat_out_we = 0;
		mobodat_in_oe = 0;
		/* we will disable those ctrl bits only on transfer */
		addr_oe = 1;
		mobodat_out_oe = 1;
		mobodat_in_we = 1;

		dbg_enable = 0;

		case(state.fcn_state)
			`C_STATE_RESET: begin
				func_next(state, next_state, `C_STATE_TEST);
			end

			`C_STATE_TEST: begin
				t1_we = 1;
				dbg_in = 3;

				dbg_enable = 1;
				func_next(state, next_state, `C_STATE_TEST + 1);
			end

			`C_STATE_TEST + 1: begin
				t2_we = 1;
				dbg_in = 5;

				dbg_enable = 1;
				func_next(state, next_state, `C_STATE_TEST + 2);
			end

			`C_STATE_TEST + 2: begin
				t1_oe = 1;
				addr_we = 1;
				addr_oe = 0;

				dbg_enable = 1;
				func_next(state, next_state, `C_STATE_TEST + 3);
			end

			`C_STATE_TEST + 3: begin
				t2_oe = 1;
				mobodat_out_we = 1;
				mobodat_out_oe = 0;

				dbg_enable = 1;
				func_call(state, next_state, `C_STATE_READ, `C_STATE_TEST + 4);
			end

			`C_STATE_TEST + 4: begin
				dbg_enable = 1;
				func_call(state, next_state, `C_STATE_WRITE, `C_STATE_HLT);
			end
// =======
// 				next_state = `C_STATE_TEST + 4;
// 			end
			
// 			`C_STATE_TEST + 4: begin
// 				dbg_enable = 1;
				
// 				next_ret_state = `C_STATE_TEST + 5;
// 				next_state = `C_STATE_WRITE;
// 			end

// 			`C_STATE_TEST + 5: begin
// 				dbg_enable = 1;
				
// 				next_ret_state = `C_STATE_TEST + 6;
// 				next_state = `C_STATE_READ;
// 			end

// 			`C_STATE_TEST + 6: begin
// 				dbg_enable = 1;
				
// 				mobodat_in_oe = 1;
// 				mobodat_in_we = 0;
// 				t1_we = 1;

// 				next_state = `C_STATE_TEST + 7;
// 			end

// 			`C_STATE_TEST + 6: begin
// 				dbg_enable = 1;
// 				t1_oe = 1;

// 				next_state = `C_STATE_HLT;
// >>>>>>> Stashed changes

			`C_STATE_HLT: begin
				func_next(state, next_state, `C_STATE_HLT);
			end
		endcase
	end
endmodule
