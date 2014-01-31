/* 
 * File:   Dict.h
 * Author: phrk
 *
 * Created on January 29, 2014, 1:17 PM
 */

#ifndef DICT_H
#define	DICT_H

#include "../htdba/htDba.h"
#include "../htdba/htConnPool.h"
#include "../htdba/htQuerier.h"
#include "../htdba/htCollWriter.h"
#include "../htdba/htCollScanner.h"

#include <tr1/unordered_map>

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
	
	std::tr1::unordered_map<std::string, uint64_t> m_word_ids;
	std::tr1::unordered_map<std::string, uint64_t> m_stem_ids;
	
	Stemmer m_stemmer;
	
	LangDetect::Lang m_lang;
	boost::function<uint64_t()> genWordId;
	
public:
	
	Dict(htConnPoolPtr _conn_pool,
		const std::string _ns,
		const std::string _word_table,
		const std::string _stem_table,
		const LangDetect::Lang &lang);
	
	uint64_t getWordId(const std::string &_word);
	uint64_t getStemId(const std::string &_word);

	uint64_t size();
	LangDetect::Lang getLang();
	
	void setIdGenFunc(boost::function<uint64_t()> _genWordId);
};

typedef boost::shared_ptr<Dict> DictPtr;

#endif	/* DICT_H */

