#include "triples.h"
using namespace std;
void Triples::init(eRole role)
{
    this->role = role;
    network.init(role, this->port);
    for (int i = 0; i < 58; i++)
        for (int j = 0; j < 58; j++)
            for (int k = 0; k < 58; k++)
                this->table[i][j][k] = -1;
    table[40][58][1] = this->m40m58m1;
    table[40][40][1] = this->m40m40m1;
    table[20][40][1] = this->m20m40m1;
    table[1][20][1] = this->m1m20m1;
    table[1][1][20] = this->m1m1m20;
    table[20][20][1] = this->m20m20m1;
    table[20][1][1] = this->m20m1m1;
    table[20][1][40] = this->m20m1m40;
    table[40][40][20] = this->m40m40m20;
    table[40][20][1] = this->m40m20m1;
    table[40][1][40] = this->m40m1m40;
    table[40][1][58] = this->m40m1m58;
    // creat(1, 1, 1, this->m1m1m1_counts);
    creat(40, 58, 1, this->m40m58m1_counts);
    // creat(40, 40, 1, this->m40m40m1_counts);
    // creat(20, 40, 1, this->m20m40m1_counts);
    // creat(1, 20, 1, this->m1m20m1_counts);
    // creat(1, 1, 20, this->m1m1m20_counts);
    // creat(20, 20, 1, this->m20m20m1_counts);
    // creat(20, 1, 1, this->m20m1m1_counts);
    // creat(20, 1, 40, this->m20m1m40_counts);
    // creat(40, 40, 20, this->m40m40m20_counts);
    // creat(40, 20, 1, this->m40m20m1_counts);
    // creat(40, 1, 40, this->m40m1m40_counts);
    // creat(40, 1, 58, this->m40m1m58_counts);
}

void Triples::creat(int m, int d, int n, int counts)
{
    if (m == 1 && d == 1 && n == 1)
    {
        cout << "\nGenerate 1×1 triples" << endl;
        for (int i = 0; i < counts; i++)
        {
            this->createIntTriple();
            cout << "No." << i << "\tGenerated" << endl;
        }
    }
    else
    {
        cout << "\nGenerate " << m << "×" << d << "×" << n << " triples" << endl;
        for (int i = 0; i < counts; i++)
        {
            this->createMatrixTriple(m, d, n);
            cout << "No." << i << "\tGenerated" << endl;
        }
    }
}

void Triples::createIntTriple()
{
    random_device rd;
    default_random_engine e(rd());
    mpz_class a, b, c, aTimesB;
    a = e() % modNum;
    b = e() % modNum;
    c = e() % modNum;
    this->tripleTools.mLocalMocheng(a, b, aTimesB);
    if (this->role == SERVER)
    {
        Matrix R, W;
        mpz_class u, v, temp_u, temp_v;
        mpz_class array[5] = {aTimesB, a, b, 1, c};
        Matrix X(1, 5, array);
        this->network.mReceive(R, W);
        for (int i = 0; i < 5; i++)
        {
            this->tripleTools.mLocalMocheng(R.matrix[0][i], X.matrix[0][i], temp_u);
            this->tripleTools.mAccu(u, temp_u);
            this->tripleTools.mLocalMocheng(W.matrix[0][i], X.matrix[0][i], temp_v);
            this->tripleTools.mAccu(v, temp_v);
        }
        this->network.mSend(u, v);
    }
    else
    {
        Matrix Z, W;
        mpz_class r = e() % modNum;
        mpz_class u, v, r_inv, temp, mod = modNum;
        mpz_class array[5] = {1, b, a, aTimesB, -1};
        Matrix Y(1, 5, array);
        Matrix R(1, 5);
        this->tripleTools.mSub(Y, R, Z);
        this->tripleTools.mConstMul(Z, W, r.get_mpz_t());
        this->network.mSend(R, W);
        this->network.mReceive(u, v);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), mod.get_mpz_t());
        this->tripleTools.mLocalMocheng(r_inv, v, temp);
        this->tripleTools.mojia(u, temp, c);
    }
    IntTriples triples{a, b, c};
    this->intTriples.push(triples);
}

void Triples::createMatrixTriple(int m, int d, int n)
{
    random_device rd;
    default_random_engine e(rd());
    mpz_class r = e() % modNum, mod = modNum, r_inv;
    Matrix A{m, d}, B{d, n}, C{m, n}, aTimesB;
    Matrix I{M_NORMAL, 1, n, n}, I_minus{M_NORMAL, -1, n, n};
    this->tripleTools.mLocalMul(A, B, aTimesB);
    if (this->role == SERVER)
    {
        Matrix R1, R2, R3, R4, R5;
        Matrix W1, W2, W3, W4, W5;
        array<Matrix, 5> R = {R1, R2, R3, R4, R5};
        array<Matrix, 5> W = {W1, W2, W3, W4, W5};
        array<Matrix, 5> X = {aTimesB, A, B, I, C};
        Matrix U{M_NORMAL, 0, m, n}, V{M_NORMAL, 0, m, n}, temp_U, temp_V;
        this->network.mReceive(R, W);
        this->tripleTools.mLocalMul(X[0], R[0], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(X[1], R[1], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(R[2], X[2], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(R[3], X[3], temp_U);
        this->tripleTools.mAccu(U, temp_U);
        this->tripleTools.mLocalMul(X[4], R[4], temp_U);
        this->tripleTools.mAccu(U, temp_U);

        this->tripleTools.mLocalMul(X[0], W[0], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(X[1], W[1], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(W[2], X[2], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(W[3], X[3], temp_V);
        this->tripleTools.mAccu(V, temp_V);
        this->tripleTools.mLocalMul(X[4], W[4], temp_V);
        this->tripleTools.mAccu(V, temp_V);

        this->network.mSend(U, V);
    }
    else
    {
        array<Matrix, 5> Y = {I, B, A, aTimesB, I_minus};
        Matrix R1{n, n}, R2{d, n}, R3{m, d}, R4{m, n}, R5{n, n};
        Matrix W1, W2, W3, W4, W5, temp, U, V;
        array<Matrix, 5> R = {R1, R2, R3, R4, R5};
        array<Matrix, 5> W = {W1, W2, W3, W4, W5};
        for (int i = 0; i < 5; i++)
        {
            this->tripleTools.mSub(Y[i], R[i], temp);
            this->tripleTools.mCopy(temp, W[i]);
            this->tripleTools.mConstMul(W[i], temp, r.get_mpz_t());
            this->tripleTools.mCopy(temp, W[i]);
        }
        this->network.mSend(R, W);
        this->network.mReceive(U, V);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), mod.get_mpz_t());
        this->tripleTools.mConstMul(V, temp, r_inv.get_mpz_t());
        this->tripleTools.mAdd(U, temp, C);
    }
    MatrixTriples triples{A, B, C};
    if (!(this->table[m][d][n] >= 0 && this->table[m][d][n] <= 11))
    {
        cout << "CreateMatrixTriple Error" << endl;
        exit(1);
    }
    this->matrixTriples[this->table[m][d][n]].push(triples);
}

IntTriples Triples::getTriples()
{
    IntTriples triples = this->intTriples.top();
    this->intTriples.pop();
    return triples;
}

MatrixTriples Triples::getTriples(int m, int d, int n)
{
    int flag = this->table[m][d][n];
    MatrixTriples triples = this->matrixTriples[flag].top();
    this->matrixTriples[flag].pop();
    return triples;
}
