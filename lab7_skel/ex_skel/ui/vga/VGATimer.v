// (<hActiv>,<hdupa>,<hsync>,<hinainte>,<vActiv>,<vdupa>,<vsync>,<vinainte>,<activ(0/1)>)
// 640@480@60Hz clk = 25MHz
// VGATimer #(640,48,96,16, 480,33,2,10, 0) timer (clk, hsync, vsync, active, x, y);
// 800x600@72Hz clk = 50MHz
// VGATimer #(800,64,120,56, 600,23,6,37, 0) timer (clk, hsync, vsync, active, x, y);
// 1280x1024@60Hz clk = 108MHz
// VGATimer #(1280,248,112,48, 1024,38,3,1, 1) timer (clk, hsync, vsync, active, x, y);
module VGATimer(clk, hsync, vsync, active, x, y);
	input clk;
	output hsync;
	output vsync;
	output active;
	output [11:0] x;	// Coloana curenta
	output [11:0] y;	// Randul curent

	// Parametrii trebuiesc setati pentru un modul real
	// Unitatile sunt perioade de ceas
	parameter XActive = 4;
	parameter XAfter  = 2;
	parameter XSync   = 1;
	parameter XBefore = 2;

	parameter YActive = 2;
	parameter YAfter  = 2;
	parameter YSync   = 1;
	parameter YBefore = 2;

	// Daca e 1, sincronizarea e activa pe high.
	parameter SyncActiveLevel = 1;

	wire hTimerSync_out;
	wire hTimerActive_out;
	wire [11:0] hTimerCount_out;
	wire vTimerSync_out;
	wire vTimerActive_out;
	wire [11:0] vTimerCount_out;

	// (<Activ>,<dupa>,<sync>,<inainte>)
	VGATimer_single #(XActive,XAfter,XSync,XBefore) hTimer(clk, hTimerSync_out, hTimerActive_out, hTimerCount_out);
	VGATimer_single #(YActive,YAfter,YSync,YBefore) vTimer(hTimerSync_out, vTimerSync_out, vTimerActive_out, vTimerCount_out);

	assign hsync = (SyncActiveLevel==1)?hTimerSync_out:~hTimerSync_out;
	assign vsync = (SyncActiveLevel==1)?vTimerSync_out:~vTimerSync_out;
	assign active = hTimerActive_out && vTimerActive_out;
	assign x = active?hTimerCount_out:0;
	assign y = active?vTimerCount_out:0;
	
endmodule