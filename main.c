#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

typedef double elem_t;

typedef enum {
    LT = -1,
    EQ = 0,
    GT = 1
} Compare;

int comp_cnt, swap_cnt;

void swap(elem_t *a, elem_t *b) {
    ++swap_cnt;
    elem_t temp = *a;
    *a = *b;
    *b = temp;
}

Compare cmp(elem_t a, elem_t b) {
    ++comp_cnt;
    if (a < b)
        return GT; // сортировка по невозрастанию
    if (a > b)
        return LT;
    return EQ;
}

void sift_down(int v, int n, elem_t t[n]) {
    while (2 * v + 1 < n) {
        int l = 2 * v + 1;
        int r = 2 * v + 2;
        int u = l;
        if (r < n && cmp(t[r], t[l]) > 0)
            u = r;
        if (cmp(t[v], t[u]) >= 0)
            break;
        swap(&t[v], &t[u]);
        v = u;
    }
}

void heap_sort(int n, elem_t arr[n]) {
    for (int i = n - 1; i >= 0; --i)
        sift_down(i, n, arr);
    int heap_size = n;
    for (int i = 0; i < n; ++i) {
        swap(&arr[0], &arr[n - 1 - i]);
        --heap_size;
        sift_down(0, heap_size, arr);
    }
}

int seq_stasevich(int k) {
    if (k == 0)
        return 1;
    return (1 << k) + 1;
}

int seq_sedgewick(int k) {
    if (k == 0)
        return 1;
    return (1 << (k << 1)) + 3 * (1 << (k - 1)) + 1;
}

void shell_sort(int n, elem_t arr[n], int (*seq)(int)) {
    int sn = 0;
    while (seq(sn) < n)
        ++sn;
    for (int s = sn - 1; s >= 0; --s) {
        int gap = seq(s);
        for (int i = gap; i < n; ++i)
            for (int j = i; j >= gap && cmp(arr[j - gap], arr[j]) > 0; j -= gap)
                swap(&arr[j], &arr[j - gap]);
    }
}

elem_t gen_elem(void) {
    uint64_t res = 0x3ffull << 52;
    uint64_t mask15 = (1ull << 15) - 1;
    uint64_t mask7 = (1ull << 7) - 1;
    res |= ((uint64_t) rand() & mask15) << 37;
    res |= ((uint64_t) rand() & mask15) << 22;
    res |= ((uint64_t) rand() & mask15) << 7;
    res |= (uint64_t) rand() & mask7;
    elem_t dst;
    memcpy(&dst, &res, sizeof(elem_t));
    return dst;
}

int main(void) {
    int n[] = {10, 100, 1000, 10000};
#define TEST_COUNT (sizeof(n) / sizeof(n[0]))
    elem_t *arr[TEST_COUNT];
    for (int i = 0; i < TEST_COUNT; ++i) {
        arr[i] = malloc(n[i] * sizeof(*arr[i]));
    }
/*
    srand(time(NULL));
    for (int i = 0; i < 10; ++i)
        printf("%.20f\n", gen_elem());
*/

    for (int i = 0; i < TEST_COUNT; ++i) {
        free(arr[i]);
    }
#undef TEST_COUNT
}