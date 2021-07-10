// JSON Engine.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>



// TODO: Reference additional headers your program requires here.

bool isNumber(const std::string& str);
int ActiveFilters();
bool AddFilter(char* value);
//bool MatchFilter(std::string op, char*, std::string);
bool Reset();
int GetFilters(char* path);
bool LoadJSON(char* path);
int Start();
