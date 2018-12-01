`include "control_pins.v"
`include "mobo_states.v"

`include "register.v"

module mobo(
		// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input 		[31:0]	ram_ctrl_in,
		output 	reg [31:0]	ram_ctrl_out,

		output	reg	[31:0]	addr,
		input		[31:0]	data_in,
		output	reg	[31:0]	data_out
	);
	
	reg [31:0] state 		= 0;
	reg [31:0] next_state 	= 0;

	reg [31:0] vga_data		= 0;
	reg [15:0] vga_addr		= 0;
	reg vga_oe 				= 0;
	reg vga_we 				= 0;

	// instantiate AM register and related connections
	reg							am_oe;
	reg							am_we;
	reg[31 : 0]					am_in;
	reg[31 : 0]					am_out;
	register am(clk, rst, am_oe, am_we, am_in, am_out);
	// vga vga(clk, rst, );

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// reset
			
		end
		else begin
			state <= next_state;
		end
	end

	always @(*) begin
		case(state)
			`M_STATE_READ: begin
				ram_ctrl_out[`RAM_READ_PIN] = 0;
				addr = 0;
				
			end
		endcase
	end

endmodule