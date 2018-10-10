INCLUDES := -I /usr/share/verilator/include/ -I Window/ -I Misc/
CXX_FLAGS := -std=c++17 -pthread
LD_FLAGS := -lGLEW -lGLU -lGL -lX11

OBJS := /usr/share/verilator/include/verilated.cpp

OBJS := ${OBJS} obj_dir/Vbridge__ALL.a
OBJS := ${OBJS} obj_dir/Vcpu__ALL.a
OBJS := ${OBJS} obj_dir/Vram__ALL.a
OBJS := ${OBJS} obj_dir/Vvga__ALL.a

all: clean computer

computer: bridge cpu ram vga computer.cpp
	g++ ${CXX_FLAGS} ${INCLUDES} computer.cpp -o computer ${OBJS} ${LD_FLAGS}

bridge: bridge.v
	verilator -cc bridge.v
	make -j -C obj_dir -f Vbridge.mk

cpu: cpu.v
	verilator -cc cpu.v
	make -j -C obj_dir -f Vcpu.mk

ram: ram.v
	verilator -cc ram.v
	make -j -C obj_dir -f Vram.mk

vga: vga.v
	verilator -cc vga.v
	make -j -C obj_dir -f Vvga.mk


clean:
	rm -f computer
	rm -rf obj_dir
