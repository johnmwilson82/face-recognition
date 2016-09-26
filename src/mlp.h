#pragma once
#include <vector>
#include <list>
#include <cstdint>
#include <cmath>
#include "classifier.h"

class Neuron;

class Edge
{
private:
    Neuron *m_input, *m_output;
    float m_weight;

public:
    Edge(Neuron *input, Neuron *output, float weight = nan(""));
    float get_weight() { return m_weight; }
    void set_weight(float weight) { m_weight = weight; }
    inline float get_output();
    inline float get_unweighted_output();
};

class Neuron
{
private:
    std::list<Edge*> m_inputs, m_outputs;
    float m_bias;
    float m_expected;

public:
    Neuron(float bias = 0.0) : m_bias(bias) {}

    virtual float get_output();
    void set_expected(float expected) { m_expected = expected; }
    void add_input(Edge* n) { m_inputs.push_back(n); }
    void add_output(Edge* n) { m_outputs.push_back(n); }
    const std::list<Edge*>& get_input_edges() const { return m_inputs; }
    const std::list<Edge*>& get_output_edges() const { return m_outputs; }

    static float activation_function(float in) { return 1.0f / (1.0f + exp(-in)); }
    static float derivative_af(float in) { return exp(in) / ((exp(in) + 1) * (exp(in) + 1)); }
};

class InputNeuron : public Neuron
{
private:
    float m_value;

public:
    //InputNeuron(float bias = 0.0) : Neuron(bias) {}
    void set_value(float val) { m_value = val; }
    virtual float get_output() { return m_value; }
};

class MLP : public Classifier
{
private:
    float m_learning_rate;

    std::vector<std::vector<Neuron> > m_neurons;
    std::vector<InputNeuron> m_inputs;
    std::list<Edge> m_edges;

    void set_expected(uint32_t cl);
    void generate_links();
    VectorXf get_output(const VectorXf &input);

public:
    MLP(std::vector<uint32_t> layers);

    float sq_error(const VectorXf &d, const VectorXf &y);

    virtual uint32_t classify(const VectorXf &input);
    virtual const std::string get_name() const { return std::string("MLP"); }
    virtual void train(const VectorXf &input, uint32_t exp_class);
};
