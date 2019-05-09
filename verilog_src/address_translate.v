
`define START_ADDR	0
`define VGA_ADDR	224000
`define RAM_ADDR	1048576 + `VGA_ADDR

`define RAM_STATE	1
`define VGA_STATE	2
`define ERR_STATE	3

module addr_translate(
		input [word_width-1 : 0] addr,
		output [word_width-1 : 0] transl_addr,
		output [word_width-1 : 0] trans_state
	);
	
	parameter word_width = `WORD_WIDTH;

	function logic in_state(
			input[word_width-1 : 0] left,
			input[word_width-1 : 0] right,
			input[word_width-1 : 0] addr
		);
		in_state = left <= addr && addr < right; 
	endfunction

	assign transl_addr =
			in_state(`START_ADDR, `VGA_ADDR, addr)	? addr - `START_ADDR :
			in_state(`VGA_ADDR, `RAM_ADDR, addr)	? addr - `VGA_ADDR :
													0;
	assign trans_state = 
			in_state(`START_ADDR, `VGA_ADDR, addr)	? `VGA_STATE :
			in_state(`VGA_ADDR, `RAM_ADDR, addr)	? `RAM_STATE :
													`ERR_STATE;
endmodule