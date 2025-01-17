#pragma once
#include "../../SFML-2.5.1/include/SFML/Network.hpp"
#include "collisionShape.h"

class CollisionRect : public CollisionShape {
protected:
    void updateShape() override {
        points[1] = sf::Vector2f(points[0].x + size.x, points[0].y);
        points[2] = sf::Vector2f(points[0].x + size.x, points[0].y + size.y);
        points[3] = sf::Vector2f(points[0].x, points[0].y + size.y);
    }
public:
    CollisionRect() : CollisionShape(4) { this->size = sf::Vector2f(0.f, 0.f); }
    CollisionRect(float x, float y, float w, float h) : CollisionShape(4) {
        this->size = sf::Vector2f(w, h);
        setPosition(x, y);
    }
    CollisionRect(sf::Vector2f position, sf::Vector2f size) : CollisionShape(4) {
        this->size = size;
        setPosition(position);
    }
    float getRight() const { return points[2].x; }
    float getBottom() const { return points[2].y; }
    sf::Vector2f getRightTop() const { return points[1]; }
    sf::Vector2f getRightBottom() const { return points[2]; }
    sf::Vector2f getLeftBottom() const { return points[3]; }

    // Get and set position
    sf::Vector2f getPosition() const { return points[0]; }
    void setPosition(float x, float y) { points[0] = sf::Vector2f(x, y); updateShape(); }
    void setPosition(sf::Vector2f v) { setPosition(v.x, v.y); }

    // Set position of rect center
    sf::Vector2f getCenter() const { return points[0] + size / 2.f; }
    void setCenter(float x, float y) { setPosition(x - size.x / 2.f, y - size.y / 2.f); }
    void setCenter(sf::Vector2f v) { setCenter(v.x, v.y); }

    // Move the rect relatively to its current position
    void move(float x, float y) { setPosition(points[0].x + x, points[0].y + y); }
    void move(sf::Vector2f v) { setPosition(points[0].x + v.x, points[0].y + v.y); }

    // Get and set size
    sf::Vector2f getSize() const { return size; }
    void setSize(float w, float h) { size.x = w; size.y = h; updateShape(); }
    void setSize(sf::Vector2f size) { setSize(size.x, size.y); }

    // Set position and size
    void setRect(float x, float y, float w, float h) { size.x = w; size.y = h; setPosition(x, y); }
    void setRect(sf::Vector2f pos, sf::Vector2f size) { this->size = size; setPosition(pos); }
    void setRect(CollisionRect rect) { size = rect.size; setPosition(rect.getPosition()); }

    sf::FloatRect getBounds() const { return sf::FloatRect(points[0], size); }
};

using vvr = std::vector<std::vector<CollisionRect>>;

sf::Packet& operator<<(sf::Packet& packet, CollisionRect& a) {
    return packet << a.getPosition().x << a.getPosition().y << a.getSize().x << a.getSize().y;
}
sf::Packet& operator>>(sf::Packet& packet, CollisionRect& a) {
    sf::Vector2f pos, size;
    packet >> pos.x >> pos.y >> size.x >> size.y;
    a.setRect(pos, size);
    return packet;
}