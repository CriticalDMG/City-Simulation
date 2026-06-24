#ifndef COMMAND_H
#define COMMAND_H

class Simulation; 

#include <vector>
#include <string>

//abstract base class for the Command Design Pattern.
//encapsulates a request as an object, allowing parameterization of clients.
class Command
{
public:
    virtual ~Command() = default;

    virtual void execute(Simulation& context, const std::vector<std::string>& args) = 0;
};

//add command
class AddCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args) override;
};

//Step command
class StepCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args);
};

//Exit command
class ExitCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args);
};

//generate command
class GenerateCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args);
};

//remove command
class RemoveCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args);
};

//info cammand
class InfoCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args) override;
};

//stat command
class StatCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args) override;
};

//save command
class SaveCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args) override;
};

//load command
class LoadCommand : public Command
{
public:
    void execute(Simulation& context, const std::vector<std::string>& args) override;
};

#endif // COMMAND_H