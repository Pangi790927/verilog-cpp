`define C_STATE_RESET			'h00 // reset state
`define C_STATE_TEST			'h10 // load instruction to instruction register
`define C_STATE_HLT				'h20 // load instruction to instruction register

function string cpu_str_state(int state);
	case (state & ~'hf)
		`C_STATE_RESET:	return "C_STATE_RESET";
		`C_STATE_TEST:	return "C_STATE_TEST";
		`C_STATE_HLT:	return "C_STATE_HLT";
		default: return "unknown, pls add this state";
	endcase
endfunction
