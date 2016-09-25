#pragma once
#include <vector>
#include <cstdint>
#include <cmath>

class Neuron
{
private:
    std::vector<Neuron*> m_inputs, m_outputs;
    std::vector<float> m_weights;

    float m_expected;

public:
    Neuron() {}
    virtual float get_output();
    void set_expected(float expected) { m_expected = expected; }
    void add_input(Neuron* n, float weight = nan("NULL"));
    void add_output(Neuron* n) { m_outputs.push_back(n); }
};

class InputNeuron : public Neuron
{
private:
    float m_value;

public:
    void set_value(float val) { m_value = val; }
    virtual float get_output() { return m_value; }
};

class MLP
{
private:
    std::vector<std::vector<Neuron> > m_neurons;
    std::vector<InputNeuron> m_inputs;
    void set_expected(uint32_t cl);
    void set_input(std::vector<float> &input);
    void generate_links();

public:
    MLP(std::vector<uint32_t> layers);

    void train(std::vector<float> &input, uint32_t exp_class);
};
