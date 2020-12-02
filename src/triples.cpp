#include "triples.h"
using namespace std;
void Triples::init(eRole role)
{
    this->role = role;
    network.init(role, this->port);
    //查找表，用于找到对应尺寸的三元组
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
    //生成三元组
    creat(1, 1, 1, this->m1m1m1_counts);
    creat(40, 58, 1, this->m40m58m1_counts);
    creat(40, 40, 1, this->m40m40m1_counts);
    creat(20, 40, 1, this->m20m40m1_counts);
    creat(1, 20, 1, this->m1m20m1_counts);
    creat(1, 1, 20, this->m1m1m20_counts);
    creat(20, 20, 1, this->m20m20m1_counts);
    creat(20, 1, 1, this->m20m1m1_counts);
    creat(20, 1, 40, this->m20m1m40_counts);
    creat(40, 40, 20, this->m40m40m20_counts);
    creat(40, 20, 1, this->m40m20m1_counts);
    creat(40, 1, 40, this->m40m1m40_counts);
    creat(40, 1, 58, this->m40m1m58_counts);
}
//根据尺寸生成不同的三元组
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
//生成普通三元组
void Triples::createIntTriple()
{
    mpz_class a, b, c, aTimesB;
    string index, ck_string = this->network.checkMSG, recv_string;
    a = this->randNum();
    b = this->randNum();
    c = this->randNum();
    if (this->role == SERVER)
    { //SERVER接收对方的R和W，计算u和v发送给对方
        Matrix R, W;
        mpz_class u = 0, v = 0, temp_u, temp_v;
        this->tripleTools.mLocalMocheng(a, b, aTimesB);
        mpz_class array[5] = {aTimesB, a, b, 1, c};
        Matrix X(1, 5, array);
        this->network.mReceive(R);
        this->network.mSend(ck_string);
        this->network.mReceive(W);
        for (int i = 0; i < 5; i++)
        {
            this->tripleTools.mLocalMocheng(R.matrix[0][i], X.matrix[0][i], temp_u);
            this->tripleTools.mAccu(u, temp_u);
            this->tripleTools.mLocalMocheng(W.matrix[0][i], X.matrix[0][i], temp_v);
            this->tripleTools.mAccu(v, temp_v);
        }
        this->network.mSend(u);
        this->network.mReceive(recv_string);
        this->network.mSend(v);
        this->network.mReceive(index);
    }
    else
    { //CLIENT计算R和W发送给对方，并接收对方生成的u和v，计算出c
        Matrix Z, W;
        mpz_class r = this->randNum();
        mpz_class u, v, r_inv, temp, mod = modNum;
        this->tripleTools.mLocalMocheng(a, b, aTimesB);
        mpz_class array[5] = {1, b, a, aTimesB, -1};
        Matrix Y(1, 5, array);
        Matrix R(1, 5);
        this->tripleTools.mSub(Y, R, Z);
        this->tripleTools.mConstMul(Z, W, r.get_mpz_t());
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(u);
        this->network.mSend(ck_string);
        this->network.mReceive(v);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), mod.get_mpz_t());
        this->tripleTools.mLocalMocheng(r_inv, v, temp);
        this->tripleTools.mojia(u, temp, c);
        //由于CLIENT计算c，因此在计算完后生成随机序列号发送给对方，作为两方三元组的对应标志
        index = this->randIndex();
        this->network.mSend(index);
    }
    // IntTriples triples{a, b, c};
    // this->intTriples.push(triples);
    //将三元组保存到文件中
    string out_string, out_a, out_b, out_c;
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    fileName += "_single.dat";
    ofstream outfile;
    outfile.open(fileName, ios::app);
    network.serialization(a, out_a);
    network.serialization(b, out_b);
    network.serialization(c, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
    outfile.close();
}
//生成矩阵三元组
void Triples::createMatrixTriple(int m, int d, int n)
{
    string index, ck_string = this->network.checkMSG, recv_string;
    mpz_class r = this->randNum();
    mpz_class mod = modNum, r_inv;
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
        this->network.mReceive(R);
        this->network.mSend(ck_string);
        this->network.mReceive(W);
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

        this->network.mSend(U);
        this->network.mReceive(recv_string);
        this->network.mSend(V);
        this->network.mReceive(index);
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
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(U);
        this->network.mSend(ck_string);
        this->network.mReceive(V);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), mod.get_mpz_t());
        this->tripleTools.mConstMul(V, temp, r_inv.get_mpz_t());
        this->tripleTools.mAdd(U, temp, C);
        //由于CLIENT计算c，因此在计算完后生成随机序列号发送给对方，作为两方三元组的对应标志
        index = this->randIndex();
        this->network.mSend(index);
    }
    if (!(this->table[m][d][n] >= 0 && this->table[m][d][n] <= 11))
    {
        cout << "CreateMatrixTriple Error" << endl;
        // exit(1);
    }
    //将三元组保存到文件中
    string out_string, out_a, out_b, out_c;
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    fileName += ("_matrix" + to_string(m) + "-" + to_string(d) + "-" + to_string(n) + ":" + to_string(this->table[m][d][n]) + ".dat");
    ofstream outfile;
    outfile.open(fileName, ios::app);
    network.serialization(A, out_a);
    network.serialization(B, out_b);
    network.serialization(C, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
    outfile.close();
}
//获取一个普通三元组
IntTriples Triples::getTriples()
{
    IntTriples triples = this->intTriples.top();
    this->intTriples.pop();
    return triples;
}
//获取一个矩阵三元组
MatrixTriples Triples::getTriples(int m, int d, int n)
{
    int flag = this->table[m][d][n];
    MatrixTriples triples = this->matrixTriples[flag].top();
    this->matrixTriples[flag].pop();
    return triples;
}
//在大素数范围内生成随机数
mpz_class Triples::randNum()
{
    mpz_class ans;
    mpz_t z, r;
    mpz_init(z);
    mpz_init(r);
    random_device rd;
    default_random_engine e(rd());
    gmp_randstate_t grt;
    gmp_randinit_mt(grt);
    gmp_randseed_ui(grt, e());
    mpz_urandomb(z, grt, randBit);
    mpz_mod(r, z, modNum.get_mpz_t());
    ans = mpz_class(r);
    mpz_clear(z);
    mpz_clear(r);
    return ans;
}
//生成随机序列号
string Triples::randIndex()
{
    time_t times = time(NULL);
    clock_t clocks = clock();
    int randBit = rand() % 10;
    string index = to_string(times) + to_string(clocks) + to_string(randBit);
    return index;
}