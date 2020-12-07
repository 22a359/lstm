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
    void mMul(Matrix &x, Matrix &y, Matrix &ans);  //矩阵乘法
    void mMull(Matrix &x, Matrix &y, Matrix &ans); //矩阵并列乘法
    void mPow(Matrix &x, Matrix &ans);             //矩阵乘方
    void mPoww(Matrix &x, Matrix &ans);            //矩阵并列乘方
    void mocheng(mpz_class &x, mpz_class &y, mpz_class &z);
    void init(eRole role, int flag);

private:
    MatrixTools mulgateTools;
    Network network;
    Triples MulTriples;
    int port = 30434;
};
#endif
