#ifndef __TRAID__
#define __TRAID__
#include "network.h"
#include "matrix.h"
using namespace std;

typedef struct
{
    mpz_class index;
    Matrix a;
    Matrix b;
    Matrix c;
} MatrixTriples;

typedef struct
{
    mpz_class index;
    mpz_class a;
    mpz_class b;
    mpz_class c;
} IntTriples;

class Triples
{
public:
    eRole role;
    array<stack<MatrixTriples>, 13> matrixTriples;
    stack<IntTriples> intTriples;
    IntTriples getTriples();
    MatrixTriples getTriples(int m, int d, int n);
    void triplesGen(eRole role, int epochs);
    bool deserialization(string in_string, mpz_class &index, mpz_class &a, mpz_class &b, mpz_class &c);
    bool deserialization(string in_string, mpz_class &index, Matrix &a, Matrix &b, Matrix &c);
    Triples();
    void init(eRole role,int flag);

private:
    Network network;
    MatrixTools tripleTools;
    int port = 40343;
    void creat(int m, int d, int n, int counts, int flag);
    void createIntTriple(string fileName, int flag);
    void createMatrixTriple(string fileName, int m, int d, int n, int flag);
    void readIntTriples(int flag);
    void readMatrixTriples(int m, int d, int n, int flag);
    string randIndex();                         //生成随机序列号
    array<array<array<int, 58>, 58>, 58> table; //查找表
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
};

#endif