#pragma once
#ifndef BEAST_H_INCLUDED
#define BEAST_H_INCLUDED

#include <json.hpp>
#include <vector>
#include <utility>

//! Boolean Expression Simplifier.
//! This class uses the Quine–McCluskey algorithm to simplify a boolean expression in disjunctive normal form.
class Beast
{
public:
	class Expression;
	
	Expression simplify(nlohmann::json const & json) const;
};

//! An Expression represents a boolean expression in disjunctive normal form
class Beast::Expression
{
public:
	struct Clause;

	// Constructor
	Expression() {}

	// Constructor
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
//! The each element of each vector represents a variable. The values of the elements in the first vector represent whether
//! the variable is . The values of the elements in the second vector represent whether
//! the variable is present in the clause.
struct Beast::Expression::Clause
{
	std::vector<bool> terms;    //!< unnegated (true) or negated (false)
	std::vector<bool> mask;     //!< Present (true) or not (false)

	Clause & operator &=(Clause const & rhs);
	friend Clause operator &(Clause lhs, Clause const & rhs)
	{
		lhs &= rhs;
		return lhs;
	}

	//! Returns a JSON object representing this clause
	nlohmann::json toJson() const;

	//! Returns a string representing the expression in an algebraic format
	std::string toAlgebraic() const;
};


Beast::Expression operator ~(Beast::Expression const & rhs);

#endif // BEAST_H_INCLUDED