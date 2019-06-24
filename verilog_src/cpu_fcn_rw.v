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
			`C_STATE_READ: begin
				dbg_enable = 1;
				if (mobo_stat == `STAT_IDLE) begin
					mobo_ctrl = `CTRL_READ;

					func_next(state, next_state, `C_STATE_READ + 1);
				end
			end

			`C_STATE_READ + 1: begin
				dbg_enable = 1;
				if (mobo_stat == `STAT_DONE) begin
					$display("Done reading");
					mobo_ctrl = `CTRL_NONE;

					func_ret(state, next_state);
				end
			end

		/* fcn */
			`C_STATE_WRITE: begin
				dbg_enable = 1;
				if (mobo_stat == `STAT_IDLE) begin
					mobo_ctrl = `CTRL_WRITE;

					func_next(state, next_state, `C_STATE_WRITE + 1);
				end
			end

			`C_STATE_WRITE + 1: begin
				dbg_enable = 1;
				if (mobo_stat == `STAT_DONE) begin
					$display("Done writing");
					mobo_ctrl = `CTRL_NONE;

					func_ret(state, next_state);
				end
			end
		endcase
	end
endmodule
