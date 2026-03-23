// problem2 (ATM)
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;
string FileClientName="Client#.txt";
enum enATMMainMenueOptions {AQuickWithdraw = 1,aNormalWithdraw = 2,aDeposite = 3,aCheckBalance = 4,aLogout = 5};
struct stClient
{
    string AccountNumber="";
    short PinCode=0;
    string ClientName="";
    string Phone="";
    double Balance=0;
    bool MarkForDelete=false;
};
stClient CurrentClient;
void ShowMainMenueScreen();
void ShowQuickWithdrawScreen();
void ShowNormalWithdrawScreen();
void Login();
string ReadAccountNumber()
{
    string AccountNumber;
    cout<<"\n Please Enter Account Number? ";
    cin>>AccountNumber;
    return AccountNumber;
}
int ReadPinCode()
{
    int PinCode;
    cout<<"\n Please Enter Pin Code? ";
    cin>>PinCode;
    return PinCode;
}
vector<string> Split(string S1,string Dellim="#//#")
{
    vector<string> vstring;
    string sword="";
    short pos=0;
    while((pos=S1.find(Dellim))!=std::string::npos)
    {
        sword=S1.substr(0,pos);
        if (sword!="")
        {
            vstring.push_back(sword);
        }
        S1.erase(0,pos+Dellim.length());
    }
    if (S1!="")
    {
        vstring.push_back(S1);
    }
    return vstring;
}
stClient ConvertLineToRecord(string Line,string Dellim="#//#")
{
    stClient Client;
    vector<string> vstring=Split(Line,Dellim);
    Client.AccountNumber=vstring[0];
    Client.PinCode=stoi(vstring[1]);
    Client.ClientName=vstring[2];
    Client.Phone=vstring[3];
    Client.Balance=stod(vstring[4]);
    return Client;
}
string ConvertRecordToLine(stClient Client,string Dellim="#//#")
{
    string Line="";
    Line+=Client.AccountNumber+Dellim;
    Line+=to_string(Client.PinCode)+Dellim;
    Line+=Client.ClientName+Dellim;
    Line+=Client.Phone+Dellim;
    Line+=to_string(Client.Balance);
    return Line;
}
vector<stClient> LoadDataFromFile(string FileName)
{
    fstream MyFile;
    vector<stClient> vclient;
    MyFile.open(FileName,ios::in);
    if (MyFile.is_open())
    {
        string Line;
        stClient Client;
        while (getline(MyFile,Line))
        {
            Client=ConvertLineToRecord(Line,"#//#");
            vclient.push_back(Client);
        }
        MyFile.close();
    }
    return vclient;
}
bool FindClientByAccountNumber(string AccountNumber,stClient& vClient)
{
    vector<stClient> vCLient=LoadDataFromFile(FileClientName);
    for (stClient& C : vCLient)
    {
        if (C.AccountNumber==AccountNumber)
        {
            CurrentClient=C;
            return true;
        }
    }
    return false;
}
bool IsCheakClientByAccountNumberAndPinCode(string AccountNumber,int PinCode)
{
    if (FindClientByAccountNumber(AccountNumber,CurrentClient) && CurrentClient.PinCode==PinCode)
    {
        return true;
    }
    return false;
}
enATMMainMenueOptions ReadATMMainMenueOptions()
{
    short Choose=0;
    cout<<"Choose what do you want to do? [1 to 5]? ";
    cin>>Choose;
    return enATMMainMenueOptions(Choose);
}
void GoBackToMainMenue()
{
    cout<<"\n\nPress any key to go back Main Menue...\n";
    system("pause");
    system("cls");
    ShowMainMenueScreen();
}
short ReadQuickWithdraw()
{
    short Choose;
    do
    {
        cout<<"\n\nPlease Enter Number To Take Amount? ";
        cin>>Choose;
    }while(Choose<1 || Choose>9);
    return Choose;
}
int GetQuickWithdrawAmount(short QuickWithdrawOptions)
{
    switch(QuickWithdrawOptions)
    {
        case 1:
            return 20;
        case 2:
            return 50;
        case 3:
            return 100;
        case 4:
            return 200;
        case 5:
            return 400;
        case 6:
            return 600;
        case 7:
            return 800;
        case 8:
            return 1000;
    }
}
void SaveDataToFile(string FileName,vector<stClient>& vClient)
{
    fstream MyFile;
    MyFile.open(FileName,ios::out);
    if (MyFile.is_open())
    {
        string Line;
        for (stClient &C : vClient)
        {
            if (C.MarkForDelete==false)
            {
                Line=ConvertRecordToLine(C);
                MyFile<<Line<<endl;
            }
        }
        MyFile.close();
    }
}
void DepositeAmountByAccounNumber(string AccountNumber,int Amount,vector<stClient>& vClient)
{
    for (stClient& C : vClient)
    {
        if (AccountNumber==C.AccountNumber)
        {
            C.Balance+=Amount;
        }
    }
    SaveDataToFile(FileClientName,vClient);
}
void PerFormQuickWithdrawOptions(short QuickWithdrawOptions)
{
    if (QuickWithdrawOptions==9)
        return;
    short WithdrawAmount=GetQuickWithdrawAmount(QuickWithdrawOptions);
    if (WithdrawAmount>CurrentClient.Balance)
    {
        system("cls");
        cout<<"\n The amount exceeds your balance, make another choice.\n";
        ShowQuickWithdrawScreen();
        return;
    }
    vector<stClient> vClient=LoadDataFromFile(FileClientName);
    DepositeAmountByAccounNumber(CurrentClient.AccountNumber,WithdrawAmount*-1,vClient);
    CurrentClient.Balance-=WithdrawAmount;
    cout<<"\n Quick Withdraw Success\n";
    cout<<"\n\n Your Balance is: "<<CurrentClient.Balance<<endl;
}
void ShowQuickWithdrawScreen()
{
    cout<<"\n===================================\n";
    cout<<"\t\t Quick Withdraw.";
    cout<<"\n===================================\n";
    cout<<"\n [1] 20    ";
    cout<<" [2] 50     ";
    cout<<"\n [3] 100   ";
    cout<<" [4] 200    ";
    cout<<"\n [5] 400  ";
    cout<<" [6] 600    ";
    cout<<"\n [7] 800  ";
    cout<<" [8] 1000";
    cout<<"\n [9] Exit";
    cout<<"\n===================================\n";
    cout<<"\n\n Your Balance is: "<<CurrentClient.Balance<<endl;
    PerFormQuickWithdrawOptions(ReadQuickWithdraw());
}
void NormalWithdrawByAccountNumber(string AccountNumber,int Amount,vector<stClient> vClient)
{
    if (CurrentClient.Balance>Amount)
    {
        char Answer='n';
        cout<<"\n Are you sure you want perform this Transaction? (y/n)? ";
        cin>>Answer;
        if (toupper(Answer)=='Y')
        {
            DepositeAmountByAccounNumber(CurrentClient.AccountNumber,Amount*-1,vClient);
            CurrentClient.Balance-=Amount;
            cout<<"\nDone Withdrw sucess..., New Balance is: "<<CurrentClient.Balance<<endl;
        }
    }
    else
    {
        cout<<"\n The amount exceeds your balance, make another choice.\n";
        system("pause");
        system("cls");
        ShowNormalWithdrawScreen();
    }
}
void ShowNormalWithdrawScreen()
{
    cout<<"\n==========================================\n";
    cout<<"\t\t Normal Withdraw Screen.";
    cout<<"\n==========================================\n";
    int Amount=0;
    do
    {
        cout<<"\n Enter an amount multiple 5's ? ";
        cin>>Amount;
    }while(Amount%5!=0);
    vector<stClient> vClient=LoadDataFromFile(FileClientName);
    NormalWithdrawByAccountNumber(CurrentClient.AccountNumber,Amount,vClient);
}
void ShowCheckBalanceScreen()
{
    cout<<"\n==========================================\n";
    cout<<"\t\t Check Balance Screen.";
    cout<<"\n==========================================\n";
    cout<<"Your Balance is "<<CurrentClient.Balance<<endl;
}
void ShowDepositScreen()
{
    cout<<"\n==========================================\n";
    cout<<"\t\t Deposit Screen.";
    cout<<"\n==========================================\n";
    int Amount=0;
    do
    {
        cout<<"Enter a positive Deposit amount? ";
        cin>>Amount;
    }while(Amount<=0);
    vector<stClient> vClient=LoadDataFromFile(FileClientName);
    DepositeAmountByAccounNumber(CurrentClient.AccountNumber,Amount,vClient);
    CurrentClient.Balance+=Amount;
    cout<<"\nDone Deposit sucess..., New Balance is: "<<CurrentClient.Balance<<endl;
}
void PerformATMMainMenueOptions(enATMMainMenueOptions ATMMainMenueOptions)
{
    switch(ATMMainMenueOptions)
    {
        case enATMMainMenueOptions::AQuickWithdraw:
        {
            system("cls");
            ShowQuickWithdrawScreen();
            GoBackToMainMenue();
            break;
        }
        case enATMMainMenueOptions::aNormalWithdraw:
        {
            system("cls");
            ShowNormalWithdrawScreen();
            GoBackToMainMenue();
            break;
        }
        case enATMMainMenueOptions::aDeposite:
        {
            system("cls");
            ShowDepositScreen();
            GoBackToMainMenue();
            break;
        }
        case enATMMainMenueOptions::aCheckBalance:
        {
            system("cls");
            ShowCheckBalanceScreen();
            GoBackToMainMenue();
            break;
        }
        case enATMMainMenueOptions::aLogout:
        {
            system("cls");
            Login();
        }
    }
}
void ShowMainMenueScreen()
{
    cout << "\n =======================================================================\n";
    cout << "\t\t\t\t ATM Main Menue Screen";
    cout << "\n =======================================================================\n";
    cout << "\t\t [1] Quick Withdraw." << endl;
    cout << "\t\t [2] Normal Withdraw." << endl;
    cout << "\t\t [3] Deposit." << endl;
    cout << "\t\t [4] Chaek Balance." << endl;
    cout << "\t\t [5] Logout." << endl;
    cout << " =======================================================================\n";
    PerformATMMainMenueOptions(ReadATMMainMenueOptions());
}
void Login()
{
    bool Fail = false;
    do
    {
        cout << "\n _____________________________________________________________" << endl;
        cout << "\t\t\t Login Screen";
        cout << "\n _____________________________________________________________" << endl;
        if (Fail)
        {
            cout << "Invalid Account Number/PinCode" << endl;
        }
        string AccountNumber = ReadAccountNumber();
        int PinCode = ReadPinCode();
        Fail = !(IsCheakClientByAccountNumberAndPinCode(AccountNumber, PinCode));
        system("cls");
    } while (Fail);
    system("cls");
    ShowMainMenueScreen();
}
int main()
{
    cout << "Bsm allah ahrahman alrahem" << endl;

    Login();
    system("pause");
    return 0;
}
/////////////////////////////