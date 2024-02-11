
#pragma once
#include <iostream>
#include <memory>
#include <stack>
#include <string>

namespace Serf
{

    struct Node
    {
    public:
        virtual double Evaluate() const = 0;
    };

    class Value : public Node
    {
    public:
        Value(double number) : _value(number) {}
        double Evaluate() const override { return _value; }

    private:
        const double _value;
    };

    class Operation : public Node
    {
    public:
        Operation(char value);

        const int precedence;

        double Evaluate() const override ;

        void SetLeft(std::shared_ptr<Node> node);
        void SetRight(std::shared_ptr<Node> node);
        bool GetFlagDivisionZero(){return flagZero;}

    private:
        const char _op;
        bool flagZero;
        std::shared_ptr<const Node> _left, _right;
    };

    class mathparse {
    public:
    template <class Iterator>
    std::shared_ptr<Node> Parse(Iterator& token, Iterator end);

    };

template <class Iterator>
std::shared_ptr<Node> mathparse::Parse(Iterator& token, Iterator end){
    std::stack<std::shared_ptr<Node>> values;
    std::stack<std::shared_ptr<Operation>> ops;

    auto PopOps = [&](int precedence){
        while (!ops.empty() && ops.top()->precedence >= precedence){
            auto value1 = values.top();
            values.pop();
            auto value2 = values.top();
            values.pop();
            auto op = ops.top();
            ops.pop();

            op->SetRight(value1);
            op->SetLeft(value2);

            values.push(op);
        }
    };

    while (token != end){
        if (*token >= '0' && *token <= '9'){
            std::string number;
            while (token != end && *token >= '0' && *token <= '9'){
                number += *token;
                ++token;
            }
            double value = std::stoi(number);
            values.push(std::make_shared<Value>(value));
            continue;
        }
        const auto &value = *token;
        if (value == '*' || value == '/'){
            PopOps(2);
            ops.push(std::make_shared<Operation>(value));
        }else if (value == '+' || value == '-'){
            PopOps(1);
            ops.push(std::make_shared<Operation>(value));
        }

        ++token;
    }

    while (!ops.empty()){
        PopOps(0);
    }

    return values.top();
}

}
