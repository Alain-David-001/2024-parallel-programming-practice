#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>

int min(int a, int b)
{
    return (a < b) ? a : b;
}

std::vector<std::vector<double>> read_matrix() {
    size_t rows, cols;
    std::cin >> rows >> cols;

    size_t a, b, x, y, z, p;
    std::cin >> a >> b >> x >> y >> z >> p;
    std::vector<std::vector<size_t>> intermediate(rows, std::vector<size_t>(cols, b % p));
    intermediate[0][0] = a % p;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (i > 0 && j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j - 1] * x) % p;
            }
            if (i > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i - 1][j] * y) % p;
            }
            if (j > 0) {
                intermediate[i][j] = (intermediate[i][j] + intermediate[i][j - 1] * z) % p;
            }
        }
    }
    size_t max_value = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            max_value = std::max(max_value, intermediate[i][j]);
        }
    }

    std::vector<std::vector<double>> result(rows, std::vector<double>(cols));
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result[i][j] = static_cast<double>(intermediate[i][j]) / static_cast<double>(max_value);
        }
    }

    return result;
}

std::vector<std::vector<double>> left, right, result;
size_t left_rows, left_cols, right_cols;

void work(int l, int r)
{
    for (int i = l; i <= r && i < left_rows; i++)
        for (int k = 0; k < left_cols; ++k)
            for (int j = 0; j < right_cols; ++j)
                result[i][j] += left[i][k] * right[k][j];
}

int main() {
    left = read_matrix();
    right = read_matrix();
    left_rows = left.size();
    left_cols = left[0].size();
    right_cols = right[0].size();

    if (left.empty() || right.empty() || left[0].size() != right.size()) {
        std::cerr << "Wrong matrices";
        return 1;
    }

    result = std::vector<std::vector<double>>(left_rows, std::vector<double>(right_cols, 0));
    int chunk_size = left_rows / 16;
    int remainder = left_rows % 16;
    int lll = 0;
    int l[16], r[16];
    for (int i = 0; i < 16; i++)
    {
        int extra = (i < remainder) ? 1 : 0; // Assign extra work to the first few threads

        if (lll > left_rows) // Not enough work for all threads
            l[i] = r[i] = -1;
        else
        {
            l[i] = lll;
            r[i] = min(lll + chunk_size - 1 + extra, left_rows);
        }
        lll += chunk_size;
        if (i < remainder)
            lll++;
    }

    std::thread threads[16];
    for (int i = 0; i < 16; i++)
        threads[i] = std::thread(work, l[i], r[i]);

    for (int i = 0; i < 16; i++)
        threads[i].join();

    std::cout << left_rows << ' ' << right_cols << "\n";
    for (int i = 0; i < left_rows; ++i) {
        for (int j = 0; j < right_cols; ++j) {
            std::cout << std::setprecision(12) << result[i][j] << ' ';
        }
        std::cout << "\n";
    }

    return 0;
}