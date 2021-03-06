#ifndef LOCALTIGHTNESSEXPANSION_H_
#define LOCALTIGHTNESSEXPANSION_H_

#include "SelectiveCommunityDetector.h"

namespace NetworKit {

/**
 * The Local Tightness Expansion (LTE) algorithm.
 *
 * The algorithm can handle weighted graphs.
 *
 * This is the local community expansion algorithm described in
 *
 * Huang, J., Sun, H., Liu, Y., Song, Q., & Weninger, T. (2011). 
 * Towards Online Multiresolution Community Detection in Large-Scale Networks.
 * PLOS ONE, 6(8), e23829.
 * https://doi.org/10.1371/journal.pone.0023829
 */
class LocalTightnessExpansion : public NetworKit::SelectiveCommunityDetector {
private:
	double alpha;
public:
	/**
	 * Constructs the Local Tightness Expansion algorithm.
	 *
	 * @param[in] G The graph to detect communities on
	 * @param[in] alpha Tightness coefficient - smaller values lead to larger communities
	 */
	LocalTightnessExpansion(const Graph& G, double alpha = 1.0);

	/**
	 * Expands a set of seed nodes into a community.
	 *
	 * @param[in] s The seed nodes
	 * @return A community of the seed nodes
	 */
	std::set<node> expandOneCommunity(const std::set<node>& s) override;

	using SelectiveCommunityDetector::expandOneCommunity;
};

}

#endif /* TRIANGLEDETECT_H_ */
