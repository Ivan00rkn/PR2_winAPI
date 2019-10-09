#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>

using namespace std;

void ShowError()
{
	LPTSTR errorText = NULL;
	DWORD errCode = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, 
		errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,
		0,
		NULL);

	if (NULL != errorText)
	{
		MessageBox(NULL, errorText, NULL, 0);

		LocalFree(errorText);
		errorText = NULL;
	}
}

int main()
{	
	setlocale(LC_ALL, "");

	WIN32_FIND_DATA MyFile;
	HANDLE hFind = FindFirstFile(_T("C:\\pr2\\*"), &MyFile);

	TCHAR fileName[4095];

	bool correct = true;
	bool next;
	
	int counter;

	if (INVALID_HANDLE_VALUE != hFind) // find file
	{
		do
		{
			_tcscpy(fileName, MyFile.cFileName);

			if (((MyFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) && ((MyFile.dwFileAttributes & FILE_ATTRIBUTE_DEVICE) == 0)) // is regular file
			{
				cout << fileName << endl;

				int counter = 0;
				TCHAR simbols[50] = { 'q','Q','w','W','r','R','t','T','p','P','s','S','d','D','f','F','g','G','h','H','j','J','k','K','l','L','z','Z','x','X','c','C','v','V','b','B','n','N','m','M', };
				TCHAR way[4095] = TEXT("C:\\pr2\\");

				if (!_tcscat(way, MyFile.cFileName))
				{
					correct = false;
					ShowError();
				}

				

				for (int i = 0; (i < _tcslen(fileName) && fileName[i] != '.'); i++) {
					TCHAR letter = fileName[i];
					for (int j = 0; j < _tcslen(simbols); j++) {
						if (letter == simbols[j]) {
							counter++;
						}
					}
				}

				cout << counter;
				if (counter == 2) {
					printf("%s\n", fileName);
					if (DeleteFile(way) != 0) // удаление
					{
						cout << "Deleting Sucess!"<< endl; // вывод сообщения успешном удалении
					}
					else
					{
						ShowError();
					}
				}

				counter = 0;
			}
			
			next = FindNextFile(hFind, &MyFile);

			if (!GetLastError() == ERROR_NO_MORE_FILES)
			{
				correct = false;
				ShowError();
			}
		} while (NULL != next && correct);
	}
	FindClose(hFind);
}
