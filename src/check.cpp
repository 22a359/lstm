#include "lstm.h"
int main()
{
    mpz_set_str(modNum.get_mpz_t(), modNumStr.c_str(), 10);
    MatrixTools tool;
    mpz_class a0, b0, c0;
    mpz_class a1, b1, c1;
    mpz_class A, B, C, temp_mul, ans;
    mpz_class temp_mul1, temp_mul2;
    mpz_set_str(a0.get_mpz_t(), "259128784049357172805560083", 10);
    mpz_set_str(b0.get_mpz_t(), "94556905782911890522436802", 10);
    mpz_set_str(c0.get_mpz_t(), "362463793872708517268344304", 10);
    mpz_set_str(a1.get_mpz_t(), "403209929668914510540813341", 10);
    mpz_set_str(b1.get_mpz_t(), "183344772914832232510737557", 10);
    mpz_set_str(c1.get_mpz_t(), "238338428940344171086489615", 10);
    tool.mojia(a0, a1, A);
    tool.mojia(b0, b1, B);
    tool.mojia(c0, c1, C);
    tool.mLocalMocheng(A, B, temp_mul);
    mpz_sub(ans.get_mpz_t(), C.get_mpz_t(), temp_mul.get_mpz_t());
    if(ans==0)
        cout<<"PASS"<<endl;;
    return 0;
}