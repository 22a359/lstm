#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"

using namespace std;

void matrix_triple2(Matrix &x1,Matrix &y1,Matrix &z1,client &recver); 

typedef struct
{
	Matrix triple[3];
} MatrixTriples;

typedef struct
{
	mpz_class triple[3];
} IntTriples;

class Triple2{

	public:
	vector<MatrixTriples> matrixsyz;
	vector<IntTriples> intsyz;
	client recvier;

	void run(){
		this->recvier.run();
	}

	void CreateMatrixTriple(){
	//	this->recvier.run();
		MatrixTriples X;
		X.triple[0]=Matrix(5,1);
		X.triple[1]=Matrix(1,5);
		X.triple[2]=Matrix(5,5);
		
		matrix_triple2(X.triple[0],X.triple[1],X.triple[2],this->recvier);
		this->matrixsyz.push_back(X);
	}

	void CreateIntTriple();

	void MatrixMul(Matrix &x, Matrix &y, Matrix &xy);

	void IntMul(mpz_class &x, mpz_class &y, mpz_class &xy);

	void mMull(Matrix &x, Matrix &y, Matrix &ans)
	{ //���о���˷����ö�ε��ó˷���Ԫ��
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

void matrix_triple2(Matrix &x1,Matrix &y1,Matrix &z1,client &recver)
{
	int i;
	int m,d,n;
	m = x1.row;
	d = x1.col;
	n = y1.col;
	MatrixTools tool;

	Matrix xy1;
	xy1=Matrix(x1.row,y1.col);
	tool.mMul(x1,y1,xy1);
	
	Matrix I1(1,n,n);
	Matrix IN1(-1,n,n);
	
	Matrix R[5];
	R[0]=Matrix(n,n);
	R[1]=Matrix(d,n);
	R[2]=Matrix(m,d);
	R[3]=Matrix(m,n);
	R[4]=Matrix(n,n);
	
	Matrix W[5];
	W[0]=Matrix(n,n);
	W[1]=Matrix(d,n);
	W[2]=Matrix(m,d);
	W[3]=Matrix(m,n);
	W[4]=Matrix(n,n);
	/*
	for(i=0;i<5;i++){
		matrixconstmul(Z[i],W[i],r.get_mpz_t());
	}
	*/
	recver.recv1(R);
	recver.recv1(W);
	
	Matrix u,u1,u2;
	u=Matrix(0,m,n);
	u1=Matrix(0,m,n);
	u2=Matrix(0,m,n);
	
	tool.mMul(xy1,R[0],u1);
	tool.mAdd(u1,u2,u);
	tool.mMul(u,I1,u2);
	tool.mMul(x1,R[1],u1);
	tool.mAdd(u1,u2,u);
	tool.mMul(u,I1,u2);
	tool.mMul(R[2],y1,u1);
	tool.mAdd(u1,u2,u);
	tool.mMul(u,I1,u2);
	tool.mMul(R[3],I1,u1);
	tool.mAdd(u1,u2,u);
	tool.mMul(u,I1,u2);
	tool.mMul(z1,R[4],u1);
	tool.mAdd(u1,u2,u);
	
	Matrix v,v1,v2;
	v=Matrix(0,m,n);
	v1=Matrix(0,m,n);
	v2=Matrix(0,m,n);
	
	tool.mMul(xy1,W[0],v1);
	tool.mAdd(v1,v2,v);
	tool.mMul(v,I1,v2);
//	matrixmul(W[1],x1,v1);
	tool.mMul(x1,W[1],v1);
	tool.mAdd(v1,v2,v);
	tool.mMul(v,I1,v2);
	tool.mMul(W[2],y1,v1);
	tool.mAdd(v1,v2,v);
	tool.mMul(v,I1,v2);
	tool.mMul(W[3],I1,v1);
	tool.mAdd(v1,v2,v);
	tool.mMul(v,I1,v2);
	tool.mMul(z1,W[4],v1);
	tool.mAdd(v1,v2,v);
	
	recver.send2(u);
	recver.send2(v);
}

void Triple2::MatrixMul(Matrix &x, Matrix &y, Matrix &xy){
	
	MatrixTriples &triplesB = this->matrixsyz.back();
	client &recver = this->recvier;
	MatrixTools tool;
	MatrixTriples triplesA,triplesE,triplesF;
	
	Matrix E;	Matrix F;	Matrix N1;	Matrix N2;	Matrix N3;
	
	int m,d,n;
	m = x.row;
	d = x.col;
	n = y.col;
	printf("begin\n");
	
	triplesE.triple[0]=Matrix(m,d);	triplesE.triple[1]=Matrix(d,n);	triplesE.triple[2]=Matrix(0,m,n);
	triplesF.triple[0]=Matrix(m,d);	triplesF.triple[1]=Matrix(d,n);	triplesF.triple[2]=Matrix(0,m,n);	
	printf("create done\n");
	
	tool.mSub(x,triplesB.triple[0],triplesF.triple[0]);
	tool.mSub(y,triplesB.triple[1],triplesF.triple[1]);
	
	printf("exchange\n");
	recver.recv2(triplesE.triple[0]);
	recver.recv2(triplesE.triple[1]);
	recver.send2(triplesF.triple[0]);
	recver.send2(triplesF.triple[1]);
	
	//ע������С���� 
	
	E=Matrix(m,d);
	F=Matrix(d,n);
	N1=Matrix(m,n);
	N2=Matrix(m,n);
	N3=Matrix(m,n);
	tool.mAdd(triplesE.triple[0],triplesF.triple[0],E);
	tool.mAdd(triplesE.triple[1],triplesF.triple[1],F);
	
	
	printf("EF done\n");	
	
	tool.mMul(E,F,N1);
	tool.mMul(triplesB.triple[0],F,N2);
	tool.mAdd(N1,N2,N3);
	tool.mMul(E,triplesB.triple[1],N1);
	tool.mAdd(N1,N3,N2);
	tool.mAdd(triplesB.triple[2],N2,xy);
	
	this->matrixsyz.pop_back();
	
} 

void Triple2::CreateIntTriple(){
	
	client &recver = this->recvier;
	mpz_class x1 = 1123;
	mpz_class y1 = 3373;
	mpz_class z1 = 35564;
	IntTriples xyz;
	xyz.triple[0] = x1;
	xyz.triple[1] = y1;
	xyz.triple[2] = z1;
	MatrixTools tool;
	this->intsyz.push_back(xyz);

	mpz_class xy1;
	tool.mocheng(x1,y1,xy1);

    Matrix R,Z,W;
    R=Matrix(1,5);
    Z=Matrix(1,5);
    W=Matrix(1,5);

	recver.recv2(R);
	recver.recv2(W);

    mpz_class u,u1,v,z;

    z = 0;
    tool.mocheng(xy1,R.matrix[0][0],u1);
    u = u1;
    z = u;
    tool.mocheng(x1,R.matrix[0][1],u1);
    tool.mojia(z,u1,u);
    z = u;
    tool.mocheng(y1,R.matrix[0][2],u1);
    tool.mojia(z,u1,u);
    tool.mojia(R.matrix[0][3],u,z);
    u = z;
    tool.mocheng(R.matrix[0][4],z1,u1);
    tool.mojia(u1,z,u);
	
    z = 0;
    tool.mocheng(xy1,W.matrix[0][0],u1);
    v = u1;
    z = v;
    tool.mocheng(x1,W.matrix[0][1],u1);
    tool.mojia(z,u1,v);
    z = v;
    tool.mocheng(y1,W.matrix[0][2],u1);
    tool.mojia(z,u1,v);
    tool.mojia(W.matrix[0][3],v,z);
    v = z;
    tool.mocheng(W.matrix[0][4],z1,u1);
    tool.mojia(u1,z,v);
	
	recver.send3(u);
	recver.send3(v);

}

void Triple2::IntMul(mpz_class &x, mpz_class &y, mpz_class &xy){
	
	mpz_class e0,e1,f0,f1,e,f;
	mpz_class a1,b1,c1;
	IntTriples &triple3= this->intsyz.back();
	MatrixTools tool;
	a1 = triple3.triple[0];
	b1 = triple3.triple[1];
	c1 = triple3.triple[2];

	client &recver = this->recvier;
	tool.mojian(x,a1,e1);
	tool.mojian(y,b1,f1);

	recver.recv3(e0);	recver.recv3(f0);	
	recver.send3(e1);	recver.send3(f1);
	tool.mojia(e0,e1,e);	tool.mojia(f0,f1,f);
	
	tool.mocheng(e,f,e0);
	tool.mocheng(f,a1,e1);
	tool.mocheng(e,b1,f0);
	tool.mojia(e0,e1,e);
	tool.mojia(e,f0,f);
	tool.mojia(f,c1,xy);
/*	gmp_printf("%Zd	\n", x.get_mpz_t());
	gmp_printf("%Zd	\n", y.get_mpz_t());
	gmp_printf("%Zd	\n", xy.get_mpz_t());*/
	this->intsyz.pop_back();
}
