module vga(
	// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input		[31:0]	addr,
				reg	[31:0]	data,
				reg	[15:0]	ctrl,
		output		[31:0]	phy_addr,
				reg	[31:0]	phy_data
	);

	reg [15:0] state;
	reg [15:0] next_state;

	initial begin
		// initate values
	end

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// reset all
		end
		else if (clk) begin
			// here the ram will change states
		end
	end

	always @(*) begin
		// here the ram will execute state
	end

endmodule