
function void func_next(
		input	state_t state,
		output	state_t next_state,
		input	[word_width-1 : 0] next,
	);
	next_state = state;
	next_state.fcn_state = next;
endfunction

function void func_ret(
		input	state_t state,
		output	state_t next_state,
	);
	if (state.stack_level == 0)
		$display("Stack Underflow");
	next_state = state;
	next_state.fcn_state = state.ret_state[state.stack_level - 1];
	next_state.stack_level = state.stack_level - 1;
endfunction

function void func_call(
		input	state_t 			state,
		output	state_t				next_state,
		input 	[word_width-1 : 0]	call_state,
		input 	[word_width-1 : 0]	ret_state,
	);
	next_state = state;
	next_state.fcn_state = call_state;
	next_state.ret_state[state.stack_level] = ret_state;
	next_state.stack_level = state.stack_level + 1;

	if (state.stack_level >= max_stack_lvl)
		$display("Stack Overflow");
endfunction