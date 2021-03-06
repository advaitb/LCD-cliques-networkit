/* GCE.cpp
 *
 * Created on: 06.05.2013
 * Author: cls
 */


#include "TwoPhaseL.h"
#include "../structures/LocalCommunity.h"
#include "../auxiliary/UniformRandomSelector.h"

namespace NetworKit {


TwoPhaseL::TwoPhaseL(const Graph& G) : SelectiveCommunityDetector(G) {
	if (G.numberOfSelfLoops() > 0) {
		throw std::runtime_error("Graphs with self-loops are not supported in TwoPhaseL");
	}
}

std::set<node> TwoPhaseL::expandOneCommunity(const std::set<node>& seeds) {
	LocalCommunity<true, true, true> com(G);

	for (node s : seeds) {
		com.addNode(s);
		assert(com.contains(s));
	}

	using shell_info_t = typename decltype(com)::ShellInfo;
	using community_info_t = typename decltype(com)::CommunityInfo;

	auto newLinAdd = [&](const shell_info_t& info) {
		return 2.0 * (com.internalEdgeWeight() + (*info.intDeg)) / (com.size() + 1);
	};

	auto newLexAdd = [&](const shell_info_t& info) {
		return (com.cut() - (*info.intDeg) + (*info.extDeg)) / (com.boundarySize() + info.boundaryChange());
	};

	auto newLinRemove = [&](const community_info_t& info) {
		return 2.0 * (com.internalEdgeWeight() - (*info.intDeg)) / (com.size() - 1);
	};

	auto newLexRemove = [&](const community_info_t& info) {
		return (com.cut() + (*info.intDeg) - (*info.extDeg)) / (com.boundarySize() + info.boundaryChange());
	};

	double currentLin = 2.0 * com.internalEdgeWeight() / com.size();
	double currentLex = com.cut() / com.boundarySize();
	double currentL = currentLin / currentLex;

	// first phase: add possible candidates
	node vMax;
	do {
		// scan shell for node with maximum quality improvement
		double lMax = currentL; 	// maximum quality improvement
		vMax = none;
		Aux::UniformRandomSelector selector;

		com.forShellNodes([&](const node v, const shell_info_t& info) {
			// get values for current node
			double nLin = newLinAdd(info);
			double nLex = newLexAdd(info);
			double nL = nLin / nLex;

			TRACE("nLin: ", nLin);
			TRACE("nLex: ", nLex);

			if (nLin > currentLin) {
				if (nL > lMax) {
					vMax = v;
					lMax = nL;
					selector.reset();
				} else if (nL == lMax && selector.add_element()) {
					vMax = v;
				}
			}
		});

		TRACE("vMax: ", vMax);
		TRACE("LMax: ", lMax);
		if (vMax != none) {
			com.addNode(vMax);	// add best node to community
			// update current community quality
			currentLin = 2.0 * com.internalEdgeWeight() / com.size();
			currentLex = com.cut() / com.boundarySize();
			currentL = currentLin / currentLex;
			//TRACE("community: ", community);
		}
	} while (vMax != none);

	// second phase: remove unwanted nodes
	com.forCommunityNodes([&](const node v, const community_info_t& info) {
		double nLin = newLinRemove(info);
		double nLex = newLexRemove(info);

		if (!(currentLin > nLin && currentLex < nLex)) {
			TRACE("Removing ", v);
			com.removeNode(v);
		}
	});

	for (node s : seeds) {
		if (!com.contains(s)) {
			return {};
		}
	}

	return com.toSet();
}

} /* namespace NetworKit */
