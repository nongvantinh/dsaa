// cutting a rod into rods of smaller length in way that maximizes their total value.
/*
Serling Enterprises buys long steel rods and cuts them into shorter rods, 
which it then sells. Each cut is free. The management of Serling Enterprises 
wants to know the best way to cut up the rods.

for i = 1, 2, ... the price "pi" in dollars that Serling Enterprises 
charges for a rod of length i inches. Rod lengths are always an integral number of inches.

Given a rod of length "n" inches and a table of prices "pi" for i = 0, 1, ..., n - 1, 
determine the maximum revenue "rn" obtainable by cutting up the rod and selling the pieces. 

Note that if the price "pn" for a rod of length n is large enough, an optimal solution may require no cutting at all.
length i | 1   2   3   4   5   6   7   8   9   10
price pi | 1   5   8   9   10  17  17  20  24  30

example:
    input:
        n = 4
    putput:
        10
Explain: We see that cutting a 4-inch rod into two 2-inch pieces produces revenue p1 + p1 = 5 + 5 = 10,
which is optimal.

We can cut up a rod of length n in 2^(n - 1) different ways, since we have an independent option of cutting,
or not cutting, at distance i inches from the left end.
*/

#include <iostream>
#include <vector>

int cut_rod(const std::vector<int> &prices, int length, const int cost_cutting);
int top_down_memoized_cut_rod(const std::vector<int> &prices, int length, const int cost_cutting);
void top_down_memoized_cut_rod_with_solutions(const std::vector<int> &prices, int length, const int cost_cutting);
int top_down_memoized_cut_rod_aux(const std::vector<int> &prices, int length, std::vector<int> &results, const int cost_cutting);
int top_down_memoized_cut_rod_with_solutions_aux(const std::vector<int> &prices, int length, std::vector<int> &results, std::vector<int> &solutions, const int cost_cutting);
int bottom_up_cut_rod(const std::vector<int> &prices, int length, const int cost_cutting);
int bottom_up_cut_rod_reconstruct_solution(const std::vector<int> &prices, int length, const int cost_cutting);

int main()
{
    // std::vector<int> i{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> pi{1, 5, 8, 9, 10, 17, 17, 20, 24, 30, 37, 96, 100, 101, 106, 120, 207, 245, 256, 279, 310, 366, 368, 459, 470, 494, 519, 597, 601, 610, 716, 731, 775, 779, 807, 846, 933, 960, 994, 1034, 1085, 1088, 1108, 1185, 1213};
    // std::cout << cut_rod(pi, 4, 1) << "\n";
    // std::cout << top_down_memoized_cut_rod(pi, 4, 1) << "\n";
    // top_down_memoized_cut_rod_with_solutions(pi, 4, 1);
    // std::cout << bottom_up_cut_rod(pi, 4, 1) << "\n";
    // bottom_up_cut_rod_reconstruct_solution(pi, 4, 1);
    return 0;
}

// Simple rod cutting.
int cut_rod(const std::vector<int> &prices, int length, const int cost_cutting)
{
    if (0 == length)
        return 0;

    int max_revenue = INT32_MIN;
    // Index start at 0 so prices[i] mean price at index i with length i + 1.
    for (int i = 0; i != length; ++i)
        max_revenue = std::max(max_revenue, prices[i] + cut_rod(prices, length - i - 1, cost_cutting) - cost_cutting);

    return max_revenue;
}

// Dynamic programming: Top-down cut rod procedure with memoized added.
int top_down_memoized_cut_rod(const std::vector<int> &prices, int length, const int cost_cutting)
{
    std::vector<int> results(length + 1, INT32_MIN);
    return top_down_memoized_cut_rod_aux(prices, length, results, cost_cutting);
}

void top_down_memoized_cut_rod_with_solutions(const std::vector<int> &prices, int length, const int cost_cutting)
{
    std::vector<int> results(length + 1, INT32_MIN);
    std::vector<int> solutions(length + 1, INT32_MIN);

    std::cout << "Length: " << length << "\nDecomposition: ";
    const int cost_cut = 1;
    int r = top_down_memoized_cut_rod_with_solutions_aux(prices, length, results, solutions, cost_cutting);
    while (0 < length)
    {
        std::cout << solutions[length] << " ";
        length -= solutions[length];
    }
    std::cout << "\nmax revenue: " << r << std::endl;
}

int top_down_memoized_cut_rod_aux(const std::vector<int> &prices, int length, std::vector<int> &results, const int cost_cutting)
{
    if (0 <= results[length])
        return results[length];

    int max_revenue = INT32_MIN;
    if (0 == length)
        max_revenue = 0;
    else
    {
        for (int i = 0; i != length; ++i)
            max_revenue = std::max(max_revenue, prices[i] + top_down_memoized_cut_rod_aux(prices, length - i - 1, results, cost_cutting) - cost_cutting);
    }

    results[length] = max_revenue;
    return max_revenue;
}

int top_down_memoized_cut_rod_with_solutions_aux(const std::vector<int> &prices, int length, std::vector<int> &results, std::vector<int> &solutions, const int cost_cutting)
{
    if (0 <= results[length])
        return results[length];

    int max_revenue = INT32_MIN;
    if (0 == length)
        max_revenue = 0;
    else
    {
        for (int i = 0; i != length; ++i)
        {
            int r = prices[i] + top_down_memoized_cut_rod_with_solutions_aux(prices, length - i - 1, results, solutions, cost_cutting) - cost_cutting;
            if (max_revenue < r)
            {
                max_revenue = r;
                solutions[length] = i + 1;
            }
        }
    }

    results[length] = max_revenue;
    return max_revenue;
}

// Dynamic programming: Bottom up cut rod.
int bottom_up_cut_rod(const std::vector<int> &prices, int length, const int cost_cutting)
{
    std::vector<int> results(length + 1, INT32_MIN);
    results[0] = 0;

    for (int i = 1; i != results.size(); ++i)
    {
        int max_revenue = INT32_MIN;
        for (int k = 0; k != i; ++k)
            max_revenue = std::max(max_revenue, prices[k] + results[i - k - 1] - cost_cutting);

        results[i] = max_revenue;
    }

    return results[length];
}

// Dynamic programming: Bottom up cut rod and reconstruct the solution.
int bottom_up_cut_rod_reconstruct_solution(const std::vector<int> &prices, int length, const int cost_cutting)
{
    std::vector<int> results(length + 1, INT32_MIN);
    std::vector<int> solutions(length + 1, INT32_MIN);
    results[0] = 0;

    for (int i = 1; i != results.size(); ++i)
    {
        int max_revenue = INT32_MIN;
        for (int k = 0; k != i; ++k)
        {
            if (max_revenue < (prices[k] + results[i - k - 1] - cost_cutting))
            {
                max_revenue = prices[k] + results[i - k - 1] - cost_cutting;
                solutions[i] = k + 1;
            }
        }

        results[i] = max_revenue;
    }

    std::cout << "Length: " << length << "\nDecomposition: ";
    int piece = length;
    while (0 < piece)
    {
        std::cout << solutions[piece] << " ";
        piece -= solutions[piece];
    }
    std::cout << "\nmax revenue: " << results[length] << std::endl;
    return results[length];
}