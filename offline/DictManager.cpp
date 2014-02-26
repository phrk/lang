#include "hiconfig.h"
#include "DictManager.h"

DictManager::DictManager()
{
	m_next_id = 1;
}

void DictManager::addDict(DictOfflinePtr dict)
{
	m_dicts.insert(std::pair<LangDetect::Lang, DictOfflinePtr>
							(dict->getLang(), dict) );
	m_next_id += dict->size();
}

uint64_t DictManager::getWordId(const std::string &_word)
{
	hLockTicketPtr lock = m_lock.lock();
	LangDetect::Lang lang = LangDetect::getLang(_word);
	
	std::map<LangDetect::Lang, DictOfflinePtr>::iterator it = m_dicts.find(lang);
	if (it == m_dicts.end())
		return 0;
	DictOfflinePtr dict = it->second;
	return dict->getWordId(_word);
}

uint64_t DictManager::getStemId(const std::string &_word)
{
	hLockTicketPtr lock = m_lock.lock();
	LangDetect::Lang lang = LangDetect::getLang(_word);
	
	std::map<LangDetect::Lang, DictOfflinePtr>::iterator it = m_dicts.find(lang);
	if (it == m_dicts.end())
		return 0;
	DictOfflinePtr dict = it->second;
	return dict->getStemId(_word);
}

std::string DictManager::getWord(uint64_t _id)
{
	std::map<LangDetect::Lang, DictOfflinePtr>::iterator it = 
			m_dicts.begin();
	while (it != m_dicts.end()) {
		std::string word = it->second->getWord(_id);
		if (word!="")
			return word;
		it++;
	}
	return "";
}

uint64_t DictManager::genId()
{
	return ++m_next_id;
}
