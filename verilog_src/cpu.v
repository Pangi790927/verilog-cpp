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

endmodule