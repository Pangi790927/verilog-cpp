

INCLUDES := -I /usr/share/verilator/include/
CXX_FLAGS := -std=c++11
OBJS := obj_dir/Vcpu__ALL.a /usr/share/verilator/include/verilated.cpp

all: clean computer

LDFLAGS := -std=c++11

computer: cpu computer.cpp
	make -j -C obj_dir -f Vcpu.mk
	g++ ${CXX_FLAGS} ${INCLUDES} computer.cpp -o computer ${OBJS}

cpu: cpu.v computer.cpp
	iverilog cpu.v -o cpu.vpp
	verilator -cc cpu.v

clean:
	rm -f cpu.vpp
	rm -f computer
	rm -rf obj_dir