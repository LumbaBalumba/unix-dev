#include <stdio.h>
#include <stdlib.h>

void
print_help()
{
    printf("Usage: range [start] end [step]\n");
    printf("Generate an arithmetic progression similar to Python's range().\n");
    printf("Without arguments, displays this help.\n");
    printf("With one argument end, outputs [0, 1, ..., end-1].\n");
    printf("With two arguments start end, outputs [start, start+1, ..., end-1].\n");
    printf("With three arguments start end step, outputs [start, start+step, ..., end-1].\n");
}

int
main(int argc, char *argv[])
{
    long start = 0;
    long end = 0;
    long step = 1;
    long i;
    int index = 0;

    if (argc == 1) {
        print_help();
        return 0;
    } else if (argc == 2) {
        end = atol(argv[1]);
    } else if (argc == 3) {
        start = atol(argv[1]);
        end = atol(argv[2]);
    } else if (argc == 4) {
        start = atol(argv[1]);
        end = atol(argv[2]);
        step = atol(argv[3]);
    } else {
        fprintf(stderr, "Error: too many arguments\n");
        return 1;
    }

    if (step == 0) {
        fprintf(stderr, "Error: step cannot be zero\n");
        return 1;
    }

    if ((step > 0 && start >= end) || (step < 0 && start <= end)) {
        return 0;
    }

    for (i = start; (step > 0) ? (i < end) : (i > end); i += step) {
        index++;
        printf("%ld\n", i);
    }

    return 0;
}
