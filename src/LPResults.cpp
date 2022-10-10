#include "LPResults.h"

#include <iostream>

#include <eigen3/Eigen/Core>

#include "LPModel.h"

LPResults::LPResults(const LPModel &model, const Eigen::MatrixXd &finalTableau)
{
    computeAnswer(finalTableau, model.nConstraints, model.nDecisionVars, 0, decisionValues);
    computeAnswer(finalTableau, model.nConstraints, model.nConstraints, model.nDecisionVars, slackValues);
    computeSensitivity(finalTableau, model.nDecisionVars, 0, reducedCost);
    computeSensitivity(finalTableau, model.nConstraints, model.nDecisionVars, shadowPrice);

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

auto LPResults::computeAnswer(const Eigen::MatrixXd &finalTableau, Eigen::Index nConstraints, Eigen::Index size,
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

auto LPResults::computeSensitivity(const Eigen::MatrixXd &finalTableau, const Eigen::Index size,
                                   const Eigen::Index offset, Eigen::RowVectorXd &param) -> void
{
    param = Eigen::VectorXd(size);
    auto objectiveFunctionIdx = finalTableau.rows() - 1;

    for (Eigen::Index i = 0; i < size; ++i)
        param[i] = finalTableau.coeff(objectiveFunctionIdx, offset + i);
}
