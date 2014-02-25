/*
 * File:   hdict_test.cpp
 * Author: phrk
 *
 * Created on January 29, 2014, 1:23 PM
 */

#include <cstdlib>


#include <iostream>
#include <fstream>

#include "lang/offline/LangDetect.h"
#include "lang/offline/Stemmer.h"

#include <algorithm>
#include <locale>

#include <boost/locale.hpp>

#include "htdba/htConnPool.h"
#include "htdba/htQuerier.h"

#include "lang/online/Dict.h"
#include "lang/offline/LangDetect.h"
#include "lang/offline/DictOffline.h"
#include "lang/offline/DictOffline.h"

using namespace std;

uint64_t nid ;

uint64_t genStringId()
{
	return ++nid;
}

void testStupid()
{
	nid = 0;
	std::cout << "testStupid\n";
	DictOfflinePtr en_dict (
		new DictOffline(LangDetect::EN, boost::bind(&genStringId) ) );
	
	{
		std::vector< std::pair<uint64_t, std::string> > words;

		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("adolf"), "adolf") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("hitler"), "hitler") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("main"), "main") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("pool"), "pool") );

		for (int i = 0; i<words.size(); i++) {
			uint64_t id = en_dict->getWordId(words[i].second);
			//std::cout << "ID: " << id << std::endl;
			if (id != words[i].first) 
				std::cout << "TEST FAILED: word:" << words[i].second
						<< " id: " << words[i].first << std::endl;
		}
	}
	{
		std::vector< std::pair<uint64_t, std::string> > words;

		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("adolf"), "adolf") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("hitler"), "hitler") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("main"), "main") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("pool"), "pool") );

		for (int i = 0; i<words.size(); i++) {
			uint64_t id = en_dict->getStemId(words[i].second);
			//std::cout << "ID: " << id << std::endl;
			if (id != words[i].first)
				std::cout << "TEST FAILED: stem:" << words[i].second
						<< " id: " << words[i].first << std::endl;
		}
	
	}
}

void testDumping()
{
	nid = 0;
	std::cout << "testDumping\n";
	DictOfflinePtr en_dict (
		new DictOffline(LangDetect::EN, boost::bind(&genStringId) ) );
	
	std::vector< std::pair<uint64_t, std::string> > words;
	std::vector< std::pair<uint64_t, std::string> > stems;
	
	{
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("adolf"), "adolf") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("hitler"), "hitler") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("main"), "main") );
		words.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setWordId("pool"), "pool") );

	}
	{
		stems.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("adolf"), "adolf") );
		stems.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("hitler"), "hitler") );
		stems.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("main"), "main") );
		stems.push_back(std::pair<uint64_t, std::string> 
					( en_dict->setStemId("pool"), "pool") );	
	}
	
	en_dict->dumpToFile("w_dump", "s_dump");
	
	DictOfflinePtr en_dict_restored (
		new DictOffline(LangDetect::EN, boost::bind(&genStringId) ) );
	en_dict_restored->loadFromFile("w_dump", "s_dump");
	
	nid = en_dict_restored->size();
	
	for (int i = 0; i<words.size(); i++) {
		uint64_t id = en_dict_restored->getWordId(words[i].second);
		std::cout << "ID: " << id << " word: " << words[i].second << std::endl;
		if (id != words[i].first) 
			std::cout << "TEST FAILED: word:" << words[i].second
					<< " id: " << words[i].first << std::endl;
	}
	
	for (int i = 0; i<stems.size(); i++) {
		uint64_t id = en_dict_restored->getStemId(stems[i].second);
		std::cout << "ID: " << id << std::endl;
		if (id != stems[i].first)
			std::cout << "TEST FAILED: stem:" << stems[i].second
					<< " id: " << stems[i].first << std::endl;
	}
	
}

int main(int argc, char** argv) {
	
	testStupid();
	testDumping();
	std::cout << "All tests ended\n";
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
