#pragma once
#ifndef BEAST_H_INCLUDED
#define BEAST_H_INCLUDED

#include <json.hpp>
#include <utility>
#include <vector>

//! Boolean Expression Simplifier.
//!
//! This uses the Quine–McCluskey algorithm to simplify a boolean expression in disjunctive normal form.
class Beast
{
public:
    class Expression;

    //! Returns a simplified expression.
    //!
    //! @param  json    json object containing the expression to be simplified. This expression must be in disjunctive normal form.
    Expression simplify(nlohmann::json const & json) const;

    //! Returns a simplified expression.
    //!
    //! @param  x    expression to be simplified
    Expression simplify(Expression const & x) const;
};

//! This represents a boolean expression in disjunctive normal form
class Beast::Expression
{
public:
    struct Clause;

    // Constructor
    Expression() {}

    // Constructor
    //!
    //! @param  clauses     vector of clauses that make up the expression
    Expression(std::vector<Clause> const & clauses) : clauses_(clauses) {}

    //! Returns a JSON object representing the expression
    nlohmann::json toJson() const;

    //! Returns a string representing the expression in an algebraic format
    std::string toAlgebraic() const;

private:

    Expression & operator |=(Expression const & rhs);
    Expression & operator &=(Expression const & rhs);
    friend Expression operator |(Expression lhs, Expression const & rhs)
    {
        lhs |= rhs;
        return lhs;
    }

    friend Expression operator &(Expression lhs, Expression const & rhs)
    {
        lhs &= rhs;
        return lhs;
    }

    friend Expression operator ~(Expression const & rhs);

    std::vector<Clause> clauses_;
};

//! Conjunctive clause.
struct Beast::Expression::Clause
{
    std::vector<bool> terms;    //!< unnegated (true) or negated (false)
    std::vector<bool> mask;     //!< Present (true) or not (false)

    //! Ands this clause with another clause
    Clause & operator &=(Clause const & rhs);

    //! Returns a JSON object representing this clause
    nlohmann::json toJson() const;

    //! Returns a string representing the expression in an algebraic format
    std::string toAlgebraic() const;
};

//! Returns the inverse of an expression
Beast::Expression operator ~(Beast::Expression const & rhs);

//! Returns the one clause anded with another clause.
//!
//! @note       A clause with no terms is possible. It represents the value false.
inline Beast::Expression::Clause operator &(Beast::Expression::Clause lhs, Beast::Expression::Clause const & rhs)
{
    lhs &= rhs;
    return lhs;
}

#endif // BEAST_H_INCLUDED
