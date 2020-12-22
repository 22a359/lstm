#include "share.h"

void shareGen()
{
    start = time(nullptr);
    ifstream _csvInput;
    //指定文件路径
    string p = "input/data/dataSet.csv";
    _csvInput.open(p, ios::in);
    //定义一行数据为字符串
    string _Oneline;
    string fileName1 = "input/data/SERVER.dat";
    string fileName2 = "input/data/CLIENT.dat";
    ofstream outfile1, outfile2;
    outfile1.open(fileName1, ios::out | ios::trunc);
    outfile2.open(fileName2, ios::out | ios::trunc);
    mpf_class x_f;
    mpz_class x_z;
    mpz_class server, client;
    float temp;
    int index = 0;
    while (getline(_csvInput, _Oneline))
    {
        //定义字符串流对象
        istringstream _Readstr(_Oneline);
        //定义一行数据中的各个字串
        string _partOfstr;
        //将一行数据按'，'分割
        for (int i = 0; i < 58; i++)
        {
            getline(_Readstr, _partOfstr, ',');
            temp = stof(_partOfstr);
            x_f = temp;
            mpf_mul_2exp(x_f.get_mpf_t(), x_f.get_mpf_t(), eAndC);
            server = randNumGen();
            x_z = x_f;
            client = x_z + modNum - server;
            outfile1 << server.get_str();
            outfile2 << client.get_str();
            if (i < 57)
            {
                outfile1 << ",";
                outfile2 << ",";
            }
        }
        outfile1 << endl;
        outfile2 << endl;
        index++;
        cout << symbol[index / 10 % 4] << " " << index << " \r" << flush;
    }
    outfile1.close();
    outfile2.close();
    printf("Share Gen OK\n");
}

CSVreader::CSVreader(const char *path)
{
    string p = "./dataSet.csv";
    _csvInput.open(p, ios::in);
    if (!_csvInput)
        cout << "read error" << endl;
}

//读取csv文件数据
int CSVreader::readline()
{
    //定义一行数据为字符串
    string _Oneline;
    //读取一行数据
    getline(_csvInput, _Oneline);

    //定义字符串流对象
    istringstream _Readstr(_Oneline);
    //定义一行数据中的各个字串
    string _partOfstr;
    //将一行数据按'，'分割
    for (int i = 0; i < 58; i++)
    {
        getline(_Readstr, _partOfstr, ',');
        data[i] = atof(_partOfstr.c_str());
    }
    //判断是否读完数据
    if ((data[49]) == 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
