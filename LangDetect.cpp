#include "LangDetect.h"

LangDetect::Lang LangDetect::getLang(const std::string &_str)
{
	LangDetect::Lang ret;
	bool is_reliable;
	CLD2::Language lang =
			CLD2::DetectLanguage(_str.c_str(), _str.size(), 1, &is_reliable);
			
	if (lang == CLD2::ENGLISH) {
		ret = LangDetect::EN;
	}
	else if (lang == CLD2::RUSSIAN) {
		ret = LangDetect::RU;
	} else {
		ret = LangDetect::UNKNOWN;
	}
	
	return ret;
}
