#include "Stemmer.h"

Stemmer::Stemmer(const LangDetect::Lang &lang)
{
	if (lang == LangDetect::RU)
		m_stemmer = sb_stemmer_new("russian", "UTF_8");
	else if (lang == LangDetect::EN)
		m_stemmer = sb_stemmer_new("english", "UTF_8");
	else
		throw "UNKNOWN LANGUAGE";
}

Stemmer::~Stemmer()
{
	sb_stemmer_delete(m_stemmer);
}

void Stemmer::stem(const std::string &word, std::string &stem)
{
	stem = std::string((const char *)sb_stemmer_stem(m_stemmer, 
						(const sb_symbol *) word.c_str(),
						word.size() ) );
}
