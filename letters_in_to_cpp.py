# -*- coding: utf-8 -*
import sys

f = open ('utf8_letters.in', 'r')
out = open('utf8_letters.out', 'w')

"""h_file = open('utf8_letters.h', 'w')
cpp_file = open('utf8_letters.cpp', 'w')

h_file.write("#ifndef _UTF8_LETTERS_H_\n\
#define _UTF8_LETTERS_H_\n\
\n\
#include <tr1/unordered_map>\n\
\n\
\n\
class Utf8LettersIniter\n\
{\n\
public:\n\
	Utf8LettersIniter();\n\
};\n\
\n\
class Utf8Letters\n\
{\n\
	static std::tr1::unordered_map<wchar_t, wchar_t> m_letter_map;\n\
	static Utf8LettersIniter initer;\n\
public:\n\
	static void init();\n\
	static wchar_t lower(wchar_t c);\n\
\n\
};\n\
#endif\n")

cpp_file.write('#include "utf8_letters.h"\n\
\n\
Utf8LettersIniter Utf8Letters::initer;\n\
\n\
Utf8LettersIniter::Utf8LettersIniter()\n\
{\n\
	Utf8Letters::init();\n\
}\n\
\n\
wchar_t Utf8Letters::lower(wchar_t c)\n\
{\n\
	std::tr1::unordered_map<wchar_t, wchar_t>::iterator it = m_letter_map.find(c);\n\
	if (it != m_letter_map.end())\n\
		return it->second;\n\
	return c;\n\
}\n\
\n\
void Utf8Letters::init()\n\
{\n')"""

for line in f:
	lems = line.split('<>')
	cap = lems[0][0:1]
	low = lems[1][1:2]
	#print '|'+cap+'|'+low+'|'
#	cpp_file.write('	m_letter_map[\'' + cap + '\'] = \'' + low + '\';\n')
	out.write(cap+' '+low+'\n')
#cpp_file.write('}\n');
