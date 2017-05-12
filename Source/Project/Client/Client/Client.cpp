// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Client.h"
#include <afxsock.h>
#include <iostream>
#include <string>
#include <time.h>
#include <Windows.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;
struct InfoOfAllPlayer{
	string Name[100];
	int Score[100];
	int NumberOfPlayer;
};
struct Data{
	char Mess[100];
	char GuessKey[50];
	int Score;
	char Hint[200];
	int nGuess;
	int ID;
};
bool checkName(string A)
{
	for (int i = 0; i < A.length() - 1; ++i)
	{
		if (!(('A' <= A[i] <= 'Z') || ('a' <= A[i] <= 'z') || ('0' <= A[i] <= '9')))
		{
			return false;
		}
	}
	return true;
}
string ConvertCharToString(char a[])
{
	string temp(a);
	return temp;
}
char* ConvertStringToChar(string a)
{
	char* A = new char[a.length()];
	strcpy(A, a.c_str());
	return A;

}
int Random(int a, int b)
{
	int number;
	srand(time(NULL));
	number = (rand() % (b - a + 1)) + a;
	return number;
}
void gotoxy(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			cout << "_______________________________________________________________________________________________\n";
			cout << "|                                                                       |                     |\n";
			cout << "|                                                                       |                     |\n";
			cout << "_______________________________________________________________________________________________\n";
			cout << "|                                                                       |       Left Turn     |\n";
			cout << "|                                                                       |                     |\n";
			cout << "|                                                                       |                     |\n";
			cout << "|                                                                       _______________________\n";
			cout << "|                                                                       |Your Score This Turn |\n";
			cout << "|                                                                       |                     |\n";
			cout << "|                                                                       |                     |\n";
			cout << "_______________________________________________________________________________________________\n";
			cout << "|                                                                       |      Total Score    |\n";
			cout << "|                                                                       |                     |\n";
			cout << "|                                                                       |           0         |\n";
			cout << "|                                                                       |                     |\n";
			cout << "_______________________________________________________________________________________________\n";
			int score, ID, Total = 0;
			Data r_data, s_data;
			bool GameOver = false;
			CSocket client;
			AfxSocketInit(NULL);
			client.Create();
			string Name, GuessKey;		
			
			if (client.Connect(_T("127.0.0.1"), 6969))
			{
				do {
					
					
					client.Receive(&r_data, sizeof(r_data), 0);
					char *temp;
					
					if (ConvertCharToString(r_data.Mess) == "Please Input Your Name")
					{
						do{
							gotoxy(6, 13);
							cout << r_data.Mess << ": ";
							cin >> Name;
							temp = ConvertStringToChar(Name);
						} while (checkName(Name) != true);
						strcpy(s_data.Mess, temp);
						ID = r_data.ID;
						client.Send(&s_data, sizeof(s_data), 0);
						gotoxy(6, 13);
						cout << "                                                         ";
						gotoxy(6, 13);
						gotoxy(75, 2);
						cout <<"Name: "<< Name;
						gotoxy(35, 2);
						cout << "The Player: " << ID+1;
					}
					if (ConvertCharToString(r_data.Mess) == "Your Turn To Guess")
					{

						score = Random(0, 100);
						gotoxy(83, 10);
						cout <<score;
						gotoxy(3, 5);
						cout << ">> Hint: " << ConvertCharToString(r_data.Hint);
						s_data.Score = score;
						client.Send(&s_data, sizeof(s_data), 0);
					}
					if (ConvertCharToString(r_data.Mess) == "Sad!!!")
					{
						gotoxy(10, 13);
						cout << "                                                                ";
						gotoxy(10, 13);
						cout << ConvertCharToString(r_data.Mess) << "\n";
						GuessKey = "0";
						strcpy(s_data.GuessKey, ConvertStringToChar(GuessKey));
						client.Send(&s_data, sizeof(s_data), 0);
					}
					if (ConvertCharToString(r_data.Mess) == "Enter the letter please")
					{
						gotoxy(83, 6);
						cout << "       ";
					    gotoxy(83, 6);
						cout <<r_data.nGuess;
						gotoxy(10, 13);
						cout << "                                               ";
						gotoxy(10, 13);
						cout<< ConvertCharToString(r_data.Mess)<<" ";
						cin >> GuessKey;
						s_data.Score = score;
						strcpy(s_data.GuessKey, ConvertStringToChar(GuessKey));
						client.Send(&s_data, sizeof(s_data), 0);
					}
					if (ConvertCharToString(r_data.Mess) == "Congratulation")
					{
						gotoxy(10, 15);
						cout << "                                                      ";
						gotoxy(10, 15);
						cout <<ConvertCharToString(r_data.Mess) << ", You Got " << r_data.Score << " point";
						Total = Total + r_data.Score;
						gotoxy(83, 14);
						cout << Total;
						strcpy(s_data.GuessKey, "00");
						client.Send(&s_data, sizeof(s_data), 0);

					}
					if (ConvertCharToString(r_data.Mess) == "You Miss")
					{
						gotoxy(10, 15);
						cout << "                                                       ";
						gotoxy(10, 15);
						cout << "You Guess Wrong";
						strcpy(s_data.GuessKey, "00");
						client.Send(&s_data, sizeof(s_data), 0);

					}
					if (ConvertCharToString(r_data.Mess) == "Game Over")
					{
						Sleep(5000);
						system("cls");
						/////////////////////////////////////////
						cout << "   d888b   .d8b.  .88b  d88. d88888b    .d88b.  db    db d88888b d8888b."<<endl;
						cout << " 88'  Y8b d8' `8b 88'YbdP`88 88'       .8P  Y8. 88    88 88'     88  `8D " << endl;
						cout << " 88       88ooo88 88  88  88 88ooooo   88    88 Y8    8P 88ooooo 88oobY' " << endl;
						cout << " 88   ooo 88~~~88 88  88  88 88~~~~~   88    88 `8b  d8' 88~~~~~ 88`8b   " << endl;
						cout << " 88.  ~8~ 88   88 88  88  88 88.       `8b  d8'  `8bd8'  88.     88 `88." << endl;
						cout << "   Y888P  YP   YP YP  YP  YP Y88888P    `Y88P'     YP    Y88888P 88   YD" << endl;
						//////////////////////////////////////////
						strcpy(s_data.GuessKey, "00");
						client.Send(&s_data, sizeof(s_data), 0);
						GameOver = true;

					}
					if (r_data.Score==101)
					{
						int k = 0;
						gotoxy(30, 7);
						cout << "                                 ";
						gotoxy(30, 7);
						cout << "Key Word: ";
						while (r_data.Mess[k] != '0')
						{
							cout << r_data.Mess[k];
							++k;
						}
						
						strcpy(s_data.GuessKey, "00");
						client.Send(&s_data, sizeof(s_data), 0);

					}
					


				} while (GameOver == false);
			}
			InfoOfAllPlayer List;
			client.Receive(&List, sizeof(List), 0);
			client.Send(&List, sizeof(List), 0);
			int max = 0;
			cout << "The Final Score Of All Player: " << endl;
			for (int i = 0; i < List.NumberOfPlayer; ++i)
			{
				cout << List.Name[i] <<": "<<List.Score[i]<< endl;
				if (max < List.Score[i])
					max = List.Score[i];
			}
			for (int i = 0; i < List.NumberOfPlayer; ++i)
			{
				if (max == List.Score[i])
					cout<< List.Name[i] << " Won!!!" << endl;
			}
			gotoxy(0, 18);
			client.Close();
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
