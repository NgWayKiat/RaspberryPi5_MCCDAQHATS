#include "Misc.h"

//Get Current Date Time in String Format "yyyy-mm-dd hh:mm:ss"
string currentDateTimeInString()
{
    char buf[80] = {0};
    time_t now = time(0);

    struct tm tmStruct = *localtime(&now);
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tmStruct);

    return buf;
}

string currentDateLogFileNameInString()
{
    char buf[80] = {0};
    time_t now = time(0);

    struct tm tmStruct = *localtime(&now);
    memset(buf, 0, sizeof(buf));
    strftime(buf, sizeof(buf), "%Y%m%d", &tmStruct);

    return buf;
}

bool is_int(const string& s)
{
    return( strspn( s.c_str(), "-.0123456789" ) == s.size() );
}

vector<string> splitString(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);

    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

vector<string> splitStringByDelimiter(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }
    tokens.push_back(s.substr(start));

    return tokens;
}