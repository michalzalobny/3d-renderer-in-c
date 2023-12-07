#include <stdio.h>

void print_int(const int *ptr) {
    if (ptr != NULL) {
        printf("%d\n", *ptr);
    } else {
        printf("NULL pointer\n");
    }
}

void print_char(const char *ptr) {
    if (ptr != NULL) {
        printf("%c\n", *ptr);
    } else {
        printf("NULL pointer\n");
    }
}

void print_float(const float *ptr) {
    if (ptr != NULL) {
        printf("%f\n", *ptr);
    } else {
        printf("NULL pointer\n");
    }
}

void print_double(const double *ptr) {
    if (ptr != NULL) {
        printf("%lf\n", *ptr);
    } else {
        printf("NULL pointer\n");
    }
}

void print_string(const char *ptr) {
    if (ptr != NULL) {
        printf("%s\n", ptr);
    } else {
        printf("NULL pointer\n");
    }
}


