#include "global.h"

bool role;
int peopleNum;
int baseNum = 10;
mp_bitcnt_t randBit = 512;
mp_bitcnt_t eAndC = 32;
mpz_class modNum{"1248878241464690322159961483223834105349154894079", 10};//160bit
//mpz_class eAndC{"4294967296", 10};//2^32
//mpz_class eAndC{"16777216", 10};//2^24
//string modNumStr = "40343";//小素数
//string modNumStr = "618970019642690137449562111"; //大素数
char tDelim[2] = "-";
char mDelim[2] = ",";
char iDelim[2] = ":";
char xDelim[2] = "|";
char checkMSG[3] = "ok";
mpz_class sig0{"2147483648", 10};
mpz_class sig1{"1071300947", 10};
mpz_class sig2{"0", 10};
mpz_class sig3{"-79372572", 10};
mpz_class learningRate{"1073741824", 10};//0.25
mpz_class learningRate2{"107374182", 10};//0.025
char symbol[4] = {'|', '/', '-', '\\'};
string prefix[2] = {"Training_", "Test_"};
int m1m1m1_counts = 49626;
int m40m58m1_counts = 80;
int m40m40m1_counts = 472;
int m20m40m1_counts = 1;
int m1m20m1_counts = 1;
int m1m1m20_counts = 1;
int m20m20m1_counts = 1;
int m20m1m1_counts = 1;
int m20m1m40_counts = 1;
int m40m40m20_counts = 1;
int m40m20m1_counts = 1;
int m40m1m40_counts = 240;
int m40m1m58_counts = 80;

//int m1m1m1_counts = 1000;
//int m40m58m1_counts = 0;
//int m40m40m1_counts = 0;
//int m20m40m1_counts = 0;
//int m1m20m1_counts = 0;
//int m1m1m20_counts = 0;
//int m20m20m1_counts = 0;
//int m20m1m1_counts = 0;
//int m20m1m40_counts = 0;
//int m40m40m20_counts = 0;
//int m40m20m1_counts = 0;
//int m40m1m40_counts = 0;
//int m40m1m58_counts = 0;

int m1m1m1_num = 0;
int mTriples_num[12] = {0};
int triples_num[13] = {m1m1m1_counts,
                       m40m58m1_counts,
                       m40m40m1_counts,
                       m20m40m1_counts,
                       m1m20m1_counts,
                       m1m1m20_counts,
                       m20m20m1_counts,
                       m20m1m1_counts,
                       m20m1m40_counts,
                       m40m40m20_counts,
                       m40m20m1_counts,
                       m40m1m40_counts,
                       m40m1m58_counts};
time_t start, lastPoint;
double speedS = 0, speedR = 0;
int countS = 0, countR = 0;

//将string转换为char*
char *stringToChar(string in_string)
{
    char *cstr = new char[in_string.size() + 1];
    strcpy(cstr, in_string.c_str());
    return cstr;
}

//在大素数范围内生成随机数
mpz_class randNumGen()
{
    random_device rd;
    default_random_engine e(rd());
    mpz_class ans, randNum;
    gmp_randstate_t grt;
    gmp_randinit_mt(grt);
    gmp_randseed_ui(grt, e());
    mpz_urandomb(randNum.get_mpz_t(), grt, randBit);
    mpz_mod(ans.get_mpz_t(), randNum.get_mpz_t(), modNum.get_mpz_t());
    gmp_randclear(grt);
    return ans;
}

void showTime()
{
    time_t point = time(nullptr);
    time_t allTime = (double) (point - start);
    time_t thisTime = (double) (point - lastPoint);
    cout << "Using time: " << thisTime / 3600 % 24 << "h " << thisTime / 60 % 60 << "m " << thisTime % 60 << "s | "
         << "Total time: " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m " << allTime % 60 << "s" << endl;
    if (SPEED)
    {
        cout << "Send speed: " << speedS / countS << "MiB/s | Recv speed:" << speedR / countR << "MiB/s"
             << endl;
        speedS = speedR = countS = countR = 0;
    }
    lastPoint = point;
}

void showTime(int flag)
{
    time_t point = time(nullptr);
    time_t allTime = (double) (point - start);
    cout << "Total time: " << allTime / 3600 % 24 << "h " << allTime / 60 % 60 << "m " << allTime % 60 << "s" << endl;
    if (SPEED)
        cout << "Send speed: " << speedS << "MiB/s | Recv speed:" << speedR << "MiB/s"
             << endl;
}

void mpz_print(mpz_class num, string out_word)
{
    cout << out_word << ": ";
    cout << num.get_str() << endl;
}
