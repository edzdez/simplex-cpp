#ifndef LP_SOLVER_LPRESULTS_H
#define LP_SOLVER_LPRESULTS_H

#include <iostream>

#include <eigen3/Eigen/Core>

#include "LPModel.h"

// TODO: Find a way to compute allowable increase/decrease
struct LPResults
{
    Eigen::RowVectorXd decisionValues;
    Eigen::RowVectorXd slackValues;
    Eigen::RowVectorXd reducedCost;
    Eigen::RowVectorXd shadowPrice;
    double finalResult;

    LPResults(const LPModel &model, const Eigen::MatrixXd &finalTableau);
    auto printResults() const -> void;

  private:
    static auto computeAnswer(const Eigen::MatrixXd &finalTableau, Eigen::Index nConstraints, Eigen::Index size,
                              Eigen::Index offset, Eigen::RowVectorXd &param) -> void;
    static auto computeSensitivity(const Eigen::MatrixXd &finalTableau, Eigen::Index size, Eigen::Index offset,
                                   Eigen::RowVectorXd &param) -> void;
};

#endif // LP_SOLVER_LPRESULTS_H
