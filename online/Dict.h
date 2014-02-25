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

#include "../offline/DictOffline.h"

class Dict : public DictOffline
{	
	htConnPoolPtr m_conn_pool;
	std::string m_ns;
	std::string m_word_table;
	std::string m_stem_table;
	
	htQuerierPtr m_word_querier;
	htQuerierPtr m_stem_querier;
	htCollWriterConcPtr m_word_writer;
	htCollWriterConcPtr m_stem_writer;
	
public:
	
	enum Reset {
		RESET,
		DONT_RESET
	};
	
	Dict(htConnPoolPtr _conn_pool,
		const std::string _ns,
		const std::string _word_table,
		const std::string _stem_table,
		boost::function<uint64_t()> _genWordId,
		const LangDetect::Lang &lang,
		Reset _reset = Dict::DONT_RESET);
	
	virtual uint64_t getWordId(const std::string &_word);
	virtual uint64_t getStemId(const std::string &_word);
	
	virtual ~Dict();
};

typedef boost::shared_ptr<Dict> DictPtr;

#endif	/* DICT_H */

