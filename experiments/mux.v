module mux2(
	input	[1:0]	in,
	input			selector,
	output	reg		out
	);

	always @(*) begin
		assign out = (in[0] && selector) || (in[1] && selector);
	end
endmodule

module mux4(
	input	[4:0]	in,
	input	[2:0]	selector,
	output	reg		out
	);
	
	wire a;
	wire b;
	mux2 ma(in[1:0], selector[0], a);
	mux2 mb(in[3:2], selector[0], b);
	assign out = (a && selector[1]) || (b && selector[1]);
	always @() begin
		
	end
	
endmodule