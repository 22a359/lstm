#ifndef __CHECK__
#define __CHECK__

#include "mulgate.h"

int check()
{
    MatrixTools ckTool;
    Triples ckTriples;
    bool flag = false;
    //string in_string1, in_string2;
    string sName, cName;
    mpz_class index1, index2;
    char type;
    /*do
    {
        cout << "\nTriples type:\n(1)Interger  (2)Matrix\ntype:" << flush;
        cin.get(type).get();
    } while (type != '1' && type != '2');*/
    cout << "Input one of the file names:" << flush;
    cin >> sName;
    cName = sName;
    if (sName[0] == 'S')
    {
        cName[0] = 'C';
        cName[1] = 'L';
        cName[2] = 'I';
        cName[3] = 'E';
        cName[4] = 'N';
        cName[5] = 'T';
    } else
    {
        cName[0] = 'S';
        cName[1] = 'E';
        cName[2] = 'R';
        cName[3] = 'V';
        cName[4] = 'E';
        cName[5] = 'R';
    }
    cout << "Another file name:          " << cName << endl;
    type = (sName[7] == 'i') ? '1' : '2';
    start = time(NULL);
    if (type == '1')
    {
        mpz_class a0, b0, c0, a1, b1, c1;
        mpz_class A, B, C, temp_mul, ans;
        ifstream infileS, infileC;
        string lineS, lineC;
        infileS.open(sName, ios::in);
        infileC.open(cName, ios::in);
        if (!infileS)
            cout << "Read SERVER's file failed!" << endl;
        if (!infileC)
            cout << "Read CLIENT's file failed!" << endl;
        while ((getline(infileS, lineS) && infileS.good() && !infileS.eof() && !lineS.empty()) &&
               (getline(infileC, lineC) && infileC.good() && !infileC.eof() && !lineC.empty()))
        {
            ckTriples.deserialization(lineS, index1, a0, b0, c0);
            ckTriples.deserialization(lineC, index2, a1, b1, c1);
            ckTool.mojia(a0, a1, A);
            ckTool.mojia(b0, b1, B);
            ckTool.mojia(c0, c1, C);
            ckTool.mLocalMocheng(A, B, temp_mul);
            mpz_sub(ans.get_mpz_t(), C.get_mpz_t(), temp_mul.get_mpz_t());
            /*mpz_print(A, "A");
            mpz_print(B, "B");
            mpz_print(C, "C");
            mpz_print(temp_mul, "A * B");
            mpz_print(ans, "A * B - C");*/
            if (ans == 0)
                flag = true;
        }
        infileS.close();
        infileC.close();
    } else
    {
        Matrix a0, b0, c0, a1, b1, c1;
        Matrix A, B, C, temp_mul, ans;
        ifstream infileS, infileC;
        string lineS, lineC;
        infileS.open(sName, ios::in);
        infileC.open(cName, ios::in);
        if (!infileS)
            cout << "Read SERVER's file failed!" << endl;
        if (!infileC)
            cout << "Read CLIENT's file failed!" << endl;
        while ((getline(infileS, lineS) && infileS.good() && !infileS.eof() && !lineS.empty()) &&
               (getline(infileC, lineC) && infileC.good() && !infileC.eof() && !lineC.empty()))
        {
            ckTriples.deserialization(lineS, index1, a0, b0, c0);
            ckTriples.deserialization(lineC, index2, a1, b1, c1);
            ckTool.mAdd(a0, a1, A);
            ckTool.mAdd(b0, b1, B);
            ckTool.mAdd(c0, c1, C);
            ckTool.mLocalMul(A, B, temp_mul);
            ckTool.mSub(C, temp_mul, ans);
            /*A.print("A");
            B.print("B");
            C.print("C");
            temp_mul.print("A * B");
            ans.print("A * B - C");*/
            Matrix const_zero{M_NORMAL, 0, ans.row, ans.col};
            if (ckTool.mCmp(ans, const_zero))
                flag = true;
        }
        infileS.close();
        infileC.close();
    }
    if (flag)
        cout << "Triples PASS" << endl;
    else
        cout << "Triples WRONG" << endl;
    return 0;
}

#endif
