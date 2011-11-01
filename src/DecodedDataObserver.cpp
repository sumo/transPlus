/*
 * DecodedDataObserver.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: mediabag
 */

#include "DecodedDataObserver.hpp"

template<class DecodedData>
void DecodedDataNotifier<DecodedData>::addObserver(DecodedDataObserver<DecodedData> obs) {
	observers.push_back(obs);
}

template<class DecodedData>
void DecodedDataNotifier<DecodedData>::notifyObservers(DecodedData data) {
	typedef typename vector<DecodedDataObserver<DecodedData> >::iterator Itr;
	 Itr it;
	 for (it=observers.begin() ; it < observers.end(); it++ ) {
		 DecodedDataObserver<DecodedData> obs = *it;
		 obs.dataDecoded(data);
	 }
}
