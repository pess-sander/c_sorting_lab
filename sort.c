// Космынин Иван, 105 группа
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>

void swap(double *a, double *b) {
    double tmp = *a;
    *a = *b;
    *b = tmp;
}

// Количество сравнений и перестановок
int comps = 0, swaps = 0;

// Пузырьковая сортировка
void bubble_sort(int n, double *arr) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n-1-i; j++) {
            comps++;

            if (arr[j] > arr[j+1]) {
                swap(&arr[j], &arr[j+1]);
                
                swaps++;
            }
        }
    }
}

// Просеивание, начиная с i-го элемента
void heapify(int n, int i, double *arr) {
    // Левый сын
    int l = 2*i + 1;
    // Правый сын
    int r = 2*i + 2;

    // Наибольший из текущего узла и детей
    int largest = i;
    if (l < n && arr[l] > arr[i]) {
        largest = l;
    }

    if (r < n && arr[r] > arr[largest]) {
        largest = r;
    }
    comps += 2;

    // Если какой-то из детей больше текущего узла
    if (largest != i) {
        swap(&arr[largest], &arr[i]);
        swaps++;
        heapify(n, largest, arr);
    }
}

// Пирамидальная сортировка
void heap_sort(int n, double *arr) {
    int heap_size = n;

    // Построение кучи
    for (int i = n/2 - 1; i >= 0; i--) {
        heapify(n, i, arr);
    }

    // Обмен нулевого и последнего эл-ов кучи и просеивание
    for (int i = n-1; i >= 1; i--) {
        swap(&arr[0], &arr[i]);
        swaps++;
        heapify(--heap_size, 0, arr);
    }
}

// Генерация массива определенной длины
// Типы:
// 1. Массив уже упорядочен
// 2. Обратный порядок элементов
// 3. Случайный порядок элементов
double* generate_arr(int n, int type) {
    if (type < 1 || type > 3) return NULL;

    double *arr = malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++) {
        // Элементы по неубыванию
        if (type == 1) {
            if (i == 0) {
                arr[i] = 1;
            } else {
                unsigned long long a = rand() * rand();
                unsigned long long b = rand() * rand() * rand();
                if (b == 0) b = 1000;
                arr[i] = arr[i-1] + (double)a/b;
            }
        }

        // Элементы по невозрастанию
        if (type == 2) {
            if (i == 0) {
                arr[i] = 1;
            } else {
                unsigned long long a = rand() * rand();
                unsigned long long b = rand() * rand() * rand();
                if (b == 0) b = 1000;
                arr[i] = arr[i-1] - (double)a/b;
            }
        }

        // Элементы в случайном порядке
        if (type == 3) {
            long long a = rand() * rand() * rand() * rand() * rand();
            long long b = rand() * rand() * rand() * rand();
            if (b == 0) b = 10000;
            arr[i] = (double)a/b;
        }
    }

    return arr;
}

// Печать полной таблицы
void print_table() {
    // Шапка таблицы
    printf("    n     order  bubble_sort: comparisons      swaps  heap_sort: comparisons      swaps\n");
    
    comps = swaps = 0;
    for (int i = 10; i <= 10000; i *= 10) {
        // Четыре массива, на которых производятся тесты
        double **arrs = malloc(sizeof(double*) * 4);
        arrs[0] = generate_arr(i, 1);
        arrs[1] = generate_arr(i, 2);
        arrs[2] = generate_arr(i, 3);
        arrs[3] = generate_arr(i, 3);

        for (int j = 0; j < 4; j++) {
            // Копия текущего массива (нужно проверить две сортировки)
            double *cpy = malloc(sizeof(double) * i);
            for (int k = 0; k < i; k++) {
                cpy[k] = arrs[j][k];
            }

            printf("%5d  ", i);
            if (j == 0) {
                printf("straight\t\t       ");
            } else if (j == 1) {
                printf("    back\t\t       ");
            } else {
                printf("  random\t\t       ");
            }

            bubble_sort(i, arrs[j]);
            printf("%10d %10d\t\t  ", comps, swaps);
            comps = swaps = 0;

            heap_sort(i, cpy);
            printf("%10d %10d\n", comps, swaps);
            comps = swaps = 0;

            free(cpy);
            free(arrs[j]);
        }

        free(arrs);
    }
}

// Вывод вариантов использования программы
void print_usage(char *arg) {
    printf("Использование: \n\t%s --help или\n\t%s --table [--seed [n]] или\n", arg, arg);
    printf("\t%s --heap_sort|--bubble_sort --count [n] --order 1|2|3 [--seed [n] --array [k]]\n", arg);
}

// Вывод справки
void print_help(char *arg) {
    printf("Данная программа реализует пузырьковую и пирамидальную сортировки, сравнивает кол-во операций сравнения и перемещения, которые в них используются.\n");
    printf("Автор: Космынин И.Н., 105 группа.\n");
    printf("Вариант 3 1 1 5:\n");
    printf("\t3 - действительные числа двойной точности (double);\n");
    printf("\t1 - сортировка по неубыванию;\n");
    printf("\t1 5 - пузырьковая и пирамидальная сортировки.\n");
    print_usage(arg);
    printf("Параметры:\n\t--help|-H справка\n\t--table|-T вывести полную сравнительную таблицу\n");
    printf("\t--bubble_sort|-B использовать пузырьковую сортировку\n\t--heap_sort|-P использовать пирамидальную сортировку\n");
    printf("\t--count|-C [n] задать кол-во элементов массива\n\t--order|-O 1|2|3 задать изначальный порядок эл-ов массива (1 - прямой, 2 - обратный, 3 - случайный)\n");
    printf("\t--seed|-S [s] задать сид для генерации псевдослучайных чисел (если данный аргумент не указан, сид генерируется на основании текущего времени)\n");
    printf("\t--array|-A [k] вывести k первых элементов массива до и после сортировки\n");
}

int main(int argc, char **argv) {
    // Ставит русский язык
    setlocale(LC_ALL, ".UTF-8");

    // Флаги CLI: печатать таблицу, виды сортировки, кол-во эл-ов,
    // тип упорядочивания данных в массиве, сид для генерации псевдослучайных чисел,
    // вывод массива до и после сортировки
    int fl_table = 0, fl_bs = 0, fl_hs = 0, fl_n = 0, fl_type = 0, fl_seed = 0, fl_arr = 0;
    // Кол-во эл-ов в массиве, тип упоряд. данных в массиве, сид,
    // кол-во выводимых первых эл-ов массива
    int n, type, seed, arr_k;

    // Обработка параметров
    // Неверное количество параметров
    if (argc <= 1) {
        print_usage(argv[0]);
        return 0;
    }
    // Справка
    if (!strcmp(argv[1], "--help") || !strcmp(argv[1], "-H")) {
        print_help(argv[0]);
        return 0;
    }

    // Обработка параметров
    int i = 1;
    while (i < argc) {
        if (!strcmp(argv[i], "--heap_sort") || !strcmp(argv[i], "-P")) {
            fl_hs = 1;
        } else if (!strcmp(argv[i], "--bubble_sort") || !strcmp(argv[i], "-B")) {
            fl_bs = 1;
        } else if (!strcmp(argv[i], "--count") || !strcmp(argv[i], "-C")) {
            fl_n = 1;

            if (i+1 > argc-1 || sscanf(argv[i+1], "%d", &n) != 1) {
                printf("Параметр '%s' требует целочисленный аргумент.\n", argv[i]);
                return 0;
            }
            i++;
        } else if (!strcmp(argv[i], "--order") || !strcmp(argv[i], "-O")) {
            fl_type = 1;

            if (i+1 > argc-1 || sscanf(argv[i+1], "%d", &type) != 1 || type < 1 || type > 3) {
                printf("Параметр '%s' требует аргумент 1|2|3\n", argv[i]);
                return 0;
            }
            i++;
        } else if (!strcmp(argv[i], "--table") || !strcmp(argv[i], "-T")) {
            fl_table = 1;
        } else if (!strcmp(argv[i], "--seed") || !strcmp(argv[i], "-S")) {
            fl_seed = 1;

            if (i+1 > argc-1 || sscanf(argv[i+1], "%d", &seed) != 1) {
                printf("Параметр '%s' требует целочисленный аргумент.\n", argv[i]);
                return 0;
            }
            i++;
        } else if (!strcmp(argv[i], "--array") || !strcmp(argv[i], "-A")) {
            fl_arr = 1;

            if (i+1 > argc-1 || sscanf(argv[i+1], "%d", &arr_k) != 1 || arr_k <= 0) {
                printf("Параметр '%s' требует положительный целочисленный аргумент.\n", argv[i]);
                return 0;
            }
            if (arr_k > n) {
                printf("Параметр '%s' не может превосходить число элементов массива.\n", argv[i]);
                return 0;
            }
            i++;
        } else {
            printf("Некорретный аргумент: '%s'", argv[i]);
            return 0;
        }

        i++;
    }

    // Некорректное использование
    if (fl_table && (fl_n || fl_bs || fl_hs || fl_type)) {
        print_usage(argv[0]);
        return 0;
    }

    // Для генерации псевдослучайных чисел
    if (fl_seed)
        srand(seed);
    else
        srand(time(NULL));

    // Вывод полной таблицы
    if (fl_table) {
        print_table();
        return 0;
    }

    // Применение сортировки к массиву, указанному в параметрах
    if (fl_n && (fl_hs || fl_bs) && fl_type) {
        printf("Обработка массива длины %d, элементы которого изначально расположены в ", n);
        if (type == 1)
            printf("прямом");
        if (type == 2)
            printf("обратном");
        if (type == 3)
            printf("случайном");
        printf(" порядке.\n");

        double *arr = generate_arr(n, type);
        double *cpy = malloc(sizeof(double) * n);
        for (int i = 0; i < n; i++) {
            cpy[i] = arr[i];
        }

        if (fl_arr) {
            printf("Первые %d эл-ов массива до сортировки: ", arr_k);
            for (int i = 0; i < arr_k; i++) {
                printf("%lf ", arr[i]);
            }
            printf("\n");
        }

        swaps = comps = 0;
        if (fl_bs) {
            bubble_sort(n, arr);
            printf("Пузырьковая сортировка: %d сравнений, %d перемещений.\n", comps, swaps);
            swaps = comps = 0;

            if (fl_arr) {
                printf("Первые %d эл-ов массива после сортировки пузырьком: ", arr_k);
                for (int i = 0; i < arr_k; i++) {
                    printf("%lf ", arr[i]);
                }
                printf("\n");
            }
        }
        
        if (fl_hs) {
            heap_sort(n, cpy);
            printf("Пирамидальная сортировка: %d сравнений, %d перемещений.\n", comps, swaps);
            comps = swaps = 0;

            if (fl_arr) {
                printf("Первые %d эл-ов массива после пирамидальной сортировки: ", arr_k);
                for (int i = 0; i < arr_k; i++) {
                    printf("%lf ", cpy[i]);
                }
                printf("\n");
            }
        }

        free(arr);
        free(cpy);
    // Указаны не все необходимые аргументы
    } else {
        print_usage(argv[0]);
    }

    return 0;
}