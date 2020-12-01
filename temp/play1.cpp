#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
#include <cstring>
#include "play1.h"


using namespace std;
//void mul(triples &triplesX, triples &triplesA, server &sender);

int main(){
	cout<<"hello"<<endl;
	int ROW,COL;int aa;
	ROW = 5;
	COL = 5;
	MatrixTriples triplesX;
	triplesX.triple[0]=Matrix(ROW,1);
	triplesX.triple[1]=Matrix(1,5);
	triplesX.triple[2]=Matrix(ROW,5);
	cout<<"1"<<endl;
	Triple1 triple;
	triple.run();
	triple.CreateMatrixTriple();
	triple.MatrixMul(triplesX.triple[0],triplesX.triple[1],triplesX.triple[2]);
//	out2(triplesX.triple[0]);
//	out2(triplesX.triple[1]);
//	out2(triplesX.triple[2]);
	mpz_class a,b,c;
	a = 10;
	b = 20;
//	triple.CreateIntTriple();	
//	triple.IntMul(a,b,c);
/*	for(int i = 0;i<100;i++) 
		triple.CreateIntTriple();
	triple.mMull(triplesX.triple[0],triplesX.triple[1],triplesX.triple[2]);*/ 
	cout<<"2"<<endl;
	MatrixTools tool;
	tool.print(triplesX.triple[0]);
	tool.print(triplesX.triple[1]);
	tool.print(triplesX.triple[2]);
	scanf("%d",&aa);
	cout<<"3"<<endl;
	return 0;
}

