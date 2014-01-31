#ifndef _LANG_DETECT_H_
#define _LANG_DETECT_H_

#include <cld/public/compact_lang_det.h>
#include <cld/public/encodings.h>
#include <string>

class LangDetect
{
public:
	
	enum Lang {
		EN,
		RU,
		UNKNOWN
	};
	
	static LangDetect::Lang getLang(const std::string &_str);
};

#endif
