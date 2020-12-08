#include "lstm.h"
int main(int argc, char *argv[])
{
    // char action = '2';
    // int epochs = 2;
    // bool Role = CLIENT;

    char action;
    int epochs, ch;
    bool Role;
    if(argc==1)
    {
        cout<<"Use \"lstm -h\" to get help."<<endl;
        return 0;
    }
    while ((ch = getopt(argc, argv, "r:a:e:h")) != -1)
    {
        switch (ch)
        {
        case 'r':
            Role = (!strcmp(optarg, "1")) ? SERVER : CLIENT;
            break;
        case 'a':
            action = optarg[0];
            break;
        case 'e':
            epochs = atoi(optarg);
            break;
        case 'h':
        default:
            cout << "\n Usage :"
                 << "\tlstm [Options] <Destination>\n\n"
                 << " Options            Destination\n"
                 << "  -r --role         (1)SERVER        (2)CLIENT\n"
                 << "  -a --action       (1)Triples Gen   (2)LSTM\n"
                 << "  -e --epochs       Input an interger\n"
                 << "  -h --help\n"
                 << endl;
                 return 0;
            break;
        }
    }
    cout << "\nYou are " << ((Role == SERVER) ? "SERVER" : "CLIENT")
         << "\nEpochs is: " << epochs << endl;
    /* do
    {
        cout << "\nInput your action:\n(1)Triples Gen   (2)LSTM\naction:" << flush;
        cin.get(action).get();
    } while (action != '1' && action != '2');
    do
    {
        cout << "\nInput your role:\n(1)SERVER  (2)CLIENT\nrole:" << flush;
        cin.get(role).get();
    } while (role != '1' && role != '2');
    bool Role = (role == '1') ? SERVER : CLIENT;
    cout << "\nInput epochs:" << flush;
    cin >> epochs;
    cin.get();
    cout << "\nYou are " << ((role == '1') ? "SERVER" : "CLIENT")
         << "\nEpochs is: " << epochs << endl;
         */
    Lstm lstm(Role, epochs);
    switch (action)
    {
    case '1':
        lstm.triplesGen();
        break;
    case '2':
        lstm.train();
        break;
    default:
        break;
    }
    showTime(1); 
    return 0;
}