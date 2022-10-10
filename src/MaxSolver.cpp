#include "MaxSolver.h"

#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"
#include "Solver.h"

MaxSolver::MaxSolver(const LPModel &model)
    : Solver(model)
{
    const auto nSlack = model.nConstraints;
    Eigen::MatrixXd initialTableau(model.nConstraints + 1, model.nDecisionVars + nSlack + 2);

    populateConstraints(model, initialTableau);
    populateObjectiveFunction(model, initialTableau);

    m_tableau = std::move(initialTableau);
}

auto MaxSolver::populateConstraints(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void
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

auto MaxSolver::populateObjectiveFunction(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void
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
