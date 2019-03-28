`include "verilog_src/alu.v"
`include "verilog_src/register.v"
`include "verilog_src/cpu_states.v"

module cpu(
		input		clk,
		input		rst,
		output	reg	[word_width-1 : 0] mobo_ctrl,
		input		[word_width-1 : 0] mobo_stat,
		output	reg	[word_width-1 : 0] addr,
		output	reg	[word_width-1 : 0] data_out,
		input		[word_width-1 : 0] data_in
	);

	parameter word_width = 32;

	reg[word_width-1 : 0] 		state 		= 0;
	reg[word_width-1 : 0] 		next_state	= 0;

	// instantiate T1 register and related connections
	wire                        t1_oe;
	wire                        t1_we;
	wire[word_width-1 : 0]      t1_in;
	wire[word_width-1 : 0]      t1_out;
	register #(word_width) t1(clk, rst, t1_oe, t1_we, t1_in, t1_out);

	// instantiate T2 register and related connections
	wire                        t2_oe;
	wire                        t2_we;
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

	wire dbg_enable;

	// FSM - sequential part
	always @(posedge clk) begin
		state <= `C_STATE_RESET;

		if(!rst)
			state <= next_state;

		if (dbg_enable) begin
			$display("----------------- DEBUG ------------------");
			$display("state: %x ", state);
			$display("t1 content: %d", t1_out);
			$display("t2 content: %d", t2_out);
			$display("------------------------------------------");
		end
	end

	// FSM - combinational part
	always @(state) begin
		t1_we = 0;
		t1_oe = 0;

		t2_we = 0;
		t2_oe = 0;

		dbg_enable = 0;

		case(state)
			`C_STATE_RESET: begin
				next_state = `C_STATE_TEST;
			end

			`C_STATE_TEST: begin
				t1_in = 5;
				t1_we = 1;

				t2_in = 6;
				t2_we = 1;

				next_state = `C_STATE_TEST + 1;
			end

			`C_STATE_TEST + 1: begin
				t1_oe = 1;
				t2_oe = 1;

				dbg_enable = 1;
				next_state = `C_STATE_TEST + 2;
			end

			`C_STATE_TEST + 2: begin
				t1_oe = 1;
				t2_oe = 1;

				dbg_enable = 1;
				next_state = `C_STATE_HLT;
			end

			`C_STATE_HLT: begin
				next_state = `C_STATE_HLT;
			end
		endcase
	end
endmodule

/*
	Ce vrem sa facem:
		sa scriem din cpu in vga
	Cum:



*/