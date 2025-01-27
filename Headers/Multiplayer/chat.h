#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "../UI/PlacedText.h"

#define SPACE_BETWEEN_LINES_IN_PIXELS 35
#define CHARACTER_SIZE 27

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Chat : public sf::Drawable {
public:
    static const int len = 11;
    float PosX, PosY;
    sf::Vector2f size;
    std::vector<sf::Clock*> clocks;
    PlacedText lines[len];
    std::vector<std::string> strings;
    int pos;
    size_t cursorPos;
    sf::RectangleShape rect, cursor;
    bool inputted;
    std::map<std::string, void (*)(void)> commands;

    Chat(int, int);
    ~Chat();
    virtual void draw(sf::RenderTarget&, sf::RenderStates = sf::RenderStates::Default) const;
    bool InputText(sf::Event&);
    bool Entered();
    void addLine(std::string);
    std::string Last() { return strings.size() ? strings.back() : ""; }

    void SetCommand(std::string CommandString, void (*CommandFunction)(void)) { commands[CommandString] = CommandFunction; }
    bool UseCommand(std::string Commandstring) {
        if (commands.count(Commandstring) != 0) {
            commands[Commandstring]();
            return true;
        }
        return false;
    }

private:
    void setLines();
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Chat::Chat(int scw, int sch) : inputted(false), pos(0) {
    PosX = 350; PosY = sch - SPACE_BETWEEN_LINES_IN_PIXELS * (len + 2);
    size.x = scw - PosX * 2; size.y = 35;

    for (int i = 0; i < len; i++) {
        lines[i].setCharacterSize(CHARACTER_SIZE);
        lines[i].setFillColor(sf::Color(199, 199, 199, 180));
        lines[i].setString("");
        lines[i].setPosition(PosX + 5, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * (len - i) + 4);
    }

    rect.setFillColor(sf::Color(50, 50, 50, 100));
    rect.setOutlineColor(sf::Color::White);
    rect.setOutlineThickness(2);
    rect.setPosition(PosX, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
    rect.setSize(size);

    cursor.setFillColor(sf::Color::White);
    cursor.setSize({3, size.y});
}

Chat::~Chat() {
}

void Chat::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (inputted) {
        target.draw(rect, states);
        if (GameTime % sf::seconds(1.f) > sf::seconds(1.f / 2)) {
            target.draw(cursor, states);
        }
    }
    if (inputted) {
        target.draw(lines[0], states);
    }
    for (int i = 0; i < len - 1; i++) {
        if ((i + pos < clocks.size() && clocks[i + pos]->getElapsedTime() < sf::seconds(5)) || inputted) {
            target.draw(lines[i + 1], states);
        }
    }
}

bool Chat::InputText(sf::Event& event) {
    setlocale(LC_ALL, "rus");

    if (keyPressed(event, sf::Keyboard::Enter)) {
        Entered();
    }

    if (inputted) {
        if (event.key.control && keyPressed(event, sf::Keyboard::V)) {
            lines[0].setString(lines[0].getString().substring(0, cursorPos) +
                            sf::Clipboard::getString() +
                            lines[0].getString().substring(cursorPos, lines[0].getText().size()));
            cursorPos += sf::Clipboard::getString().getSize();
        }

        std::string buffer;
        if (event.type == sf::Event::TextEntered && 32 <= event.text.unicode) {
            if (event.text.unicode <= 127) {
                buffer.push_back(event.text.unicode);
            } else {
                buffer.push_back(event.text.unicode - 1072 - 32);
            }
            lines[0].insert(cursorPos, buffer);
            cursorPos++;
        }

        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                inputted = false;
                return true;
            }

            if (event.key.code == sf::Keyboard::BackSpace && cursorPos > 0) {
                lines[0].setString(lines[0].getString().substring(0, cursorPos - 1) +
                                lines[0].getString().substring(cursorPos, lines[0].getText().size()));
                cursorPos--;
            }
            if (event.key.code == sf::Keyboard::Delete && cursorPos < lines[0].getText().size())
                lines[0].setString(lines[0].getString().substring(0, cursorPos) +
                                lines[0].getString().substring(cursorPos + 1, lines[0].getText().size()));
            if (event.key.code == sf::Keyboard::Left && cursorPos > 0) cursorPos--;
            if (event.key.code == sf::Keyboard::Right && cursorPos < lines[0].getText().size()) cursorPos++;
            if (event.key.code == sf::Keyboard::Home) cursorPos = 0;
            if (event.key.code == sf::Keyboard::End) cursorPos = lines[0].getText().size();
            if (event.key.code == sf::Keyboard::Down) {
                pos = std::max(pos - 1, 0);
                setLines();
            }
            if (event.key.code == sf::Keyboard::Up)   {
                pos = std::min(pos + 1, std::max(0, (int)strings.size() - len + 1));
                setLines();
            }
        }

        PlacedText tempText; tempText.setString(lines[0].getString().substring(0, cursorPos));
        tempText.setCharacterSize(CHARACTER_SIZE);
        cursor.setPosition(PosX + 5 + tempText.Width, PosY + SPACE_BETWEEN_LINES_IN_PIXELS * len);
    }

    return inputted || keyPressed(event, sf::Keyboard::Enter);
}

bool Chat::Entered() {
    if (inputted && lines[0].getText().size() > 0) {
        if (!UseCommand(lines[0].getText())) {
            addLine(lines[0].getText());
        }
    }
    lines[0].setString("");
    pos = cursorPos = 0;
    inputted = !inputted;
    return !inputted;
}

void Chat::addLine(std::string word) {
    strings.push_back(word);
    setLines();
    for (int i = clocks.size() - 1; i >= 0 && clocks[i]->getElapsedTime() >= sf::seconds(5); i--) {
        delete clocks[i];
        clocks.pop_back();
    }
    clocks.insert(clocks.begin(), new sf::Clock());
}

void Chat::setLines() {
    for (int i = 1; i < std::min(len, (int)strings.size() + 1); i++) {
        lines[i].setString(strings[strings.size() - i - pos]);
    }
}