/* pttest.h
 *
 * Header file containing function signatures for pttest.c.
 */

struct _page_table;

// A page table entry pointing to either a physical frame or another page table.
struct _entry {
    int valid;
    int to_frame;
    struct _page_table *to_pt;
};

// A page table containing a pointer to a list of entries and a count of how
// full the table it is
struct _page_table {
    int used_count;
    struct _entry **entries;
};

typedef struct _page_table page_table;
typedef struct _entry entry;

void pttest();
page_table *initializePageTable();

