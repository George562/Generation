﻿#pragma once
#include "PlacedText.h"
#include "Frame.h"

class ItemSlot : public UIElement {
public:
    Frame* background;
    PlacedText* amountText;
    bool isInitialized;  // A marker so that we don't have to use arbitrary checks for null pointers or something as dumb

    ItemSlot() {
        isInitialized = false;
    }

    virtual void init(std::string name) {
        background = new Frame(name + "_BG");
        amountText = new PlacedText(name + "_AmText");
        isInitialized = true;
    };

    virtual void init(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, sf::Vector2f size = { 0, 0 }) {
        init(name);
        setAnchors(anchor, anchoringPoint);
        setSize(size.x, size.y);
    };

    virtual void init(std::string name, sf::FloatRect rect) {
        init(name, UI::none, UI::none, rect.getSize());
        setPosition(rect.getPosition());
    };

    virtual void erase() {
        isInitialized = false;
        delete background;
        delete amountText;
    }

    virtual void setPosition(float x, float y) {
        UIRect::setPosition(x, y);
        amountText->setCenter(sf::Vector2f(9 * background->getGlobalBounds().getSize().x / 8,
                                           9 * background->getGlobalBounds().getSize().y / 8));
    }

    virtual void setPosition(sf::Vector2f v) {
        setPosition(v.x, v.y);
    }

    void setTexture(sf::Texture& texture, UI::TextureResize mode, sf::Vector2f size = { 0, 0 }) {
        if (!background->sprite.getTexture() && getSize() != sf::Vector2u{ 0, 0 } && mode == UI::element) {
            background->setTexture(texture, UI::size, getSize());
        }
        else background->setTexture(texture, mode, (sf::Vector2u)size);
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        states.transform *= getTransform();
        target.draw(*background, states);
        target.draw(*amountText, states);
    }
};

class ShopSlot : public ItemSlot {
public:
    PlacedText* priceText;

    ShopSlot() : ItemSlot() {}
    void init(std::string name) {
        ItemSlot::init(name);
        priceText = new PlacedText(name + "_PrText");
    };

    virtual void init(std::string name, UI::Anchor anchor, UI::Anchor anchoringPoint, sf::Vector2f size = { 0, 0 }) {
        init(name);
        setAnchors(anchor, anchoringPoint);
        setSize(size.x, size.y);
    };

    virtual void init(std::string name, sf::FloatRect rect) {
        init(name, UI::none, UI::none, rect.getSize());
        setPosition(rect.getPosition().x, rect.getPosition().y);
    };

    virtual void erase() {
        ItemSlot::erase();
        delete priceText;
    }

    virtual void setPosition(float x, float y) {
        ItemSlot::setPosition(x, y);
        priceText->setCenter(sf::Vector2f(-background->getGlobalBounds().getSize().x / 8,
                                          9 * background->getGlobalBounds().getSize().y / 8));
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        ItemSlot::draw(target, states);
        states.transform *= getTransform();
        if (priceText->getText() == "0")
            priceText->setString("Free");
        target.draw(*priceText, states);
    }
};