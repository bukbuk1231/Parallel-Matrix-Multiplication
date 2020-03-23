// Parallel Matrix Multiplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Junda Lou

#include <iostream>
#include <omp.h>
#include <vector>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h>
#include <string>

using namespace std;

vector<vector<int>> mmser(vector<vector<int>>, vector<vector<int>>);
vector<vector<int>> mmpar(vector<vector<int>>, vector<vector<int>>);
vector<vector<int>> mm1d(vector<vector<int>>, vector<vector<int>>);
vector<vector<int>> mm2d(vector<vector<int>>, vector<vector<int>>);
vector<vector<int>> init_matrix(int, int, bool);
int get_random_number(int);
string matrix_to_string(vector<vector<int>>);

int main(int argc, char **argv) {
    int threads = atoi(argv[1]);
    int m = atoi(argv[2]);
    int n = atoi(argv[3]);
    int q = atoi(argv[4]);

    srand(time(NULL));
    omp_set_num_threads(threads);

    vector<vector<int>> m1 = init_matrix(m, n, true);
    vector<vector<int>> m2 = init_matrix(n, q, true);
    //vector<vector<int>> m1{ {1, 2, 3}, {4, 5, 6} };
    //vector<vector<int>> m2{ {7, 8}, {9, 10}, {11, 12} };

    vector<vector<int>> mult;
    mult = mmser(m1, m2);
    mult = mmpar(m1, m2);
    mult = mm1d(m1, m2);
    mult = mm2d(m1, m2);

    // cout << "left\n" << matrix_to_string(m1) << "right\n" << matrix_to_string(m2) << "result\n" << matrix_to_string(mult);
}

vector<vector<int>> mmser(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    double timeSpent = 0.0;
    clock_t begin = clock();
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n2; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    clock_t end = clock();
    timeSpent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %f seconds\n", timeSpent);
    return res;
}

vector<vector<int>> mmpar(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    double timeSpent = 0.0;
    clock_t begin = clock();

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        //int id = omp_get_thread_num();
        //cout << id << endl;

        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n2; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }

    clock_t end = clock();
    timeSpent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %f seconds\n", timeSpent);
    return res;
}

vector<vector<int>> mm1d(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    double timeSpent = 0.0;
    clock_t begin = clock();

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        if (thread_id < m) {
            for (int i = 0; i < n2; i++) {
                for (int j = 0; j < n; j++) {
                    res[thread_id][i] += m1[thread_id][j] * m2[j][i];
                }
            }
        }
        
    }

    clock_t end = clock();
    timeSpent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %f seconds\n", timeSpent);
    return res;
}

vector<vector<int>> mm2d(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    double timeSpent = 0.0;
    clock_t begin = clock();

    omp_set_nested(1);
    #pragma omp parallel
    {
        int thread_id1 = omp_get_thread_num();
        if (thread_id1 < m) {
            #pragma omp parallel
            {
                int thread_id2 = omp_get_thread_num();
                if (thread_id2 < n2) {
                    for (int j = 0; j < n; j++) {
                        res[thread_id1][thread_id2] += m1[thread_id1][j] * m2[j][thread_id2];
                    }
                }
            }
        }
    }

    clock_t end = clock();
    timeSpent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Elapsed Time: %f seconds\n", timeSpent);
    return res;
}

vector<vector<int>> init_matrix(int row, int col, bool random) {
    vector<vector<int>> matrix;
    for (int i = 0; i < row; i++) {
        vector<int> tmp;
        for (int j = 0; j < col; j++) {
            tmp.push_back(random ? get_random_number(10) : 0);
        }
        matrix.push_back(tmp);
    }
    return matrix;
}

int get_random_number(int bound) {
    return rand() % bound + 1;
}

string matrix_to_string(vector<vector<int>> matrix) {
    string res = "";
    int m = matrix.size(), n = matrix[0].size();
    for (int i = 0; i < m; i++) {
        res += "[";
        for (int j = 0; j < n - 1; j++) {
            res += to_string(matrix[i][j]) + ", ";
        }
        if (n > 0) {
            res += to_string(matrix[i][n - 1]);
        }
        res += "]\n";
    }
    res += "\n";
    return res;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
