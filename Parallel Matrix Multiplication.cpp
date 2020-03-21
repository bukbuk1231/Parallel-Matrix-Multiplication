// Parallel Matrix Multiplication.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
vector<vector<int>> init_matrix(int, int, bool);
int get_random_number(int);
string matrix_to_string(vector<vector<int>>);

int main() {
    srand(time(NULL));

    vector<vector<int>> m1 = init_matrix(2, 2, true);
    vector<vector<int>> m2 = init_matrix(2, 2, true);

    // vector<vector<int>> mult = mmser(m1, m2);
    vector<vector<int>> mult = mmpar(m1, m2);

    cout << "left\n" << matrix_to_string(m1) << "right\n" << matrix_to_string(m2) << "result\n" << matrix_to_string(mult);
}

vector<vector<int>> mmser(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n2; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return res;
}

vector<vector<int>> mmpar(vector<vector<int>> m1, vector<vector<int>> m2) {
    int m = m1.size(), n = m1[0].size(), n2 = m2[0].size();
    vector<vector<int>> res = init_matrix(m, n2, false);

    #pragma omp parallel
    for (int i = 0; i < m; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n2; j++) {
                res[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
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
