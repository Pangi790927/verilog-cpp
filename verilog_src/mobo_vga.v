
module mobo_vga(
		input	[word_width-1 : 0]	state,
		output reg		[word_width-1 : 0]	next_state,
		input	[word_width-1 : 0]	transl_addr,
		output reg		[word_width-1 : 0]	cpu_data_in,
		input	[word_width-1 : 0]	cpu_data_out,
		input 	[word_width-1 : 0]	vga_stat,
		output reg		[word_width-1 : 0]	vga_ctrl,
		output reg		[word_width-1 : 0]	addr,
		input	[word_width-1 : 0]	data_in,
		output reg		[word_width-1 : 0]	data_out
	);

	parameter word_width = `WORD_WIDTH;

	always @(*) begin
		case (state)
			`MOBO_VGA_WRITE: begin
				if (vga_stat[`VGA_ACK] == 0) begin
					vga_ctrl[`VGA_WRITE_PIN] = 1;

					addr = transl_addr;
					data_out = cpu_data_out;

					next_state = `MOBO_VGA_WRITE + 1;
				end
			end

			`MOBO_VGA_WRITE + 1: begin
				if (vga_stat[`VGA_ACK]) begin
					vga_ctrl = 0;

					next_state = `MOBO_DONE;
				end
			end

			`MOBO_VGA_READ: begin
				if (vga_stat[`VGA_ACK] == 0) begin
					vga_ctrl[`VGA_READ_PIN] = 1;

					addr = transl_addr;
					
					next_state = `MOBO_VGA_READ + 1;
				end
			end

			`MOBO_VGA_READ + 1: begin
				if (vga_stat[`VGA_ACK]) begin
					vga_ctrl = 0;
					
					cpu_data_in = data_in;

					next_state = `MOBO_DONE;
				end
			end
		endcase
	end

endmodule