#pragma once

#include <vector>
#include <map>
using namespace std;

string             string_format(const string fmt_str, ...) ;
vector<string>     string_split(string str, char delim);
string             string_tokenize(string str, string delimiter,size_t &iStartPos);
map<string,string> string_uri_split(string str);
bool               string_is_regex_match(string str, string exp);
vector<string>     string_regex_match(string str, string exp);
string             string_left(string str, int n);
string             string_right(string str, int n);
string             string_replace(string str, string search, string replace);
int                string_search_count(string str, string search);
void               string_trim(string &str);
string             string_padding (string Str, char cPad, int nMaxStrNo, bool bFront);
bool               string_is_numeric (string Str);
