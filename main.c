#include <stdio.h>
#include <stdlib.h>

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
    return EQ; //TODO: сделать нормальный компаратор
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

void shell_sort(int n, elem_t arr[n]) {

}

int main(void) {
    int n[] = {10, 100, 1000, 10000};
#define TEST_COUNT sizeof(n) / sizeof(n[0])
    elem_t *arr[TEST_COUNT];
    for (int i = 0; i < TEST_COUNT; ++i) {
        arr[i] = malloc(n[i] * sizeof(*arr[i]));
    }

    for (int i = 0; i < TEST_COUNT; ++i) {
        free(arr[i]);
    }
#undef TEST_COUNT
}