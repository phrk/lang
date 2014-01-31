#include "DictManager.h"

DictManager::DictManager(std::vector<DictPtr> &_dicts)
{
	m_next_id = 1;
	for (int i = 0; i<_dicts.size(); i++) {
		m_dicts.insert(std::pair<LangDetect::Lang, DictPtr>
				(_dicts[i]->getLang(),_dicts[i]) );
		m_next_id += _dicts[i]->size();
		_dicts[i]->setIdGenFunc(boost::bind(&DictManager::genId, this) );
	}
}

uint64_t DictManager::getWordId(const std::string &_word)
{
	LangDetect::Lang lang = LangDetect::getLang(_word);
	
	std::map<LangDetect::Lang, DictPtr>::iterator it = m_dicts.find(lang);
	if (it == m_dicts.end())
		return 0;
	DictPtr dict = it->second;
	return dict->getWordId(_word);
}

uint64_t DictManager::getStemId(const std::string &_word)
{
	LangDetect::Lang lang = LangDetect::getLang(_word);
	
	std::map<LangDetect::Lang, DictPtr>::iterator it = m_dicts.find(lang);
	if (it == m_dicts.end())
		return 0;
	DictPtr dict = it->second;
	return dict->getStemId(_word);
}

uint64_t DictManager::genId()
{
	return ++m_next_id;
}
