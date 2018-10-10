module cpu(
	// clk, rst - those are required on all devices
		input				clk,
		input				rst,
	// in, out - i/o mapping
		output		[15:0]	ioaddr,
				reg [15:0]	iodata,
	// addr, data - memory mapping
		output	reg [31:0]	addr,
				reg	[31:0]	data,
	// interrupts
		// ...
	// cpu ctrl register
				reg [15:0]	ctrl
	);

	reg [15:0] state;
	reg [15:0] next_state;

	initial begin
		state = 16'hFFFF;
		next_state = 0;
	end

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			state <= 16'hFFFF;
			next_state <= 0;
		end
		else if (clk) begin
			addr = addr + 1;
			// $display("cpu: %d", addr);
			if (!ctrl[0]) begin
				ctrl[0] = 1;
				// $display("cpu: ctrl changed");
			end else begin
				// $display("cpu: ctrl unchanged");
			end

			// here the processor will change states
		end
	end

	always @(*) begin
		// here the processor will execute state
	end

endmodule