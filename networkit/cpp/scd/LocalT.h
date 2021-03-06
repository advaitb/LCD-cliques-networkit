#ifndef LOCALT_H_
#define LOCALT_H_

#include "SelectiveCommunityDetector.h"

namespace NetworKit {

/**
 * The local community expansion algorithm optimizing the T measure.
 *
 * This implements the algorithm published in:
 *
 * Fagnan, J., Zaiane, O., & Barbosa, D. (2014).
 * Using triads to identify local community structure in social networks.
 * In 2014 IEEE/ACM International Conference on Advances in Social Networks Analysis and Mining (ASONAM) (pp. 108–112).
 * https://doi.org/10.1109/ASONAM.2014.6921568
 */
class LocalT : public NetworKit::SelectiveCommunityDetector {
private:
	double alpha;
public:
	/**
	 * Constructs the Local T algorithm.
	 *
	 * @param[in] G The graph to detect communities on
	 */
	LocalT (const Graph& G);

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

#endif /* LOCALT_H_ */
