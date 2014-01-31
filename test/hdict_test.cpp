/*
 * File:   hdict_test.cpp
 * Author: phrk
 *
 * Created on January 29, 2014, 1:23 PM
 */

#include <cstdlib>


#include <iostream>
#include <fstream>

#include "LangDetect.h"
#include "Stemmer.h"

#include <algorithm>
#include <locale>

#include <boost/locale.hpp>

#include "utf8_letters.h"

#include "../htdba/htConnPool.h"
#include "../htdba/htQuerier.h"

#include "Dict.h"

using namespace std;

/*
 * 
 */



/////////////////////////////////////////////////////////
// NativeToUtf16 - Convert a string from the native 
//                 encoding to Unicode UTF-16
// Parameters:
//   sNative (in): Input String
// Returns:        Converted string
/////////////////////////////////////////////////////////
std::wstring NativeToUtf16(const std::string &sNative)
{
  std::locale locNative;

  // The UTF-16 will never be longer than the input string
  std::vector<wchar_t> vUtf16(1+sNative.length());

  // convert
  std::use_facet< std::ctype<wchar_t> >(locNative).widen(
        sNative.c_str(), 
        sNative.c_str()+sNative.length(), 
        &vUtf16[0]);

  return std::wstring(vUtf16.begin(), vUtf16.end());
}

/////////////////////////////////////////////////////////
// Utf16ToUtf8 -   Convert a character from UTF-16 
//                 encoding to UTF-8.
//                 NB: Does not handle Surrogate pairs.
//                     Does not test for badly formed 
//                     UTF-16
// Parameters:
//   chUtf16 (in): Input char
// Returns:        UTF-8 version as a string
/////////////////////////////////////////////////////////
std::string Utf16ToUtf8(wchar_t chUtf16)
{
    // From RFC 3629
    // 0000 0000-0000 007F   0xxxxxxx
    // 0000 0080-0000 07FF   110xxxxx 10xxxxxx
    // 0000 0800-0000 FFFF   1110xxxx 10xxxxxx 10xxxxxx

    // max output length is 3 bytes (plus one for Nul)
    unsigned char szUtf8[4] = "";

    if (chUtf16 < 0x80)
    {
        szUtf8[0] = static_cast<unsigned char>(chUtf16);
    }
    else if (chUtf16 < 0x7FF)
    {
        szUtf8[0] = static_cast<unsigned char>(0xC0 | ((chUtf16>>6)&0x1F));
        szUtf8[1] = static_cast<unsigned char>(0x80 | (chUtf16&0x3F));
    }
    else
    {
        szUtf8[0] = static_cast<unsigned char>(0xE0 | ((chUtf16>>12)&0xF));
        szUtf8[1] = static_cast<unsigned char>(0x80 | ((chUtf16>>6)&0x3F));
        szUtf8[2] = static_cast<unsigned char>(0x80 | (chUtf16&0x3F));
    }

    return reinterpret_cast<char *>(szUtf8);
}

/////////////////////////////////////////////////////////
// Utf16ToUtf8 -   Convert a string from UTF-16 encoding
//                 to UTF-8
// Parameters:
//   sNative (in): Input String
// Returns:        Converted string
/////////////////////////////////////////////////////////
std::string Utf16ToUtf8(const std::wstring &sUtf16)
{
    std::string sUtf8;
    std::wstring::const_iterator itr;

    for (itr=sUtf16.begin(); itr!=sUtf16.end(); ++itr)
        sUtf8 += Utf16ToUtf8(*itr);
    return sUtf8;
}

int main(int argc, char** argv) {
	
/*	
	htConnPoolPtr conn_pool( new htConnPool("localhost", 38080, 1));
	Dict en_dict(conn_pool, "highinit_q", "words", "stems", LangDetect::EN);
	std::string word;
	
	word = "Nationalism";
	en_dict.getStemId(word);
	en_dict.getWordId(word);
	word = "Hitler";
	en_dict.getStemId(word);
	en_dict.getWordId(word);
	word = "Калатушкина";
	en_dict.getStemId(word);
	en_dict.getWordId(word);
	*/
	//std::cout << "word: " << word << std::endl;
	//std::cout << "id: " << id << std::endl;
	return 0;
	/*
	
	setlocale(LC_ALL, "");
	//std::locale::global(std::locale("ru_RU.UTF-8"));
	std::cout.imbue(std::locale(""));
	std::string word;
	//std::setlocale(LC_ALL, "ru_RU.UTF-8");
	
	word = "Салат из Кальмаров с плавленым сыром";
	//word = "QWERTYUIOPASDFGHJKLZXCVBNM";
	
	htConnPoolPtr conn_pool(new htConnPool("localhost", 38080, 1));
	
	htQuerier q(conn_pool, "highinit_q", "pages");
	
	ifstream in ("utf_string.in");
	std::ofstream out("output.txt");
	
	in >> word;
	
	
	std::string text;
	q.getColl("http://www.gamedev.ru/", "text", text);
	
	//std::cout << "Got " << text << std::endl;
	//word = text;
	
	
	

	//std::wstring sUtf16 = NativeToUtf16(word);
	//word = Utf16ToUtf8(sUtf16);
	
	//std::string word = "fuck this shit i am going home";
	
	//word = "Nationalism";
	
	LangDetect::Lang lang = LangDetect::getLang(word);
	
	if (lang == LangDetect::EN) {
		std::cout << "EN\n"; 
	} else if (lang == LangDetect::RU) {
		std::cout << "RU\n"; 
	} else if (lang == LangDetect::UNKNOWN) {
		std::cout << "UNKNOWN\n"; 
		return 0;
	}
	
	//return 0;
	
	//std::cout << word << std::endl;
	//out << "orig: " << word << std::endl;
	utf8_tolower(word);
	std::cout << "word: " <<  word << std::endl;
	out << "word: " << word << std::endl;
	
	Stemmer stemmer(lang);
	std::string stem;
	stemmer.stem(word, stem);
	std::cout << "stem: " << stem << std::endl;
	
	
	
	out << "stem: " << stem << std::endl;
	out.close();
	
	return 0;*/
}
