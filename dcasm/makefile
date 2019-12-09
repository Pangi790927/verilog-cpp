CPPFLAGS += -DREGS_PATH="\"regs.json\"" -DINSTR_PATH="\"instr.json\""

all: debug dcasm.cpp dcdisasm.cpp
	clang++-6.0 -std=c++17 dcasm.cpp -o dcasm
	clang++-6.0 -std=c++17 dcdisasm.cpp -o dcdisasm libdebug.o

debug: debug.cpp
	clang++-6.0 -std=c++17 $(CPPFLAGS) -c debug.cpp -o libdebug.o

clear:
	rm -f dcasm