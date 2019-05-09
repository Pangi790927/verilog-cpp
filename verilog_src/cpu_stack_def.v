`ifndef CPU_STACK_DEF
`define CPU_STACK_DEF

parameter max_stack_lvl = 10;
parameter word_width = 32;

typedef struct packed {
	reg[max_stack_lvl-1 : 0][word_width-1 : 0] ret_state;
	reg[word_width-1 : 0] fcn_state;
	reg[word_width-1 : 0] stack_level;
} test_struct;

`endif