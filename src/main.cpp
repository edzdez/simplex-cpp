#include <memory>
#include <stdexcept>

#include "toml.hpp"

#include "LPModel.h"
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
    auto solver = Solver(model);

    auto results = solver.solve();
    results.printResults();

    return EXIT_SUCCESS;
}
