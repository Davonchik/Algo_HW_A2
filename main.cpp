#include <iostream>
#include <random>
#include <algorithm>
#include <chrono>
#include <fstream>

static constexpr int START = 5000;
static constexpr int END = 100000;
static constexpr int STEP = 1000;

struct TestResult {
    int n;
    long long avg_time;
    TestResult(int n, long long avg_time): n(n), avg_time(avg_time) {}
};

std::ostream& operator <<(std::ostream& out, const TestResult& res) {
    out << res.n << " " << res.avg_time;
    return out;
}

class ArrayGenerator {
public:
    ArrayGenerator(int seed, int min, int max) : gen(seed), value_dist(min, max) {}

    int* generate_random_array(int n) {
        int* array = new int[n];
        for (int i = 0; i < n; ++i) {
            array[i] = value_dist(gen);
        }
        return array;
    }

    int* generate_reversed(int n) {
        int* array = generate_random_array(n);
        std::sort(array, array+n);
        std::reverse(array, array+n);
        return array;
    }

    int* generate_near_sorted(int n, int swap_cnt) {
        int* array = generate_random_array(n);
        std::sort(array, array+n);
        std::uniform_int_distribution<> dist(0, n-1);
        for (int i = 0; i < swap_cnt; ++i) {
            int g1 = dist(gen);
            int g2 = dist(gen);
            std::swap(array[g1], array[g2]);
        }
        return array;
    }

private:
    std::mt19937 gen;
    std::uniform_int_distribution<> value_dist;
};

void merge(int* first1, int* last1, int* first2, int* last2, int* dst) {
    while (first1 < last1 || first2 < last2) {
        if (first2 == last2 || (first1 < last1 && *first1 < *first2)) {
            *dst++ = *first1++;
        } else {
            *dst++ = *first2++;
        }
    }
}

void insertion_sort(int* first, int* last) {
    for (int* it = first + 1; it < last; ++it) {
        int* j = it;
        while (j > first && *j < *(j - 1)) {
            std::swap(*j, *(j-1));
            j--;
        }
    }
}

void merge_sort_impl(int* first, int* last, int* buf, bool use_insert_sort) {
    if (first + 1 >= last) {
        return;
    }
    if (use_insert_sort && last - first <= 15) {
        insertion_sort(first, last);
        return;
    }
    int* mid = first + (last - first) / 2;
    merge_sort_impl(first, mid, buf, use_insert_sort);
    merge_sort_impl(mid, last, buf, use_insert_sort);
    merge(first, mid, mid, last, buf);
    std::copy(buf, buf+(last-first), first);
}

void merge_sort_improved(int* first, int* last) {
    int* buf = new int[last - first];
    merge_sort_impl(first, last, buf, true);
    delete[] buf;
}

void merge_sort(int* first, int* last) {
    int* buf = new int[last - first];
    merge_sort_impl(first, last, buf, false);
    delete[] buf;
}

void print_array(int* array, int n) {
    for (int i = 0; i < n; ++i) {
        std::cout << array[i] << ' ';
    }
    std::cout << std::endl;
}

void write_into_file(std::string file_name, const std::vector<TestResult>& data) {
    std::ofstream out(file_name);
    for (const auto& res: data) {
        out << res << std::endl;
    }
}

enum gen_type{
    random,
    sort_reversed,
    part_sort
};

int* copy_mas(int* a, int n) {
    int* copied_mas = new int[n];
    for (int i = 0; i < n; ++i) {
        copied_mas[i] = a[i];
    }
    return copied_mas;
}

class SortTester {
public:
    SortTester(int start, int end, int step) : start(start), end(end), step(step) {}

    std::vector<TestResult> test(gen_type gt, void(*sort)(int*, int*)) {
        ArrayGenerator generator(42, 0, 6000);
        std::vector<TestResult> res;

        int* arr;
        for (int i = start; i <= end; i += step) {
            long long full_time = 0;
            if (gt == random) {
                arr = generator.generate_random_array(i);
            } else if (gt == sort_reversed) {
                arr = generator.generate_reversed(i);
            } else {
                arr = generator.generate_near_sorted(i, 100);
            }
            for (int j = 0; j < 10; ++j) {
                int* arr_copy = copy_mas(arr, i);
                auto start = std::chrono::high_resolution_clock::now();
                sort(arr_copy, arr_copy+i);
                auto elapsed = std::chrono::high_resolution_clock::now() - start;
                long long msec = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
                full_time += msec;
                delete[] arr_copy;
            }
            delete[] arr;
            res.emplace_back(i, full_time / 10);
        }
        return res;
    }
private:
    int start;
    int end;
    int step;
};

int main() {
    SortTester tester(START, END, STEP);
    auto m_s_test1 = tester.test(random, merge_sort);
    auto m_s_test2 = tester.test(sort_reversed, merge_sort);
    auto m_s_test3 = tester.test(part_sort, merge_sort);

    auto m_s_i_test1 = tester.test(random, merge_sort_improved);
    auto m_s_i_test2 = tester.test(sort_reversed, merge_sort_improved);
    auto m_s_i_test3 = tester.test(part_sort, merge_sort_improved);

    write_into_file("m_s_test1.txt", m_s_test1);
    write_into_file("m_s_test2.txt", m_s_test2);
    write_into_file("m_s_test3.txt", m_s_test3);

    write_into_file("m_s_i_test1.txt", m_s_i_test1);
    write_into_file("m_s_i_test2.txt", m_s_i_test2);
    write_into_file("m_s_i_test3.txt", m_s_i_test3);

    return 0;
}