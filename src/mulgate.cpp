#include "mulgate.h"
using namespace std;
void TriplesMul::init(eRole role, int flag)
{
    MulTriples.init(role, flag);
    if (!flag)
    {
        this->role = role;
        network.init(this->role, port);
    }
}
//矩阵三元组乘法
void TriplesMul::mMul(Matrix &x, Matrix &y, Matrix &ans)
{ //得多次调用对应尺寸的矩阵乘法三元组
    assert(x.col == y.row);
    if (x.row == 1 && x.col == 1 && y.col == 1)
    {
        mocheng(x.matrix[0][0], y.matrix[0][0], ans.matrix[0][0]);
        return;
    }
    string ck_string = this->network.checkMSG, recv_string;
    this->mulgateTools.mResize(x.row, y.col, ans);
    Matrix Ex, Ey, E, Fx, Fy, F, temp;
    MatrixTriples triad = MulTriples.getTriples(x.row, x.col, y.col);
    this->mulgateTools.mSub(x, triad.a, Ex);
    this->mulgateTools.mSub(y, triad.b, Fx);
    if (role == SERVER)
    {
        this->network.mSend(Ex);
        this->network.mReceive(Ey);
        this->network.mSend(Fx);
        this->network.mReceive(Fy);
    }
    else
    {
        this->network.mReceive(Ey);
        this->network.mSend(Ex);
        this->network.mReceive(Fy);
        this->network.mSend(Fx);
    }
    this->mulgateTools.mAdd(Ex, Ey, E);
    this->mulgateTools.mAdd(Fx, Fy, F);
    if (role == SERVER)
    {
        this->mulgateTools.mLocalMul(E, F, temp);
        this->mulgateTools.mAccu(ans, temp);
    }
    this->mulgateTools.mLocalMul(triad.a, F, temp);
    this->mulgateTools.mAccu(ans, temp);
    this->mulgateTools.mLocalMul(E, triad.b, temp);
    this->mulgateTools.mAccu(ans, temp);
    this->mulgateTools.mAccu(ans, triad.c);
}
//矩阵三元组LSTM乘法
void TriplesMul::mMull(Matrix &x, Matrix &y, Matrix &ans)
{ //得多次调用1*1乘法三元组
    assert(x.col == y.col && x.row == y.row);
    this->mulgateTools.mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            this->mocheng(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
        }
    }
}
//矩阵三元组乘方
void TriplesMul::mPow(Matrix &x, Matrix &ans)
{
    this->mMul(x, x, ans);
}
//矩阵三元组并列乘方
void TriplesMul::mPoww(Matrix &x, Matrix &ans)
{
    this->mMull(x, x, ans);
}
//三元组模乘，计算后缩小
void TriplesMul::mocheng(mpz_class &x, mpz_class &y, mpz_class &z)
{
    mpz_class temp, Ex, Ey, E, Fx, Fy, F;
    string ck_string = this->network.checkMSG, recv_string;
    temp = 0;
    IntTriples triad = MulTriples.getTriples();
    this->mulgateTools.mojian(x, triad.a, Ex);
    this->mulgateTools.mojian(y, triad.b, Ey);
    if (role == SERVER)
    {
        this->network.mSend(Ex);
        this->network.mReceive(Ey);
        this->network.mSend(Fx);
        this->network.mReceive(Fy);
    }
    else
    {
        this->network.mReceive(Ey);
        this->network.mSend(Ex);
        this->network.mReceive(Fy);
        this->network.mSend(Fx);
    }
    this->mulgateTools.mojia(Ex, Ey, E);
    this->mulgateTools.mojia(Fx, Fy, F);
    if (role == SERVER)
        temp = E * F;
    z = F * triad.a + E * triad.b + triad.c + temp;
    mpz_div_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC);         //缩小2^32倍
    mpz_mod(z.get_mpz_t(), z.get_mpz_t(), modNum.get_mpz_t()); //取模
}
