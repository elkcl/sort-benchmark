// Денисьев Илья, вариант 3 / 2 / 3 / 5

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

typedef double elem_t;

// Тесты: отсортированный, перевёрнутый, случайный
// Два крайних значения для итерации
typedef enum {
    TEST_START,
    TEST_ORD,
    TEST_REV,
    TEST_RAND1,
    TEST_RAND2,
    TEST_SENTINEL
} TestType;

// счётчики сравнений и перестановок
int comp_cnt, swap_cnt;

/* Вспомогательные функции */

void swap(elem_t *a, elem_t *b) {
    ++swap_cnt;
    elem_t temp = *a;
    *a = *b;
    *b = temp;
}

// компаратор по возрастанию
int cmp_asc(const void *ap, const void *bp) {
    ++comp_cnt;
    elem_t a = *(elem_t*) ap;
    elem_t b = *(elem_t*) bp;
    if (a < b)
        return -1;
    if (a > b)
        return 1;
    return 0;
}

// компаратор по убыванию
int cmp_desc(const void *ap, const void *bp) {
    ++comp_cnt;
    elem_t a = *(elem_t*) ap;
    elem_t b = *(elem_t*) bp;
    if (a < b)
        return 1;
    if (a > b)
        return -1;
    return 0;
}

/* Сортировки */

// просеивание вниз в max-куче
void sift_down(int v, int n, elem_t t[n], int (*cmp)(const void*, const void*)) {
    while (2 * v + 1 < n) {
        int l = 2 * v + 1;
        int r = 2 * v + 2;
        int u = l;
        // если правый сын существует и больше левого, выбираем его
        if (r < n && cmp(&t[r], &t[l]) > 0)
            u = r;
        if (cmp(&t[v], &t[u]) >= 0)
            break;
        swap(&t[v], &t[u]);
        v = u;
    }
}

// пирамидальная сортировка
void heapsort(int n, elem_t arr[n], int (*cmp)(const void*, const void*)) {
    // построение кучи за O(n)
    for (int i = n - 1; i >= 0; --i)
        sift_down(i, n, arr, cmp);
    int heap_size = n;
    // вынимаем максимум с верхушки кучи и кладём в массив с конца, пока куча не кончится
    for (int i = 0; i < n; ++i) {
        swap(&arr[0], &arr[n - 1 - i]);
        --heap_size;
        sift_down(0, heap_size, arr, cmp);
    }
}

// сортировка Шелла
void shellsort(int n, elem_t arr[n], int (*cmp)(const void*, const void*), int (*seq)(int)) {
    // находим первый член последовательности
    int sn = 0;
    while (seq(sn) < n)
        ++sn;
    for (int s = sn - 1; s >= 0; --s) {
        int gap = seq(s);
        // сортировка вставками каждого из списков
        for (int i = gap; i < n; ++i)
            for (int j = i; j >= gap && cmp(&arr[j - gap], &arr[j]) > 0; j -= gap)
                swap(&arr[j], &arr[j - gap]);
    }
}

int seq_stasevich(int k) {
    // A083318
    // асимптотика в худшем случае — ϴ(n^(3/2))
    if (k == 0)
        return 1;
    return (1 << k) + 1;
}

void shellsort_stasevich(int n, elem_t arr[n], int (*cmp)(const void*, const void*)) {
    shellsort(n, arr, cmp, seq_stasevich);
}

int seq_sedgewick(int k) {
    // A036562
    // асимптотика в худшем случае — ϴ(n^(4/3))
    if (k == 0)
        return 1;
    return (1 << (k << 1)) + 3 * (1 << (k - 1)) + 1;
}

void shellsort_sedgewick(int n, elem_t arr[n], int (*cmp)(const void*, const void*)) {
    shellsort(n, arr, cmp, seq_sedgewick);
}

/* Генерация */

// равномерная генерация случайного double в диапазоне [2^16; 2^17)
// предполагается, что double соответствует IEEE 754 (на всех адекватных платформах это так)
elem_t gen_elem(void) {
    // устанавливаем знак в 0, экспоненту в 16
    uint64_t res = 0x40full << 52;   // 0x40f = 16 + 1023 (сдвиг экспоненты), 52 бита — мантисса
    // RAND_MAX гарантированно хотя бы 2^15 - 1, так что генерируем мантиссу кусками по 15 бит
    // 52 = 3 * 15 + 7
    uint64_t mask15 = (1ull << 15) - 1;
    uint64_t mask7 = (1ull << 7) - 1;
    res |= ((uint64_t) rand() & mask15) << (52 - 15);
    res |= ((uint64_t) rand() & mask15) << (52 - 2 * 15);
    res |= ((uint64_t) rand() & mask15) << (52 - 3 * 15);
    res |= (uint64_t) rand() & mask7;
    elem_t dst;
    // конвертируем получившуюся битовую репрезентацию в настоящий double
    memcpy(&dst, &res, sizeof(elem_t));
    return dst;
}

// генерация массива нужного типа и размера
void gen_arr(TestType type, int n, elem_t arr[n]) {
    for (int i = 0; i < n; ++i)
        arr[i] = gen_elem();
    switch (type) {
        case TEST_ORD:
            qsort(arr, n, sizeof(elem_t), cmp_desc);
            break;
        case TEST_REV:
            qsort(arr, n, sizeof(elem_t), cmp_asc);
            break;
        case TEST_RAND1:
        case TEST_RAND2:
            break;
        case TEST_START:
        case TEST_SENTINEL:
            // такого происходить не должно
            puts("ERROR: Incorrect TestType");
            exit(EXIT_FAILURE);
    }
}

/* Тестирование */

void run_test(TestType type, const char *title, void (*sort)(int n, elem_t arr[n], int (*cmp)(const void*, const void*)), int n, const elem_t arr_orig[n], const elem_t arr_sorted[n]) {
    // обнуляем счётчики
    comp_cnt = swap_cnt = 0;
    elem_t *arr = malloc(n * sizeof(elem_t));
    memcpy(arr, arr_orig, n * sizeof(elem_t));
    sort(n, arr, cmp_desc);
    switch (type) {
        case TEST_ORD:
            printf("%s test %d: in order, size %d\n", title, type, n);
            break;
        case TEST_REV:
            printf("%s test %d: reverse order, size %d\n", title, type, n);
            break;
        case TEST_RAND1:
        case TEST_RAND2:
            printf("%s test %d: random, size %d\n", title, type, n);
            break;
        case TEST_START:
        case TEST_SENTINEL:
            // такого происходить не должно
            puts("ERROR: Incorrect TestType");
            exit(EXIT_FAILURE);
    }
    // проверяем, что массив был отсортирован корректно
    for (int i = 0; i < n; ++i) {
        if (arr[i] != arr_sorted[i]) {
            printf("ERROR: expected value %.17lf, found %.17lf\n", arr_sorted[i], arr[i]);
            exit(EXIT_FAILURE);
        }
    }
    printf("Comparasions: %d\n", comp_cnt);
    printf("Swaps: %d\n\n", swap_cnt);
    free(arr);
}

int main(void) {
    intmax_t seed = time(NULL);   // сюда можно подставить свой сид для детерминированного тестирования
    srand(seed);
    printf("Seed: %jd\n", seed);

    // размеры массивов, на которых будет производиться тестирование
    int n[] = {10, 100, 1000, 10000, 100000};
    int test_count = sizeof(n) / sizeof(n[0]);

    elem_t *arr, *sorted = NULL;
    for (int i = 0; i < test_count; ++i) {
        arr = realloc(arr, n[i] * sizeof(elem_t));
        sorted = realloc(sorted, n[i] * sizeof(elem_t));
        printf("======= Array size %d =======\n", n[i]);
        for (TestType t = TEST_START + 1; t != TEST_SENTINEL; ++t) {
            printf("--- Test %d ---\n", t);

            gen_arr(t, n[i], arr);
            // отсортированный эталон для проверки
            memcpy(sorted, arr, n[i] * sizeof(elem_t));
            qsort(sorted, n[i], sizeof(elem_t), cmp_desc);

            run_test(t, "Heapsort", heapsort, n[i], arr, sorted);
            run_test(t, "Shellsort (Stasevich sequence)", shellsort_stasevich, n[i], arr, sorted);
            run_test(t, "Shellsort (Sedgewick sequence)", shellsort_sedgewick, n[i], arr, sorted);
        }
        putchar('\n');
    }
    free(arr);
    free(sorted);
    return 0;
}