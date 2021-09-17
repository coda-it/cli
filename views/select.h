#ifndef H_VIEW_SELECT
#define H_VIEW_SELECT

#include <functional>
#include "constants.h"

class SelectView {
public:
  int selected = 0;
  std::map<std::string, SelectView> child;
  std::function<void()> handler;

  SelectView() {
    this->handler = [](void) {};
  }

  SelectView(std::function<void()> callback) {
    this->handler = callback;
  }

  void render() {
    int index = 0;

    for (auto const &item : this->child) {
      if (index == this->selected) {
        std::cout << "> " + item.first + "\n\r";
      } else {
        std::cout << "  " + item.first + "\n\r";
      }

      index++;
    }
  }

  void handleInput(int parsedInput, SelectView **path, int *level) {
    if (parsedInput == KEY_UP && this->selected > 0) {
      this->selected -= 1;
    } else if (parsedInput == KEY_DOWN &&
               this->selected < this->child.size() - 1) {
      this->selected += 1;
    } else if (parsedInput == KEY_ENTER) {
      int index = 0;
      for (auto const &item : this->child) {
        if (index == this->selected) {
          if (this->child[item.first].child.size() == 0) {
            this->child[item.first].handler();
          } else {
            *level = *level + 1;
            path[*level] = &this->child[item.first];
            this->selected = 0;
          }
        }
        index++;
      }
    } else if (parsedInput == KEY_LEFT) {
      if (*level > 0) {
        this->selected = 0;
        *level = *level - 1;
      }
    }
  }
};

#endif /* H_VIEW_SELECT */