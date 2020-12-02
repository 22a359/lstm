#include "matrix.h"
using namespace std;
mpz_class modNum;
mpz_class eAndC = 4294967296;
// string modNumStr = "618970019642690137449562111";
string modNumStr = "40343";
Matrix::Matrix()
{
    this->col = this->row = 0;
}
//生成随机矩阵
Matrix::Matrix(int row, int col)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        random_device rd;
        default_random_engine e(rd());
        gmp_randstate_t grt;
        gmp_randinit_mt(grt);
        gmp_randseed_ui(grt, e());
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            mpz_t z, r;
            mpz_init(z);
            mpz_init(r);
            mpz_urandomb(z, grt, randBit);
            mpz_mod(r, z, modNum.get_mpz_t());
            this->matrix[i][j] = mpz_class(r);
            mpz_clear(z);
            mpz_clear(r);
        }
        gmp_randclear(grt);
    }
}
//生成符合正态分布的矩阵
Matrix::Matrix(bool flag, int row, int col)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            if (flag)
            {
                normal_distribution<> dist;
                random_device rd;
                default_random_engine rng{rd()};
                double randNum;
                while ((randNum = dist(rng)) > 1.0 || randNum < -1.0)
                    ;
                int num = randNum * 1000;
                this->matrix[i][j] = mpz_class(num);
            }
        }
    }
}
//生成满矩阵或对角矩阵
Matrix::Matrix(bool flag, mpz_class num, int row, int col)
{
    this->row = row;
    this->col = col;
    mpz_class zero = 0;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            if (flag)
                this->matrix[i][j] = mpz_class(num);
            else
            {
                if (j == i)
                    this->matrix[i][j] = mpz_class(num);
                else
                    this->matrix[i][j] = mpz_class(zero);
            }
        }
    }
}
//生成指定内容的矩阵
Matrix::Matrix(int row, int col, int *array)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            this->matrix[i][j] = mpz_class(*array);
            array++;
        }
    }
}
//生成指定内容的矩阵
Matrix::Matrix(int row, int col, mpz_class *array)
{
    this->row = row;
    this->col = col;
    this->matrix.resize(this->row);
    for (int i = 0; i < this->row; i++)
    {
        this->matrix[i].resize(this->col);
        for (int j = 0; j < this->col; j++)
        {
            this->matrix[i][j] = *array;
            array++;
        }
    }
}
//将第positionRow行，第positionCol列的数替换为num
void Matrix::change(int positionRow, int positionCol, mpz_class num)
{
    this->matrix[positionRow][positionCol] = mpz_class(num);
}
//矩阵输出
void Matrix::print()
{
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            gmp_printf("%Zd	", this->matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}
void Matrix::print(int flag)
{
    for (int i = 0; i < this->row; i++)
    {
        for (int j = 0; j < this->col; j++)
        {
            gmp_printf("(%d-%d) %Zd	", i, j, this->matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}

//矩阵输出
void MatrixTools::print(Matrix &m)
{
    for (int i = 0; i < m.row; i++)
    {
        for (int j = 0; j < m.col; j++)
        {
            gmp_printf("%Zd	", m.matrix[i][j].get_mpz_t());
        }
        printf("\n");
    }
    printf("\n");
}
//矩阵复制
void MatrixTools::mCopy(Matrix &from, Matrix &to)
{
    this->mResize(from.row, from.col, to);
    for (int i = 0; i < from.row; i++)
    {
        for (int j = 0; j < from.col; j++)
        {
            to.matrix[i][j] = from.matrix[i][j];
        }
    }
}
//矩阵转置
void MatrixTools::mTrans(Matrix &from, Matrix &to)
{
    this->mResize(from.col, from.row, to);
    for (int i = 0; i < to.row; i++)
    {
        for (int j = 0; j < to.col; j++)
        {
            to.matrix[i][j] = from.matrix[j][i];
        }
    }
}
//向量截取，舍弃向量from的前trunNum个元素，截取后面的部分存于向量ans中
void MatrixTools::vTrun(int trunNum, Matrix &from, Matrix &to)
{
    this->mResize(1, from.col - trunNum, to);
    for (int i = 0; i < to.col; i++)
    {
        to.matrix[0][i] = from.matrix[0][i + trunNum];
    }
}
//向量拼接
void MatrixTools::vConcat(Matrix &x, Matrix &y, Matrix &ans)
{
    this->mResize(1, x.col + y.col, ans);
    int i, j;
    j = x.col;
    for (i = 0; i < x.col; i++)
    {
        ans.matrix[0][i] = x.matrix[0][i];
    }
    for (i = 0; i < y.col; i++)
    {
        ans.matrix[0][i + j] = y.matrix[0][i];
    }
}
//矩阵加法
void MatrixTools::mAdd(Matrix &x, Matrix &y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mojia(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
        }
    }
}
//矩阵累加,x+=y
void MatrixTools::mAccu(Matrix &x, Matrix &y)
{
    Matrix temp;
    this->mAdd(x, y, temp);
    this->mCopy(temp, x);
}
//矩阵累减,x-=y
void MatrixTools::mAccuSub(Matrix &x, Matrix &y)
{
    Matrix temp;
    this->mSub(x, y, temp);
    this->mCopy(temp, x);
}
//矩阵常数乘法
void MatrixTools::mConstMul(Matrix &x, Matrix &ans, int num)
{
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < x.col; j++)
        {
            mpz_class zz(num);
            this->mLocalMocheng(x.matrix[i][j], zz, ans.matrix[i][j]); //使用本地模乘
        }
    }
}
//矩阵常数乘法
void MatrixTools::mConstMul(Matrix &x, Matrix &ans, mpz_ptr num)
{
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < x.col; j++)
        {
            mpz_class zz(num);
            this->mLocalMocheng(x.matrix[i][j], zz, ans.matrix[i][j]); //使用本地模乘
        }
    }
}
//矩阵减法
void MatrixTools::mSub(Matrix &x, Matrix &y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            mpz_class a, b;
            a = -1;
            this->mLocalMocheng(a, y.matrix[i][j], b); //使用本地模乘
            this->mojia(x.matrix[i][j], b, ans.matrix[i][j]);
        }
    }
}
//矩阵本地乘法，用于三元组生成
void MatrixTools::mLocalMul(Matrix &x, Matrix &y, Matrix &ans)
{
    assert(x.col == y.row);
    this->mResize(x.row, y.col, ans);
    for (int i = 0; i < x.row; i++)
    {
        for (int j = 0; j < y.col; j++)
        {
            mpz_class sum1, sum4, sum5;
            sum1 = 0;
            sum4 = 0;
            for (int m = 0; m < y.row; m++)
            {
                this->mLocalMocheng(x.matrix[i][m], y.matrix[m][j], sum1); //使用本地模乘
                this->mojia(sum1, sum4, sum5);
                sum4 = sum5;
            }
            ans.matrix[i][j] = sum4;
        }
    }
}
//矩阵三元组LSTM乘法
void MatrixTools::mLocalMull(Matrix &x, Matrix &y, Matrix &ans)
{
    assert(x.col == y.col && x.row == y.row);
    this->mResize(x.row, x.col, ans);
    for (int i = 0; i < ans.row; i++)
    {
        for (int j = 0; j < ans.col; j++)
        {
            this->mLocalMocheng(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
        }
    }
}
//矩阵与向量三元组内积
// void MatrixTools::mDot(Matrix &matrix, Matrix &vector, Matrix &ans)
// {
//     assert(vector.row == 1 && matrix.col == vector.col);
//     this->mResize(1, vector.col, ans);
//     Matrix vectorT(M_NORMAL, 0, vector.col, 1);
//     Matrix ansT(M_NORMAL, 0, vector.col, 1);
//     this->mTrans(vector, vectorT);
//     this->mMul(matrix, vectorT, ansT);
//     this->mTrans(ansT, ans);
// }
// //向量三元组外积
// void MatrixTools::vOuter(Matrix &x, Matrix &y, Matrix &ans)
// {
//     assert(x.row == y.row == 1);
//     this->mResize(x.col, y.col, ans);
//     Matrix xt(M_NORMAL, 0, x.col, 1);
//     this->mTrans(x, xt);
//     this->mMul(xt, y, ans);
// }
//矩阵尺寸重设

//矩阵尺寸重设
void MatrixTools::mResize(int row, int col, Matrix &matrix)
{
    if (matrix.col != col || matrix.row != row)
    {
        matrix.row = row;
        matrix.col = col;
        matrix.matrix.resize(matrix.row);
        for (int i = 0; i < matrix.row; i++)
        {
            vector<mpz_class>(matrix.matrix[i]).swap(matrix.matrix[i]);
            matrix.matrix[i].resize(matrix.col);
            for (int j = 0; j < matrix.col; j++)
                matrix.matrix[i][j] = mpz_class("0", baseNum);
        }
    }
}
//向量扩展为对角矩阵
void MatrixTools::mVector2Matrix(Matrix vector, Matrix &matrix)
{
    assert(vector.col == 1);
    this->mResize(vector.row, vector.row, matrix);
    for (int i = 0; i < vector.row; i++)
        matrix.change(i, i, vector.matrix[i][0]);
}
//模加
void MatrixTools::mojia(mpz_class &x, mpz_class &y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_init(a);
    mpz_init(b);
    mpz_init(r);
    mpz_add(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum.get_mpz_t());
    mpz_mod(r, b, modNum.get_mpz_t());
    z = mpz_class(r);
}
//模累加,x+=y
void MatrixTools::mAccu(mpz_class &x, mpz_class &y)
{
    this->mojia(x, y, x);
}
//模减
void MatrixTools::mojian(mpz_class &x, mpz_class &y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_init(a);
    mpz_init(b);
    mpz_init(r);
    mpz_sub(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum.get_mpz_t());
    mpz_mod(r, b, modNum.get_mpz_t());
    z = mpz_class(r);
}
//本地模乘
void MatrixTools::mLocalMocheng(mpz_class &x, mpz_class &y, mpz_class &z)
{
    mpz_t a, b, r;
    mpz_init(a);
    mpz_init(b);
    mpz_init(r);
    mpz_mul(a, x.get_mpz_t(), y.get_mpz_t());
    mpz_add(b, a, modNum.get_mpz_t());
    mpz_mod(r, b, modNum.get_mpz_t());
    z = mpz_class(r);
}