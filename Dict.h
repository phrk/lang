/* 
 * File:   Dict.h
 * Author: phrk
 *
 * Created on January 29, 2014, 1:17 PM
 */

#ifndef DICT_H
#define	DICT_H

#include "hiconfig.h"

#include "hiaux/structs/hashtable.h"

#include "htdba/htDba.h"
#include "htdba/htConnPool.h"
#include "htdba/htQuerier.h"
#include "htdba/htCollWriter.h"
#include "htdba/htCollScanner.h"

#include <cld/public/compact_lang_det.h>
#include <cld/public/encodings.h>

#include "LangDetect.h"
#include "Stemmer.h"

void utf8_tolower(std::string &_str);

class Dict
{	
	htConnPoolPtr m_conn_pool;
	std::string m_ns;
	std::string m_word_table;
	std::string m_stem_table;
	
	htQuerierPtr m_word_querier;
	htQuerierPtr m_stem_querier;
	htCollWriterConcPtr m_word_writer;
	htCollWriterConcPtr m_stem_writer;
	
	hiaux::hashtable<std::string, uint64_t> m_word_ids;
	hiaux::hashtable<std::string, uint64_t> m_stem_ids;
	
	hiaux::hashtable<uint64_t, std::string> m_id_words;
	
	Stemmer m_stemmer;
	
	LangDetect::Lang m_lang;
	boost::function<uint64_t()> genWordId;
	
public:
	
	enum Reset {
		RESET,
		DONT_RESET
	};
	
	Dict(htConnPoolPtr _conn_pool,
		const std::string _ns,
		const std::string _word_table,
		const std::string _stem_table,
		const LangDetect::Lang &lang,
		Reset _reset = Dict::DONT_RESET);
	
	uint64_t getWordId(const std::string &_word);
	uint64_t getStemId(const std::string &_word);

	std::string getWord(uint64_t _id);
	
	uint64_t size();
	LangDetect::Lang getLang();
	
	void setIdGenFunc(boost::function<uint64_t()> _genWordId);
};

typedef boost::shared_ptr<Dict> DictPtr;

#endif	/* DICT_H */

