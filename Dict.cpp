#include "Dict.h"

Dict::Dict(htConnPoolPtr _conn_pool,
		const std::string _ns,
		const std::string _word_table,
		const std::string _stem_table,
		const LangDetect::Lang &_lang):
	m_conn_pool(_conn_pool),
	m_ns(_ns),
	m_word_table(_word_table),
	m_stem_table(_stem_table),
	m_stemmer(_lang),
	m_lang(_lang)
{
	//std::cout << "Dict::Dict setting accessors\n";
	m_word_querier.reset(new htQuerier(_conn_pool, _ns, _word_table));
	m_stem_querier.reset(new htQuerier(_conn_pool, _ns, _stem_table));
	
	//std::cout << "Dict::Dict queuers set\n";
	
	m_word_writer.reset(new htCollWriterConc(_conn_pool, _ns, _word_table));
	m_stem_writer.reset(new htCollWriterConc(_conn_pool, _ns, _stem_table));
	
	//std::cout << "Dict::Dict writers set\n";
	
	htCollScanner word_scanner(_conn_pool, _ns, _word_table, "id");
	htCollScanner stem_scanner(_conn_pool, _ns, _stem_table, "id");
	
	//std::cout << "Dict::Dict accessors set\n";
	
	while (!word_scanner.end()) {
		KeyValue kv = word_scanner.getNextCell();
		uint64_t id;
		sscanf(kv.value.c_str(), "%llu", &id);
		m_word_ids.insert(std::pair<std::string, uint64_t>(kv.key, id));
	}
	while (!stem_scanner.end()) {
		KeyValue kv = stem_scanner.getNextCell();
		uint64_t id;
		sscanf(kv.value.c_str(), "%llu", &id);
		m_stem_ids.insert(std::pair<std::string, uint64_t>(kv.key, id));
	}
	
}

std::wstring widestring(const std::string &text)
{
  std::wstring result;
  result.resize(text.length());
  mbstowcs(&result[0], &text[0], text.length());
  return result;
}

std::string mbstring(const std::wstring &text)
{
  std::string result;
  result.resize(text.length());
  wcstombs(&result[0], &text[0], text.length());
  return result;
}

void utf8_tolower(std::string &_str)
{	
	std::wstring str = widestring(_str);
	std::wstring str_low;
	for (std::wstring::iterator it = str.begin(); it!=str.end(); ++it) {
		str_low.push_back(std::towlower(*it));
	}
	_str = mbstring(str_low);
	/*
	const char * locale = "rus";
	UErrorCode pErrorCode;
	//u_strToLower(dest, destCapacity, src, srcLength, locale, pErrorCode);
	UChar str_low_short[_str.size()];
	int32_t destCapacity = _str.size();	
	u_strToLower(str_low_short,
				destCapacity,
				(const UChar*)strtoul(_str.c_str(), NULL, 0),
				_str.size(),
				locale,
				&pErrorCode);
*/
}

uint64_t Dict::getWordId(const std::string &_word)
{
	std::string word = _word;
	utf8_tolower(word);
	std::tr1::unordered_map<std::string, uint64_t>::iterator it
		= m_word_ids.find(word);
	if (it != m_word_ids.end()) {
	//	std::cout << "WORD EXISTS " << word << " id: " << it->second << std::endl;
		return it->second;
	}
	
	uint64_t id = genWordId();
	
	//std::cout << "CREATING WORD " << word << " id: " << id << std::endl;
	char id_bf[50];
	sprintf(id_bf, "%llu", id);
	m_word_writer->insertSync(KeyValue(word, std::string(id_bf)), "id");
	
	m_word_ids.insert(std::pair<std::string, uint64_t>(word, id));
}

uint64_t Dict::getStemId(const std::string &_word)
{
	std::string word = _word;
	utf8_tolower(word);
	std::string stem;
	m_stemmer.stem(word, stem);
	
	std::tr1::unordered_map<std::string, uint64_t>::iterator it
		= m_stem_ids.find(stem);
	if (it != m_stem_ids.end()) {
	//	std::cout << "STEM EXISTS " << stem << " id: " << it->second << std::endl;
		return it->second;
	}
	
	uint64_t id = genWordId();
	
	//std::cout << "CREATING STEM " << stem << " id: " << id << std::endl;
	char id_bf[50];
	sprintf(id_bf, "%llu", id);
	m_stem_writer->insertSync(KeyValue(stem, std::string(id_bf)), "id");
	
	m_stem_ids.insert(std::pair<std::string, uint64_t>(stem, id));
	return id;
}

uint64_t Dict::size()
{
	return m_word_ids.size() + m_stem_ids.size();
}

LangDetect::Lang Dict::getLang()
{
	return m_lang;
}

void Dict::setIdGenFunc(boost::function<uint64_t()> _genWordId)
{
	genWordId = _genWordId;
}
