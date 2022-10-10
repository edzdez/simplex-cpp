#ifndef LP_SOLVER_MINSOLVER_H
#define LP_SOLVER_MINSOLVER_H

#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"
#include "Solver.h"

class MinSolver : public Solver
{
  public:
    explicit MinSolver(const LPModel &model);

  private:
    static auto populateConstraints(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;
    static auto populateObjectiveFunction(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;
};

#endif // LP_SOLVER_MINSOLVER_H
