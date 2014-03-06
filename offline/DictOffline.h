
#ifndef _DICTOFFLINE_H_
#define _DICTOFFLINE_H_

#include "hiaux/strings/string_utils.h"

#include "hiaux/structs/hashtable.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

//#include "../online/Dict.h"
#include "../offline/LangDetect.h"
#include "../offline/Stemmer.h"

#include "Dict.pb.h"

#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <dirent.h>

#include <sys/uio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>

#include <errno.h>

class DictOffline : boost::noncopyable
{
protected:
	hiaux::hashtable<std::string, uint64_t> m_word_ids;
	hiaux::hashtable<std::string, uint64_t> m_stem_ids;
	
	hiaux::hashtable<uint64_t, std::string> m_id_words;
	
	Stemmer m_stemmer;
	LangDetect::Lang m_lang;
	
	boost::function<uint64_t()> genWordId;
	
public:
	
	DictOffline(const LangDetect::Lang &_lang,
				boost::function<uint64_t()> _genWordId);
	
	virtual uint64_t getWordId(const std::string &_word);
	virtual uint64_t getStemId(const std::string &_word);
	virtual uint64_t setWordId(const std::string &_word);
	virtual uint64_t setStemId(const std::string &_word);

	virtual std::string getWord(uint64_t _id);
	
	virtual uint64_t size();
	virtual LangDetect::Lang getLang();
	
	void dumpToFile(const std::string &_words_filename,
					const std::string &_stems_filename);
	
	void loadFromFile(const std::string &_filename,
					const std::string &_stems_filename);
	
	virtual ~DictOffline();
};

typedef boost::shared_ptr<DictOffline> DictOfflinePtr;

#endif // _DICTOFFLINE_H_
