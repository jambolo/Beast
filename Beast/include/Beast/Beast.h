#ifndef BEAST_H_INCLUDED
#define BEAST_H_INCLUDED

#pragma once

#include <nlohmann/json.hpp>
// #include <utility>
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
    static Expression simplify(nlohmann::json const & json);

    //! Returns a simplified expression.
    //!
    //! @param  x    expression to be simplified
    static Expression simplify(Expression const & x);
};

//! This represents a boolean expression in disjunctive normal form.
class Beast::Expression
{
public:
    struct Clause;

    //! Constructor.
    Expression() = default;

    //! Constructor.
    //!
    //! @param  clauses     vector of clauses that make up the expression.
    Expression(std::vector<Clause> const & clauses) : clauses_(clauses) {}

    //! Returns a JSON object representing the expression.
    nlohmann::json toJson() const;

    //! Returns a string representing the expression in an algebraic format.
    std::string toAlgebraic() const;

    //! Returns this expression ored with another.
    Expression & operator |=(Expression const & rhs);

    //! Returns this expression anded with another.
    Expression & operator &=(Expression const & rhs);

    //! Returns the inverse
    Expression operator ~();

private:
    std::vector<Clause> clauses_;
};

inline Beast::Expression operator |(Beast::Expression lhs, Beast::Expression const & rhs)
{
    return lhs |= rhs;
}

inline Beast::Expression operator &(Beast::Expression lhs, Beast::Expression const & rhs)
{
    return lhs &= rhs;
}

//! Conjunctive clause.
struct Beast::Expression::Clause
{
    std::vector<bool> terms;    //!< unnegated (true) or negated (false).
    std::vector<bool> mask;     //!< Present (true) or not (false).

    //! Ands this clause with another clause
    Clause & operator &=(Clause const & rhs);

    //! Returns a JSON object representing this clause.
    nlohmann::json toJson() const;

    //! Returns a string representing the expression in an algebraic format.
    std::string toAlgebraic() const;
};

//! Returns the one clause anded with another clause.
//!
//! @note       A clause with no terms represents the value false.
inline Beast::Expression::Clause operator &(Beast::Expression::Clause lhs, Beast::Expression::Clause const & rhs)
{
    return lhs &= rhs;
}

//! Returns an expression oring one clause with another clause.
//!
//! @note       A clause with no terms represents the value false.
inline Beast::Expression operator |(Beast::Expression::Clause const & lhs, Beast::Expression::Clause const & rhs)
{
    return std::vector<Beast::Expression::Clause> { lhs, rhs };
}

#endif // BEAST_H_INCLUDED
