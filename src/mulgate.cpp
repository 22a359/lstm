#include "mulgate.h"

using namespace std;

//初始化
void TriplesMul::init(eRole role, int flag)
{
    if (!flag)
    {
        this->role = role;
        if (this->network.sockSer == -1)
            this->network.init(this->role, port);
    }
    MulTriples.init(role, flag);
}

//矩阵三元组乘法
void TriplesMul::mMul(Matrix x, Matrix y, Matrix &ans)
{ //得多次调用对应尺寸的矩阵乘法三元组
    assert(x.col == y.row);
    if (x.row == 1 && x.col == 1 && y.col == 1)
    {
        mocheng(x.matrix[0][0], y.matrix[0][0], ans.matrix[0][0]);
        return;
    }
    string ck_string = checkMSG, recv_string;
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
    } else
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

    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mpz_class z = ans.matrix[i][j];
            //mpz_print(z, "模后");
            //getPlain(z, "模后明文");
            if (z != 0)
            {
                if (role == SERVER)
                {
                    mpz_class temp_div;
                    temp_div = modNum - z; //先模减
                    //mpz_print(temp_div, "模减");
                    mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
                    //mpz_print(z, "截断");
                    z = modNum - z; //再模减
                    //mpz_print(z, "模减");
                } else
                {
                    mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小2^32倍
                    //mpz_print(z, "直接截断");
                }
                //getPlain(z, "计算明文");
            }
            ans.matrix[i][j] = z;
        }
    }
}

//矩阵三元组LSTM乘法
void TriplesMul::mMull(Matrix x, Matrix y, Matrix &ans)
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

//矩阵三元组并列乘方
void TriplesMul::mPoww(Matrix x, Matrix &ans)
{
    this->mMull(x, x, ans);
}

//三元组模乘，计算后缩小
void TriplesMul::mocheng(mpz_class x, mpz_class y, mpz_class &z)
{
    mpz_class Ex, Ey, E, Fx, Fy, F;
    string ck_string = checkMSG, recv_string;
    mpz_class temp1, temp2, temp3, temp4 = 0;
    IntTriples triad = MulTriples.getTriples();
    //三元组乘法
    this->mulgateTools.mojian(x, triad.a, Ex);
    this->mulgateTools.mojian(y, triad.b, Fx);
    if (role == SERVER)
    {
        this->network.mSend(Ex);
        this->network.mReceive(Ey);
        this->network.mSend(Fx);
        this->network.mReceive(Fy);
    } else
    {
        this->network.mReceive(Ey);
        this->network.mSend(Ex);
        this->network.mReceive(Fy);
        this->network.mSend(Fx);
    }
    this->mulgateTools.mojia(Ex, Ey, E);
    this->mulgateTools.mojia(Fx, Fy, F);
    if (role == SERVER)
        mpz_mul(temp4.get_mpz_t(), E.get_mpz_t(), F.get_mpz_t());
    mpz_mul(temp1.get_mpz_t(), F.get_mpz_t(), triad.a.get_mpz_t());
    mpz_mul(temp2.get_mpz_t(), E.get_mpz_t(), triad.b.get_mpz_t());
    temp3 = triad.c;
    z = temp1 + temp2 + temp3 + temp4;
    mpz_mod(z.get_mpz_t(), z.get_mpz_t(), modNum.get_mpz_t()); //取模
    //截断操作
    if (z != 0)
    {
        if (role == SERVER)
        {
            mpz_class temp_div;
            temp_div = modNum - z; //先模减
            mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
            z = modNum - z;                                              //再模减
        } else
            mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小2^32倍
    }
}

//矩阵常数乘法，截断
void TriplesMul::mConstMul(Matrix x, Matrix &ans, mpz_ptr num)
{
    this->mulgateTools.mResize(x.row, x.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < x.col; j++)
        {
            mpz_class zz(num);
            this->mulgateTools.mLocalMocheng(x.matrix[i][j], zz, ans.matrix[i][j]); //使用本地模乘
            mpz_class z = ans.matrix[i][j];
            if (z != 0)
            {
                if (role == SERVER)
                {
                    mpz_class temp_div;
                    temp_div = modNum - z; //先模减
                    mpz_tdiv_q_2exp(z.get_mpz_t(), temp_div.get_mpz_t(), eAndC); //缩小
                    z = modNum - z;                                              //再模减
                } else
                    mpz_tdiv_q_2exp(z.get_mpz_t(), z.get_mpz_t(), eAndC); //缩小
            }
            ans.matrix[i][j] = z;
        }
    }
}

void TriplesMul::sigmoid(Matrix &matrix)
{ //a+bx+cx^2+dx^3
    Matrix powerAns1, powerAns2, addAns1, addAns2, addAns3;
    Matrix onceAns, twiceAns, thriceAns, msig_0;
    if (this->role)
    {
        Matrix temp{M_NORMAL, sig0, matrix.row, matrix.col}; //零次项系数,a
        mulgateTools.mCopy(temp, msig_0);
    } else
    {
        Matrix temp{M_NORMAL, 0, matrix.row, matrix.col}; //零次项系数,a
        mulgateTools.mCopy(temp, msig_0);
    }
    this->mConstMul(matrix, onceAns, sig1.get_mpz_t()); //乘一次项系数，bx
    this->mPoww(matrix, powerAns1); //二次项，x^2
    this->mConstMul(powerAns1, twiceAns, sig2.get_mpz_t()); //二次项系数，cx^2
    this->mMull(matrix, powerAns1, powerAns2); //三次项，x^3
    this->mConstMul(powerAns2, thriceAns, sig3.get_mpz_t()); //三次项系数，dx^3
    mulgateTools.mAdd(thriceAns, twiceAns, addAns1); //三次加二次，cx^2 + dx^3
    mulgateTools.mAdd(addAns1, onceAns, addAns2);    //再加一次，bx + cx^2 + dx^3
    mulgateTools.mAdd(addAns2, msig_0, addAns3);     //再加零次，a + bx + cx^2 + dx^3
    mulgateTools.mCopy(addAns3, matrix);             //结果
}

void TriplesMul::tanh(Matrix matrix, Matrix &ans)
{
    Matrix temp{M_NORMAL, 0, matrix.row, matrix.col}, temp2{M_NORMAL, 0, matrix.row, matrix.col};
    Matrix const_one(M_CIPHER, 1, matrix.row, matrix.col);
    mpz_class two = 2;
    mulgateTools.mConstMulOrigin(matrix, temp, two.get_mpz_t());
    this->sigmoid(temp);
    mulgateTools.mConstMulOrigin(temp, temp2, two.get_mpz_t());
    mulgateTools.mSub(temp2, const_one, matrix);
    mulgateTools.mCopy(matrix, ans);
}

//恢复矩阵明文结果
void TriplesMul::getPlain(Matrix cipher, string outputWord)
{
    Matrix cipher1, cipher2, temp;
    this->mulgateTools.mCopy(cipher, cipher1);
    if (this->role == SERVER)
    {
        this->network.mSend(cipher1);
        this->network.mReceive(cipher2);
    } else
    {
        this->network.mReceive(cipher2);
        this->network.mSend(cipher1);
    }
    this->mulgateTools.mAdd(cipher1, cipher2, temp);
    cout << outputWord << ": ";
    if (!(cipher.row == 1 && cipher.col == 1))
        cout << endl;
    for (int i = 0; i < temp.row; i++)
    {
        for (int j = 0; j < temp.col; j++)
        {
            mpz_class tempZ, tempMod;
            mpf_class tempF;
            if (temp.matrix[i][j].get_d() != 0)
            {
                mpf_class tempF1{temp.matrix[i][j]};
                if (tempF1 > modNum / 2)
                    tempF1 -= modNum;
                mpf_div_2exp(tempF.get_mpf_t(), tempF1.get_mpf_t(), eAndC);
                gmp_printf("(%d-%d) %.5Ff\t", i, j, tempF.get_mpf_t());
            } else
                gmp_printf("(%d-%d) %Zd\t", i, j, temp.matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

//恢复数明文结果
mpf_class TriplesMul::getPlain(mpz_class cipher, string outputWord)
{
    mpz_class cipher2, temp;
    if (this->role == SERVER)
    {
        this->network.mSend(cipher);
        this->network.mReceive(cipher2);
    } else
    {
        this->network.mReceive(cipher2);
        this->network.mSend(cipher);
    }
    temp = (cipher + cipher2);
    temp = temp % modNum;
    cout << outputWord << ": ";
    mpf_class tempF{temp};
    if (tempF != 0)
    {
        if (tempF > modNum / 2)
            tempF -= modNum;
        mpf_div_2exp(tempF.get_mpf_t(), tempF.get_mpf_t(), eAndC);
        gmp_printf("%.8Ff\n", tempF.get_mpf_t());
    } else
        gmp_printf("%.8Ff\n", tempF.get_mpf_t());
    return tempF;
}