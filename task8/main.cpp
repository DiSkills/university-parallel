#include <amp.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

typedef std::vector<int> Matrix;

static void matrix_multiply(const Matrix &A, const Matrix &B, Matrix &C,
                            int M, int N, int K)
{
    concurrency::array_view<const int, 2> a(M, N, A);
    concurrency::array_view<const int, 2> b(N, K, B);
    concurrency::array_view<int, 2> c(M, K, C);
    c.discard_data();

    concurrency::parallel_for_each(c.extent,
        [=](concurrency::index<2> idx) restrict(amp) {
            int row = idx[0];
            int col = idx[1];
            int sum = 0;
            for (int i = 0; i < N; ++i) {
                sum += a(row, i) * b(i, col);
            }
            c[idx] = sum;
        });

    c.synchronize();
}

static void generate_matrix(Matrix &matrix, int rows, int cols)
{
    for (int i = 0; i < rows * cols; ++i) {
        matrix[i] = rand() % 100;
    }
}

static void print_matrix(const Matrix &matrix, int rows, int cols)
{
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << matrix[i * cols + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main()
{
    std::srand(std::time(0));

    int M = 3, N = 3, K = 3;

    Matrix A(M * N);
    Matrix B(N * K);
    Matrix C(M * K, 0);

    generate_matrix(A, M, N);
    generate_matrix(B, N, K);

    print_matrix(A, M, N);
    print_matrix(B, N, K);

    matrix_multiply(A, B, C, M, N, K);

    print_matrix(C, M, K);
    return 0;
}
