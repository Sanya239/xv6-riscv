#include "kernel/types.h"

#include "user/user.h"

int smth_on_stack[100];
int main() {
    printf("before mallocs\n");
    pagemap(0, 0, 0);
    char *bebebe = (char *) malloc(40000);
    char buf[2000];
    printf("with memory allocated\n");
    pagemap(buf, 0, 0);
    bebebe[0] = 'b';
    bebebe[10000] = 'e';
    bebebe[20000] = bebebe[30000];
    printf("with memory accessed\n");
    pagemap(bebebe, 40000, 3);
    delete_DA(0, 0, 3);
    printf("with memory cleaned\n");
    pagemap(bebebe, 40000, 3);
    bebebe[0] = 'b';
    bebebe[10000] = 'e';
    bebebe[20000] = bebebe[30000];
    printf("with memory accessed once again\n");
    pagemap(bebebe, 40000, 3);
    free(bebebe);
    printf("after free\n");
    pagemap(0, 0, 0);
    exit(0);
}
