#ifndef TCE_H_
#define TCE_H_

#include "SelectiveCommunityDetector.h"

namespace NetworKit {

/**
 * Triangle-based community expansion
 *
 * The algorithm can handle weighted graphs.
 */
class TCE : public NetworKit::SelectiveCommunityDetector {
private:
	bool refine;
	bool useJaccard;

public:
	/**
	 * Construct a TCE object.
	 *
	 * @param[in] G The graph to detect communities on
	 * @param[in] refine If nodes shall be removed again if this makes the community better
	 * @param[in] useJaccard use jaccard index for weight calculation.
	 */
	TCE(const Graph& G, bool refine = false, bool useJaccard = false);

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
