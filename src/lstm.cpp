#include "lstm.h"
int main()
{
    char role, action;
    int epochs;
    do
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
    return 0;
}