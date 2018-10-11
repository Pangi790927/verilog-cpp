module adder(
		input		[32:0]	tempA,
		input		[32:0]	tempB,
		output		[33:0]	out
	);

	assign {co, out} = in1 + in2 + ci;
endmodule