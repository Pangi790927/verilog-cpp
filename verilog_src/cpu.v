`include "alu.v"

module cpu(
		input	clk,
		input	rst,
		output	[width-1 : 0] mobo_ctrl,
		input	[width-1 : 0] mobo_stat,
		output	[width-1 : 0] addr,
		output	[width-1 : 0] data_out,
		input	[width-1 : 0] data_in
	);

endmodule