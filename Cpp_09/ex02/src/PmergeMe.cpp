#include "../include/PmergeMe.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <utility>
#include <algorithm>

// ─── Orthodox Canonical Form ────────────────────────────────────────────────

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other)
	: _vec(other._vec), _deq(other._deq) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vec = other._vec;
		_deq = other._deq;
	}
	return *this;
}

PmergeMe::~PmergeMe() {}

// ─── Input Parsing ───────────────────────────────────────────────────────────

void PmergeMe::parseInput(int argc, char **argv)
{
	for (int i = 1; i < argc; ++i)
	{
		std::string token(argv[i]);

		if (token.empty())
			throw std::runtime_error("Error: empty token.");

		for (std::size_t j = 0; j < token.size(); ++j)
		{
			if (token[j] < '0' || token[j] > '9')
				throw std::runtime_error("Error: invalid character in '" + token + "'.");
		}

		long val = std::atol(token.c_str());
		if (val < 0)
			throw std::runtime_error("Error: only positive integers allowed.");
		if (val > 2147483647L)
			throw std::runtime_error("Error: value too large: " + token);

		_vec.push_back(static_cast<int>(val));
		_deq.push_back(static_cast<int>(val));
	}

	if (_vec.empty())
		throw std::runtime_error("Error: no input provided.");
}

// ─── Main Run ────────────────────────────────────────────────────────────────

void PmergeMe::run(int argc, char **argv)
{
	parseInput(argc, argv);

	printContainer("Before: ", _vec);

	std::clock_t startVec = std::clock();
	fordJohnsonVector(_vec);
	std::clock_t endVec = std::clock();

	std::clock_t startDeq = std::clock();
	fordJohnsonDeque(_deq);
	std::clock_t endDeq = std::clock();

	printContainer("After:  ", _vec);

	double timeVec = static_cast<double>(endVec - startVec)
	                 / CLOCKS_PER_SEC * 1000000.0;
	double timeDeq = static_cast<double>(endDeq - startDeq)
	                 / CLOCKS_PER_SEC * 1000000.0;

	std::cout << std::fixed << std::setprecision(5);
	std::cout << "Time to process a range of " << _vec.size()
	          << " elements with std::vector : " << timeVec << " us" << std::endl;
	std::cout << "Time to process a range of " << _deq.size()
	          << " elements with std::deque  : " << timeDeq << " us" << std::endl;
}

// ─── Display Helpers ─────────────────────────────────────────────────────────

void PmergeMe::printContainer(const std::string &label,
                               const std::vector<int> &vec) const
{
	std::cout << label;
	for (std::size_t i = 0; i < vec.size(); ++i)
	{
		if (i > 0) std::cout << " ";
		std::cout << vec[i];
	}
	std::cout << std::endl;
}

void PmergeMe::printContainer(const std::string &label,
                               const std::deque<int> &deq) const
{
	std::cout << label;
	for (std::size_t i = 0; i < deq.size(); ++i)
	{
		if (i > 0) std::cout << " ";
		std::cout << deq[i];
	}
	std::cout << std::endl;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  JACOBSTHAL SEQUENCE GENERATOR
//
//  J(0)=0, J(1)=1, J(n) = J(n-1) + 2·J(n-2)
//  Sequence: 0, 1, 1, 3, 5, 11, 21, 43, 85, 171, ...
//
//  Returns 0-based indices [0..n) in the Jacobsthal-dictated insertion order.
//  Groups: [J(0), J(1)], [J(1), J(3)], [J(3), J(5)], ...
//  Within each group, indices are inserted in DECREASING order.
//
//  Example n=5: order = [0, 2, 1, 4, 3]
// ═══════════════════════════════════════════════════════════════════════════════

std::vector<std::size_t> PmergeMe::generateJacobsthal(std::size_t n) const
{
	std::vector<std::size_t> order;
	if (n == 0)
		return order;

	// Build Jacobsthal sequence until it surpasses n
	std::vector<std::size_t> jac;
	jac.push_back(0);
	jac.push_back(1);
	while (jac.back() < n)
	{
		std::size_t sz = jac.size();
		jac.push_back(jac[sz - 1] + 2 * jac[sz - 2]);
	}

	std::vector<bool> inserted(n, false);

	for (std::size_t g = 1; g < jac.size(); ++g)
	{
		std::size_t hi = (jac[g] < n) ? jac[g] : n; // cap at n (1-based)
		std::size_t lo = jac[g - 1];                 // previous value (1-based)

		// Insert from hi down to lo+1 (1-based), i.e. hi-1 down to lo (0-based)
		for (std::size_t i = hi; i > lo; --i)
		{
			std::size_t idx = i - 1; // convert to 0-based
			if (!inserted[idx])
			{
				order.push_back(idx);
				inserted[idx] = true;
			}
		}
		if (hi >= n)
			break;
	}

	// Insert any remaining (shouldn't happen, but for robustness)
	for (std::size_t i = 0; i < n; ++i)
		if (!inserted[i])
			order.push_back(i);

	return order;
}

std::deque<std::size_t> PmergeMe::generateJacobsthalDeque(std::size_t n) const
{
	std::deque<std::size_t> order;
	if (n == 0)
		return order;

	std::vector<std::size_t> jac;
	jac.push_back(0);
	jac.push_back(1);
	while (jac.back() < n)
	{
		std::size_t sz = jac.size();
		jac.push_back(jac[sz - 1] + 2 * jac[sz - 2]);
	}

	std::vector<bool> inserted(n, false);

	for (std::size_t g = 1; g < jac.size(); ++g)
	{
		std::size_t hi = (jac[g] < n) ? jac[g] : n;
		std::size_t lo = jac[g - 1];

		for (std::size_t i = hi; i > lo; --i)
		{
			std::size_t idx = i - 1;
			if (!inserted[idx])
			{
				order.push_back(idx);
				inserted[idx] = true;
			}
		}
		if (hi >= n)
			break;
	}

	for (std::size_t i = 0; i < n; ++i)
		if (!inserted[i])
			order.push_back(i);

	return order;
}

// ─── Binary search helpers ────────────────────────────────────────────────────

std::vector<int>::iterator PmergeMe::binarySearchInsert(
	std::vector<int> &vec,
	std::vector<int>::iterator end,
	int value)
{
	std::vector<int>::iterator lo = vec.begin();
	std::vector<int>::iterator hi = end;
	while (lo < hi)
	{
		std::vector<int>::iterator mid = lo + (hi - lo) / 2;
		if (*mid < value) lo = mid + 1;
		else              hi = mid;
	}
	return lo;
}

std::deque<int>::iterator PmergeMe::binarySearchInsert(
	std::deque<int> &deq,
	std::deque<int>::iterator end,
	int value)
{
	std::deque<int>::iterator lo = deq.begin();
	std::deque<int>::iterator hi = end;
	while (lo < hi)
	{
		std::deque<int>::iterator mid = lo + (hi - lo) / 2;
		if (*mid < value) lo = mid + 1;
		else              hi = mid;
	}
	return lo;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  FORD-JOHNSON FOR STD::VECTOR
//
//  Algorithm:
//  1. Form pairs from input. Each pair: (larger, smaller). O(n/2) comparisons.
//  2. Sort the "larger" values recursively using Ford-Johnson.
//     Key: to keep (larger[i] ↔ smaller[i]) associations intact after the
//     recursive sort, we recursively sort the sequence of "larger" values,
//     then re-associate each sorted larger value with its original smaller
//     partner by value matching (using a "matched" flag to handle duplicates).
//  3. Build the main chain from sorted larger values.
//     partnerPos[i] = index of larger[i]'s partner position in main chain
//                   = i initially (since main starts as sorted larger[]).
//  4. Insert smaller values in Jacobsthal order.
//     For pend[k], binary-search within [begin, partnerPos[k]+1).
//     After each insertion, shift partnerPos[] for partners to the right.
//  5. Insert straggler (if n was odd) into the final chain.
// ═══════════════════════════════════════════════════════════════════════════════

void PmergeMe::fordJohnsonVector(std::vector<int> &seq)
{
	std::size_t n = seq.size();

	if (n <= 1)
		return;
	if (n == 2)
	{
		if (seq[0] > seq[1])
			std::swap(seq[0], seq[1]);
		return;
	}

	// ── Step 1: form pairs ────────────────────────────────────────────────────
	bool hasStraggler = (n % 2 != 0);
	int  straggler    = 0;
	if (hasStraggler)
		straggler = seq.back();

	std::size_t pairCount = n / 2;

	std::vector<std::pair<int, int> > pairs(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int a = seq[2 * i];
		int b = seq[2 * i + 1];
		pairs[i] = (a >= b) ? std::make_pair(a, b) : std::make_pair(b, a);
	}

	// ── Step 2: recursively sort the larger values ────────────────────────────
	std::vector<int> largerVals(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		largerVals[i] = pairs[i].first;

	fordJohnsonVector(largerVals); // recursive call

	// ── Step 3: re-associate sorted larger values with their smaller partners ─
	std::vector<bool> matched(pairCount, false);
	std::vector<std::pair<int, int> > sortedPairs(pairCount);

	for (std::size_t i = 0; i < pairCount; ++i)
	{
		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (!matched[j] && pairs[j].first == largerVals[i])
			{
				sortedPairs[i] = pairs[j];
				matched[j]     = true;
				break;
			}
		}
	}

	// ── Step 4: build main chain and pend array ───────────────────────────────
	std::vector<int> main(pairCount);
	std::vector<int> pend(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		main[i] = sortedPairs[i].first;
		pend[i] = sortedPairs[i].second;
	}

	// partnerPos[i] = current index of pend[i]'s partner in main chain
	// Initially partnerPos[i] = i because main is just the sorted larger values
	std::vector<std::size_t> partnerPos(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		partnerPos[i] = i;

	// ── Step 5: insert pend elements in Jacobsthal order ─────────────────────
	std::vector<std::size_t> order = generateJacobsthal(pairCount);

	for (std::size_t oi = 0; oi < order.size(); ++oi)
	{
		std::size_t k     = order[oi];
		int         value = pend[k];

		// pend[k] <= its partner main[partnerPos[k]], so search in
		// [begin, partnerPos[k]+1) (the +1 makes it an exclusive end iterator)
		std::vector<int>::iterator bound =
			main.begin() + static_cast<std::ptrdiff_t>(partnerPos[k] + 1);
		std::vector<int>::iterator pos = binarySearchInsert(main, bound, value);

		std::size_t insertIdx = static_cast<std::size_t>(pos - main.begin());
		main.insert(pos, value);

		// Shift all partnerPos[] entries that were at or beyond insertIdx
		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (partnerPos[j] >= insertIdx)
				partnerPos[j]++;
		}
	}

	// ── Step 6: insert straggler ──────────────────────────────────────────────
	if (hasStraggler)
	{
		std::vector<int>::iterator pos =
			binarySearchInsert(main, main.end(), straggler);
		main.insert(pos, straggler);
	}

	seq = main;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  FORD-JOHNSON FOR STD::DEQUE  (same algorithm, std::deque instead)
// ═══════════════════════════════════════════════════════════════════════════════

void PmergeMe::fordJohnsonDeque(std::deque<int> &seq)
{
	std::size_t n = seq.size();

	if (n <= 1)
		return;
	if (n == 2)
	{
		if (seq[0] > seq[1])
			std::swap(seq[0], seq[1]);
		return;
	}

	bool hasStraggler = (n % 2 != 0);
	int  straggler    = 0;
	if (hasStraggler)
		straggler = seq.back();

	std::size_t pairCount = n / 2;

	std::deque<std::pair<int, int> > pairs(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		int a = seq[2 * i];
		int b = seq[2 * i + 1];
		pairs[i] = (a >= b) ? std::make_pair(a, b) : std::make_pair(b, a);
	}

	std::deque<int> largerVals(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		largerVals[i] = pairs[i].first;

	fordJohnsonDeque(largerVals);

	std::vector<bool> matched(pairCount, false);
	std::deque<std::pair<int, int> > sortedPairs(pairCount);

	for (std::size_t i = 0; i < pairCount; ++i)
	{
		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (!matched[j] && pairs[j].first == largerVals[i])
			{
				sortedPairs[i] = pairs[j];
				matched[j]     = true;
				break;
			}
		}
	}

	std::deque<int> main(pairCount);
	std::deque<int> pend(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
	{
		main[i] = sortedPairs[i].first;
		pend[i] = sortedPairs[i].second;
	}

	std::vector<std::size_t> partnerPos(pairCount);
	for (std::size_t i = 0; i < pairCount; ++i)
		partnerPos[i] = i;

	std::deque<std::size_t> order = generateJacobsthalDeque(pairCount);

	for (std::size_t oi = 0; oi < order.size(); ++oi)
	{
		std::size_t k     = order[oi];
		int         value = pend[k];

		std::deque<int>::iterator bound =
			main.begin() + static_cast<std::ptrdiff_t>(partnerPos[k] + 1);
		std::deque<int>::iterator pos = binarySearchInsert(main, bound, value);

		std::size_t insertIdx = static_cast<std::size_t>(pos - main.begin());
		main.insert(pos, value);

		for (std::size_t j = 0; j < pairCount; ++j)
		{
			if (partnerPos[j] >= insertIdx)
				partnerPos[j]++;
		}
	}

	if (hasStraggler)
	{
		std::deque<int>::iterator pos =
			binarySearchInsert(main, main.end(), straggler);
		main.insert(pos, straggler);
	}

	seq = main;
}
