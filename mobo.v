`include "control_pins.v"
`include "mobo_states.v"

`include "register.v"

module mobo(
		// clk, rst - those are required on all devices
		input				clk,
		input				rst,
		
		input 		[31:0]	ram_ctrl_from_hw,
		output 	reg [31:0]	ram_ctrl_to_hw,

		input 		[31:0]	vga_ctrl_from_hw,
		output 	reg [31:0]	vga_ctrl_to_hw,

		output	reg	[31:0]	addr,
		input		[31:0]	data_from_hw,
		output	reg	[31:0]	data_to_hw
	);
	
	reg [31:0] state 		= 0;
	reg [31:0] next_state 	= 0;
	reg [31:0] ram_state 	= 0;

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
	reg [31 : 0] idx = 0;

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// reset
			
		end
		else begin
			state <= next_state;

			if (ram_state == 1) begin
				idx += 1;
			end
		end
	end

	always @(*) begin
		//$display("(%d) CLC start", idx);
		ram_state = 0;

		case(state)
			`M_STATE_WRITE: begin
				$display("(%d) write ram", idx);

				if (ram_ctrl_from_hw[`RAM_ACK] == 0) begin //advance to the next state only when the physical chip is done doing its job
					ram_ctrl_to_hw[`RAM_WRITE_PIN] = 1;
					addr = idx;
					data_to_hw = idx;

					next_state = `M_STATE_WAIT_WRITE;
				end
			end
			`M_STATE_WAIT_WRITE: begin
				$display("(%d) waiting for ram to write", idx);

				if (ram_ctrl_from_hw[`RAM_ACK]) begin
					// here ram responded to us
					ram_ctrl_to_hw = 0;
					$display("(%d) ACK write, addr %x", idx, addr);

					next_state = `M_STATE_READ;
				end
			end
			`M_STATE_READ: begin
				$display("(%d) ask ram", idx);

				if (ram_ctrl_from_hw[`RAM_ACK] == 0) begin //advance to the next state only when the physical chip is done doing its job
					ram_ctrl_to_hw[`RAM_READ_PIN] = 1;
					addr = idx;

					next_state = `M_STATE_WAIT_READ;
				end
			end

			`M_STATE_WAIT_READ: begin
				$display("(%d) waiting for ram respond", idx);

				if (ram_ctrl_from_hw[`RAM_ACK]) begin
					// here ram responded to us
					ram_ctrl_to_hw = 0;
					$display("(%d) ACK read: %d", idx, data_from_hw);

					next_state = `M_STATE_WRITE;
					ram_state = 1;
				end
			end
		endcase

		//$display("(%d) CLC end", idx);
	end

endmodule