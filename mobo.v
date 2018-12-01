`include "control_pins.v"
`include "mobo_states.v"

`include "register.v"

module mobo(
		// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input 		[31:0]	ram_ctrl_from_hw,
		output 	reg [31:0]	ram_ctrl_to_hw,

		output	reg	[31:0]	addr,
		input		[31:0]	data_from_hw,
		output	reg	[31:0]	data_to_hw
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
	reg [31 : 0] vari = 0;

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
				ram_ctrl_to_hw[`RAM_READ_PIN] = 1;
				addr = vari;

				next_state = `M_STATE_WAIT;
			end

			`M_STATE_WAIT: begin
				if (ram_ctrl_from_hw[`RAM_READ_DONE]) begin
					// here ram responded to us
					$display("(%d) Ram got us: %d", vari, data_from_hw);
					
					next_state = `M_STATE_READ;
				end
			end
		endcase
	end

endmodule