#include "DictOffline.h"

DictOffline::DictOffline(const LangDetect::Lang &_lang,
				boost::function<uint64_t()> _genWordId):
	m_stemmer(_lang),
	m_lang(_lang),
	genWordId(_genWordId)
{
}

DictOffline::~DictOffline()
{
}

uint64_t DictOffline::getWordId(const std::string &_word)
{
	std::string word = _word;
	fix_utf8_string(word);
	toLowerUtf8(word);
	hiaux::hashtable<std::string, uint64_t>::const_iterator it
		= m_word_ids.find(word);
	if (it != m_word_ids.end()) {
		return it->second;
	}
	return 0;
}

uint64_t DictOffline::getStemId(const std::string &_word)
{
	std::string word = _word;
	fix_utf8_string(word);
	toLowerUtf8(word);
	std::string _stem;
	m_stemmer.stem(word, _stem);
	
	hiaux::hashtable<std::string, uint64_t>::const_iterator it
		= m_stem_ids.find(_stem);
	if (it != m_stem_ids.end()) {
		return it->second;
	}
	return 0;
}

uint64_t DictOffline::setWordId(const std::string &_word)
{
	std::string word = _word;
	fix_utf8_string(word);
	toLowerUtf8(word);
	
	hiaux::hashtable<std::string, uint64_t>::const_iterator it
		= m_word_ids.find(word);
	if (it != m_word_ids.end()) {
		return it->second;
	}		
	
	uint64_t id = genWordId();
	m_word_ids.insert(std::pair<std::string, uint64_t>(word, id) );
	return id;
}

uint64_t DictOffline::setStemId(const std::string &_word)
{
	std::string word = _word;
	fix_utf8_string(word);
	toLowerUtf8(word);
	m_stemmer.stem(word, word);
	uint64_t id = 0;
	
	hiaux::hashtable<std::string, uint64_t>::const_iterator it
		= m_stem_ids.find(word);
	if (it != m_stem_ids.end()) {
		return it->second;
	}
	
	id = genWordId();
	m_stem_ids.insert(std::pair<std::string, uint64_t>(word, id) );
	return id;
}

std::string DictOffline::getWord(uint64_t _id)
{
	hiaux::hashtable<uint64_t, std::string>::iterator it = 
			m_id_words.find(_id);
	if (it != m_id_words.end())
		return it->second;
	return "";
}

uint64_t DictOffline::size()
{
	return m_word_ids.size() + m_stem_ids.size();
}

LangDetect::Lang DictOffline::getLang()
{
	return m_lang;
}

void DictOffline::dumpToFile(const std::string &_words_filename,
						const std::string &_stems_filename)
{
	{
		int words_f = open(_words_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		hiaux::hashtable<std::string, uint64_t>::iterator w_it = m_word_ids.begin();
		hiaux::hashtable<std::string, uint64_t>::iterator w_it_end = m_word_ids.end();
		while (w_it != w_it_end) {
			lang::StringId stringid_pb;
			std::string word = w_it->first;
			if (word.size()>40)
				word = word.substr(0, 40);
			fix_utf8_string(word);
			stringid_pb.set_id(w_it->second);
			stringid_pb.set_str(word);
			std::string id_dump = stringid_pb.SerializeAsString();
			uint64_t dump_size = id_dump.size();
			
			write(words_f, (void*)&dump_size, sizeof(uint64_t) );
			write(words_f, (void*)id_dump.data(), sizeof(char)*dump_size);

			w_it++;
		}
		close(words_f);
	}
	{
		int stems_f = open(_stems_filename.c_str(), O_RDWR | O_CREAT | O_TRUNC,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		hiaux::hashtable<std::string, uint64_t>::iterator s_it = m_stem_ids.begin();
		hiaux::hashtable<std::string, uint64_t>::iterator s_it_end = m_stem_ids.end();
		while (s_it != s_it_end) {
			lang::StringId stringid_pb;
			std::string stem = s_it->first;
			if (stem.size()>40)
				stem = stem.substr(0, 40);
			fix_utf8_string(stem);
			stringid_pb.set_id(s_it->second);
			stringid_pb.set_str(stem);
			std::string id_dump = stringid_pb.SerializeAsString();
			uint64_t dump_size = id_dump.size();

			write(stems_f, (void*)&dump_size, sizeof(uint64_t) );
			write(stems_f, (void*)id_dump.data(), sizeof(char)*dump_size);

			s_it++;
		}
		close(stems_f);
	}
}

void DictOffline::loadFromFile(const std::string &_words_filename,
						const std::string &_stems_filename)
{
	{
		int words_f = open(_words_filename.c_str(),  O_RDONLY,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		
		while (true) {
			lang::StringId stringid_pb;
			uint64_t dump_size;
			
			if (read(words_f, (void*)&dump_size, sizeof(uint64_t)) <= 0)
				break;
			//std::cout << "dump size: " << dump_size << std::endl;
			//return;
			char dump[dump_size];
			
			if (read(words_f, dump, sizeof(char)*dump_size) <= 0)
				break;

			stringid_pb.ParseFromString(dump);
			m_word_ids.insert( std::pair<std::string, uint64_t> (stringid_pb.str(),
															stringid_pb.id()) );
			
		}
		close(words_f);
	}
	{
		int stems_f = open(_stems_filename.c_str(),  O_RDONLY,
					S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		
		while (true) {
			lang::StringId stringid_pb;
			uint64_t dump_size;
			if (read(stems_f, (void*)&dump_size, sizeof(uint64_t)) <= 0)
				break;
			//return;
			char dump[dump_size];
			if (read(stems_f, dump, sizeof(char)*dump_size) <= 0)
				break;
			stringid_pb.ParseFromString(dump);
			m_stem_ids.insert( std::pair<std::string, uint64_t> (stringid_pb.str(),
															stringid_pb.id()) );

		}
		close(stems_f);
	}
}
