/* #include "lstm.h"
int main()
{
    Lstm_network lstm;
    char role, temp;
    int epochs;
    do
    {
        cout << "\nInput your role:\n(1)SERVER  (2)CLIENT\nrole:" << flush;
        cin.get(role).get();
    } while (role != '1' && role != '2');
    bool Role = (role == '1') ? SERVER : CLIENT;
    cout << "\nInput epochs:" << flush;
    cin >> epochs;
    cout << "\nYou are " << ((role == '1') ? "SERVER" : "CLIENT")
         << "\nEpochs is: " << epochs << endl;
    lstm.triplesGen(Role, epochs);
    return 0;
} */