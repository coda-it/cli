#include "persistence.h"
#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

#define COMMAND_SEPARATOR " "
#define KEY_SEPARATOR "x"
#define KEY_DEPLOYMENT "deployment"
#define KEY_PROVISION "provision"

class Cli {
public:
  Cli() {
    this->persistence = new Persistence();
    this->menu["add-deploy"] = &Cli::storeDeploy;
    this->menu["deploy"] = &Cli::deploy;
    this->menu["add-provision"] = &Cli::storeProvision;
    this->menu["provision"] = &Cli::provision;
    this->menu["exit"] = &Cli::exit;

    this->command = this->persistence->getAllRecords();
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
  Persistence *persistence;
  std::string userInput;
  std::map<std::string, std::string> command;
  std::map<std::string, void (Cli::*)(std::string)> menu;

  void handleInput(std::string input) {
    const std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR);
    const std::string command = inputVector[0];

    if (this->menu.find(command) != this->menu.end()) {
      (this->*menu[command])(this->userInput);
    }
  }

  void store(std::string input, std::string prefix) {
    const std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR, 3);
    const std::string key = prefix + KEY_SEPARATOR + inputVector[1];
    const std::string value = inputVector[2];

    this->persistence->addRecord(key, value);
    this->command[key] = value;

    std::cout << "> " + prefix + " stored\n";
  }

  void storeDeploy(std::string input) { this->store(input, "deployment"); }

  void storeProvision(std::string input) { this->store(input, "provision"); }

  void execute(std::string input, std::string prefix) {
    const std::vector<std::string> inputVector =
        coda::split(input, COMMAND_SEPARATOR);
    const std::string key =
        prefix + std::string(KEY_SEPARATOR) + inputVector[1];

    if (this->command.find(key) != this->command.end()) {
      const char *cmd = this->command[key].c_str();
      std::system(cmd);
    }
  }

  void deploy(std::string input) { this->execute(input, KEY_DEPLOYMENT); }

  void provision(std::string input) { this->execute(input, KEY_PROVISION); }

  void exit(std::string input) { ::exit(0); }
};