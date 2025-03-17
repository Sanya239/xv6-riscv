typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int  uint32;
typedef unsigned long uint64;

typedef uint64 pde_t;

enum procinfostate {
    sus_value, sus_value2, sleeping, runnable, running, zombie
};

struct procinfo {
    enum procinfostate state;
    int pid;
    char name[16];

    int parent_id;
    char parent_name[16];
};
