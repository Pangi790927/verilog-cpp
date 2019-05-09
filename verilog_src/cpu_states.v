`ifndef CPU_STATES_V
`define CPU_STATES_V

`define C_STATE_RESET			'h00 // reset state
`define C_STATE_TEST			'h10 // load instruction to instruction register
`define C_STATE_HLT				'h20 // load instruction to instruction register
`define C_STATE_TEST_WRITE		'h30 // 
`define C_STATE_READ			'h40 //
`define C_STATE_WRITE			'h50 //

function string cpu_str_state(int state);
	case (state & ~'hf)
		`C_STATE_RESET:	return "C_STATE_RESET";
		`C_STATE_TEST:	return "C_STATE_TEST";
		`C_STATE_HLT:	return "C_STATE_HLT";
		`C_STATE_READ:	return "C_STATE_READ";
		`C_STATE_WRITE:	return "C_STATE_WRITE";
		`C_STATE_TEST_WRITE: return "C_STATE_TEST_WRITE";
		default: return "unknown, pls add this state";
	endcase
endfunction

`endif
