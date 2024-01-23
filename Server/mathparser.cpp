#include "include/logging.h"
#include "include/mathparser.h"

namespace Serf
{
Operation::Operation(char value) : precedence([value]{
                                    if(value == '*' || value == '/'){
                                        LOGGING_SOURCES(normal, "Приоритет операции умножение и деление" + 2);
                                        return 2;
                                    }else {
                                        LOGGING_SOURCES(normal, "Приоритет операции сложение и вычитание" + 1);
                                        return 1;
                                    } }()),
                                        _op(value) {}



int Operation::Evaluate() const {
    if (_op == '*'){
        LOGGING_SOURCES(normal, "Операция умножение");
        return _left->Evaluate() * _right->Evaluate();
    }else if (_op == '+'){
        LOGGING_SOURCES(normal, "Операция сложения");
        return _left->Evaluate() + _right->Evaluate();
    }else if (_op == '-'){
        LOGGING_SOURCES(normal, "Операция вычитания");
        return _left->Evaluate() - _right->Evaluate();
    }else if (_op == '/'){
        LOGGING_SOURCES(normal, "Операция деления");
        if (_right->Evaluate() == 0){   
            LOGGING_SOURCES(critical, "Деление на ноль запрещено");
            std::string del = std::to_string(_left->Evaluate()) + "/" + std::to_string(_right->Evaluate()) ;
            LOGGING_SOURCES(warning, "Убрал из формулф деление на ноль: " + del);         
            return 0;
        }
        return _left->Evaluate() / _right->Evaluate();
    }
    return 0;
}

void Operation::SetLeft(std::shared_ptr<Node> node) { _left = node; }
void Operation::SetRight(std::shared_ptr<Node> node) { _right = node; }

}


