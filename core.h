#include "persistence.h"
#include "types.h"
#include "views/select.h"
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <strings.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

class Cli {
private:
  Persistence *persistence;
  std::string userInput;
  SelectView *selectedItem;
  SelectView *rootTask;
  SelectView *path[2];
  int level = 0;
  bool shouldRerender = false;

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

public:
  int selected = 0;

  Cli() {
    // set terminal
    struct termios tTYSettings;
    tcgetattr(fileno(stdin), &tTYSettings);
    struct termios newTTYSettings = tTYSettings;
    newTTYSettings.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(fileno(stdin), TCSANOW, &newTTYSettings);

    this->persistence = new Persistence();

    SelectView *rootTask = new SelectView();
    this->rootTask = rootTask;

    SelectView *deployTask = new SelectView();
    SelectView *deploySubTask =
        new SelectView([](void) { std::cout << "deploying ..."; });
    deployTask->child["deploy"] = *deploySubTask;
    rootTask->child["deploy"] = *deployTask;

    SelectView *exitTask = new SelectView([tTYSettings](void) {
      tcsetattr(fileno(stdin), TCSANOW, &tTYSettings);
      // clear screen
      std::cout << "\033[2J\033[1;1H";
      // show cursor
      std::cout << "\e[?25h";
      system("stty cooked");
      ::exit(EXIT_SUCCESS);
    });
    rootTask->child["exit"] = *exitTask;

    this->selectedItem = rootTask;
  }

  void run() {
    do {
      // hide cursor
      std::cout << "\e[?25l";
      // clear screen
      std::cout << "\033[2J\033[1;1H";
      std::cout << "=== CODA_ CLI ===\n\r\n\r";
      this->selectedItem->render();
      this->selectedItem->handleInput(this->getInput(), this->path,
                                      &this->level);

      if (this->level > 0) {
        this->selectedItem = this->path[this->level];
      } else {
        this->selectedItem = rootTask;
      }
    } while (true);
  }
};