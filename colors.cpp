#include <string>
std::string getColorCode(std::string color) {
  if (color == "red")
    return "\033[31m";
  else if (color == "green")
    return "\033[32m";
  else if (color == "yellow")
    return "\033[33m";
  else if (color == "blue")
    return "\033[34m";
  else if (color == "magenta")
    return "\033[35m";
  else if (color == "cyan")
    return "\033[36m";
  else if (color == "white")
    return "\033[37m";
  else
    return "\033[0m"; // Default color
}
