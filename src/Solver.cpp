#include "Solver.h"

#include <iostream>
#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"
#include "LPResults.h"

Solver::Solver(const LPModel &model)
{
    if (model.type == LPModel::Type::MAX)
        m_model = model;
    else
        m_model = model.dual();

    const auto nSlack = m_model.nConstraints;
    Eigen::MatrixXd initialTableau(m_model.nConstraints + 1, m_model.nDecisionVars + nSlack + 2);

    populateConstraints(m_model, initialTableau);
    populateObjectiveFunction(m_model, initialTableau);

    m_tableau = std::move(initialTableau);
}

auto Solver::tableau() const -> Eigen::MatrixXd
{
    return m_tableau;
}

auto Solver::isSolved() const -> bool
{
    auto i = m_tableau.rows() - 1;
    return std::none_of(m_tableau.row(i).cbegin(), m_tableau.row(i).cend(), [](const double val) { return val < 0.0; });
}

auto Solver::solve() -> LPResults
{
    std::cout << "Initial Tableau:\n";
    std::cout << m_tableau << '\n' << '\n';

    // TODO: Find a way to detect if it converges
    bool solved = false;
    for (; m_steps < 10; ++m_steps)
    {
        solveStep();
        std::cout << "New Tableau:\n";
        std::cout << m_tableau << '\n' << '\n';

        if (isSolved())
        {
            solved = true;
            break;
        }
    }

    if (solved)
    {
        simplifyObjective();
        std::cout << "Final Tableau:\n";
        std::cout << m_tableau << '\n' << '\n';

        return {m_model, m_tableau};
    }
    else
        throw std::runtime_error("did not converge after 10 steps");
}

auto Solver::solveStep() -> void
{
    const auto [row, col] = findPivot();
    std::cout << "Pivoting on {" << row << ", " << col << "}\n";

    makeBasic(row, col);
}

auto Solver::populateConstraints(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void
{
    const auto nSlack = model.nConstraints;
    const auto &constraints = model.constraints;

    for (Eigen::Index i = 0; i < model.nConstraints; ++i)
    {
        auto rowIt = initialTableau.row(i).begin();

        // copy the coefficients
        rowIt = std::copy(constraints.row(i).cbegin(), constraints.row(i).cend() - 1, rowIt);

        // fill in slackValues values
        for (Eigen::Index s = 0; s < nSlack; ++s)
            *(rowIt++) = s == i ? static_cast<double>(model.constraintOperators[s]) : 0.;

        // z should be 0
        *(rowIt++) = 0;

        // fill in rhs value
        *rowIt = constraints.coeff(i, model.nDecisionVars);
    }
}

auto Solver::populateObjectiveFunction(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void
{
    const auto nSlack = model.nConstraints;

    auto objectiveFunctionFrom = model.objectiveFunction * -1;
    auto objectiveFunctionIt = initialTableau.row(nSlack).begin();

    // copy the opposite of the coefficients
    objectiveFunctionIt = std::copy(objectiveFunctionFrom.cbegin(), objectiveFunctionFrom.cend(), objectiveFunctionIt);

    // slackValues aren't part of the objectiveFunction
    for (auto i = nSlack - 1; i >= 0; --i)
        *(objectiveFunctionIt++) = 0.;

    // z should be 1
    *(objectiveFunctionIt++) = 1.;

    // value should be 0
    *objectiveFunctionIt = 0.;
}

auto Solver::findPivot() const -> std::pair<Eigen::Index, Eigen::Index>
{
    auto objectiveFunctionIt = m_tableau.row(m_tableau.rows() - 1).cbegin();

    // pivot column corresponds to the largest reduced cost coefficient
    const auto pivotCell = std::min_element(objectiveFunctionIt, objectiveFunctionIt + m_model.nDecisionVars);
    std::cout << *pivotCell << '\n';
    auto pivotCol = static_cast<Eigen::Index>(std::distance(objectiveFunctionIt, pivotCell));

    // pivot row corresponds to the row with the lowest quotient
    std::vector<double> quotients;
    auto pivotColIt = (m_tableau.colwise().cbegin() + pivotCol);
    auto rhsIt = m_tableau.colwise().crbegin();
    std::transform(pivotColIt->cbegin(), pivotColIt->cend() - 1, rhsIt->cbegin(), std::back_inserter(quotients),
                   [](const double divisor, const double rhs) {
                       // quotient is invalid if undefined or negative
                       if (divisor == 0)
                           return std::numeric_limits<double>::max();
                       const auto quotient = rhs / divisor;
                       if (quotient < 0)
                           return std::numeric_limits<double>::max();

                       return quotient;
                   });
    auto minElement = std::min_element(quotients.cbegin(), quotients.cend());
    auto pivotRow = static_cast<Eigen::Index>(std::distance(quotients.cbegin(), minElement));

    return std::make_pair(pivotRow, pivotCol);
}

auto Solver::makeBasic(Eigen::Index row, Eigen::Index col) -> void
{
    auto rowIt = m_tableau.rowwise().rbegin();

    const auto b = m_tableau.row(row).coeff(col);
    for (auto i = m_tableau.rows() - 1; i >= 0; --i, ++rowIt)
    {
        if (i == row)
            continue;

        const auto a = rowIt->coeff(col);
        *rowIt *= b;
        *rowIt -= a * m_tableau.row(row);
    }
}

auto Solver::simplifyObjective() -> void
{
    auto objectiveFunctionIt = m_tableau.rowwise().rbegin();
    const auto divisor = objectiveFunctionIt->coeff(m_model.nDecisionVars + m_model.nConstraints);
    *objectiveFunctionIt /= divisor;
}
