#ifndef H_VIEW_QUESTION
#define H_VIEW_QUESTION

#include <functional>
#include "constants.h"
#include "select.h"
#include "../models/view.h"

class QuestionView: public AbstractView {
  std::string question;
  std::string response;

public:
  QuestionView(std::string question) {
    this->type = "question";
    this->handler = [](void) {};
    this->question = question;
    this->response = "";
  }

  QuestionView(std::string question, std::function<void()> callback) {
    this->type = "question";
    this->handler = callback;
    this->question = question;
    this->response = "";
  }

  void render() {
    std::cout << this->question + ": " + this->response + "\n\r\n\r";
  }

  void handleInput(int parsedInput, AbstractView **path, int *level) {
    if (parsedInput == KEY_LEFT) {
      if (*level > 0) {
        *level = *level - 1;
      }
    } else if (parsedInput == KEY_ENTER) {
      this->handler();
      *level = *level - 1;
    } else {
      this->response += parsedInput;
    }
  }
};

#endif /* H_VIEW_QUESTION */