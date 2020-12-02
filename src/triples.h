#ifndef __TRAID__
#define __TRAID__
#include "network.h"
#include "matrix.h"
using namespace std;

typedef struct
{
    Matrix a;
    Matrix b;
    Matrix c;
} MatrixTriples;

typedef struct
{
    mpz_class a;
    mpz_class b;
    mpz_class c;
} IntTriples;

class Triples
{
public:
    eRole role;
    array<stack<MatrixTriples>, 12> matrixTriples;
    stack<IntTriples> intTriples;
    enum mSize
    {
        m40m58m1,
        m40m40m1,
        m20m40m1,
        m1m20m1,
        m1m1m20,
        m20m20m1,
        m20m1m1,
        m20m1m40,
        m40m40m20,
        m40m20m1,
        m40m1m40,
        m40m1m58
    };

    IntTriples getTriples();
    MatrixTriples getTriples(int m, int d, int n);
    void init(eRole role);

private:
    Network network;
    MatrixTools tripleTools;
    int port=40343;
    void createMatrixTriple(int m, int d, int n);
    void createIntTriple();
    void creat(int m, int d, int n, int counts);
    mpz_class randNum();//在大素数范围内生成随机数
    string randIndex();//生成随机序列号
    array<array<array<int, 58>, 58>, 58> table;
    int m1m1m1_counts = 100;
    int m40m58m1_counts = 100;
    int m40m40m1_counts = 100;
    int m20m40m1_counts = 100;
    int m1m20m1_counts = 100;
    int m1m1m20_counts = 100;
    int m20m20m1_counts = 100;
    int m20m1m1_counts = 100;
    int m20m1m40_counts = 100;
    int m40m40m20_counts = 100;
    int m40m20m1_counts = 100;
    int m40m1m40_counts = 100;
    int m40m1m58_counts = 100;
};

#endif