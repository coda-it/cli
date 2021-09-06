#include "persistence.h"
#include "types.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define COMMAND_SEPARATOR " "
#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_LEFT 68
#define KEY_ENTER 10
#define KEY_SQUARE_BRACKER 91
#define KEY_ESC 27
#define STDIN 0

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

    tcgetattr(fileno(stdin), &this->defaultTTYSettings);
    struct termios newTTYSettings = defaultTTYSettings;
    newTTYSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newTTYSettings);
  }

  void render() {
    // hide cursor
    std::cout << "\e[?25l";
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    std::cout << "\n=== CODA_ CLI ===\n\r\n\r";

    int index = 0;

    for (auto const &item : this->selectedItem->child) {
      if (index == this->selected) {
        std::cout << "> " + item.first + "\n\r";
      } else {
        std::cout << "  " + item.first + "\n\r";
      }

      index++;
    }
  }

  void run() {
    do {
      this->render();
      this->handleInput(this->getInput());

    } while (true);
  }

private:
  Persistence *persistence;
  std::string userInput;
  MenuItem menu;
  MenuItem *selectedItem = &this->menu;
  MenuItem *path[2];
  int level = 0;
  bool shouldRerender = false;
  struct termios defaultTTYSettings;

  int getInput() {
    fd_set set;

    struct timeval tv;
    tv.tv_sec = 10;
    tv.tv_usec = 0;

    FD_ZERO(&set);
    FD_SET(fileno(stdin), &set);

    select(fileno(stdin) + 1, &set, NULL, NULL, &tv);

    char input;
    read(fileno(stdin), &input, 1);

    return input;
  }

  void handleInput(int parsedInput) {
    if (parsedInput == KEY_UP && this->selected > 0) {
      this->selected -= 1;
    } else if (parsedInput == KEY_DOWN &&
               this->selected < this->selectedItem->child.size() - 1) {
      this->selected += 1;
    } else if (parsedInput == KEY_ENTER) {
      int index = 0;
      for (auto const &item : this->selectedItem->child) {
        if (index == this->selected) {
          if (this->selectedItem->child[item.first].child.size() == 0) {
            (this->*selectedItem->child[item.first].handler)();
          } else {
            this->selectedItem = &this->selectedItem->child[item.first];
            this->path[this->level] = &this->selectedItem->child[item.first];
            this->level++;
            this->selected = 0;
          }
        }
        index++;
      }
    } else if (parsedInput == KEY_LEFT) {
      if (this->level > 1) {
        this->selectedItem = this->path[this->level - 1];
        this->level--;
        this->selected = 0;
      } else if (this->level == 1) {
        this->selectedItem = &this->menu;
        this->level = 0;
        this->selected = 0;
      }
    }
  }

  void deploy() {}

  void exit() {
    tcsetattr(fileno(stdin), TCSANOW, &this->defaultTTYSettings);
    // clear screen
    std::cout << "\033[2J\033[1;1H";
    // show cursor
    std::cout << "\e[?25h";
    system("stty cooked");
    ::exit(EXIT_SUCCESS);
  }
};