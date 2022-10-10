#include <memory>
#include <stdexcept>

#include "toml.hpp"

#include "LPModel.h"
#include "MaxSolver.h"
#include "MinSolver.h"
#include "Solver.h"

auto main(int argc, char **argv) -> int
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
        return EXIT_FAILURE;
    }

    toml::table tbl;
    try
    {
        tbl = toml::parse_file(argv[1]);
    }
    catch (const toml::parse_error &err)
    {
        std::cout << "Failed to parse:\n" << err << '\n';
        return EXIT_FAILURE;
    }

    auto model = LPModel(tbl);

    std::unique_ptr<Solver> solver;
    if (model.type == LPModel::Type::MAX)
        solver = std::make_unique<MaxSolver>(model);
    else if (model.type == LPModel::Type::MIN)
        solver = std::make_unique<MinSolver>(model);
    else
        throw std::runtime_error("unknown solver");

    auto results = solver->solve();
    results.printResults();

    return EXIT_SUCCESS;
}
