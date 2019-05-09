`ifndef MY_DEFINES_SV
`define MY_DEFINES_SV

parameter word_width = 32;

typedef struct packed {
	reg[word_width-1 : 0] a;
	reg[word_width-1 : 0] b;
} test_struct;

`endif