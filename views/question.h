#ifndef H_VIEW_QUESTION
#define H_VIEW_QUESTION

#include <functional>
#include "constants.h"
#include "select.h"
#include "../models/view.h"

class QuestionView: public AbstractView {
public:
  QuestionView() {
    this->type = "question";
    this->handler = [](void) {};
  }

  QuestionView(std::function<void()> callback) {
    this->type = "question";
    this->handler = callback;
  }

  void render() {
    std::cout << "question content \n\r\n\r";
  }

  void handleInput(int parsedInput, AbstractView **path, int *level) {
    if (parsedInput == KEY_LEFT) {
      if (*level > 0) {
        *level = *level - 1;
      }
    }
  }
};

#endif /* H_VIEW_QUESTION */