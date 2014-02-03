/* 
 * File:   DictManager.h
 * Author: phrk
 *
 * Created on January 31, 2014, 4:32 PM
 */

#ifndef DICTMANAGER_H
#define	DICTMANAGER_H

#include "Dict.h"
#include <tr1/unordered_map>
#include <map>
#include "../tetramorph/threadpool/locks.h"

class DictManager
{
	uint64_t m_next_id;
	std::map<LangDetect::Lang, DictPtr> m_dicts;
	hAutoLock m_lock;
	
public:
	DictManager(std::vector<DictPtr> &_dicts);
	
	uint64_t genId();
	
	uint64_t getWordId(const std::string &_word);
	uint64_t getStemId(const std::string &_word);
	
};

typedef boost::shared_ptr<DictManager> DictManagerPtr;

#endif	/* DICTMANAGER_H */
