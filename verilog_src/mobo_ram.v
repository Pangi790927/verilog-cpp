
module mobo_ram(
		input	[word_width-1 : 0]	state,
		output	[word_width-1 : 0]	next_state,
		input	[word_width-1 : 0]	transl_addr,
		output	[word_width-1 : 0]	cpu_data_in,
		input	[word_width-1 : 0]	cpu_data_out,
		input 	[word_width-1 : 0]	ram_stat,
		output	[word_width-1 : 0]	ram_ctrl,
		output	[word_width-1 : 0]	addr,
		input	[word_width-1 : 0]	data_in,
		output	[word_width-1 : 0]	data_out
	);

	parameter word_width = `WORD_WIDTH;

	always @(*) begin
		case (state)
			`MOBO_RAM_WRITE: begin
				if (ram_stat[`RAM_ACK] == 0) begin
					ram_ctrl[`RAM_WRITE_PIN] = 1;

					addr = transl_addr;
					data_out = cpu_data_out;

					next_state = `MOBO_RAM_WRITE + 1;
				end
			end

			`MOBO_RAM_WRITE + 1: begin
				if (ram_stat[`RAM_ACK]) begin
					ram_ctrl = 0;

					next_state = `MOBO_DONE;
				end
			end

			`MOBO_RAM_READ: begin
				if (ram_stat[`RAM_ACK] == 0) begin
					ram_ctrl[`RAM_READ_PIN] = 1;

					addr = transl_addr;
					
					next_state = `MOBO_RAM_READ + 1;
				end
			end

			`MOBO_RAM_READ + 1: begin
				if (ram_stat[`RAM_ACK]) begin
					ram_ctrl = 0;
					
					cpu_data_in = data_in;

					next_state = `MOBO_DONE;
				end
			end
		endcase
	end

endmodule