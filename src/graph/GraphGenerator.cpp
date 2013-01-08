/*
 * Generator.cpp
 *
 *  Created on: 05.12.2012
 *      Author: cls
 */

#include "GraphGenerator.h"

namespace EnsembleClustering {

GraphGenerator::GraphGenerator() {
	// TODO Auto-generated constructor stub

}

GraphGenerator::~GraphGenerator() {
	// TODO Auto-generated destructor stub
}


// TODO: parallel? is insertEdge thread safe?


Graph GraphGenerator::makeErdosRenyiGraph(int64_t n, double p) {
	RandomProbability randP;
	Graph G;
	for (node u = 1; u <= n; ++u) {
		for (node v = u; v <= n; ++v) {
			if (randP.generate() <= p) {
				G.insertEdge(u, v);
			}
		}
	}
	return G;
}
Graph GraphGenerator::makeCircularGraph(int64_t n) {
	Graph G;
	for (node u = G.firstNode(); u <= n; ++u) {
		G.insertEdge(u, (u + 1) % n);
	}
	return G;
}

Graph GraphGenerator::makeCompleteGraph(int64_t n) {
	RandomProbability randP;
	Graph G;
	for (node u = 1; u <= n; ++u) {
		for (node v = u; v <= n; ++v) {
			G.insertEdge(u, v);
		}
	}
	return G;
}

Graph GraphGenerator::makeClusteredRandomGraph(int64_t n, int64_t k, double pin, double pout) {
	assert(pin >= pout);

	Graph G;
	RandomProbability randP;
	// assign nodes evenly to clusters
	Clustering zeta(n);
	cluster c = 1;
	for (node v = 1; v <= n; ++v) {
		zeta.addToCluster(c, v);
		c++;
		if (c > k) {
			c = 1;
		}
	}
	assert (zeta.numberOfClusters() == k);

	// TODO: add nodes first?
	for (node u = 1; u <= n; ++u) {
		for (node v = u; v <= n; ++v) {
			if (zeta.clusterOf(u) == zeta.clusterOf(v)) {
				if (randP.generate() <= pin) {
					G.insertEdge(u, v);
				}
			} else {
				if (randP.generate() <= pout) {
					G.insertEdge(u, v);
				}
			}
		}
	}

	return G;
}

} /* namespace EnsembleClustering */
