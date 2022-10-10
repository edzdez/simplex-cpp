#include "LPModel.h"
#include "Solver.h"

auto main() -> int
{
    LPModel model;
    model.nDecisionVars = 3;
    model.nConstraints = 3;

    model.objectiveFunction = Eigen::Vector3d(2., 3., 5.);

    model.constraints = std::vector(3, Eigen::VectorXd(4));

    model.constraints[0][0] = 2.;
    model.constraints[0][1] = 2.;
    model.constraints[0][2] = 1.;
    model.constraints[0][3] = 60.;

    model.constraints[1][0] = 1.;
    model.constraints[1][1] = 1.;
    model.constraints[1][2] = 3.;
    model.constraints[1][3] = 40.;

    model.constraints[2][0] = 4.;
    model.constraints[2][1] = 2.;
    model.constraints[2][2] = 5.;
    model.constraints[2][3] = 80.;

    auto solver = Solver(model);

    solver.solve();
}
