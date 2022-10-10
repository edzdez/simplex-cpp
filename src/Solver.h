#ifndef LP_SOLVER_SOLVER_H
#define LP_SOLVER_SOLVER_H

#include <iostream>
#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"

class Solver
{
  public:
    explicit Solver(const LPModel &model);

    [[nodiscard]] auto tableau() const -> Eigen::MatrixXd;
    [[nodiscard]] auto isSolved() const -> bool;

    auto solve() -> void;
    auto solveStep() -> void;

  private:
    static auto populateConstraints(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;
    static auto populateObjectiveFunction(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;

    [[nodiscard]] auto findPivot() const -> std::pair<Eigen::Index, Eigen::Index>;
    auto makeBasic(Eigen::Index row, Eigen::Index col) -> void;
    auto simplifyObjective() -> void;

    LPModel m_model;
    Eigen::MatrixXd m_tableau;
    size_t m_steps{0};
};

#endif // LP_SOLVER_SOLVER_H
