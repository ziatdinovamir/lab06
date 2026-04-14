#include "formatter.h"

std::string formatter(const std::string& message)
{
    std::string res;
    res += "-------------------------\n";
    res += message + "\n";
    res += "-------------------------\n";
    return res;
}

void formatter(std::ostream& out, const std::string& message)
{
    out << "-------------------------\n" << message << "\n-------------------------\n";
}
