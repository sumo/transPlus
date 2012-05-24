/*
 * FFmpegEncoder.cpp
 *
 *  Created on: 21 May 2012
 *      Author: mediabag
 */

#include "FFmpegEncodingContainer.hpp"

string ContainerList::getContainer(string name) {
	map<string, string>::iterator itr = formats.find(name);
	if (itr != formats.end()) {
		return itr->second;
	} else {
		return "";
	}

}

map<string, string> ContainerList::getContainers() {
	return formats;
}

FFmpegEncodingContainer::FFmpegEncodingContainer(AVFormatContext* fc) :
		FFmpegContainer(Logger::getInstance(LOG4CPLUS_TEXT("FFmpegEncoder"))), oc(fc) {
	// TODO Auto-generated constructor stub

}

FFmpegEncodingContainer::~FFmpegEncodingContainer() {
	// TODO Auto-generated destructor stub
}

string FFmpegEncodingContainer::getContainerId() {
	return string(oc->oformat->name);
}

string FFmpegEncodingContainer::getContainerName() {
	return string(oc->oformat->long_name);
}

FFmpegEncodingContainer* FFmpegEncodingContainer::makeContainer(string ct) {
	if (!getContainerList().getContainer(ct).empty()) {
		AVFormatContext *oc;
		int err = avformat_alloc_output_context2(&oc, (AVOutputFormat*) NULL,
				ct.c_str(), NULL);
		if (!oc) {
			checkForError(err);
		}
		return new FFmpegEncodingContainer(oc);
	} else {
		return NULL;
	}

}

