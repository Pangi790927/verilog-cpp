INCLUDES := -I /usr/share/verilator/include/ -I Window/ -I Misc/ -I dcasm/
CXX_FLAGS := -std=c++17 -pthread -O3 -g
LD_FLAGS := -lGLEW -lGLU -lGL -lX11

OBJS := /usr/share/verilator/include/verilated.cpp
OBJS := ${OBJS} obj_dir/Vmobo__ALL.a

all: clean computer

computer: mobo computer.cpp
	g++-7 ${CXX_FLAGS} ${INCLUDES} computer.cpp -o computer ${OBJS} ${LD_FLAGS}

mobo: mobo.v
	verilator -cc -O3 mobo.v
	make OPT="-O3 -fstrict-aliasing" -j -C obj_dir -f Vmobo.mk

clean:
	rm -f computer
	rm -rf obj_dir
