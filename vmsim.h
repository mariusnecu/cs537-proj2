typedef struct _mem_loc {
    struct _mem_loc *next;
    unsigned int address;
} mem_loc;

mem_loc *mem_head;
