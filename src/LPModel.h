#ifndef LP_SOLVER_LPMODEL_H
#define LP_SOLVER_LPMODEL_H

#include <vector>

#include <eigen3/Eigen/Core>

struct LPModel
{
    enum class Op : int
    {
        LT = 1,
        EQ = 0,
        GT = -1,
    };

    Eigen::Index nDecisionVars;
    Eigen::Index nConstraints;

    Eigen::VectorXd objectiveFunction;
    // TODO: Store <=, >=, or =
    std::vector<Eigen::VectorXd> constraints;
    std::vector<Op> constraintOperators;
};

#endif // LP_SOLVER_LPMODEL_H
