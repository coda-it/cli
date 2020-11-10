#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

#define DEFAULT_PATH "~"
#define COMMAND_SEPARATOR " "

class Cli {
public:
  Cli() {
    this->menu["add-deploy"] = &Cli::storeDeploy;
    this->menu["deploy"] = &Cli::deploy;
    this->menu["exit"] = &Cli::exit;
  }

  void run() {
    std::cout << "CODA_ CLI\n";
    std::cout << "< ";
    while (std::getline(std::cin, this->userInput)) {
      this->handleInput(this->userInput);
      std::cout << "< ";
    }
  }

private:
  std::string userInput;
  std::map<std::string, std::string> deployCommand;
  std::map<std::string, void (Cli::*)(std::string)> menu;

  void handleInput(std::string input) {
    std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR);
    std::string command = inputVector[0];

    if (this->menu.find(command) != this->menu.end()) {
      (this->*menu[command])(this->userInput);
    }
  }

  void storeDeploy(std::string input) {
    std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR, 3);
    std::cout << "Adding: " + inputVector[2] + "/" + inputVector[1] + "/" +
                     inputVector[0];
    this->deployCommand[inputVector[1]] = inputVector[2];
  }

  void deploy(std::string input) {
    std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR);
    std::string deployment = inputVector[1];

    if (this->deployCommand.find(deployment) != this->deployCommand.end()) {
      const char *cmd = this->deployCommand[deployment].c_str();
      std::system(cmd);
    }
  }

  void exit(std::string input) { ::exit(0); }
};