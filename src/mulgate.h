#ifndef __MULGATE__
#define __MULGATE__

#include "network.h"
#include "triples.h"
#include "matrix.h"

using namespace std;

class TriplesMul
{
public:
    eRole role;

    void mMul(Matrix x, Matrix y, Matrix &ans);  //矩阵乘法

    void mMull(Matrix x, Matrix y, Matrix &ans); //矩阵并列乘法
    void mPoww(Matrix x, Matrix &ans);            //矩阵并列乘方
    void mocheng(mpz_class x, mpz_class y, mpz_class &z);

    void mConstMul(Matrix x, Matrix &ans, mpz_ptr num);

    void sigmoid(Matrix &matrix);

    void tanh(Matrix matrix, Matrix &ans);

    void init(eRole role, int flag);

    void getPlain(Matrix cipher, string outputWord);

    mpf_class getPlain(mpz_class cipher, string outputWord);

private:
    MatrixTools mulgateTools;
    Network network;
    Triples MulTriples;
    int port = 30434;
};

#endif
