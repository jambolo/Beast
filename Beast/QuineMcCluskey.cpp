//#include "QuineMcCluskey.h"

#include <algorithm>
#include <cassert>
#include <vector>

static size_t const MAX_VARIABLES = 32;
typedef uint32_t Term;

static bool isPowerOf2(Term x)
{
    return (x != 0) && (((x - 1) & x) == 0);
}

static int numberOfOneBits(uint32_t n) {
    n = (n & 0x55555555) + ((n & 0xAAAAAAAA) >> 1);
    n = (n & 0x33333333) + ((n & 0xCCCCCCCC) >> 2);
    n = (n & 0x0f0f0f0f) + ((n & 0xf0f0f0f0) >> 4);
    n = (n & 0x00ff00ff) + ((n & 0xff00ff00) >> 8);
    n = (n & 0x0000ffff) + ((n & 0xffff0000) >> 16);
    return n;
}

struct Implicant
{
    Term v;
    Term d;
};

typedef std::vector<Implicant> ImplicantList;
typedef std::vector<ImplicantList> Round; // One per number of ones

bool operator ==(Implicant const & i0, Implicant const & i1)
{
    return (i0.d == i1.d) && (i0.v == i1.v);
}

static bool differByOneBit(Implicant const & i0, Implicant const & i1)
{
    return (i0.d == i0.d) && isPowerOf2(i0.v ^ i1.v);
}

static Implicant combine(Implicant const & i0, Implicant const & i1)
{
    assert(differByOneBit(i0, i1));
    Implicant r;
    r.d = i0.d | (i0.v ^ i1.v);
    r.v = i0.v & ~r.d;
}

static bool covers(Implicant const & i0, Implicant const & i1)
{
    return (i0.d | i1.d) == i0.d && i0.v == (i1.v & ~i0.d);
}

template <typename T>
void removeDuplicates(std::vector<T> & v)
{
    std::vector<T>::iterator e = v.end();
    for (std::vector<T>::iterator i = v.begin(); i != e; ++i)
    {
        e = std::remove(i + 1, e, *i);
    }
    v.erase(e, v.end());
}

static ImplicantList findAllMatches(ImplicantList const & i0, ImplicantList const & i1)
{
    ImplicantList combined;

    // For each element in this list, match with all elements in the other list, adding the matched elements to the next
    // round
    for (auto const & i : i0)
    {
        for (auto const & j : i1)
        {
            // If the two implicants differ by only one bit, then add the combined implicant to the next round
            if (differByOneBit(i, j))
            {
                combined.push_back(combine(i, j));
            }
        }
    }
    removeDuplicates(combined);
    return combined;
}

static void removeCombinedImplicants(Round & lower, Round const & higher)
{
    for (auto const & i : higher) // For each implicant list in the higher round
    {
        for (auto const & j : i) // For each implicant in the list
        {
            for (auto & m : lower) // For each implicant list in the lower round
            {
                // Erase all implicants in the lower list covered by the higher list
                m.erase(
                    std::remove_if(
                        m.begin(),
                        m.end(),
                        [j] (Implicant const & n) {
                    return covers(j, n);
                }),
                    m.end());
            }
        }
    }
}

static bool coversNoneOf(Implicant const & i, std::vector<Term> terms)
{
    return std::none_of(terms.begin(),
                        terms.end(),
                        [i](Term const & t) {
        return covers(i, Implicant { t, 0 });
    });
}

std::vector<Term> minimize(std::vector<Term> const & minTerms, std::vector<Term> dontCares)
{
    // Add the minterms and don't cares as the first round

    Round r;
    r.resize(MAX_VARIABLES);

    for (auto const & t : minTerms)
    {
        r[numberOfOneBits(t) - 1].emplace_back(t, 0);
    }

    for (auto const & t : dontCares)
    {
        r[numberOfOneBits(t) - 1].emplace_back(t, 0);
    }

    ImplicantList primeImplicants;

    // Combine all implicants that vary by exactly one bit until no more combinations can be made

    bool done = true;
    do
    {
        Round next;

        done = true;
        for (size_t n = 0; n < r.size() - 1; ++n)
        {
            ImplicantList combined = findAllMatches(r[n], r[n + 1]);
            next.push_back(combined);
            if (!combined.empty())
            {
                done = false;
            }
        }
        if (!done)
        {
            removeCombinedImplicants(r, next); // Remove any implicants that have been combined to leave prime implicants
            for (auto const & l : r)           // For each implicant list in the round
            {
                primeImplicants.insert(primeImplicants.end(), l.begin(), l.end());
            }
            r = next;
        }
    } while (!done);

    // Remove the don't cares (any implicant that doesn't cover any of the minterms).
    primeImplicants.erase(
        std::remove_if(primeImplicants.begin(),
                       primeImplicants.end(),
                       [minTerms](Implicant const & i) {
        return coversNoneOf(i, minTerms);
    }),
        primeImplicants.end());
}
