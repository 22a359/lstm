#include "triples.h"
using namespace std;
//生成查找表
Triples::Triples()
{
    //查找表，用于找到对应尺寸的三元组
    for (int i = 0; i < 58; i++)
        for (int j = 0; j < 58; j++)
            for (int k = 0; k < 58; k++)
                this->table[i][j][k] = -1;
    table[40][58][1] = this->m40m58m1;
//    table[1][2][1] = this->m40m58m1;
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
}
//读入文件中的三元组
void Triples::init(eRole role, int flag)
{
    this->role = role;
    cout << "Triples preparing " << flush;
    this->readIntTriples(flag);
    this->readMatrixTriples(40, 58, 1, flag);
//    this->readMatrixTriples(1, 2, 1, flag);
    this->readMatrixTriples(40, 40, 1, flag);
    this->readMatrixTriples(20, 40, 1, flag);
    this->readMatrixTriples(1, 20, 1, flag);
    this->readMatrixTriples(1, 1, 20, flag);
    this->readMatrixTriples(20, 20, 1, flag);
    this->readMatrixTriples(20, 1, 1, flag);
    this->readMatrixTriples(20, 1, 40, flag);
    this->readMatrixTriples(40, 40, 20, flag);
    this->readMatrixTriples(40, 20, 1, flag);
    this->readMatrixTriples(40, 1, 40, flag);
    this->readMatrixTriples(40, 1, 58, flag);
    cout << "Triples OK!" << endl;
}
//生成三元组
void Triples::triplesGen(eRole role, int epochs)
{
    this->role = role;
    if (this->network.sockSer == -1)
        this->network.init(role, this->port);
    //生成三元组
    for (int i = 0; i < epochs; i++)
    {
        creat(1, 1, 1, m1m1m1_counts, i);
        creat(40, 58, 1, m40m58m1_counts, i);
//        creat(1, 2, 1, m40m58m1_counts, i);
        creat(40, 40, 1, m40m40m1_counts, i);
        creat(20, 40, 1, m20m40m1_counts, i);
        creat(1, 20, 1, m1m20m1_counts, i);
        creat(1, 1, 20, m1m1m20_counts, i);
        creat(20, 20, 1, m20m20m1_counts, i);
        creat(20, 1, 1, m20m1m1_counts, i);
        creat(20, 1, 40, m20m1m40_counts, i);
        creat(40, 40, 20, m40m40m20_counts, i);
        creat(40, 20, 1, m40m20m1_counts, i);
        creat(40, 1, 40, m40m1m40_counts, i);
        creat(40, 1, 58, m40m1m58_counts, i);
    }
}
//根据尺寸生成不同的三元组
void Triples::creat(int m, int d, int n, int counts, int flag)
{
    if (this->network.sockSer == -1)
        this->network.init(this->role, this->port);
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    // fileName = "./triples/" + fileName;
    if (m == 1 && d == 1 && n == 1)
    {
        fileName += ("_int_" + to_string(flag) + ".dat");
        cout << "Generate " << triples_num[0] << " 1×1×1 triples" << endl;
        for (int i = 0; i < counts; i++)
        {
            this->createIntTriple(fileName, flag + i);
            // printf("No.%.7d Generated\r", i);
            cout << "No." << i + 1 << " Generated\r" << flush;
        }
        showTime();
    }
    else
    {
        fileName += ("_matrix:" + to_string(this->table[m][d][n]) + "_" + to_string(m) + "-" + to_string(d) + "-" + to_string(n) + "_" + to_string(flag) + ".dat");
        cout << "Generate " << triples_num[this->table[m][d][n] + 1] << " " << m << "×" << d << "×" << n << " triples" << endl;
        for (int i = 0; i < counts; i++)
        {
            this->createMatrixTriple(fileName, m, d, n, flag + i);
            // printf("No.%.7d Generated\r", i);
            cout << "No." << i + 1 << " Generated\r" << flush;
        }
        showTime();
    }
}
//生成普通三元组，并写入文件
void Triples::createIntTriple(string fileName, int flag)
{
    mpz_class a, b, c, aTimesB;
    string index, ck_string = checkMSG, recv_string;
    a = randNumGen();
    b = randNumGen();
    c = randNumGen();
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
        mpz_class r = randNumGen();
        mpz_class u, v, r_inv, temp;
        this->tripleTools.mLocalMocheng(a, b, aTimesB);
        mpz_class array[5] = {1, b, a, aTimesB, -1};
        Matrix Y(1, 5, array);
        Matrix R(1, 5);
        this->tripleTools.mSub(Y, R, Z);
        this->tripleTools.mConstMulOrigin(Z, W, r.get_mpz_t());
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(u);
        this->network.mSend(ck_string);
        this->network.mReceive(v);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), modNum.get_mpz_t());
        this->tripleTools.mLocalMocheng(r_inv, v, temp);
        this->tripleTools.mojia(u, temp, c);
        //由于CLIENT计算c，因此在计算完后生成随机序列号发送给对方，作为两方三元组的对应标志
        index = this->randIndex();
        this->network.mSend(index);
    }
    //将三元组保存到文件中
    string out_string, out_a, out_b, out_c;
    ofstream outfile;
    if (flag)
        outfile.open(fileName, ios::app);
    else
        outfile.open(fileName, ios::out | ios::trunc);
    network.serialization(a, out_a);
    network.serialization(b, out_b);
    network.serialization(c, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
    outfile.close();
}
//生成矩阵三元组，并写入文件
void Triples::createMatrixTriple(string fileName, int m, int d, int n, int flag)
{
    string index, ck_string = checkMSG, recv_string;
    mpz_class r = randNumGen();
    mpz_class r_inv;
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
            this->tripleTools.mConstMulOrigin(W[i], temp, r.get_mpz_t());
            this->tripleTools.mCopy(temp, W[i]);
        }
        this->network.mSend(R);
        this->network.mReceive(recv_string);
        this->network.mSend(W);
        this->network.mReceive(U);
        this->network.mSend(ck_string);
        this->network.mReceive(V);
        mpz_invert(r_inv.get_mpz_t(), r.get_mpz_t(), modNum.get_mpz_t());
        this->tripleTools.mConstMulOrigin(V, temp, r_inv.get_mpz_t());
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
    ofstream outfile;
    if (flag)
        outfile.open(fileName, ios::app);
    else
        outfile.open(fileName, ios::out | ios::trunc);
    network.serialization(A, out_a);
    network.serialization(B, out_b);
    network.serialization(C, out_c);
    out_string = index + ":" + out_a + "-" + out_b + "-" + out_c;
    outfile << out_string << endl;
    outfile.close();
}
//读入文件里的普通三元组
void Triples::readIntTriples(int flag)
{
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    fileName += ("_int_" + to_string(flag) + ".dat");
    string fileNameNew = "USED_" + fileName;
    ifstream infile;
    infile.open(fileName, ios::in);
    if (!infile)
    { //三元组文件不存在，则生成
        cout << "\n1×1×1 triples not found" << endl;
        this->creat(1, 1, 1, triples_num[0], 0);
        infile.open(fileName, ios::in);
    }
    string line;
    mpz_class index, a, b, c;
    while (getline(infile, line) && infile.good() && !infile.eof() && line != "")
    {
        this->deserialization(line, index, a, b, c);
        IntTriples triples{index, a, b, c};
        this->intTriples.push(triples);
    }
    infile.close();
    //已读取的文件进行改名
    if (RELEASE)
        if (rename(fileName.c_str(), fileNameNew.c_str()) == -1)
            cout << "Rename error" << endl;
    cout << ". " << flush;
}
//读入文件里的矩阵三元组
void Triples::readMatrixTriples(int m, int d, int n, int flag)
{
    int indexFlag = this->table[m][d][n];
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    fileName += ("_matrix:" + to_string(indexFlag) + "_" + to_string(m) + "-" + to_string(d) + "-" + to_string(n) + "_" + to_string(flag) + ".dat");
    string fileNameNew = "USED_" + fileName;
    ifstream infile;
    infile.open(fileName, ios::in);
    if (!infile)
    { //三元组文件不存在，则生成
        cout << "\n"
             << m << "×" << d << "×" << n << " triples not found" << endl;
        this->creat(m, d, n, triples_num[indexFlag + 1], 0);
        infile.open(fileName, ios::in);
    }
    string line;
    Matrix a, b, c;
    mpz_class index;
    while (getline(infile, line) && infile.good() && !infile.eof() && line != "")
    {
        this->deserialization(line, index, a, b, c);
        MatrixTriples triples{index, a, b, c};
        this->matrixTriples[indexFlag].push(triples);
    }
    infile.close();
    //已读取的文件进行改名
    if (RELEASE)
        if (rename(fileName.c_str(), fileNameNew.c_str()) == -1)
            cout << "Rename error" << endl;
    cout << ". " << flush;
}
//三元组反序列化
bool Triples::deserialization(string in_string, mpz_class &index, mpz_class &a, mpz_class &b, mpz_class &c)
{
    char *pch;
    char *cstr = stringToChar(in_string);
    pch = strtok(cstr, iDelim);
    index = mpz_class(pch, baseNum); //分离出序号
    pch = strtok(NULL, iDelim);      //剩下的三元组
    pch = strtok(pch, tDelim);
    a = mpz_class(pch, baseNum); //分离出a
    pch = strtok(NULL, tDelim);
    b = mpz_class(pch, baseNum); //分离出b
    pch = strtok(NULL, tDelim);
    c = mpz_class(pch, baseNum); //分离出c
    delete[] cstr;
    return true;
}
bool Triples::deserialization(string in_string, mpz_class &index, Matrix &a, Matrix &b, Matrix &c)
{
    char *pch1, *pch2;
    char *cstr = stringToChar(in_string);
    pch1 = strtok(cstr, iDelim);
    index = mpz_class(pch1, baseNum); //分离出序号
    pch1 = strtok(NULL, iDelim);      //剩下的三元组
    pch2 = strtok(pch1, tDelim);
    string a_string = pch2; //分离出a
    pch2 = strtok(NULL, tDelim);
    string b_string = pch2; //分离出b
    pch2 = strtok(NULL, tDelim);
    string c_string = pch2; //分离出c
    network.deserialization(a_string, a);
    network.deserialization(b_string, b);
    network.deserialization(c_string, c);
    delete[] cstr;
    return true;
}
//获取一个普通三元组
IntTriples Triples::getTriples()
{
    if (this->intTriples.empty())
    {
        cout << "ERROR: Int triples run out!" << endl;
        exit(1);
    }
    IntTriples triples = this->intTriples.top();
    if (RELEASE)
        this->intTriples.pop();
    else
        m1m1m1_num++;
    return triples;
}
//获取一个矩阵三元组
MatrixTriples Triples::getTriples(int m, int d, int n)
{
    int flag = this->table[m][d][n];
    if (this->matrixTriples[flag].empty())
    {
        cout << "ERROR: " << m << "×" << d << "×" << n << " matrix triples run out!" << endl;
        exit(1);
    }
    MatrixTriples triples = this->matrixTriples[flag].top();
    if (RELEASE)
        this->matrixTriples[flag].pop();
    else
        mTriples_num[flag]++;
    return triples;
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