#include "persistence.h"
#include "types.h"
#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <vector>

#define COMMAND_SEPARATOR " "
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_ENTER 13

class Cli {
public:
  int selected = 0;

  Cli() {
    this->persistence = new Persistence();
    std::map<std::string, MenuItem> deploy;
    deploy["add-deploy"].handler = &Cli::deploy;
    deploy["deploy"].handler = &Cli::deploy;
    this->selectedItem->child["deployment"].child = deploy;
    this->selectedItem->child["exit"].handler = &Cli::exit;
  }

  void render() {
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    std::cout << "=== CODA_ CLI ===\n\r\n\r";

    int index = 0;

    for (auto const &item : this->selectedItem->child) {
      if (index == this->selected) {
        std::cout << "> " + item.first + "\n\r";
      } else {
        std::cout << "  " + item.first + "\n\r";
      }

      index++;
    }
    // hide cursor
    std::cout << "\e[?25l";
  }

  void run() {
    system("stty raw");

    int key;
    do {
      this->render();
      key = getchar();
      this->handleInput(key);
    } while (true);
  }

private:
  Persistence *persistence;
  std::string userInput;
  MenuItem menu;
  MenuItem *selectedItem = &this->menu;

  void handleInput(char input) {
    if (input == KEY_UP && this->selected > 0) {
      this->selected -= 1;
    } else if (input == KEY_DOWN &&
               this->selected < this->selectedItem->child.size() - 1) {
      this->selected += 1;
    } else if (input == KEY_ENTER) {
      int index = 0;
      for (auto const &item : this->selectedItem->child) {
        if (index == this->selected) {
          if (this->selectedItem->child.size() == 0) {
            (this->*selectedItem->child[item.first].handler)();
          } else {
            this->selectedItem = &this->selectedItem->child[item.first];
          }
        }
        index++;
      }
    }
  }

  void deploy() {}

  void exit() {
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    // show cursor
    std::cout << "\e[?25h";
    system("stty cooked");
    ::exit(0);
  }
};