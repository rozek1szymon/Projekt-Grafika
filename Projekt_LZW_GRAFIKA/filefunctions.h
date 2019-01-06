#pragma once
//#include "ExclusivePicture.h"
#include <fstream>
#include <cstring>
#include <string>
#include <commdlg.h>
#include "Shlobj.h"
#include <tchar.h>
#include <algorithm>
#include <vector>
#include <Windows.h>

void SaveExp();
void LoadExp(string path);
//void To7BitColor();
void SaveBmp();
void LoadFile();
string  GetFileName(const string & prompt);
string GetFolderName();
