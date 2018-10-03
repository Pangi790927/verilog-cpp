

all: computer

computer: cpu computer.cpp
	g++ -std=c++11 obj_dir/*.cpp computer.cpp -o computer

cpu: cpu.v
	iverilog cpu.v -o cpu.vpp
	verilator -cc cpu.v

clean:
	rm -f cpu.vpp
	rm -f computer