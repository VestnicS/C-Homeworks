#include <iostream>
#include <map>
#include <string>
#include <stdexcept>

class Expression 
{
public:
    virtual ~Expression() = default;
    virtual void print(std::ostream& os) const = 0;
    virtual double calculate(const std::map<std::string, double>& context) const = 0;
};

class Flyweight : public Expression 
{
    int refCount;
public:
    Flyweight() : refCount(1) {}
    virtual ~Flyweight() = default;
    void addRef() { ++refCount; }
    void release() { if (--refCount == 0) delete this; }
};

class Constant : public Flyweight 
{
    double value;
    Constant(double v) : value(v) {}
    friend class ExpressionFactory;
public:
    void print(std::ostream& os) const override { os << value; }
    double calculate(const std::map<std::string, double>&) const override { return value; }
};

class Variable : public Flyweight 
{
    std::string name;
    Variable(const std::string& n) : name(n) {}
    friend class ExpressionFactory;
public:
    void print(std::ostream& os) const override { os << name; }
    double calculate(const std::map<std::string, double>& context) const override 
    {
        auto it = context.find(name);
        if (it != context.end()) 
            return it->second;
        throw std::runtime_error("Variable " + name + " not found");
    }
};

class ExpressionFactory 
{
    static std::map<double, Constant*> constants;
    static std::map<std::string, Variable*> variables;
public:
    static Constant* createConstant(double value) 
    {
        auto it = constants.find(value);
        if (it != constants.end()) 
        {
            it->second->addRef();
            return it->second;
        }
        Constant* c = new Constant(value);
        constants[value] = c;
        c->addRef();
        return c;
    }

    static Variable* createVariable(const std::string& name) 
    {
        auto it = variables.find(name);
        if (it != variables.end()) 
        {
            it->second->addRef();
            return it->second;
        }
        Variable* v = new Variable(name);
        variables[name] = v;
        v->addRef();
        return v;
    }

    static void release(Expression* e) 
    {
        if (auto f = dynamic_cast<Flyweight*>(e))
            f->release();
        else 
            delete e;
    }

    static void init() 
    {
        for (int i = -5; i <= 256; ++i) 
        {
            createConstant(i);
        }
    }
};

std::map<double, Constant*> ExpressionFactory::constants;
std::map<std::string, Variable*> ExpressionFactory::variables;

class BinaryOperation : public Expression 
{
protected:
    Expression* left;
    Expression* right;
public:
    BinaryOperation(Expression* l, Expression* r) : left(l), right(r) {}
    virtual ~BinaryOperation() 
    {
        ExpressionFactory::release(left);
        ExpressionFactory::release(right);
    }
};

class Addition : public BinaryOperation 
{
public:
    Addition(Expression* l, Expression* r) : BinaryOperation(l, r) {}
    void print(std::ostream& os) const override
    {
        os << '(';
        left->print(os);
        os << '+';
        right->print(os);
        os << ')';
    }
    double calculate(const std::map<std::string, double>& context) const override 
    {
        return left->calculate(context) + right->calculate(context);
    }
};

class Subtraction : public BinaryOperation 
{
public:
    Subtraction(Expression* l, Expression* r) : BinaryOperation(l, r) {}
    void print(std::ostream& os) const override 
    {
        os << '(';
        left->print(os);
        os << '-';
        right->print(os);
        os << ')';
    }
    double calculate(const std::map<std::string, double>& context) const override 
    {
        return left->calculate(context) - right->calculate(context);
    }
};

class Multiplication : public BinaryOperation 
{
public:
    Multiplication(Expression* l, Expression* r) : BinaryOperation(l, r) {}
    void print(std::ostream& os) const override 
    {
        os << '(';
        left->print(os);
        os << '*';
        right->print(os);
        os << ')';
    }
    double calculate(const std::map<std::string, double>& context) const override 
    {
        return left->calculate(context) * right->calculate(context);
    }
};

class Division : public BinaryOperation 
{
public:
    Division(Expression* l, Expression* r) : BinaryOperation(l, r) {}
    void print(std::ostream& os) const override 
    {
        os << '(';
        left->print(os);
        os << '/';
        right->print(os);
        os << ')';
    }
    double calculate(const std::map<std::string, double>& context) const override 
    {
        return left->calculate(context) / right->calculate(context);
    }
};

int main() 
{
    ExpressionFactory::init();

    Constant* two = ExpressionFactory::createConstant(2);
    Variable* x = ExpressionFactory::createVariable("x");
    Addition* expr = new Addition(two, x);

    std::map<std::string, double> context;
    context["x"] = 3;

    std::cout << "Expression: ";
    expr->print(std::cout);
    std::cout << " = " << expr->calculate(context) << std::endl;

    delete expr;
}