#ifndef LP_SOLVER_LPMODEL_H
#define LP_SOLVER_LPMODEL_H

#include <stdexcept>
#include <string_view>
#include <vector>

#include "toml.hpp"
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
    std::vector<Eigen::VectorXd> constraints;
    std::vector<Op> constraintOperators;

    explicit LPModel(const toml::table &tbl);
};

#endif // LP_SOLVER_LPMODEL_H
