#pragma once
#include <string>
#include <eigen3/Eigen/Dense>

using Eigen::MatrixXf;
using Eigen::VectorXf;

class Classifier
{
public:
    virtual void train(const VectorXf &input, uint32_t exp_class) = 0;
    virtual uint32_t classify(const VectorXf &input) = 0;
    virtual const std::string get_name() const = 0;
};
