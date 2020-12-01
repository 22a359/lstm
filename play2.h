#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cstring>
#include "client.h"

using namespace std;
MatrixTools tool;
void matrix_triple1(Matrix &x2, Matrix &y2, Matrix &z2, client &sender);

typedef struct
{
	Matrix triple[3];
} MatrixTriples;

typedef struct
{
	mpz_class triple[3];
} IntTriples;

class Triple1 //使用方法：定义一个triple1类，运行run函数，启动socket。分别运行两个create函数，生成一定数量的三元组
{
public:
	vector<MatrixTriples> matrixsyz; //存放三元组的vector,目测有九组三元组需要生成；也可以考虑生成一个二维的vector，只是需要提前设定好每一个位置。
	vector<IntTriples> intsyz;
	client servier;

	void run()
	{
		this->servier.run();
	}

	void CreateMatrixTriple() //设置三个输入参数，m,n,d;对应x(m,n),x(n,d);	或者设置九个？creat函数，预设m,n,d三个参数；直接将9个vector分别放入。
	{
		//	this->servier.run();
		MatrixTriples X;
		X.triple[0] = Matrix(5, 1);
		X.triple[1] = Matrix(1, 5);
		X.triple[2] = Matrix(5, 5);
		matrix_triple1(X.triple[0], X.triple[1], X.triple[2], this->servier);
		this->matrixsyz.push_back(X);
	}

	void CreateIntTriple();

	void MatrixMul(Matrix &x, Matrix &y, Matrix &xy); //添加一组判断函数，找到合适的vector，指定对应的triple

	void IntMul(mpz_class &x, mpz_class &y, mpz_class &xy);

	void mMull(Matrix &x, Matrix &y, Matrix &ans)
	{ //并行矩阵乘法，得多次调用乘法三元组
		assert(x.col == y.col && x.row == y.row);
		for (int i = 0; i < ans.row; i++)
		{
			for (int j = 0; j < ans.col; j++)
			{
				IntMul(x.matrix[i][j], y.matrix[i][j], ans.matrix[i][j]);
			}
		}
	}
};

void matrix_triple1(Matrix &x2, Matrix &y2, Matrix &z2, client &sender)
{
	int i;
	int m, d, n;
	m = x2.row;
	d = x2.col;
	n = y2.col;

	Matrix xy2(x2.row, y2.col);
	tool.mLocalMul(x2, y2, xy2); //mMul改成localmMul,在matrix.cpp中添加localMul;除了三元组的生成和使用，其余矩阵乘法均为在线的；

	Matrix I1(1, n, n);
	Matrix IN1(-1, n, n);

	Matrix R[5];
	R[0] = Matrix(n, n);
	R[1] = Matrix(d, n);
	R[2] = Matrix(m, d);
	R[3] = Matrix(m, n);
	R[4] = Matrix(n, n);

	Matrix Z[5];
	Z[0] = Matrix(n, n);
	Z[1] = Matrix(d, n);
	Z[2] = Matrix(m, d);
	Z[3] = Matrix(m, n);
	Z[4] = Matrix(n, n);

	tool.mSub(I1, R[0], Z[0]);
	tool.mSub(y2, R[1], Z[1]);
	tool.mSub(x2, R[2], Z[2]);
	tool.mSub(xy2, R[3], Z[3]);
	tool.mSub(IN1, R[4], Z[4]);

	int r_int = rand();
	mpz_class r = mpz_class(r_int);

	Matrix W[5];
	W[0] = Matrix(n, n);
	W[1] = Matrix(d, n);
	W[2] = Matrix(m, d);
	W[3] = Matrix(m, n);
	W[4] = Matrix(n, n);

	for (i = 0; i < 5; i++)
	{
		tool.mConstMul(Z[i], W[i], r_int);
	}

	sender.send1(R);
	sender.send1(W);

	Matrix u, v, v1;

	u = Matrix(0, m, n);
	v = Matrix(0, m, n);
	v1 = Matrix(0, m, n);

	sender.recv2(u);
	sender.recv2(v);

	mpz_class p;
	p = "40343";
	mpz_t rn;
	mpz_init(rn);
	mpz_invert(rn, r.get_mpz_t(), p.get_mpz_t());
	mpz_class rw(rn);
	tool.mConstMul(v, v1, rw.get_mpz_t());
	tool.mAdd(u, v1, z2);
}

void Triple1::MatrixMul(Matrix &x, Matrix &y, Matrix &xy)
{

	MatrixTriples &triplesA = this->matrixsyz.back();
	client &sender = this->servier;

	MatrixTriples triplesB, triplesE, triplesF;

	Matrix E;
	Matrix F;
	Matrix N1;
	Matrix N2;
	Matrix N3;

	int m, d, n;
	m = x.row;
	d = x.col;
	n = y.col;
	printf("begin\n");
	triplesE.triple[0] = Matrix(m, d);
	triplesE.triple[1] = Matrix(d, n);
	triplesF.triple[0] = Matrix(m, d);
	triplesF.triple[1] = Matrix(d, n);

	printf("create done\n");

	tool.mSub(x, triplesA.triple[0], triplesE.triple[0]);
	tool.mSub(y, triplesA.triple[1], triplesE.triple[1]);

	printf("exchange\n");
	sender.send2(triplesE.triple[0]);
	sender.send2(triplesE.triple[1]);
	sender.recv2(triplesF.triple[0]);
	sender.recv2(triplesF.triple[1]);

	//注意矩阵大小问题

	E = Matrix(m, d);
	F = Matrix(d, n);
	N1 = Matrix(m, n);
	N2 = Matrix(m, n);
	N3 = Matrix(m, n);

	tool.mAdd(triplesE.triple[0], triplesF.triple[0], E);
	tool.mAdd(triplesE.triple[1], triplesF.triple[1], F);

	printf("EF done\n");
	tool.mMul(triplesA.triple[0], F, N1); //mMul改成localmMul,在matrix.cpp中添加localMul;除了三元组的生成和使用，其余矩阵乘法均为在线的；
	tool.mMul(E, triplesA.triple[1], N2);
	tool.mAdd(N1, N2, N3);
	tool.mAdd(N3, triplesA.triple[2], xy);

	this->matrixsyz.pop_back();
}

void Triple1::CreateIntTriple()
{

	client &sender = this->servier;
	mpz_class x2 = 2314;
	mpz_class y2 = 5253;
	mpz_class z2 = 5254;

	mpz_class xy2;
	tool.mocheng(x2, y2, xy2);

	Matrix R, Z, W;
	R = Matrix(1, 5);
	Z = Matrix(1, 5);
	W = Matrix(1, 5);

	mpz_class I, II;
	I = 1;
	II = -1;

	tool.mojian(I, R.matrix[0][0], Z.matrix[0][0]);
	tool.mojian(y2, R.matrix[0][1], Z.matrix[0][1]);
	tool.mojian(x2, R.matrix[0][2], Z.matrix[0][2]);
	tool.mojian(xy2, R.matrix[0][3], Z.matrix[0][3]);
	tool.mojian(II, R.matrix[0][4], Z.matrix[0][4]);

	mpz_class r = rand();
	for (int i = 0; i < 5; i++)
	{
		tool.mocheng(r, Z.matrix[0][i], W.matrix[0][i]);
	}

	sender.send2(R);
	sender.send2(W);

	mpz_class u, v;

	sender.recv3(u);
	sender.recv3(v);

	//	gmp_printf("%Zd	\n", u.get_mpz_t());
	//	gmp_printf("%Zd	\n", v.get_mpz_t());

	mpz_class p;
	p = "40343";
	mpz_t rn;
	mpz_init(rn);
	mpz_invert(rn, r.get_mpz_t(), p.get_mpz_t());
	mpz_class rw(rn);
	tool.mocheng(rw, v, p);
	tool.mojia(u, p, z2);

	IntTriples xyz;
	xyz.triple[0] = x2;
	xyz.triple[1] = y2;
	xyz.triple[2] = z2;

	this->intsyz.push_back(xyz);
	//	gmp_printf("%Zd	\n", x2.get_mpz_t());
	//	gmp_printf("%Zd	\n", y2.get_mpz_t());
	//	gmp_printf("%Zd	\n", z2.get_mpz_t());
}

void Triple1::IntMul(mpz_class &x, mpz_class &y, mpz_class &xy)
{

	mpz_class e0, e1, f0, f1, e, f;
	mpz_class a1, b1, c1;
	IntTriples &triple = this->intsyz.back();

	a1 = triple.triple[0];
	b1 = triple.triple[1];
	c1 = triple.triple[2];

	server &sender = this->servier;
	tool.mojian(x, a1, e1);
	tool.mojian(y, b1, f1);

	sender.send3(e1);
	sender.send3(f1);
	sender.recv3(e0);
	sender.recv3(f0);

	tool.mojia(e1, e0, e);
	tool.mojia(f1, f0, f);
	tool.mocheng(f, a1, e1);
	tool.mocheng(e, b1, f0);
	tool.mojia(e1, f0, f);
	tool.mojia(f, c1, xy);
	/*	gmp_printf("%Zd	\n", x.get_mpz_t());
	gmp_printf("%Zd	\n", y.get_mpz_t());
	gmp_printf("%Zd	\n", xy.get_mpz_t());*/
	this->intsyz.pop_back();
}