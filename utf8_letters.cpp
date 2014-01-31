#include "utf8_letters.h"

std::tr1::unordered_map<wchar_t, wchar_t> Utf8Letters::m_letter_map;
Utf8LettersIniter Utf8Letters::initer;


Utf8LettersIniter::Utf8LettersIniter()
{
	Utf8Letters::init();
}

wchar_t Utf8Letters::lower(wchar_t c)
{
	/*
	if (c >= 0x90 && c<= 0xAF) {
		std::cout << "LOWING! " << std::hex << c << " \n";
		//return c;
		return c + 0xAF - 0x90 + 1;
		//return c+33;
	}
	
	if (c >= L'A' && c <= L'Z')
		return c+32;*/
	
	std::tr1::unordered_map<wchar_t, wchar_t>::iterator it = m_letter_map.find(c);
	if (it != m_letter_map.end())
		return it->second;
	
	return c;
}

void Utf8Letters::init()
{
	return;
	std::wifstream in("utf8_letters.in");

	std::wstring buf;
	for (int i = 0; i<7; i++)
		in >> buf;
	
//	int i = 0;
	while (!in.eof()) {
		wchar_t cap, low;
		
		//in >> cap;
		//in >> low;
		//std::wcout << buf << std::endl;
		
		in >> buf;
		cap = buf[0];
		in >> buf;
		in >> buf;
		low = buf[0];
		//std::wcout << cap << " " << low << std::endl;
		//m_letter_map[L'A'] = L'a';
		m_letter_map.insert(std::pair<wchar_t, wchar_t>(cap, low));
		
		//if(i==10) break;
		//i++;
	}	
	in.close();
}
