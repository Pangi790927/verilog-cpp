

INCLUDES := -I /usr/share/verilator/include/
CXX_FLAGS := -std=c++11

all: computer

computer: cpu computer.cpp
	make -j -C obj_dir -f Vcpu.mk
	mv obj_dir/Vcpu__ALL.a obj_dir/libVcpu__ALL.a
	g++ ${INCLUDES} ${CXX_FLAGS} -L obj_dir -l Vcpu__ALL computer.cpp -o computer

cpu: cpu.v
	iverilog cpu.v -o cpu.vpp
	verilator -cc cpu.v


clean:
	rm -f cpu.vpp
	rm -f computer