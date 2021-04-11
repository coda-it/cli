#ifndef TYPES_H
#define TYPES_H

class Cli;

using Handler = void (Cli::*)();

struct MenuItem {
  std::map<std::string, MenuItem> child;
  Handler handler;
};

#endif /* TYPES_H */