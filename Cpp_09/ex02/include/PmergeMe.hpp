#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <vector>
# include <deque>
# include <string>
# include <cstdlib>

// ─────────────────────────────────────────────────────────────────────────────
// PmergeMe: Ford-Johnson (merge-insert sort) algorithm
//
// Containers used:
//   • std::vector<int>  — Ex02 container #1
//   • std::deque<int>   — Ex02 container #2
//
// Algorithm:
//  1. Form pairs; ensure each pair has max first (larger[i], smaller[i]).
//  2. Recursively sort the "larger" elements (the main chain).
//  3. Insert all "smaller" elements (pend) into the sorted main chain.
//     The insertion order follows the Jacobsthal sequence to minimise
//     comparisons. For each pend[k], binary search is bounded above by
//     the current position of its partner larger[k] in the main chain
//     (since smaller[k] <= larger[k] by construction).
//  4. Handle the odd straggler (if n was odd) as a last insertion.
// ─────────────────────────────────────────────────────────────────────────────

class PmergeMe
{
public:
	// Orthodox Canonical Form
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	// Parse + sort + display
	void run(int argc, char **argv);

private:
	std::vector<int> _vec;
	std::deque<int>  _deq;

	// ── Parsing ──────────────────────────────────────────────────────────────
	void parseInput(int argc, char **argv);

	// ── Jacobsthal order generators ──────────────────────────────────────────
	std::vector<std::size_t> generateJacobsthal(std::size_t n) const;
	std::deque<std::size_t>  generateJacobsthalDeque(std::size_t n) const;

	// ── Ford-Johnson sorts ───────────────────────────────────────────────────
	void fordJohnsonVector(std::vector<int> &seq);
	void fordJohnsonDeque(std::deque<int>   &seq);

	// ── Binary search (returns insert position) ──────────────────────────────
	std::vector<int>::iterator binarySearchInsert(std::vector<int> &vec,
	                                              std::vector<int>::iterator end,
	                                              int value);
	std::deque<int>::iterator  binarySearchInsert(std::deque<int>  &deq,
	                                              std::deque<int>::iterator  end,
	                                              int value);

	// ── Display ──────────────────────────────────────────────────────────────
	void printContainer(const std::string &label,
	                    const std::vector<int> &vec) const;
	void printContainer(const std::string &label,
	                    const std::deque<int>  &deq) const;
};

#endif // PMERGEME_HPP
