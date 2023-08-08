#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Interactible : public Rect, public sf::Drawable {
public:
    sf::Texture texture;
    sf::Sprite sprite;
    void (*function)(void);

    Interactible() {}
    virtual void setFunction(void (*func)(void)) { function = func; }
    virtual bool isActivated(Rect&, sf::Event&, std::vector<sf::Drawable*>&) { return false; }
    virtual bool CanBeActivated(Rect&) { return false; }
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(sprite, states);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////