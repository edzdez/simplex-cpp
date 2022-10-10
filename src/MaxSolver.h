#ifndef LP_SOLVER_MAXSOLVER_H
#define LP_SOLVER_MAXSOLVER_H

#include <stdexcept>

#include <eigen3/Eigen/Core>

#include "LPModel.h"
#include "Solver.h"

class MaxSolver : public Solver
{
  public:
    explicit MaxSolver(const LPModel &model);

  private:
    static auto populateConstraints(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;
    static auto populateObjectiveFunction(const LPModel &model, Eigen::MatrixXd &initialTableau) -> void;
};

#endif // LP_SOLVER_MAXSOLVER_H
