/* pttest.h
 *
 * Header file containing function signatures for pttest.c.
 */

struct _page_table;

// A page table entry pointing to either a physical frame or another page table.
struct _entry {
    int vpn;
    int to_frame;
    struct _page_table *to_pt;
};

// A page table containing a pointer to a list of entries and a count of how
// full the table it is
struct _page_table {
    struct _entry **entries;
    int frame_number;
};

struct _frame_list {
    struct _frame_list *next;
    struct _entry *ent;
};

typedef struct _page_table page_table;
typedef struct _entry entry;
typedef struct _frame_list frame_list;

void pttest();
page_table *initializePageTable();

