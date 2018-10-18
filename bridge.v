// bios 0 - 32k
// vga 1M - 2M
// hdd - pci sata ??
// ram - 2M -> inf
// control - 32k - 48k
// sunete - 48k - 112k

`include "control_pins.v"

module bridge(
	// clk, rst - those are required on all devices
		input				clk,
		input				rst,
	// CPU socket:
		inout	reg	[15:0]	cpu_iodata,
		input		[15:0]	cpu_ioaddr,
		input	reg [31:0]	cpu_addr,
		inout	reg	[31:0]	cpu_data,
		inout	reg [15:0]	cpu_ctrl,
	// RAM socket:
		input		[31:0]	ram_addr,
		inout	reg	[31:0]	ram_data,
		inout	reg	[15:0]	ram_ctrl,

	// VGA socket:
		output	reg [31:0]	vga_addr,
		inout	reg	[31:0]	vga_data,
		inout	reg	[15:0]	vga_ctrl
	);

	reg [15:0] state;
	reg [15:0] next_state;

	initial begin
		// bridge initialization
	end

	always @(posedge clk or posedge rst) begin
		if (rst) begin
			// on reset
		end
		else if (clk) begin
			/* write to vga */

			if (!vga_ctrl[`VGA_WRITE_PIN]) begin
				vga_data = 32'h00_00_01_2B;
				vga_addr = 1;
				vga_ctrl[`VGA_WRITE_PIN] = 1; 
			end /* else don't switch state */

			// // $display("bridge: %d", cpu_addr);
			// if (cpu_ctrl[0]) begin
			// 	cpu_ctrl[0] = 0;
			// 	// $display("bridge: ctrl changed");
			// end else begin
			// 	// $display("bridge: ctrl unchanged");
			// end
			// // here the bridge will change states
		end
	end

	always @(*) begin
		// here the bridge will execute state
	end

endmodule