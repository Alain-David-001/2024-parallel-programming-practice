#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

__int128 n;
vector<__int128> partial_answers[16], final_answer;

typedef struct worker
{
    pthread_t thread;
    int id, lf, rg;
    vector<__int128> ans;
} worker_t;

void *find(void *arg)
{
    worker_t *me = (worker_t *)arg;

    __int128 myN = n;
    for (__int128 p = me->lf; p <= myN / p && p <= me->rg; ++p)
    {
        while (myN % p == 0)
        {
            partial_answers[me->id].push_back(p);
            myN /= p;
        }
    }

    return NULL;
}

std::istream &operator>>(std::istream &in, __int128 &value)
{
    std::string s;
    in >> s;
    value = 0;
    bool negative = false;
    size_t i = 0;
    if (s[0] == '-')
    {
        negative = true;
        i = 1;
    }
    for (; i < s.size(); ++i)
    {
        value = value * 10 + (s[i] - '0');
    }
    if (negative)
        value = -value;
    return in;
}

std::ostream &operator<<(std::ostream &out, __int128 value)
{
    if (value == 0)
    {
        out << '0';
        return out;
    }
    std::string s;
    bool negative = false;
    if (value < 0)
    {
        negative = true;
        value = -value;
    }
    while (value > 0)
    {
        s += '0' + static_cast<int>(value % 10);
        value /= 10;
    }
    if (negative)
        s += '-';
    std::reverse(s.begin(), s.end());
    out << s;
    return out;
}

__int128 sqrt_int128(__int128 x)
{
    if (x < 0)
    {
        throw std::domain_error("Negative input is not allowed for sqrt");
    }
    if (x == 0 || x == 1)
    {
        return x; // Base cases
    }

    __int128 low = 1, high = x, result = 0;

    while (low <= high)
    {
        __int128 mid = low + (high - low) / 2;
        __int128 mid_squared = mid * mid;

        if (mid_squared == x)
        {
            return mid; // Exact square root
        }
        else if (mid_squared < x)
        {
            result = mid; // Store the floor of the sqrt
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return result; // Floor value of the square root
}

int main()
{
    std::cin >> n;
    if (n <= 1)
    {
        return 0;
    }

    int root = sqrt_int128(n);

    std::vector<__int128>
        factors;

    worker_t workers[16];
    int chunk_size = root / 16;
    // TODO: Think about corner cases
    for (int i = 0; i < 16; i++)
    {
        workers[i].id = i;
        workers[i].ans = partial_answers[i];
        if (i == 0)
        {
            workers[i].lf = 2;
            workers[i].rg = chunk_size;
        }
        if (i > 0 && i < 15)
        {
            workers[i].lf = workers[i - 1].rg + 1;
            workers[i].rg = workers[i].lf + chunk_size;
        }
        if (i == 15)
        {
            workers[i].lf = workers[i - 1].rg + 1;
            workers[i].rg = root;
        }
        pthread_create(&workers[i].thread, NULL, find, &workers[i]);
    }

    for (int i = 0; i < 16; i++)
        pthread_join(workers[i].thread, NULL);

    for (int i = 0; i < 16; i++)
    {
        vector<__int128> aux;
        for (auto x : partial_answers[i])
        {
            for (auto y : final_answer)
                while (x % y == 0)
                    x /= y;
            if (x > 1)
                aux.push_back(x);
        }
        for (auto x : aux)
            final_answer.push_back(x);
    }

    for (auto x : final_answer)
        if (n % x == 0)
            n /= x;
    if (n > 1)
        final_answer.push_back(n);

    for (const auto &factor : final_answer)
    {
        std::cout << factor << ' ';
    }
    std::cout << '\n';

    return 0;
}