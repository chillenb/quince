
#include <string>

struct employee
{
    int age;
    std::string surname;
    std::string forename;
    double salary;
};

struct employee parseEmployee(const std::string &input);

