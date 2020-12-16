#include "mulgate.h"
#include "network.h"
#include "matrix.h"

using namespace std;
TriplesMul triplesMul;
MatrixTools lstmTools;
#define mx                 \
    {                      \
        M_NORMAL, 0, 58, 1 \
    }
#define mo                 \
    {                      \
        M_NORMAL, 0, 40, 1 \
    }

class Lstm;

class Lstm_layer1;

class Lstm_layer2;

class Lstm_layer3;

class Lstm_layer4;

class Lstm_layer_block;

class Lstm_layer_block
{
public:
    Matrix x{M_NORMAL, 0, 58, 1};
    Matrix h{M_NORMAL, 0, 40, 1};
    Matrix f{M_NORMAL, 0, 40, 1};
    Matrix i{M_NORMAL, 0, 40, 1};
    Matrix g{M_NORMAL, 0, 40, 1};
    Matrix o{M_NORMAL, 0, 40, 1};
    Matrix s{M_NORMAL, 0, 40, 1};

    //void sigmoid(Matrix &matrix);

    //    void sigmoid(Matrix &matrix, Matrix &ans);

    // void tanh(Matrix &matrix);
    //void tanh(Matrix &matrix, Matrix &ans);

    // void relu(Matrix &matrix);
    float learningRate = 0.25;
    // float learningRate = 0.02;
};

class Lstm_layer4
{
public:
    //    Matrix W4{M_GAUSS, 1, 20}, B4{M_NORMAL, 0, 1, 1};
    Matrix W4{M_NORMAL, 0, 1, 20}, B4{M_NORMAL, 0, 1, 1};
    Matrix A4{M_NORMAL, 0, 1, 1}, Z4{M_NORMAL, 0, 1, 1};
    Lstm_layer_block block;

    void forward(Lstm_layer3 layer3);

    void backward(Lstm_layer3 layer3);

    Matrix et{M_NORMAL, 0, 1, 1};
    Matrix delta_b4{M_NORMAL, 0, 1, 1};
};

class Lstm_layer3
{
public:
    //    Matrix W3{M_GAUSS, 20, 40}, B3{M_NORMAL, 0, 20, 1};
    Matrix W3{M_NORMAL, 0, 20, 40}, B3{M_NORMAL, 0, 20, 1};
    Matrix A3{M_NORMAL, 0, 20, 1}, Z3{M_NORMAL, 0, 20, 1};
    Lstm_layer_block block;

    void forward(Lstm_layer2 layer2);

    void backward(Lstm_layer4 layer4, Lstm_layer2 layer2);

    Matrix delta_b3{M_NORMAL, 0, 20, 1};
};

class Lstm_layer2
{
public:
    Matrix H[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix S[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix F[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix I[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix G[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix O[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};        //逆向用
    Matrix delta_Bf[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bi[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bg[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix delta_Bo[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix h_minus1{M_NORMAL, 0, 40, 1};
    Matrix s_minus1{M_NORMAL, 0, 40, 1};
    Matrix Z2{M_NORMAL, 0, 40, 1};
    Matrix A2{M_NORMAL, 0, 40, 1};
    Matrix X20{M_NORMAL, 0, 40, 1};
    //    Matrix Wfx2{M_GAUSS, 40, 40}, Wfh2{M_GAUSS, 40, 40}, Bf2{M_NORMAL, 0, 40, 1};
    //    Matrix Wix2{M_GAUSS, 40, 40}, Wih2{M_GAUSS, 40, 40}, Bi2{M_NORMAL, 0, 40, 1};
    //    Matrix Wgx2{M_GAUSS, 40, 40}, Wgh2{M_GAUSS, 40, 40}, Bg2{M_NORMAL, 0, 40, 1};
    //    Matrix Wox2{M_GAUSS, 40, 40}, Woh2{M_GAUSS, 40, 40}, Bo2{M_NORMAL, 0, 40, 1};

    Matrix Wfx2{M_NORMAL, 0, 40, 40}, Wfh2{M_NORMAL, 0, 40, 40}, Bf2{M_NORMAL, 0, 40, 1};
    Matrix Wix2{M_NORMAL, 0, 40, 40}, Wih2{M_NORMAL, 0, 40, 40}, Bi2{M_NORMAL, 0, 40, 1};
    Matrix Wgx2{M_NORMAL, 0, 40, 40}, Wgh2{M_NORMAL, 0, 40, 40}, Bg2{M_NORMAL, 0, 40, 1};
    Matrix Wox2{M_NORMAL, 0, 40, 40}, Woh2{M_NORMAL, 0, 40, 40}, Bo2{M_NORMAL, 0, 40, 1};
    Lstm_layer_block block;

    void forward(Lstm_layer1 layer1);

    void backward(Lstm_layer3 layer3, Lstm_layer1 layer1);

private:
};

class Lstm_layer1
{
public:
    Matrix X[20]{mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx, mx};
    Matrix H[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix S[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo};
    Matrix F[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix I[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix G[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix O[20]{mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo, mo}; //逆向用
    Matrix h_minus1{M_NORMAL, 0, 40, 1};
    Matrix s_minus1{M_NORMAL, 0, 40, 1};
    //    Matrix Wfx1{M_GAUSS, 40, 58}, Wfh1{M_GAUSS, 40, 40}, Bf1{M_NORMAL, 0, 40, 1};
    //    Matrix Wix1{M_GAUSS, 40, 58}, Wih1{M_GAUSS, 40, 40}, Bi1{M_NORMAL, 0, 40, 1};
    //    Matrix Wgx1{M_GAUSS, 40, 58}, Wgh1{M_GAUSS, 40, 40}, Bg1{M_NORMAL, 0, 40, 1};
    //    Matrix Wox1{M_GAUSS, 40, 58}, Woh1{M_GAUSS, 40, 40}, Bo1{M_NORMAL, 0, 40, 1};

    Matrix Wfx1{M_NORMAL, 0, 40, 58}, Wfh1{M_NORMAL, 0, 40, 40}, Bf1{M_NORMAL, 0, 40, 1};
    Matrix Wix1{M_NORMAL, 0, 40, 58}, Wih1{M_NORMAL, 0, 40, 40}, Bi1{M_NORMAL, 0, 40, 1};
    Matrix Wgx1{M_NORMAL, 0, 40, 58}, Wgh1{M_NORMAL, 0, 40, 40}, Bg1{M_NORMAL, 0, 40, 1};
    Matrix Wox1{M_NORMAL, 0, 40, 58}, Woh1{M_NORMAL, 0, 40, 40}, Bo1{M_NORMAL, 0, 40, 1};
    Lstm_layer_block block;

    void forward();

    void backward(Lstm_layer2 layer2);

private:
};

class Lstm
{
public:
    eRole role;
    Lstm_layer1 layer1;
    Lstm_layer2 layer2;
    Lstm_layer3 layer3;
    Lstm_layer4 layer4;
    Matrix output{M_NORMAL, 0, 1, 1};
    Matrix output_train{M_NORMAL, 0, 1, 1};

    Lstm(eRole role, int epochs);

    void compute();

    void train();

    void tripesInit(int flag);

    void triplesGen();

private:
    int epochs;

    void dataReadIn();

    void forwardNetwork();

    void backwardNetwork();
};

Lstm::Lstm(eRole role, int epochs) : role(role), epochs(epochs)
{
    //    mpz_set_str(modNum.get_mpz_t(), modNumStr.c_str(), 10);
}

//三元组生成
void Lstm::triplesGen()
{
    Triples triples;
    cout << "\nTriples generat start!" << endl;
    triples.triplesGen(this->role, epochs);
    cout << "\nTriples generat done!" << endl;
}

//训练
void Lstm::train()
{
    this->dataReadIn();
    for (int i = 0; i < this->epochs; i++)
    {
        cout << "\nRound " << i + 1 << ": Begin" << endl;
        // this->tripesInit(i);
        this->tripesInit(0);

        Matrix mX, mY, manss;
        // mpz_class X1, Y1, X2, Y2;
        mpz_class X1, X2, X3, X4, X5;
        //        mpz_class rate = 4294967296;
        mpz_class rate = 16777216;
        if (this->role == SERVER)
        {
            mpz_set_str(X1.get_mpz_t(), "2", 10);
            mpz_set_str(X2.get_mpz_t(), "2", 10);
            mpz_set_str(X3.get_mpz_t(), "1", 10);
            mpz_set_str(X4.get_mpz_t(), "2", 10);
            mpz_set_str(X5.get_mpz_t(), "2", 10);
            // mpz_set_str(Y1.get_mpz_t(), "40", 10);
            // mpz_set_str(Y2.get_mpz_t(), "53", 10);
        }
        else
        {
            mpz_set_str(X1.get_mpz_t(), "-2", 10);
            mpz_set_str(X2.get_mpz_t(), "-1", 10);
            mpz_set_str(X3.get_mpz_t(), "1", 10);
            mpz_set_str(X4.get_mpz_t(), "1", 10);
            mpz_set_str(X5.get_mpz_t(), "2", 10);
            // mpz_set_str(Y1.get_mpz_t(), "20", 10);
            // mpz_set_str(Y2.get_mpz_t(), "50", 10);
        }
        Matrix temp1{M_NORMAL, 0, 1, 1};
        // Matrix temp1{M_NORMAL, 0, 1, 5};
        // Matrix temp2{M_NORMAL, 0, 2, 1};
        temp1.change(0, 0, X3);
        // temp1.change(0, 0, X1);
        // temp1.change(0, 1, X2);
        // temp1.change(0, 2, X3);
        // temp1.change(0, 3, X4);
        // temp1.change(0, 4, X5);
        // temp2.change(0, 0, Y1);
        // temp2.change(1, 0, Y2);
        lstmTools.mConstMulOrigin(temp1, mX, rate.get_mpz_t());
        // lstmTools.mConstMulOrigin(temp2, mY, rate.get_mpz_t());
        mX.print("X");
        // mY.print("Y");
        // triplesMul.mMul(mX, mY, manss);
        triplesMul.getPlain(mX, "明文X");
        triplesMul.sigmoid(mX);
        //        triplesMul.mocheng(X, Y, anss);
        // triplesMul.getPlain(mY, "明文Y");
        triplesMul.getPlain(mX, "明文sigmoid");
        return;

        //        mpz_class X, Y, anss;
        //        if (this->role == SERVER)
        //        {
        //            mpz_set_str(X.get_mpz_t(), "3257231", 10);
        //            mpz_set_str(Y.get_mpz_t(), "14361346", 10);
        //            mpz_mul_2exp(X.get_mpz_t(), X.get_mpz_t(), eAndC);
        //            mpz_mul_2exp(Y.get_mpz_t(), Y.get_mpz_t(), eAndC);
        //        } else
        //        {
        //            mpz_set_str(X.get_mpz_t(), "600143700", 10);
        //            mpz_set_str(Y.get_mpz_t(), "14714771", 10);
        //            mpz_mul_2exp(X.get_mpz_t(), X.get_mpz_t(), eAndC);
        //            mpz_mul_2exp(Y.get_mpz_t(), Y.get_mpz_t(), eAndC);
        //        }
        //        mpz_print(X, "X");
        //        mpz_print(Y, "Y");
        //        triplesMul.mocheng(X, Y, anss);
        //        triplesMul.getPlain(X, "明文X");
        //        triplesMul.getPlain(Y, "明文Y");
        //        triplesMul.getPlain(anss, "明文结果");
        //        return;

        this->forwardNetwork();

        mpf_class ans;
        triplesMul.getPlain(this->output, ans);
        cout << "ANS " << i + 1 << ": " << ans.get_d() << endl;

        this->backwardNetwork();
        cout << "Round " << i + 1 << ": OK\n"
             << endl;
        /* if (!RELEASE)
        {
            cout << "1×1×1:\t" << m1m1m1_num << endl;
            cout << "40×58×1:\t" << mTriples_num[0] << endl;
            cout << "40×40×1:\t" << mTriples_num[1] << endl;
            cout << "20×40×1:\t" << mTriples_num[2] << endl;
            cout << "1×20×1:\t" << mTriples_num[3] << endl;
            cout << "1×1×20:\t" << mTriples_num[4] << endl;
            cout << "20×20×1:\t" << mTriples_num[5] << endl;
            cout << "20×1×1:\t" << mTriples_num[6] << endl;
            cout << "20×1×40:\t" << mTriples_num[7] << endl;
            cout << "40×40×20:\t" << mTriples_num[8] << endl;
            cout << "40×20×1:\t" << mTriples_num[9] << endl;
            cout << "40×1×40:\t" << mTriples_num[10] << endl;
            cout << "40×1×58:\t" << mTriples_num[11] << endl;
        } */
    }
    this->compute();
}

//计算
void Lstm::compute()
{
    this->dataReadIn();
    this->tripesInit(0); //先生成三元组后计算
    this->forwardNetwork();
    mpf_class ans;
    triplesMul.getPlain(this->output, ans);
    cout << "ANS: " << ans.get_d() << endl;
    //    this->output.print();
    // gmp_printf("Result is: %.*Ff\n", ans.get_str(,));
}

//数据读入
void Lstm::dataReadIn()
{
    cout << "Data preparing " << flush;
    string fileName = (this->role == SERVER) ? "SERVER" : "CLIENT";
    fileName += ".dat";
    ifstream infile;
    infile.open(fileName, ios::in);
    string line;
    mpz_class index, a, b, c;
    char *pch;
    for (int i = 0; i < 20; i++)
    { //读入20组数据
        getline(infile, line);
        char *cstr = stringToChar(line);
        for (int j = 0; j < 58; j++)
        {
            if (j)
                pch = strtok(NULL, mDelim);
            else
                pch = strtok(cstr, mDelim);
            mpz_class temp = mpz_class(pch, 10);
            this->layer1.X[i].change(j, 0, temp);
        }
        delete[] cstr;
        cout << ". " << flush;
    }
    if (getline(infile, line))
    { //读入预测值
        char *cstr = stringToChar(line);
        pch = strtok(cstr, mDelim);
        mpz_class temp = mpz_class(pch, 10);
        this->output_train.change(0, 0, temp);
    }

    // lstmTools.mCopy(temp, this->output_train);
    /* while (getline(infile, line) && infile.good() && !infile.eof() && line != "")
    { //读入整个数据集
        for (int i = 1; i < 20; i++)
        { //读入一次训练的20组数据
            // Matrix matrix_temp(M_NORMAL, 0, 58, 1);
            char *pch;
            char *cstr = stringToChar(line);
            for (int j = 0; j < 58; j++)
            {
                pch = strtok(NULL, mDelim);
                mpz_class temp = mpz_class(pch, 10);
                this->layer1.X[i].change(i, j, temp);
            }
            getline(infile, line);
            delete[] cstr;
        }
    } */
    infile.close();
    cout << "Data OK!" << endl;
}

//读入三元组
void Lstm::tripesInit(int flag)
{
    triplesMul.init(this->role, flag);
}

//向前计算
void Lstm::forwardNetwork()
{
    cout << "forward begin " << flush;
    this->layer1.forward();
    cout << ". " << flush;
    this->layer2.forward(layer1);
    cout << ". " << flush;
    this->layer3.forward(layer2);
    cout << ". " << flush;
    this->layer4.forward(layer3);
    cout << ". " << flush;
    lstmTools.mCopy(this->layer4.A4, this->output);
    cout << "forward ok" << endl;
    showTime();
}

//向后计算
void Lstm::backwardNetwork()
{
    cout << "backward begin " << flush;
    lstmTools.mCopy(this->output_train, this->layer4.et);
    cout << ". " << flush;
    this->layer4.backward(layer3);
    cout << ". " << flush;
    this->layer3.backward(layer4, layer2);
    cout << ". " << flush;
    this->layer2.backward(layer3, layer1);
    cout << ". " << flush;
    this->layer1.backward(layer2);
    cout << "backward ok" << endl;
    showTime();
}

void Lstm_layer1::forward()
{
    Matrix temp40t1_1, temp40t1_2, temp40t1_3;
    Matrix temp_Sj;
    for (int round = 0; round < 20; round++)
    { //
        //求f
        //求f
        triplesMul.mMul(this->Wfx1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wfh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wfh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bf1, this->block.f);
        //triplesMul.getPlain(this->block.f, "this->block.f");
        triplesMul.sigmoid(this->block.f);
        //triplesMul.getPlain(this->block.f, "this->block.f(sigmoid)");
        lstmTools.mCopy(this->block.f, this->F[round]); //保存
        //triplesMul.getPlain(this->F[round], "F");
        //求i
        triplesMul.mMul(this->Wix1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wih1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wih1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bi1, this->block.i);
        triplesMul.sigmoid(this->block.i);
        lstmTools.mCopy(this->block.i, this->I[round]); //保存
        //triplesMul.getPlain(this->I[round], "I");
        //求g
        triplesMul.mMul(this->Wgx1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wgh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wgh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bg1, this->block.g);
        triplesMul.tanh(this->block.g, this->block.g);
        lstmTools.mCopy(this->block.g, this->G[round]); //保存
        //triplesMul.getPlain(this->G[round], "G");
        //求o
        triplesMul.mMul(this->Wox1, this->X[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Woh1, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Woh1, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bo1, this->block.o);
        triplesMul.sigmoid(this->block.o);
        lstmTools.mCopy(this->block.o, this->O[round]); //保存
        //triplesMul.getPlain(this->O[round], "O");
        //求s
        triplesMul.mMull(this->block.g, this->block.i, temp40t1_1);
        if (round)
            triplesMul.mMull(this->S[round - 1], this->block.f, temp40t1_2);
        else
            triplesMul.mMull(this->s_minus1, this->block.f, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, this->S[round]);
        //triplesMul.getPlain(this->S[round], "S");
        //求h
        triplesMul.tanh(this->S[round], temp_Sj);
        triplesMul.mMull(temp_Sj, this->block.o, this->H[round]);
        //triplesMul.getPlain(this->H[round], "H");
    }
}

void Lstm_layer2::forward(Lstm_layer1 layer1)
{
    Matrix temp40t1_1, temp40t1_2, temp40t1_3;
    Matrix temp_Sj;
    for (int round = 0; round < 20; round++)
    {
        //求f
        triplesMul.mMul(this->Wfx2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wfh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wfh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bf2, this->block.f);
        triplesMul.sigmoid(this->block.f);
        lstmTools.mCopy(this->block.f, this->F[round]); //保存
        //triplesMul.getPlain(this->F[round], "F");
        //求i
        triplesMul.mMul(this->Wix2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wih2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wih2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bi2, this->block.i);
        triplesMul.sigmoid(this->block.i);
        lstmTools.mCopy(this->block.i, this->I[round]); //保存
        //triplesMul.getPlain(this->I[round], "I");
        //求g
        triplesMul.mMul(this->Wgx2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Wgh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Wgh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bg2, this->block.g);
        triplesMul.tanh(this->block.g, this->block.g);
        lstmTools.mCopy(this->block.g, this->G[round]); //保存
        //triplesMul.getPlain(this->G[round], "G");
        //求o
        triplesMul.mMul(this->Wox2, layer1.H[round], temp40t1_1);
        if (round)
            triplesMul.mMul(this->Woh2, this->H[round - 1], temp40t1_2);
        else
            triplesMul.mMul(this->Woh2, this->h_minus1, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, temp40t1_3);
        lstmTools.mAdd(temp40t1_3, this->Bo2, this->block.o);
        triplesMul.sigmoid(this->block.o);
        lstmTools.mCopy(this->block.o, this->O[round]); //保存
        //triplesMul.getPlain(this->O[round], "O");
        //求s
        triplesMul.mMull(this->block.g, this->block.i, temp40t1_1);
        if (round)
            triplesMul.mMull(this->S[round - 1], this->block.f, temp40t1_2);
        else
            triplesMul.mMull(this->s_minus1, this->block.f, temp40t1_2);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, this->S[round]);
        //triplesMul.getPlain(this->S[round], "S");
        //求h
        triplesMul.tanh(this->S[round], temp_Sj);
        triplesMul.mMull(temp_Sj, this->block.o, this->H[round]);
        //triplesMul.getPlain(this->H[round], "H");
    }
    lstmTools.mCopy(this->H[19], this->Z2);
    lstmTools.mCopy(this->Z2, this->A2);
}

void Lstm_layer3::forward(Lstm_layer2 layer2)
{
    Matrix temp20t1_1;
    triplesMul.mMul(this->W3, layer2.H[19], temp20t1_1);
    lstmTools.mAdd(temp20t1_1, this->B3, this->Z3);
    //triplesMul.getPlain(this->Z3, "Z3");
    triplesMul.tanh(this->Z3, this->A3);
    //triplesMul.getPlain(this->A3, "A3");
}

void Lstm_layer4::forward(Lstm_layer3 layer3)
{
    Matrix temp1t1_1;
    triplesMul.mMul(this->W4, layer3.A3, temp1t1_1);
    lstmTools.mAdd(temp1t1_1, this->B4, this->Z4);
    //triplesMul.getPlain(this->Z4, "Z4");
    triplesMul.tanh(this->Z4, this->A4); //A4=Et`
    //triplesMul.getPlain(this->A4, "A4");
}

void Lstm_layer4::backward(Lstm_layer3 layer3)
{
    Matrix temp1t1_1, temp1t1_2, temp1t1_3;
    Matrix delta_w4, A3_trans;
    Matrix temp1t20_1, temp1t20_2;
    Matrix const_one{M_CIPHER, 1, 1, 1};
    lstmTools.mSub(this->A4, this->et, temp1t1_1);
    triplesMul.tanh(this->Z4, this->Z4);
    triplesMul.mPoww(this->Z4, temp1t1_2);
    lstmTools.mSub(const_one, temp1t1_2, temp1t1_3);
    triplesMul.mMul(temp1t1_1, temp1t1_3, this->delta_b4); //Δb4
    lstmTools.mTrans(layer3.A3, A3_trans);
    triplesMul.mMul(this->delta_b4, A3_trans, delta_w4); //ΔW4
    lstmTools.mConstMulF(delta_w4, temp1t20_1, this->block.learningRate);
    lstmTools.mConstMulF(this->delta_b4, temp1t1_1, this->block.learningRate);
    lstmTools.mSub(this->W4, temp1t20_1, temp1t20_2);
    lstmTools.mSub(this->B4, temp1t1_1, temp1t1_2);
    lstmTools.mCopy(temp1t20_2, this->W4);
    lstmTools.mCopy(temp1t1_2, this->B4);
}

void Lstm_layer3::backward(Lstm_layer4 layer4, Lstm_layer2 layer2)
{
    Matrix temp1t20_1, temp1t20_2, temp20t1_1, temp20t1_2, temp1t1_1, temp20t40_1, temp20t40_2;
    Matrix G3, W4_trans, A2_trans, delta_W3;
    Matrix const_one{M_CIPHER, 1, 20, 1};
    triplesMul.tanh(this->Z3, this->Z3);
    triplesMul.mPoww(this->Z3, temp1t20_1);
    lstmTools.mSub(const_one, temp1t20_1, temp1t20_2);
    lstmTools.mVector2Matrix(temp1t20_2, G3);
    lstmTools.mTrans(layer4.W4, W4_trans);
    triplesMul.mMul(G3, W4_trans, temp20t1_1);
    triplesMul.mMul(temp20t1_1, layer4.delta_b4, this->delta_b3); //Δb3
    lstmTools.mTrans(layer2.A2, A2_trans);
    triplesMul.mMul(this->delta_b3, A2_trans, delta_W3); //ΔW3
    lstmTools.mConstMulF(delta_W3, temp20t40_1, this->block.learningRate);
    lstmTools.mConstMulF(delta_b3, temp20t1_1, this->block.learningRate);
    lstmTools.mSub(this->W3, temp20t40_1, temp20t40_2);
    lstmTools.mSub(this->B3, temp20t1_1, temp20t1_2);
    lstmTools.mCopy(temp20t40_2, this->W3); //W3
    lstmTools.mCopy(temp20t1_2, this->B3);  //b3
}

void Lstm_layer2::backward(Lstm_layer3 layer3, Lstm_layer1 layer1)
{
    Matrix G2{M_DIAGONAL, 1, 40, 40}, const_one{M_CIPHER, 1, 40, 1}, W3_trans;
    Matrix temp40t20_1, temp40t1_1, temp40t1_2, temp40t1_3;
    Matrix Ht, St, Ft, It, Gt, Ot, x_trans, h_trans;
    Matrix Wfh2_trans, Wih2_trans, Wgh2_trans, Woh2_trans;
    Matrix Wfx2_trans, Wix2_trans, Wgx2_trans, Wox2_trans;
    Matrix delta_bft, delta_bit, delta_bgt, delta_bot;
    Matrix delta_Wfx2t, delta_Wix2t, delta_Wgx2t, delta_Wox2t;
    Matrix delta_Wfh2t, delta_Wih2t, delta_Wgh2t, delta_Woh2t;
    Matrix delta_bf{M_NORMAL, 0, 40, 1}, delta_bi{M_NORMAL, 0, 40, 1}, delta_bg{M_NORMAL, 0, 40, 1}, delta_bo{M_NORMAL, 0, 40, 1};
    Matrix delta_Wfx2{M_NORMAL, 0, 40, 40}, delta_Wix2{M_NORMAL, 0, 40, 40}, delta_Wgx2{M_NORMAL, 0, 40, 40}, delta_Wox2{M_NORMAL, 0, 40, 40};
    Matrix delta_Wfh2{M_NORMAL, 0, 40, 40}, delta_Wih2{M_NORMAL, 0, 40, 40}, delta_Wgh2{M_NORMAL, 0, 40, 40}, delta_Woh2{M_NORMAL, 0, 40, 40};
    lstmTools.mTrans(layer3.W3, W3_trans);
    triplesMul.mMul(G2, W3_trans, temp40t20_1);
    triplesMul.mMul(temp40t20_1, layer3.delta_b3, Ht); //H
    triplesMul.mMull(Ht, this->O[19], temp40t1_1);
    triplesMul.tanh(this->S[19], this->S[19]);
    triplesMul.mPoww(this->S[19], temp40t1_2);
    lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
    triplesMul.mMull(temp40t1_1, temp40t1_3, St); //S

    for (int t = 19; t >= 0;)
    {
        if (t)
            triplesMul.mMull(St, this->S[t - 1], Ft);
        else
            triplesMul.mMull(St, this->s_minus1, Ft);
        triplesMul.mMull(St, this->G[t], It);
        triplesMul.mMull(St, this->I[t], Gt);
        triplesMul.mMull(Ht, this->S[t], Ot); //this->S[t]已经被tanh函数处理过

        lstmTools.mSub(const_one, this->F[t], temp40t1_1);
        triplesMul.mMull(this->F[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ft, temp40t1_2, delta_bft);   //Δbf2t
        lstmTools.mCopy(delta_bft, this->delta_Bf[t]); //保存
        lstmTools.mSub(const_one, this->I[t], temp40t1_1);
        triplesMul.mMull(this->I[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(It, temp40t1_2, delta_bit);   //Δbi2t
        lstmTools.mCopy(delta_bft, this->delta_Bi[t]); //保存
        triplesMul.mPoww(this->G[t], temp40t1_1);
        lstmTools.mSub(const_one, temp40t1_1, temp40t1_2);
        triplesMul.mMull(Gt, temp40t1_2, delta_bgt);   //Δbg2t
        lstmTools.mCopy(delta_bft, this->delta_Bg[t]); //保存
        lstmTools.mSub(const_one, this->O[t], temp40t1_1);
        triplesMul.mMull(this->O[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ot, temp40t1_2, delta_bot);   //Δbo2t
        lstmTools.mCopy(delta_bft, this->delta_Bo[t]); //保存

        lstmTools.mTrans(layer1.H[t], x_trans);
        if (t)
            lstmTools.mTrans(this->H[t - 1], h_trans);
        else
            lstmTools.mTrans(this->h_minus1, h_trans);
        triplesMul.mMul(delta_bft, x_trans, delta_Wfx2t); //ΔWfx2t
        triplesMul.mMul(delta_bft, h_trans, delta_Wfh2t); //ΔWfh2t
        triplesMul.mMul(delta_bit, x_trans, delta_Wix2t); //ΔWix2t
        triplesMul.mMul(delta_bit, h_trans, delta_Wih2t); //ΔWih2t
        triplesMul.mMul(delta_bgt, x_trans, delta_Wgx2t); //ΔWgx2t
        triplesMul.mMul(delta_bgt, h_trans, delta_Wgh2t); //ΔWgh2t
        triplesMul.mMul(delta_bot, x_trans, delta_Wox2t); //ΔWox2t
        triplesMul.mMul(delta_bot, h_trans, delta_Woh2t); //ΔWoh2t

        lstmTools.mAccu(delta_bf, delta_bft);
        lstmTools.mAccu(delta_bi, delta_bit);
        lstmTools.mAccu(delta_bg, delta_bgt);
        lstmTools.mAccu(delta_bo, delta_bot);

        lstmTools.mAccu(delta_Wfx2, delta_Wfx2t);
        lstmTools.mAccu(delta_Wfh2, delta_Wfh2t);
        lstmTools.mAccu(delta_Wix2, delta_Wix2t);
        lstmTools.mAccu(delta_Wih2, delta_Wih2t);
        lstmTools.mAccu(delta_Wgx2, delta_Wgx2t);
        lstmTools.mAccu(delta_Wgh2, delta_Wgh2t);
        lstmTools.mAccu(delta_Wox2, delta_Wox2t);
        lstmTools.mAccu(delta_Woh2, delta_Woh2t);

        if (t == 19)
        {
            Matrix accu_x20{M_NORMAL, 0, 40, 1};
            lstmTools.mTrans(this->Wfx2, Wfx2_trans);
            triplesMul.mMul(Wfx2_trans, delta_bft, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wix2, Wix2_trans);
            triplesMul.mMul(Wix2_trans, delta_bit, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wgx2, Wgx2_trans);
            triplesMul.mMul(Wgx2_trans, delta_bgt, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mTrans(this->Wox2, Wox2_trans);
            triplesMul.mMul(Wox2_trans, delta_bot, temp40t1_1);
            lstmTools.mAccu(accu_x20, temp40t1_1);
            lstmTools.mCopy(accu_x20, this->X20);
        }
        if (t)
            t--;
        else
            break;
        Matrix accu_h{M_NORMAL, 0, 40, 1}, accu_s{M_NORMAL, 0, 40, 1};
        lstmTools.mTrans(this->Wfh2, Wfh2_trans);
        triplesMul.mMul(Wfh2_trans, delta_bft, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wih2, Wih2_trans);
        triplesMul.mMul(Wih2_trans, delta_bit, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wgh2, Wgh2_trans);
        triplesMul.mMul(Wgh2_trans, delta_bgt, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Woh2, Woh2_trans);
        triplesMul.mMul(Woh2_trans, delta_bot, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mCopy(accu_h, Ht); //h//?

        triplesMul.mMull(Ht, this->O[t], temp40t1_1);
        triplesMul.tanh(this->S[t], this->S[t]);
        triplesMul.mPoww(this->S[t], temp40t1_2);
        lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
        triplesMul.mMull(temp40t1_1, temp40t1_3, temp40t1_2);
        triplesMul.mMull(St, this->F[t + 1], temp40t1_1);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, St); //s
    }
    Matrix temp_W, temp_S;
    lstmTools.mConstMulF(delta_Wfx2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wfx2, temp_W);
    lstmTools.mConstMulF(delta_Wfh2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wfh2, temp_W);
    lstmTools.mConstMulF(delta_Wix2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wix2, temp_W);
    lstmTools.mConstMulF(delta_Wih2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wih2, temp_W);
    lstmTools.mConstMulF(delta_Wgx2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wgx2, temp_W);
    lstmTools.mConstMulF(delta_Wgh2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wgh2, temp_W);
    lstmTools.mConstMulF(delta_Wox2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wox2, temp_W);
    lstmTools.mConstMulF(delta_Woh2, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Woh2, temp_W);

    lstmTools.mConstMulF(delta_bf, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bf2, temp_S);
    lstmTools.mConstMulF(delta_bi, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bi2, temp_S);
    lstmTools.mConstMulF(delta_bg, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bg2, temp_S);
    lstmTools.mConstMulF(delta_bo, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bo2, temp_S);
}

void Lstm_layer1::backward(Lstm_layer2 layer2)
{
    Matrix const_one{M_CIPHER, 1, 40, 1}, W3_trans;
    Matrix temp40t20_1, temp40t1_1, temp40t1_2, temp40t1_3;
    Matrix Ht, St, Ft, It, Gt, Ot, x_trans, h_trans;
    Matrix Wfx2_trans, Wix2_trans, Wgx2_trans, Wox2_trans;
    Matrix Wfh1_trans, Wih1_trans, Wgh1_trans, Woh1_trans;
    Matrix Wfx1_trans, Wix1_trans, Wgx1_trans, Wox1_trans;
    Matrix delta_bft, delta_bit, delta_bgt, delta_bot;
    Matrix delta_Wfx1t, delta_Wix1t, delta_Wgx1t, delta_Wox1t;
    Matrix delta_Wfh1t, delta_Wih1t, delta_Wgh1t, delta_Woh1t;
    Matrix delta_bf{M_NORMAL, 0, 40, 1}, delta_bi{M_NORMAL, 0, 40, 1}, delta_bg{M_NORMAL, 0, 40, 1}, delta_bo{M_NORMAL, 0, 40, 1};
    Matrix delta_Wfx1{M_NORMAL, 0, 40, 58}, delta_Wix1{M_NORMAL, 0, 40, 58}, delta_Wgx1{M_NORMAL, 0, 40, 58}, delta_Wox1{M_NORMAL, 0, 40, 58};
    Matrix delta_Wfh1{M_NORMAL, 0, 40, 40}, delta_Wih1{M_NORMAL, 0, 40, 40}, delta_Wgh1{M_NORMAL, 0, 40, 40}, delta_Woh1{M_NORMAL, 0, 40, 40};

    lstmTools.mCopy(layer2.X20, Ht); //H
    triplesMul.mMull(Ht, this->O[19], temp40t1_1);
    triplesMul.tanh(this->S[19], this->S[19]);
    triplesMul.mPoww(this->S[19], temp40t1_2);
    lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
    triplesMul.mMull(temp40t1_1, temp40t1_3, St); //S
    for (int t = 19; t >= 0;)
    {
        if (t)
            triplesMul.mMull(St, this->S[t - 1], Ft);
        else
            triplesMul.mMull(St, this->s_minus1, Ft);
        triplesMul.mMull(St, this->G[t], It);
        triplesMul.mMull(St, this->I[t], Gt);
        triplesMul.mMull(Ht, this->S[t], Ot); //this->S[t]已经被tanh函数处理过

        lstmTools.mSub(const_one, this->F[t], temp40t1_1);
        triplesMul.mMull(this->F[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ft, temp40t1_2, delta_bft); //Δbf2t
        lstmTools.mSub(const_one, this->I[t], temp40t1_1);
        triplesMul.mMull(this->I[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(It, temp40t1_2, delta_bit); //Δbi2t
        triplesMul.mPoww(this->G[t], temp40t1_1);
        lstmTools.mSub(const_one, temp40t1_1, temp40t1_2);
        triplesMul.mMull(Gt, temp40t1_2, delta_bgt); //Δbg2t
        lstmTools.mSub(const_one, this->O[t], temp40t1_1);
        triplesMul.mMull(this->O[t], temp40t1_1, temp40t1_2);
        triplesMul.mMull(Ot, temp40t1_2, delta_bot); //Δbo2t

        lstmTools.mTrans(this->X[t], x_trans);
        if (t)
            lstmTools.mTrans(this->H[t - 1], h_trans);
        else
            lstmTools.mTrans(this->h_minus1, h_trans);
        triplesMul.mMul(delta_bft, x_trans, delta_Wfx1t); //ΔWfx2t
        triplesMul.mMul(delta_bft, h_trans, delta_Wfh1t); //ΔWfh2t
        triplesMul.mMul(delta_bit, x_trans, delta_Wix1t); //ΔWix2t
        triplesMul.mMul(delta_bit, h_trans, delta_Wih1t); //ΔWih2t
        triplesMul.mMul(delta_bgt, x_trans, delta_Wgx1t); //ΔWgx2t
        triplesMul.mMul(delta_bgt, h_trans, delta_Wgh1t); //ΔWgh2t
        triplesMul.mMul(delta_bot, x_trans, delta_Wox1t); //ΔWox2t
        triplesMul.mMul(delta_bot, h_trans, delta_Woh1t); //ΔWoh2t

        lstmTools.mAccu(delta_bf, delta_bft);
        lstmTools.mAccu(delta_bi, delta_bit);
        lstmTools.mAccu(delta_bg, delta_bgt);
        lstmTools.mAccu(delta_bo, delta_bot);

        lstmTools.mAccu(delta_Wfx1, delta_Wfx1t);
        lstmTools.mAccu(delta_Wfh1, delta_Wfh1t);
        lstmTools.mAccu(delta_Wix1, delta_Wix1t);
        lstmTools.mAccu(delta_Wih1, delta_Wih1t);
        lstmTools.mAccu(delta_Wgx1, delta_Wgx1t);
        lstmTools.mAccu(delta_Wgh1, delta_Wgh1t);
        lstmTools.mAccu(delta_Wox1, delta_Wox1t);
        lstmTools.mAccu(delta_Woh1, delta_Woh1t);

        if (t)
            t--;
        else
            break;
        Matrix accu_h{M_NORMAL, 0, 40, 1}, accu_s{M_NORMAL, 0, 40, 1};
        lstmTools.mTrans(this->Wfh1, Wfh1_trans);
        triplesMul.mMul(Wfh1_trans, delta_bft, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wih1, Wih1_trans);
        triplesMul.mMul(Wih1_trans, delta_bit, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Wgh1, Wgh1_trans);
        triplesMul.mMul(Wgh1_trans, delta_bgt, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(this->Woh1, Woh1_trans);
        triplesMul.mMul(Woh1_trans, delta_bot, temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);

        lstmTools.mTrans(layer2.Wfx2, Wfx2_trans);
        triplesMul.mMul(Wfx2_trans, layer2.delta_Bf[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wix2, Wix2_trans);
        triplesMul.mMul(Wix2_trans, layer2.delta_Bi[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wgx2, Wgx2_trans);
        triplesMul.mMul(Wgx2_trans, layer2.delta_Bg[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);
        lstmTools.mTrans(layer2.Wox2, Wox2_trans);
        triplesMul.mMul(Wox2_trans, layer2.delta_Bo[t], temp40t1_1);
        lstmTools.mAccu(accu_h, temp40t1_1);

        lstmTools.mCopy(accu_h, Ht);

        triplesMul.mMull(Ht, this->O[t], temp40t1_1);
        triplesMul.tanh(this->S[t], this->S[t]);
        triplesMul.mPoww(this->S[t], temp40t1_2);
        lstmTools.mSub(const_one, temp40t1_2, temp40t1_3);
        triplesMul.mMull(temp40t1_1, temp40t1_3, temp40t1_2);
        triplesMul.mMull(St, this->F[t + 1], temp40t1_1);
        lstmTools.mAdd(temp40t1_1, temp40t1_2, St);
    }
    Matrix temp_W, temp_S;
    lstmTools.mConstMulF(delta_Wfx1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wfx1, temp_W);
    lstmTools.mConstMulF(delta_Wfh1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wfh1, temp_W);
    lstmTools.mConstMulF(delta_Wix1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wix1, temp_W);
    lstmTools.mConstMulF(delta_Wih1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wih1, temp_W);
    lstmTools.mConstMulF(delta_Wgx1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wgx1, temp_W);
    lstmTools.mConstMulF(delta_Wgh1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wgh1, temp_W);
    lstmTools.mConstMulF(delta_Wox1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Wox1, temp_W);
    lstmTools.mConstMulF(delta_Woh1, temp_W, this->block.learningRate);
    lstmTools.mAccuSub(this->Woh1, temp_W);
    lstmTools.mConstMulF(delta_bf, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bf1, temp_S);
    lstmTools.mConstMulF(delta_bi, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bi1, temp_S);
    lstmTools.mConstMulF(delta_bg, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bg1, temp_S);
    lstmTools.mConstMulF(delta_bo, temp_S, this->block.learningRate);
    lstmTools.mAccuSub(this->Bo1, temp_S);
}
