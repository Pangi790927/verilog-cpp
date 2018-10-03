INCLUDES := -I /usr/share/verilator/include/ -I Window/ -I Misc/
CXX_FLAGS := -std=c++17
LD_FLAGS := -lGLEW -lGLU -lGL -lX11
OBJS := obj_dir/Vcpu__ALL.a /usr/share/verilator/include/verilated.cpp

all: clean computer

computer: cpu computer.cpp
	make -j -C obj_dir -f Vcpu.mk
	g++ ${CXX_FLAGS} ${INCLUDES} computer.cpp -o computer ${OBJS} ${LD_FLAGS}

cpu: cpu.v
	verilator -cc cpu.v

clean:
	rm -f computer
	rm -rf obj_dir
