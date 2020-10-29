#include <iostream>
#include <map>
#include <string>

class Cli {
public:
  Cli() {
    this->menu["deploy"] = &Cli::deploy;
    this->menu["exit"] = &Cli::exit;
  }

  void run() {
    std::cout << "CODA_ CLI\n";

    while (true) {
      std::cout << "$ ";
      std::cin >> this->userInput;
      this->handleInput(this->userInput);
    }
  }

private:
  std::string userInput;
  std::map<std::string, void (Cli::*)()> menu;

  void handleInput(std::string input) {
    if (this->menu.find(this->userInput) != this->menu.end()) {
      (this->*menu[this->userInput])();
    }
  }

  void deploy() { std::cout << "> deploy\n"; }

  void exit() { ::exit(0); }
};