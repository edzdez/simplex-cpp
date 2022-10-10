#include "LPModel.h"

#include <stdexcept>
#include <string_view>
#include <vector>

#include "toml.hpp"
#include <eigen3/Eigen/Core>

LPModel::LPModel(const toml::table &tbl)
{
    nDecisionVars = tbl["decisionVariables"]["num"].as_integer()->get();
    nConstraints = tbl["constraints"]["num"].as_integer()->get();

    auto objectiveFunctionArray = tbl["decisionVariables"]["objectiveFunction"].as_array();
    objectiveFunction = Eigen::Vector3d(nDecisionVars);
    std::transform(objectiveFunctionArray->cbegin(), objectiveFunctionArray->cend(), objectiveFunction.begin(),
                   [](const toml::node &i) { return *i.value<double>(); });

    auto constraintList = tbl["constraints"]["coefficients"].as_array()->cbegin();
    auto rhsList = tbl["constraints"]["rhs"].as_array()->cbegin();
    constraints = std::vector(3, Eigen::VectorXd(4));

    for (Eigen::Index constraintIdx = 0; constraintIdx < nConstraints; ++constraintIdx)
    {
        auto constraint = (constraintList++)->as_array()->cbegin();
        for (Eigen::Index coeffIdx = 0; coeffIdx < nDecisionVars; ++coeffIdx)
        {
            const auto coeff = *(constraint++)->value<double>();
            constraints[constraintIdx][coeffIdx] = coeff;
        }

        const auto rhs = *(rhsList++)->value<double>();
        constraints[constraintIdx][nDecisionVars] = rhs;
    }

    auto operations = tbl["constraints"]["operations"].as_array();
    constraintOperators = std::vector<LPModel::Op>(3);
    std::transform(operations->cbegin(), operations->cend(), constraintOperators.begin(), [](const toml::node &i) {
        const auto str = i.value<std::string_view>();

        if (str == "lt")
            return Op::LT;
        if (str == "eq")
            return Op::EQ;
        if (str == "gt")
            return Op::GT;

        throw std::runtime_error("invalid operation");
    });
}
