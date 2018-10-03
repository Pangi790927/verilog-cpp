module cpu(
		input		[31:0] A,
		output	reg [31:0] B
	);
	
	reg [31:0] state;

	always @(*) begin
		state = ~A;
		B = state;
	end

endmodule