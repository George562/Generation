#pragma once
#include "init.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class PlacedText : public Rect, public sf::Drawable {
public:
    sf::Font font;
    sf::Text text;
    std::vector<str> lines;

    PlacedText();

    void setPosition(float, float);
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    void setCenter(float x, float y) { setPosition(x - Width / 2, y - Height / 2); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    void setString(str s);

    void setCharacterSize(int x);
    void setFillColor(sf::Color color) { text.setFillColor(color); }
    void setScale(sf::Vector2f);
    void setScale(float, float);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const { target.draw(text); };
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

PlacedText::PlacedText() {
    font.loadFromFile("sources/ljk_Inky Thin Pixels.otf");
    text.setFont(font);
}

void PlacedText::setPosition(float x, float y) {
    PosX = x; PosY = y;
    text.setPosition(PosX, PosY);
}

void PlacedText::setString(str s) {
    text.setString(s);
    setSize(text.getGlobalBounds().width, text.getGlobalBounds().height);
}

void PlacedText::setCharacterSize(int x) {
    text.setCharacterSize(x);
    setSize(text.getGlobalBounds().width, text.getGlobalBounds().height);
}

void PlacedText::setScale(sf::Vector2f factors) {
    text.setScale(factors);
    setSize(Width * factors.x, Height * factors.y);
}
void PlacedText::setScale(float factorX, float factorY) {
    text.setScale(factorX, factorY);
    setSize(Width * factorX, Height * factorY);
}
