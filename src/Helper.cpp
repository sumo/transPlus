/*
 * Helper.cpp
 *
 *  Created on: 23 May 2012
 *      Author: mediabag
 */

#include "Helper.hpp"

CodecID CodecList::resolveCodec(string name) {
	return codecs.find(name)->second;
}

set<string> CodecList::getCodecNames() {
	set<string> codecNames;
	for(CodecResolverType::iterator it = codecs.begin(); it != codecs.end(); ++it) {
	  codecNames.insert(it->first);
	}
	return codecNames;
}
