/*
 * DecodedDataObserver.hpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#ifndef DECODEDDATAOBSERVER_HPP_
#define DECODEDDATAOBSERVER_HPP_
#include <vector>

using namespace std;

template<class DecodedData>
class DecodedDataObserver {
public:
	DecodedDataObserver();
	virtual ~DecodedDataObserver();
public:
	virtual void dataDecoded(DecodedData) = 0;
};

#endif /* DECODEDDATAOBSERVER_HPP_ */
