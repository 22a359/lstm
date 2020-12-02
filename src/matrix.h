#ifndef __MATRIX__
#define __MATRIX__

#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <array>
#include <cassert>
#include <stack>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <gmp.h>
#include <gmpxx.h>
using namespace std;

#define eRole bool
#define SERVER true
#define CLIENT false
#define M_NORMAL true
#define M_DIAGONAL false
#define M_GAUSS true
const int baseNum = 10;
extern string modNumStr;
extern mpz_class modNum;
extern mpz_class eAndC;
const mp_bitcnt_t randBit = 512;

class Matrix
{
public:
    int col;
    int row;
    vector<vector<mpz_class>> matrix;
    Matrix();
    Matrix(int row, int col);
    Matrix(int row, int col, int *array);
    Matrix(int row, int col, mpz_class *array);
    Matrix(bool flag, int row, int col);
    Matrix(bool flag, mpz_class num, int row, int col);
    void print(); //矩阵输出
    void print(int flag);
    void change(int positionRow, int positionCol, mpz_class num);

private:
    void CreateMatrix(int row, int col);          //创建随机矩阵
    void CreateMatrix(int num, int row, int col); //创建对角矩阵
};

class MatrixTools
{
public:
    void print(Matrix &m);                               //矩阵输出
    void mCopy(Matrix &from, Matrix &to);                //矩阵复制
    void mTrans(Matrix &from, Matrix &to);               //矩阵转置
    void vTrun(int turnNum, Matrix &from, Matrix &to);   //向量截取
    void vConcat(Matrix &x, Matrix &y, Matrix &ans);     //向量拼接
    void mAdd(Matrix &x, Matrix &y, Matrix &ans);        //矩阵加法
    void mAccu(Matrix &x, Matrix &y);                    //矩阵累加,x+=y
    void mAccuSub(Matrix &x, Matrix &y);                 //矩阵累减,x-=y
    void mConstMul(Matrix &x, Matrix &ans, int num);     //矩阵常数乘法
    void mConstMul(Matrix &x, Matrix &ans, mpz_ptr num); //矩阵常数乘法
    void mSub(Matrix &x, Matrix &y, Matrix &ans);        //矩阵减法
    void mLocalMul(Matrix &x, Matrix &y, Matrix &ans);   //矩阵本地乘法
    void mLocalMull(Matrix &x, Matrix &y, Matrix &ans);  //矩阵本地并列乘法
        // void mDot(Matrix &matrix, Matrix &vector, Matrix &ans); //矩阵内积
        // void vOuter(Matrix &x, Matrix &y, Matrix &ans);         //向量外积
    void mVector2Matrix(Matrix vector, Matrix &matrix);
    void mojia(mpz_class &x, mpz_class &y, mpz_class &z);
    void mAccu(mpz_class &x, mpz_class &y);
    void mojian(mpz_class &x, mpz_class &y, mpz_class &z);
    void mLocalMocheng(mpz_class &x, mpz_class &y, mpz_class &z);
    void mResize(int row, int col, Matrix &y);
};
#endif