#include "kernel/types.h"
#include "user/user.h"

const int max_figure_size = 11;
const int max_input_size = 2 * (max_figure_size + 1);

void fileread_error() {
    fprintf(2, "%s", "unexpected error\n");
}

void overflow_error() {
    fprintf(2, "%s", "Input is too long\n");

}

void format_error() {
    fprintf(2, "%s", "incorrect format\n");
}

void read_chars(char *buff, int size) {
    char *p = buff;
    int count = 0;
    int cc;
    while (count++ < size && (cc = read(0, p, 1))) {
        if (cc == -1) {
            fileread_error();
        }
        if (*p == '\n') {
            *p = 0;
            break;
        }
        p++;
    }
}

int parse_int(char *buff) {
    if (buff[0] == '-') {
        return -atoi(buff + 1);
    }
    return atoi(buff);
}

int validate(char *buff) {

    int p = 0;
    if (buff[p] == '-') {
        p++;
    }
    while (buff[p] != ' ') {
        if (buff[p] > '9' || buff[p] < '0') {
            format_error();
            return -1;
        }
        p++;
    }
    int whitespace_pos = p;
    p++;
    if (buff[p] == '-') {
        p++;
    }
    while (buff[p] != 0 && buff[p] != '\n') {
        if (buff[p] > '9' || buff[p] < '0') {
            format_error();
            return -1;
        }
        p++;
    }
    buff[p] = 0;
    return whitespace_pos;
}

int
main(int argc, char *argv[]) {
    char buff[max_input_size];
    read_chars(buff, max_input_size);
    if (argc == 2) {
        if (strcmp(argv[1], "--test") == 0) {
            printf("|%s|\n", buff);
        }
    }
    int whitespace_pos = validate(buff);
    if (whitespace_pos == -1) {
        return 1;
    }
    int num1 = parse_int(buff);
    while (buff[whitespace_pos] != 0 && buff[whitespace_pos] != ' ') {
        whitespace_pos++;
    }
    int num2 = parse_int(buff + whitespace_pos + 1);

    printf("%d\n", add(num1, num2));
    return 0;
}