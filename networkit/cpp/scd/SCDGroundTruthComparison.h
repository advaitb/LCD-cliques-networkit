#ifndef SCD_GROUND_TRUTH_COMPARISON_H
#define SCD_GROUND_TRUTH_COMPARISON_H

#include "../structures/Cover.h"
#include "../base/Algorithm.h"

#include <map>
#include <set>

namespace NetworKit {

/**
 * This class evaluates a set found communities against a ground truth cover. Each found community
 * is compared against the communities of the seed node in the ground truth cover.
 *
 * For each score, the ground truth community is chosen as comparison that maximizes the score.
 * If seeds are not ignored (a parameter of the constructor), then only ground truth communities
 * that contain the given seed are used to compare against.
 *
 * The calculated scores are:
 *
 * Precision: the size of the intersection of found and ground truth community divided by the
 * size of the found community, i.e., how much of the found community was an actual match.
 *
 * Recall: the size of the intersection of found and ground truth community divided by the
 * size of the ground truth community, i.e., how much of the ground truth community was found.
 *
 * F1 score: the harmonic mean of precision and recall.
 *
 * Jaccard index: the size of the intersection of found and ground truth community divided by the
 * size of the union of found and ground truth community.
 *
 * For each score, the range of values is between 0 and 1, where 0 is the worst and 1 the best score.
 */
class SCDGroundTruthComparison : public Algorithm {
public:
	/**
	 * Construct the SCD evaluation for the given graph, ground truth and found communities.
	 *
	 * @param G The graph to compare on
	 * @param groundTruth The ground truth cover
	 * @param found The found communities
	 * @param ignoreSeeds If the seeds shall be ignored, i.e. any ground truth community is a match
	 */
	SCDGroundTruthComparison(const Graph &G, const Cover &groundTruth, const std::map<node, std::set<node>> &found, bool ignoreSeeds = false);

	/**
	 * Calculate all measures.
	 */
	virtual void run() override;

	/**
	 * Get the Jaccard index of every found community.
	 *
	 * @return A map between seed node and the jaccard index of the seed's community.
	 */
	std::map<index, double> getIndividualJaccard();

	/**
	 * Get the precision of every found community.
	 *
	 * @return A map between seed node and the precision of the seed's community.
	 */
	std::map<index, double> getIndividualPrecision();

	/**
	 * Get the recall of every found community.
	 *
	 * @return A map between seed node and the recall of the seed's community.
	 */
	std::map<index, double> getIndividualRecall();

	/**
	 * Get the F1 score of every found community.
	 *
	 * @return A map between seed node and the F1 score of the seed's community.
	 */
	std::map<index, double> getIndividualF1();

	/**
	 * Get the (unweighted) average of the jaccard indices of every found community.
	 */
	double getAverageJaccard();

	/**
	 * Get the (unweighted) average of the F1 score of every found community.
	 */
	double getAverageF1();

	/**
	 * Get the (unweighted) average of the precision of every found community.
	 */
	double getAveragePrecision();

	/**
	 * Get the (unweighted) average of the recall of every found community.
	 */
	double getAverageRecall();
private:
	const Graph &G;
	const Cover &groundTruth;
	const std::map<node, std::set<node>> &found;
	bool ignoreSeeds;

	std::map<index, double> jaccardScores;
	double averageJaccard;
	std::map<index, double> f1Scores;
	double averageF1;
	std::map<index, double> precisionScores;
	double averagePrecision;
	std::map<index, double> recallScores;
	double averageRecall;
};

} /* namespace NetworKit */

#endif // SCD_GROUND_TRUTH_COMPARISON_H
