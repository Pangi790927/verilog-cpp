`include "adder.v"
module main();
	wire [33:0] adder_out;
	adder a(123, 123, adder_out);

	always @(*) begin
		$display("-- result: %d --", adder_out);	
	end
endmodule