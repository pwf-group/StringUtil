#include "stdafx.h"
#include "StringUtil.h"

#include <stdio.h>
#include <stdarg.h>  // for va_start, etc
#include <memory>    // for unique_ptr
#include <sstream>
#include <regex>

string string_format(const string fmt_str, ...) 
{
    int final_n, n = ((int)fmt_str.size()) * 2; /* reserve 2 times as much as the length of the fmt_str */
    string str;
    unique_ptr<char[]> formatted;
    va_list ap;
    while(1) {
        formatted.reset(new char[n]); /* wrap the plain char array into the unique_ptr */
        strcpy(&formatted[0], fmt_str.c_str());
        va_start(ap, fmt_str);
        final_n = vsnprintf(&formatted[0], n, fmt_str.c_str(), ap);
        va_end(ap);
        if (final_n < 0 || final_n >= n)
            n += abs(final_n - n + 1);
        else
            break;
    }
    return string(formatted.get());
}
vector<string> string_split(string str, char delim)
{
	vector<string> elems;

    stringstream ss(str);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }

    return elems;
}
map<string,string> string_uri_split(string str)
{
	vector<string> res_split = string_split(str, '&');
	map<string, string> input_param;
	for(vector<string>::iterator it = res_split.begin(); it != res_split.end(); it++)
	{
		vector<string> result = string_split(*it, '=');
		input_param.insert(map<string,string>::value_type(result[0], result[1]));
	}
	return input_param;
}
string string_tokenize(string str, string delimiter,size_t &iStartPos)
{
    string ret;
    size_t iFoundPos;
    ret = "";
    if(iStartPos == -1)
        return ret;

    iFoundPos = str.find(delimiter.c_str(),iStartPos);
    if(iFoundPos != -1)
    {
        ret = str.substr(iStartPos,(iFoundPos - iStartPos));
        iStartPos = iFoundPos + 1;
    }
    else
    {
        //return whole string started from the position
        ret = str.substr(iStartPos,str.length()-iStartPos);
        iStartPos = -1;
    }

    if (iStartPos > str.length())
        iStartPos = -1;
    
	string_trim(ret);

    return  ret;
}
bool string_is_regex_match(string str, string exp)
{
	regex e(exp);
	if( regex_match(str, e) )
		return true;

	return false;
}
vector<string> string_regex_match(string str, string exp)
{
	vector<string> result;
	smatch sm;
	regex e(exp);
  
	regex_match (str,sm,e);
	for (unsigned i=0; i<sm.size(); ++i)
		result.push_back(sm[i]);

	return result;
}
string string_left(string str, int n)
{
	return str.substr(0, n);
}
string string_right(string str, int n)
{
	int start_pos = str.length()-n;
	if( start_pos < 0 )
		start_pos = 0;
	return str.substr(str.length()-n, n);
}
string string_replace(string str, string search, string replace)
{
	string ret = str;
	size_t pos = 0;

	pos = ret.find(search, pos);
	while( pos != string::npos )
	{
		ret.replace(pos,search.length(),replace);
		pos += replace.length();
		pos = ret.find(search, pos);
	}

	return ret;
}
int string_search_count(string str, string search)
{
	string ret = str;
	size_t pos = 0;
	int    count = 0;

	pos = ret.find(search, pos);
	while( pos != string::npos )
	{
		count++;
		pos++;
		pos = ret.find(search, pos);
	}

	return count;
}
void string_trim(string &str)//remove whitespace and newline
{
    size_t first = str.find_first_not_of(' ');
	string ret;

    // If there is no non-whitespace character, both first and last will be string::npos (-1)
    if(first != string::npos)
	{
		size_t last  = str.find_last_not_of(' ');
		ret = str.substr(first, last-first+1);
	}

    string strfind = "\n";
    while(ret.find_last_of(strfind) != string::npos)
        ret.replace(ret.find_last_of(strfind),strfind.length(),"");

	str= ret;
    return;
}
string string_padding (string str, char cPad, int nMaxLength, bool bFront)
{
	string ret;

	int nStrLength = str.size();
	if (nStrLength >= nMaxLength)
		return str; //do nothing
	
	if( bFront)
	{
		for (int i = 0; i < nMaxLength - nStrLength; i++)
			ret.push_back(cPad);

		ret += str;
	}
	else
	{
		ret += str;
		for (int i = 0; i < nMaxLength - nStrLength; i++)
		{
			ret.push_back(cPad);
		}
	}

	return ret;
}
bool string_is_numeric ( string str )
{
	char    *p1;
	short	StrLen, i;
	int		SpaceIndex, SignIndex, DecimalIndex, StartDigitIndex;

	SpaceIndex = SignIndex = DecimalIndex = StartDigitIndex = 0;
	p1 = new char[str.size()+1];
	sprintf(p1,"%s",str.c_str());

	StrLen = strlen( p1 );
	for(i=0; i < StrLen; i++ )
	{
		if( *p1 != 32 && *p1 != 46 && *p1 != 43 && *p1 != 45 )     // space . + - excepted
		{
			if( *p1 < 48 || *p1 > 57 )
				return false;
			else
				StartDigitIndex = 1;
		}
		else
		{
			// cannot have space after digits detected
			if( *p1 == 32 && StartDigitIndex == 0 )
				SpaceIndex = 1;
			else
				return false;

			StartDigitIndex = 1;  // any other designated digits

			// can have only one decimal place
			if( *p1 == 46 && DecimalIndex == 0 )
				DecimalIndex = 1;
			else
				return false;

			// can have only one plus or minus sign
			if( (*p1 == 43 || *p1 == 45 ) && SignIndex == 0 )
				SignIndex = 1;
			else
				return false;

		}
	
		p1++;
	}


	return true;
}
