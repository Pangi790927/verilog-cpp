`include "verilog_src/global_const.v"

module cpu_fcn_rw(
		input	[word_width-1 : 0]	state,
		output	[word_width-1 : 0]	next_state,
		input	[word_width-1 : 0]	mobo_stat,
		output	[word_width-1 : 0]	mobo_ctrl,
		output						dbg_enable
	);
	`include "verilog_src/cpu_stack_fcn.v"

	parameter word_width = `WORD_WIDTH;
	
	always @(*) begin
		case (state)
		/* fcn */
			`C_STATE_READ:
				func_next(next_state, `C_STATE_READ + 1);

			`C_STATE_READ + 1:
				next_state = `C_STATE_READ + 1;
		
		/* fcn */
			`C_STATE_WRITE:
				next_state = `C_STATE_READ + 1;
		
			`C_STATE_WRITE + 1:
				next_state = `C_STATE_READ + 1;
		endcase
	end
endmodule