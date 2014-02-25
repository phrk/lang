#ifndef _LANG_DETECT_H_
#define _LANG_DETECT_H_

#include "hiconfig.h"

#include "cld2-read-only/public/compact_lang_det.h"
#include "cld2-read-only/public/encodings.h"
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
