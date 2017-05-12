// Sever.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Sever.h"
#include "afxsock.h"
#include "Player.h"
#include <sstream>
#include <time.h>
#include <string.h>
#include <vector>
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
struct FileStruct{
	string KeyWord;
	string Hint;
	int nGuess;
};
int Random(int a, int b)
{

	int number;
	srand(time(NULL)); 
	number = (rand() % (b - a + 1)) + a;
	return number;
}
FileStruct randomKeyWord()
{
	ifstream reader;

	reader.open("KeyWord.txt");
	if (!reader.is_open())
	{
	}
	int n;
	reader >> n;
	FileStruct *List = new FileStruct[n];
	for (int i = 0; i < n; ++i)
	{
		reader >> List[i].KeyWord;
		reader >> List[i].Hint;
		reader >> List[i].nGuess;
	}
	return List[Random(0, n - 1)];
	reader.close();
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
bool checkName(string A)
{
	for (int i = 0; i < A.length() - 1; ++i)
	{
		if (!((30 <= A[i] <= 39) || (65 <= A[i] <= 90) || (97 <= A[i] <= 122)))
		{
			return false;
		}
	}
	return true;
}
bool GameOver(bool *A, string KeyWord)
{
	for (int i = 0; i < KeyWord.length(); ++i)
	{
		if (A[i] == false)
			return false;
	}
	return true;
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
			char StartOver;
			do{
				int CheckEndGame = 0;//kiểm tra xem số chữ đã được đoán trúng
				Data s_data, r_data;//Mảng chứa số chui gửi đi và nhận
				bool GameOverFlag = false;
				int GuessedKey = 0;
				////////////////////////////////////////
				string *ListofNamePlayer;//chưa tên các player
				Player *ListofPlayer;//list player
				/////////////////////////////////////////
				int NumberofPlayer;
				cout << " .d8888.d88888b d8888b.db      dbd88888b d8888b." << endl;
				cout << "88'  YP 88'     88  `8D 88    88 88'     88  `8D " << endl;
				cout << "`8bo.   88ooooo 88oobY' Y8    8P 88ooooo 88oobY'" << endl;
				cout << "  `Y8b. 88~~~~~ 88`8b   `8b  d8' 88~~~~~ 88`8b   " << endl;
				cout << "db   8D 88.     88 `88.  `8bd8'  88.     88 `88." << endl;
				cout << "`8888Y' Y88888P 88   YD    YP    Y88888P 88   YD" << endl;
				cout << endl;
				cout << "Input The Number Of Players :";
				cin >> NumberofPlayer;
				//Quy định số lượng người chơi
				CSocket sever, *client;
				client = new CSocket[NumberofPlayer];
				ListofNamePlayer = new string[NumberofPlayer];
				ListofPlayer = new Player[NumberofPlayer];
				////////////////////////////////////////////////////
				//Random só thứ tự người chơi
				int *ID = new int[NumberofPlayer];
				vector<int> temp;
				for (int i = 0; i < NumberofPlayer; ++i)
				{
					bool Check1;
					do{
					    Check1 = true;
						Sleep(300);
						ID[i] = Random(0, NumberofPlayer - 1);
						//cout << ID[i];
						for (int k = 0; k < temp.size(); ++k)
						{
							if (ID[i] == temp[k])
								Check1 = false;
						}
					} while (Check1 == false);
					
					temp.push_back(ID[i]);
				}
				
				/*for (int i = 0; i < NumberofPlayer; i++)
				{
					cout <<"aaaaaaa" << ID[i] << " ";
				}
				Sleep(5000);*/

				///////////////////////////////////////////////
				FileStruct KeyWord = randomKeyWord();
				//cout << "Key Word: " << KeyWord.KeyWord << endl;
				bool *CheckKey = new bool[KeyWord.KeyWord.length()];
				for (int i = 0; i < KeyWord.KeyWord.length(); ++i)
				{
					CheckKey[i] = false;
					if (KeyWord.KeyWord[i] == ' ')
						CheckKey[i] = true;
				}
				int nGuess = KeyWord.nGuess;
				////////////////////////////////////
				AfxSocketInit(NULL);
				sever.Create(6969);



				//cout << "Waiting for a connection: \n";
				system("cls");
				cout << endl << endl << endl;
				cout << "__________________________________________________________________________________________\n";
				cout << "|      KeyWord:                                             |    Left Turn:              |\n";
				cout << "|________________________________________________________________________________________|\n";
				cout << "| Name          |                Status                                    |    Score    |\n";
				cout << "|________________________________________________________________________________________|\n";
				for (int kk = 0; kk <NumberofPlayer; ++kk)
				{
					cout << "|               |                                                          |             |\n";
					cout << "|               |                                                          |             |\n";
					cout << "|----------------------------------------------------------------------------------------|\n";
				}
				gotoxy(35, 4);
				cout << KeyWord.KeyWord;
				gotoxy(80, 4);
				cout << KeyWord.nGuess;
				//gotoxy(80, 4); left turn //3, 22, 80
				/*for (int k = 0; k < NumberofPlayer; k++)
				{
					gotoxy(80, (k + 1) * 3 + 5);

					cout << "a";
				} */
				for (int i = 0; i < NumberofPlayer; ++i)
				{
					sever.Listen(); //tìm kết nối
					if (sever.Accept(client[i]))
					{

						bool Check = true;
						do{
							Check = true;
							strcpy(s_data.Mess, "Please Input Your Name");
							int temp;
							for (int j = 0; j < NumberofPlayer; j++)
							{
								if (i == ID[j])
									temp = j;
							}
							s_data.ID = temp;
							client[i].Send(&s_data, sizeof(s_data), 0);
							client[i].Receive(&r_data, sizeof(r_data), 0);
							for (int j = 0; j < NumberofPlayer; ++j)
							{
								if (ConvertCharToString(r_data.Mess) == ListofPlayer[j].Name)
									Check = false;
							}
						} while (Check == false);
						ListofPlayer[i].Name = ConvertCharToString(r_data.Mess);
						//cout << "The " << i + 1 << "th Player" << "'s Name: " << ListofPlayer[i].Name << "\n";
					}

				}
				
				////////////////////////////////////////////////////
				//xong nhập tên
				for (int k = 0; k < NumberofPlayer; k++)
				{
				gotoxy(3, (ID[k] + 1) * 3 + 5);

				cout << ListofPlayer[ID[k]].Name;
				} 
				do{

					for (int i = 0; i < NumberofPlayer; i++)
					{
						////////////////////////////////////
						bool GuessRight = false;
						for (int k = 0; k < KeyWord.KeyWord.length(); ++k)
						{
							if (CheckKey[k] == false)
								s_data.Mess[k] = '*';
							else
								s_data.Mess[k] = KeyWord.KeyWord[k];
						}
						s_data.Mess[KeyWord.KeyWord.length()] = '0';
						s_data.Score = 101;
						client[ID[i]].Send(&s_data, sizeof(s_data), 0);
						s_data.Score = 0;
						client[ID[i]].Receive(&r_data, sizeof(r_data), 0);
						////////////////////////////////////////
						int count = 0;
						strcpy(s_data.Mess, "Your Turn To Guess");
						s_data.nGuess = nGuess;
						strcpy(s_data.Hint, ConvertStringToChar(KeyWord.Hint));
						client[ID[i]].Send(&s_data, sizeof(s_data), 0);
						client[ID[i]].Receive(&r_data, sizeof(r_data), 0);
						if (r_data.Score == 0)
						{
							gotoxy(22, (ID[i] + 1) * 3 + 5);
							cout << "                                        ";
							gotoxy(22, (ID[i] + 1) * 3 + 5);
							cout << "Pass This Turn";
							strcpy(s_data.Mess, "Sad!!!");
							client[ID[i]].Send(&s_data, sizeof(s_data), 0);
						}
						if (r_data.Score != 0)
						{

							strcpy(s_data.Mess, "Enter the letter please");
							client[ID[i]].Send(&s_data, sizeof(s_data), 0);

							client[ID[i]].Receive(&r_data, sizeof(r_data), 0);
							--nGuess;
							//cout << "The " << ID[i] + 1 << " th Player's Score This Turn Is " << r_data.Score << " And He Guess " << ConvertCharToString(r_data.GuessKey) << "\n";
							gotoxy(22, (ID[i] + 1) * 3 + 5);
							cout << "                                                       ";
							gotoxy(22, (ID[i] + 1) * 3 + 5);
							cout << "This Turn Score Is " << r_data.Score << " And He Guess " << ConvertCharToString(r_data.GuessKey);
							gotoxy(80, 4);
							cout << "     ";
							gotoxy(80, 4);
							cout<< nGuess;
							
							if (ConvertCharToString(r_data.GuessKey).length() == 1)
							{
								for (int j = 0; j < KeyWord.KeyWord.length(); ++j)
								{
									char *KeyWord1 = ConvertStringToChar(KeyWord.KeyWord);
									if (r_data.GuessKey[0] == KeyWord1[j] && CheckKey[j] == false)
									{
										count++;
										CheckKey[j] = true;
										GuessRight = true;
										++CheckEndGame;
									}

								}
							}
							if (ConvertCharToString(r_data.GuessKey).length() != 1)
							{
								if (ConvertCharToString(r_data.GuessKey) == KeyWord.KeyWord)
								{
									for (int j = 0; j < KeyWord.KeyWord.length(); ++j)
									{
										if (CheckKey[j] == false)
										{
											count++;
											
										}
									}

									CheckEndGame = KeyWord.KeyWord.length();
									count = 3;
								}
							}
						}
						ListofPlayer[ID[i]].Score = ListofPlayer[ID[i]].Score + r_data.Score*count;
						gotoxy(80, (ID[i] + 1) * 3 + 5);
						cout << "         ";
						gotoxy(80, (ID[i] + 1) * 3 + 5);
						cout << ListofPlayer[ID[i]].Score;
						if (count > 0)
						{
							strcpy(s_data.Mess, "Congratulation");
							s_data.Score = r_data.Score*count;
							client[ID[i]].Send(&s_data, sizeof(s_data), 0);
						}
						if (count == 0)
						{
							strcpy(s_data.Mess, "You Miss");
							client[ID[i]].Send(&s_data, sizeof(s_data), 0);
						}

						client[ID[i]].Receive(&r_data, sizeof(r_data), 0);
						if (GuessRight == true)
							i--;
						if (CheckEndGame == KeyWord.KeyWord.length() || nGuess == 0)
						{

							i = NumberofPlayer + 10;
							break;
						}

					}



				} while (!(CheckEndGame == KeyWord.KeyWord.length() || nGuess == 0));
				for (int i = 0; i < NumberofPlayer; ++i)
				{

					strcpy(s_data.Mess, "Game Over");
					client[ID[i]].Send(&s_data, sizeof(s_data), 0);
					client[ID[i]].Receive(&r_data, sizeof(r_data), 0);


				}
				InfoOfAllPlayer List;
				Sleep(4000);
				system("cls");
				int max = 0;
				List.NumberOfPlayer = NumberofPlayer;
				cout << "The Final Result: " << endl;
				for (int i = 0; i < NumberofPlayer; ++i)
				{
					
					List.Name[i] = ListofPlayer[i].Name;
					cout << ListofPlayer[i].Name << ": ";
					List.Score[i] = ListofPlayer[i].Score;
					cout << ListofPlayer[i].Score << endl;
					if (max < ListofPlayer[i].Score)
						max = ListofPlayer[i].Score;
				}
				cout << endl;

				for (int i = 0; i < NumberofPlayer; ++i)
				{
					client[i].Send(&List, sizeof(List), 0);
					client[i].Receive(&List, sizeof(List), 0);
				}
				for (int j = 0; j < NumberofPlayer; j++)
				{
					if (max == ListofPlayer[j].Score)
						cout << ListofPlayer[j].Name << " WON THE GAME" << endl;
				}
				sever.Close();
				for (int i = 0; i < NumberofPlayer; ++i)
				{
					client[i].Close();
				}
				do{
					cout << "Do You Want To Start Over?(y,n) \n";
					cin >> StartOver;
				} while (!(StartOver == 'y' || StartOver == 'n'));
			}while (StartOver == 'y');
			system("cls");
			
			
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
