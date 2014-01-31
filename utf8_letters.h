#ifndef _UTF8_LETTERS_H_
#define _UTF8_LETTERS_H_

#include <tr1/unordered_map>
#include <iostream>
#include <fstream>

class Utf8LettersIniter
{
public:
	Utf8LettersIniter();
};

class Utf8Letters
{
	static std::tr1::unordered_map<wchar_t, wchar_t> m_letter_map;
	static Utf8LettersIniter initer;
public:
	static void init();
	static wchar_t lower(wchar_t c);

};
#endif
