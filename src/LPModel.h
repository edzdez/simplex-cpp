#ifndef LP_SOLVER_LPMODEL_H
#define LP_SOLVER_LPMODEL_H

#include <stdexcept>
#include <string_view>
#include <vector>

#include "toml.hpp"
#include <eigen3/Eigen/Core>

struct LPModel
{
    enum class Type : int
    {
        MIN = 0,
        MAX = 1,
    };

    enum class Op : int
    {
        LT = 1,
        EQ = 0,
        GT = -1,
    };

    bool isDual{false};
    Type type;

    Eigen::Index nDecisionVars;
    Eigen::Index nConstraints;

    Eigen::VectorXd objectiveFunction;
    Eigen::MatrixXd constraints;
    std::vector<Op> constraintOperators;

    LPModel() = default;
    explicit LPModel(const toml::table &tbl);

    [[nodiscard]] auto dual() const -> LPModel;
};

#endif // LP_SOLVER_LPMODEL_H
