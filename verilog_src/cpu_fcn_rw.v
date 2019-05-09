`include "verilog_src/global_const.v"

module cpu_fcn_rw(
		input	state_t				state,
		output	state_t				next_state,
		input	[word_width-1 : 0]	mobo_stat,
		output	[word_width-1 : 0]	mobo_ctrl,
		output						dbg_enable
	);
	`include "verilog_src/cpu_stack_fcn.v"

	parameter word_width = `WORD_WIDTH;
	
	always @(*) begin
		case (state.fcn_state)
		/* fcn */
			`C_STATE_READ:
				func_next(next_state, `C_STATE_READ + 1);

			`C_STATE_READ + 1:
				func_ret(next_state);
		
		/* fcn */
			`C_STATE_WRITE:
				func_next(next_state, `C_STATE_WRITE + 1);
		
			`C_STATE_WRITE + 1:
				func_ret(next_state);
		endcase
	end
endmodule