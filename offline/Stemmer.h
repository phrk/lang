/* 
 * File:   Stemmer.h
 * Author: phrk
 *
 * Created on January 29, 2014, 8:25 PM
 */

#ifndef STEMMER_H
#define	STEMMER_H

#include "LangDetect.h"

#include "../thirdparty/libstemmer_c/include/libstemmer.h"

class Stemmer
{
	sb_stemmer *m_stemmer;
public:
	Stemmer(const LangDetect::Lang &lang);
	void stem(const std::string &word, std::string &stem) const;
	~Stemmer();
};


#endif	/* STEMMER_H */

