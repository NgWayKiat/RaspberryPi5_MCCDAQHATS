#ifndef MIC_HDR_FILE_
#define MIC_HDR_FILE_

#include <string>
#include <ctime>
#include <cstring>
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

string currentDateTimeInString();

string currentDateLogFileNameInString();

bool is_int(const string& s);

vector<string> splitString(const string& s, char delimiter);

vector<string> splitStringByDelimiter(const string& s, const string& delimiter);

#endif