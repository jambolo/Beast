#include "Beast.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;

Beast::Expression Beast::simplify(json const & json)
{
    return Expression();
}

Beast::Expression Beast::simplify(Expression const & x)
{
    return Expression();
}

json Beast::Expression::toJson() const
{
    json a = json::array();
    for (auto const & c : clauses_)
    {
        a.push_back(c.toJson());
    }

    return json({ "or", a });
}

std::string Beast::Expression::toAlgebraic() const
{
    std::string a;
    for (auto const & c : clauses_)
    {
        if (!c.terms.empty())
        {
            if (!a.empty())
                a.append(" + ");
            a.append(c.toAlgebraic());
        }
    }
    return a;
}

Beast::Expression & Beast::Expression::operator |=(Expression const & rhs)
{
    clauses_.insert(clauses_.end(), rhs.clauses_.begin(), rhs.clauses_.end());
    return *this;
}

Beast::Expression & Beast::Expression::operator &=(Beast::Expression const & rhs)
{
    std::vector<Beast::Expression::Clause> product;
    for (auto const & x : clauses_)
    {
        for (auto const & y : rhs.clauses_)
        {
            Beast::Expression::Clause c = x & y;
            if (!c.terms.empty())
                product.push_back(c);
        }
    }
    clauses_ = product;
    return *this;
}

Beast::Expression Beast::Expression::operator ~()
{
    // De Morgan's Laws:
    //      !(x | y | ...) = !x & !y & ...
    //      !(x & y & ...) = !x | !y | ...

    Beast::Expression inverse;
    for (auto const & x : clauses_)
    {
        Beast::Expression not_x;  // Inverse of clause is an expression
        not_x.clauses_.reserve(x.terms.size());
        for (size_t i = 0; i < x.terms.size(); ++i)
        {
            Beast::Expression::Clause c =
            {
                std::vector<bool>(1, !x.terms[i] && x.mask[i]),
                std::vector<bool>(1, x.mask[i])
            };
            not_x.clauses_.push_back(c);
        }

        inverse &= not_x;
    }
    return inverse;
}

Beast::Expression::Clause & Beast::Expression::Clause::operator &=(Beast::Expression::Clause const & rhs)
{
    Beast::Expression::Clause shorter = rhs;    // The shorter of the two

    if (shorter.terms.size() > terms.size())
    {
        terms.swap(shorter.terms);
        mask.swap(shorter.mask);
    }

    for (size_t i = 0; i < shorter.terms.size(); ++i)
    {
        // If the rhs clause doesn't have this term, then ignore it
        if (!shorter.mask[i])
            continue;

        // If this clause does not have this term, then add it. Otherwise, if both clauses have this term, then make sure they
        // are the same.
        if (!mask[i])
        {
            terms[i] = shorter.terms[i];
            mask[i]  = true;
        }
        else
        {
            // If the signs are different, then the clause is false.
            if (shorter.terms[i] != terms[i])
            {
                terms.clear();
                mask.clear();
                break;
            }
        }
    }
    return *this;
}

nlohmann::json Beast::Expression::Clause::toJson() const
{
    json a = json::array();
    for (size_t i = 0; i < terms.size(); ++i)
    {
        if (mask[i])
        {
            std::string term = "x" + std::to_string(i);
            if (terms[i])
                a.push_back(term);
            else
                a.push_back({ "not", term });
        }
    }

    return json({ "and", a });
}

std::string Beast::Expression::Clause::toAlgebraic() const
{
    std::string a;
    for (size_t i = 0; i < terms.size(); ++i)
    {
        if (mask[i])
        {
            std::string term = "x" + std::to_string(i);
            if (!a.empty())
                a.append(" & ");
            if (!terms[i])
                a.append("!");
            a.append(term);
        }
    }
    return a;
}
