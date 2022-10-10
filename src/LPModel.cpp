#include "LPModel.h"

#include <iostream>

#include <stdexcept>
#include <string_view>
#include <vector>

#include "toml.hpp"
#include <eigen3/Eigen/Core>

LPModel::LPModel(const toml::table &tbl)
{
    auto typeStr = *tbl["type"].value<std::string_view>();
    if (typeStr == "max")
        type = Type::MAX;
    else if (typeStr == "min")
        type = Type::MIN;
    else
        throw std::runtime_error("invalid type");

    nDecisionVars = tbl["decisionVariables"]["num"].as_integer()->get();
    nConstraints = tbl["constraints"]["num"].as_integer()->get();

    auto objectiveFunctionArray = tbl["decisionVariables"]["objectiveFunction"].as_array();
    objectiveFunction = Eigen::RowVectorXd(nDecisionVars);
    std::transform(objectiveFunctionArray->cbegin(), objectiveFunctionArray->cend(), objectiveFunction.begin(),
                   [](const toml::node &i) { return *i.value<double>(); });

    auto constraintList = tbl["constraints"]["coefficients"].as_array()->cbegin();
    auto rhsList = tbl["constraints"]["rhs"].as_array()->cbegin();
    constraints = Eigen::MatrixXd(nConstraints, nDecisionVars + 1);

    for (Eigen::Index constraintIdx = 0; constraintIdx < nConstraints; ++constraintIdx)
    {
        auto constraint = (constraintList++)->as_array()->cbegin();
        for (Eigen::Index coeffIdx = 0; coeffIdx < nDecisionVars; ++coeffIdx)
        {
            const auto coeff = *(constraint++)->value<double>();
            auto *ref = &constraints.coeffRef(constraintIdx, coeffIdx);
            *ref = coeff;
        }

        const auto rhs = *(rhsList++)->value<double>();
        auto *ref = &constraints.coeffRef(constraintIdx, nDecisionVars);
        *ref = rhs;
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

auto LPModel::dual() const -> LPModel
{
    auto model = LPModel();
    model.isDual = true;
    model.type = type == Type::MAX ? Type::MIN : Type::MAX;
    model.nDecisionVars = nConstraints;
    model.nConstraints = nDecisionVars;

    model.objectiveFunction = constraints.col(nDecisionVars);

    model.constraints = Eigen::MatrixXd(nDecisionVars, nConstraints + 1);
    const auto transposed = constraints.transpose().topRows(nDecisionVars);
    for (Eigen::Index row = 0; row < transposed.rows(); ++row)
    {
        for (Eigen::Index col = 0; col < transposed.cols(); ++col)
        {
            auto *coeff = &model.constraints.coeffRef(row, col);
            *coeff = transposed.coeff(row, col);
        }
    }
    model.constraints.col(model.constraints.cols() - 1) = objectiveFunction;

    model.constraintOperators = std::vector<Op>(nConstraints);
    std::transform(constraintOperators.cbegin(), constraintOperators.cend(), model.constraintOperators.begin(),
                   [](Op op) { return static_cast<Op>(-static_cast<int>(op)); });

    return model;
}
