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
		input		[31:0]	vga_addr,
		inout	reg	[31:0]	vga_data,
		inout	reg	[15:0]	vga_ctrl
	
	// interrupts
		// ...
	// pci  (mmap, iomap, int)
	// apic (mmap?, iomap, int)
	// rtc	(int)
	// sata (int)
	// vga  (mmap, iomap)
	// keyboard controler ps2 (int, iomap)
	);

	// too lazy, won't read whole datasheet of this stuff, so with
	// help from wikipedia those will be like this:

	// (WIP) mmu will be a part of the proccesor

	// RAM here we will see ram controller and dma (WIP) (direct memory access)
	// dma is work in progress a it is not needed for now

	// (WIP) pci devices hold address / data, interrupts, ctrl in a total of 64 bit,
	// so we will have 64bit for pci + own req, grant, IDSEL
	// also very complicated and quite useless now. One device will be master at a
	// time and he will have access to RAM

	// apic will have states and request pins from other devices
	// a total of 8 wires will go into the processor signalling different
	// interrupt numbers. The cpu will have an internal interrupt table of
	// 256 entries that will be or not called
	// a document with possible interrupts will  be available

	// (WIP) rtc ...
	// (WIP) sata ...

	// vga will have it's own driver that will be allocated at a specific place
	// in memory (mmap), using iomap the screen will be notified of changes

	// keyboard will be set by iomap commands and data from it will be able to
	// be read by iodata commands, interrupt can be enabled and used as the 
	// keyboard is connected to the apic controler
	// keep in mind that the keyboard will have an limited buffer of keys
	// key codes will be defined in a datasheet for this special keyboard

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
			// $display("bridge: %d", cpu_addr);
			if (cpu_ctrl[0]) begin
				cpu_ctrl[0] = 0;
				// $display("bridge: ctrl changed");
			end else begin
				// $display("bridge: ctrl unchanged");
			end
			// here the bridge will change states
		end
	end

	always @(*) begin
		// here the bridge will execute state
	end

endmodule