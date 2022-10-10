#ifndef LP_SOLVER_LPMODEL_H
#define LP_SOLVER_LPMODEL_H

#include <vector>

#include <eigen3/Eigen/Core>

struct LPModel
{
    Eigen::Index nDecisionVars;
    Eigen::Index nConstraints;

    Eigen::VectorXd objectiveFunction;
    // TODO: Store <=, >=, or =
    std::vector<Eigen::VectorXd> constraints;
};

#endif // LP_SOLVER_LPMODEL_H
