 #include "../lstm.h"
int main()
{
    mpz_set_str(modNum.get_mpz_t(), modNumStr.c_str(), 10);
    MatrixTools tool;
    Triples triples;
    bool flag = false;
    string in_string1, in_string2;
    mpz_class index1, index2;
    char type;
    do
    {
        cout << "\nTriples type:\n(1)Interger  (2)Matrix\ntype:" << flush;
        cin.get(type).get();
    } while (type != '1' && type != '2');
    cout << "Input SERVER's whole triples line:" << flush;
    cin >> in_string1;
    cout << "Input CLIENT's whole triples line:" << flush;
    cin >> in_string2;
    if (type == '1')
    {
        mpz_class a0, b0, c0, a1, b1, c1;
        mpz_class A, B, C, temp_mul, ans;
        triples.deserialization(in_string1, index1, a0, b0, c0);
        triples.deserialization(in_string2, index2, a1, b1, c1);
        tool.mojia(a0, a1, A);
        tool.mojia(b0, b1, B);
        tool.mojia(c0, c1, C);
        tool.mLocalMocheng(A, B, temp_mul);
        mpz_sub(ans.get_mpz_t(), C.get_mpz_t(), temp_mul.get_mpz_t());
        if (ans == 0)
            flag = true;
    }
    else
    {
        Matrix a0, b0, c0, a1, b1, c1;
        Matrix A, B, C, temp_mul, ans;
        triples.deserialization(in_string1, index1, a0, b0, c0);
        triples.deserialization(in_string2, index2, a1, b1, c1);
        tool.mAdd(a0, a1, A);
        tool.mAdd(b0, b1, B);
        tool.mAdd(c0, c1, C);
        tool.mLocalMul(A, B, temp_mul);
        tool.mSub(C, temp_mul, ans);
        Matrix const_zero{M_NORMAL, 0, ans.row, ans.col};
        if (tool.mCmp(ans, const_zero))
            flag = true;
    }
    if (flag)
        cout << "Triples PASS" << endl;
    else
        cout << "Triples WRONG" << endl;
    return 0;
}
