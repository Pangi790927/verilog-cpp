// address space 0x100'000 - 0x200'000
`include "control_pins.v"

module vga(
	// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input		[31:0]	addr,
				reg	[31:0]	data,
		inout 	reg	[15:0]	ctrl,
		output		[31:0]	phy_addr,
				reg	[31:0]	phy_data
	);

	reg [15:0] state;
	reg [15:0] next_state;

	initial begin
		// initate values
		state = 0;
	end

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// reset all
		end
		else if (clk) begin
			// here the ram will change states
			case (state)
				0: begin
					if (ctrl[`VGA_WRITE_PIN]) begin
						phy_data = data;
						phy_addr = addr;
						ctrl[`VGA_HW_WRITE_PIN] = 1;
						state = 1;
					end
				end
				1: begin
					if (!ctrl[`VGA_HW_WRITE_PIN]) begin
						ctrl[`VGA_WRITE_PIN] = 0;
						$display("Am trimis chestia la hardware");
						state = 0;
					end
				end
			endcase
		end
	end

	always @(*) begin
		// here the ram will execute state
	end

endmodule