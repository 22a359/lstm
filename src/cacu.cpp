/* #include "lstm.h"
int main()
{
    mpz_set_str(modNum.get_mpz_t(), modNumStr.c_str(), 10);
    MatrixTools tool;
    cout << "ok" << endl;
    mpz_class a0, b0, c0;
    mpz_class a1, b1, c1;
    mpz_class A, B, C, temp_mul, ans;
    mpz_class temp_mul1, temp_mul2;
    mpz_set_str(a0.get_mpz_t(), "154347849541014657496028657", 10);
    mpz_set_str(b0.get_mpz_t(), "256412560911324580014890340", 10);
    mpz_set_str(c0.get_mpz_t(), "116520562264157646734917938", 10);
    mpz_set_str(a1.get_mpz_t(), "310156710916565315380856467", 10);
    mpz_set_str(b1.get_mpz_t(), "268440509212298875155151389", 10);
    mpz_set_str(c1.get_mpz_t(), "494697028813355095080916597", 10);
    // gmp_printf("(%Zd+%Zd)*(%Zd+%Zd)=%Zd+%Zd\n", a0, a1, b0, b1, c0, c1);
    tool.mojia(a0, a1, A);
    tool.mojia(b0, b1, B);
    tool.mojia(c0, c1, C);
    tool.mLocalMocheng(A, B, temp_mul);
    mpz_sub(ans.get_mpz_t(), C.get_mpz_t(), temp_mul.get_mpz_t());
    // gmp_printf("%Zd\n", A);
    gmp_printf("%Zd\n", ans);
    return 0;
} */