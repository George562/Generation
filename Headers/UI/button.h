#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "text.h"
#include "../CollisionShapes/collisionRect.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Button : public sf::Drawable {
public:
    sf::Texture* texture, *pushedTexture;
    sf::Sprite sprite;
    PlacedText ButtonText;
    bool Pushed = false, ShowButton = true;
    void (*buttonFunction)(void);
    CollisionRect hitbox;

    Button() {}
    Button(sf::String, void (*)(void));

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }
    void setCenter(float x, float y) { setPosition(x - hitbox.getSize().x / 2, y - hitbox.getSize().y / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    void setSize(float, float);
    void setSize(sf::Vector2f v) { setSize(v.x, v.y); }

    void setWord(sf::String word) { ButtonText.setString(word); ButtonText.setCenter(hitbox.getCenter()); }
    void setTexture(sf::Texture&, sf::Texture&);
    void setCharacterSize(int size) { ButtonText.setCharacterSize(size); ButtonText.setCenter(hitbox.getCenter()); }
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool OnTheButton(int& x, int& y) { return hitbox.contains(x, y); }
    bool isActivated(sf::Event&);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Button::Button(sf::String word, void (*foo)(void)) {
    ButtonText.setCharacterSize(150);
    ButtonText.setFillColor(sf::Color(199, 199, 199));
    setWord(word);
    buttonFunction = foo;
}

void Button::setTexture(sf::Texture& texture, sf::Texture& pushedTexture) {
    this->texture = &texture;
    this->pushedTexture = &pushedTexture;
    sprite.setTexture(texture);
    setSize(texture.getSize().x, texture.getSize().y);
}

void Button::setPosition(float x, float y) {
    hitbox.setPosition(x, y);
    sprite.setPosition(x, y);
    ButtonText.setCenter(hitbox.getCenter());
}

void Button::setSize(float w, float h) {
    if (hitbox.getSize().x != 0 && hitbox.getSize().y != 0) {
        sprite.setScale(w / hitbox.getSize().x, h / hitbox.getSize().y);
    }
    hitbox.setSize(w, h);
    ButtonText.setCenter(hitbox.getCenter());
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (ShowButton) {
        target.draw(sprite, states);
        target.draw(ButtonText, states);
    }
}

bool Button::isActivated(sf::Event& event) {
    if (Pushed && event.type == sf::Event::MouseButtonReleased) {
        Pushed = false;
        sprite.setTexture(*texture);
        if (OnTheButton(event.mouseButton.x, event.mouseButton.y)) {
            buttonFunction();
            return true;
        }
    }
    if (event.type == sf::Event::MouseButtonPressed &&
        OnTheButton(event.mouseButton.x, event.mouseButton.y) && 
        event.mouseButton.button == sf::Mouse::Button::Left) {
            Pushed = true;
            sprite.setTexture(*pushedTexture);
            return true;
        }
    return false;
}
