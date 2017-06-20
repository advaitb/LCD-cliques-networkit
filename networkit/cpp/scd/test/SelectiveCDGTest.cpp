#include "SelectiveCDGTest.h"

#include "../PageRankNibble.h"
#include "../GCE.h"
#include "../../community/Modularity.h"
#include "../../community/Conductance.h"
#include "../../graph/Graph.h"
#include "../../io/METISGraphReader.h"
#include "../../auxiliary/Log.h"

#include <memory>

#ifndef NOGTEST

namespace NetworKit {

TEST_F(SCDGTest2, testSCD) {
	Aux::Random::setSeed(23, false);
	METISGraphReader reader;
	Graph G = reader.read("input/hep-th.graph");
	// parameters
	node seed = 50;
	std::set<node> seeds = {seed};
	double alpha = 0.1; // loop (or teleport) probability, changed due to DGleich from: // phi * phi / (225.0 * log(100.0 * sqrt(m)));
	double epsilon = 1e-5; // changed due to DGleich from: pow(2, exponent) / (48.0 * B);

	std::vector<std::pair<std::string, std::unique_ptr<SelectiveCommunityDetector>>> algorithms;
	algorithms.emplace_back(std::make_pair(std::string("PageRankNibble"), std::unique_ptr<SelectiveCommunityDetector>(new PageRankNibble(G, alpha, epsilon))));
	algorithms.emplace_back(std::make_pair(std::string("GCE L"), std::unique_ptr<SelectiveCommunityDetector>(new GCE(G, "L"))));
	algorithms.emplace_back(std::make_pair(std::string("GCE M"), std::unique_ptr<SelectiveCommunityDetector>(new GCE(G, "M"))));

	count idBound = G.upperNodeIdBound();

	for (auto &algIt : algorithms) {
		// run SCD algorithm and partition the graph accordingly
		DEBUG("Call ", algIt.first, "(", seed, ")");
		auto result = algIt.second->run(seeds);
		auto cluster = result[seed];

		// prepare result
		EXPECT_GT(cluster.size(), 0u);
		Partition partition(idBound);
		partition.allToOnePartition();
		partition.toSingleton(50);
		index id = partition[seed];
		for (auto entry: cluster) {
			partition.moveToSubset(id, entry);
		}

		// evaluate result
		Conductance conductance;
		double targetCond = 0.4;
		double cond = conductance.getQuality(partition, G);
		EXPECT_LT(cond, targetCond);
		INFO("Conductance of ", algIt.first, ": ", cond, "; cluster size: ", cluster.size());
	}
}

TEST_F(SCDGTest2, testGCE) {
	Aux::Random::setSeed(23, false);
	METISGraphReader reader;
	Graph G = reader.read("input/hep-th.graph");

	node seed = 50;
	GCE gce(G, "L");
	auto cluster = gce.expandOneCommunity(seed);

	EXPECT_GT(cluster.size(), 0u);

	{
		Partition partition(G.upperNodeIdBound());
		partition.allToOnePartition();
		partition.toSingleton(50);
		index id = partition[seed];
		for (auto entry: cluster) {
			partition.moveToSubset(id, entry);
		}

		// evaluate result
		Conductance conductance;
		double targetCond = 0.4;
		double cond = conductance.getQuality(partition, G);
		EXPECT_LT(cond, targetCond);
		INFO("Conductance of GCE: ", cond, "; cluster size: ", cluster.size());
	}

	auto cluster2 = gce.expandOneCommunity(cluster);

	{
		Partition partition(G.upperNodeIdBound());
		partition.allToOnePartition();
		partition.toSingleton(50);
		index id = partition[seed];
		for (auto entry: cluster2) {
			partition.moveToSubset(id, entry);
		}

		// evaluate result
		Conductance conductance;
		double targetCond = 0.4;
		double cond = conductance.getQuality(partition, G);
		EXPECT_LT(cond, targetCond);
		INFO("Conductance of GCE2: ", cond, "; cluster size: ", cluster2.size());
	}
	EXPECT_EQ(cluster, cluster2);
}


} /* namespace NetworKit */

#endif /*NOGTEST */
