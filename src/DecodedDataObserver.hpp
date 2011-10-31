/*
 * DecodedDataObserver.h
 *
 *  Created on: 31 Oct 2011
 *      Author: mediabag
 */

#ifndef DECODEDDATAOBSERVER_H_
#define DECODEDDATAOBSERVER_H_

template<class DecodedData>
class DecodedDataObserver {
public:
	DecodedDataObserver();
	virtual ~DecodedDataObserver();
public:
	virtual void dataDecoded(DecodedData) = 0;
};

#endif /* DECODEDDATAOBSERVER_H_ */
