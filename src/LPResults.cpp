#include "LPResults.h"

#include <iostream>
#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"

LPResults::LPResults(const LPModel &model, const Eigen::MatrixXd &finalTableau)
{
    if (model.type == LPModel::Type::MAX)
    {
        computeBasicValue(finalTableau, model.nConstraints, model.nDecisionVars, 0, decisionValues);
        computeBasicValue(finalTableau, model.nConstraints, model.nConstraints, model.nDecisionVars, slackValues);
        computeFromIndicators(finalTableau, model.nDecisionVars, 0, reducedCost);
        computeFromIndicators(finalTableau, model.nConstraints, model.nDecisionVars, shadowPrice);
    }
    else if (model.type == LPModel::Type::MIN)
    {
        computeFromIndicators(finalTableau, model.nDecisionVars, model.nConstraints, decisionValues);
    }
    else
        throw std::runtime_error("invalid model type");

    finalResult = finalTableau.coeff(finalTableau.rows() - 1, finalTableau.cols() - 1);
}

auto LPResults::printResults() const -> void
{
    std::cout << "================================================\n\n";

    std::cout << "Answer Report:\n";
    std::cout << "Objective Value: " << finalResult << "\n\n";

    std::cout << "Decision Values:\n";
    std::cout << decisionValues << "\n\n";

    std::cout << "Slack Values:\n";
    std::cout << slackValues << "\n\n";

    std::cout << "Sensitivity Report:\n";
    std::cout << "Reduced Cost:\n";
    std::cout << reducedCost << "\n\n";

    std::cout << "Shadow Price:\n";
    std::cout << shadowPrice << "\n\n";

    std::cout << "================================================\n\n";
}

auto LPResults::computeBasicValue(const Eigen::MatrixXd &finalTableau, Eigen::Index nConstraints, Eigen::Index size,
                                  Eigen::Index offset, Eigen::RowVectorXd &param) -> void
{
    param = Eigen::VectorXd(size);

    for (Eigen::Index col = 0; col < size; ++col)
    {
        int nonZero{};
        for (Eigen::Index row = 0; row < nConstraints; ++row)
        {
            const auto coeff = finalTableau.coeff(row, col + offset);
            if (coeff != 0)
            {
                const auto rhs = finalTableau.coeff(row, finalTableau.cols() - 1);
                param[col] = rhs / coeff;
                ++nonZero;
            }
        }

        // if the variable isn't basic, it's not in the optimal solution
        if (nonZero != 1)
            param[col] = 0;
    }
}

auto LPResults::computeFromIndicators(const Eigen::MatrixXd &finalTableau, long size, long offset,
                                      Eigen::RowVectorXd &param) -> void
{
    param = Eigen::VectorXd(size);
    auto objectiveFunctionIdx = finalTableau.rows() - 1;

    for (Eigen::Index i = 0; i < size; ++i)
        param[i] = finalTableau.coeff(objectiveFunctionIdx, offset + i);
}
