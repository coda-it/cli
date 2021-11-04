#ifndef H_VIEW
#define H_VIEW

class AbstractView {
    public:
    std::string type;
    std::map<std::string, AbstractView*> child;
    std::function<void()> handler;
 
    virtual void render() = 0;
    virtual void handleInput(int, AbstractView**, int*) = 0;
};

#endif /* H_VIEW */