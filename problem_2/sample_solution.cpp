#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <bits/stdc++.h>

int min(int a, int b)
{
    return (a < b) ? a : b;
}

std::vector<size_t> read_array() {
    size_t length, a, b, p;
    std::cin >> length >> a >> b >> p;
    std::vector<size_t> result(length);
    result[0] = a % p;
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = (result[i - 1] * a + b) % p;
    }
    return result;
}

std::priority_queue<std::pair<size_t, std::pair<int, int>>, std::vector<std::pair<size_t, std::pair<int, int>>>, std::greater<std::pair<size_t, std::pair<int, int>>>> pq;
std::vector<size_t> array;

void work(int l, int r)
{
    std::sort(array.begin() + l, array.begin() + r + 1);
}

int main() {
    array = read_array();

    int n = array.size();

    int chunk_size = n / 16;
    int remainder = n % 16;
    int lll = 0;
    int l[16], r[16];
    for (int i = 0; i < 16; i++)
    {
        int extra = (i < remainder) ? 1 : 0; // Assign extra work to the first few threads

        if (lll > n) // Not enough work for all threads
            l[i] = r[i] = -1;
        else
        {
            l[i] = lll;
            r[i] = min(lll + chunk_size - 1 + extra, n);
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

    std::vector<size_t> result(n);
    int pos = 0;

    for (int i = 0; i < 16; i++)
        pq.push({array[l[i]], {l[i], i}});

    while (!pq.empty())
    {
        auto p = pq.top();
        pq.pop();
        int ind = p.second.first, owner = p.second.second;
        result[pos] = p.first;
        pos++;

        ind++;
        if (ind <= r[owner])
            pq.push({array[ind], {ind, owner}});
    }

    size_t k;
    std::cin >> k;
    for (size_t i = k - 1; i < array.size(); i += k) {
        std::cout << result[i] << ' ';
    }
    std::cout << "\n";

    return 0;
}