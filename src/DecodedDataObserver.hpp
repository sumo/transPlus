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
	virtual void dataDecoded(DecodedData);
};

template<class DecodedData>
class DecodedDataNotifier {
	vector<DecodedDataObserver<DecodedData> > observers;
public:
	void notifyObservers(DecodedData);
	void addObserver(DecodedDataObserver<DecodedData>);
};
#endif /* DECODEDDATAOBSERVER_HPP_ */
