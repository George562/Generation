#include "tools.h"

#define COMMON_BULLET_RADIUS 7

struct Bullet : public sf::Drawable {
    enum Type {
        Common,
        Bubble,
        // Explosion
    };

    float PosX, PosY, dx, dy;
    sf::Color color;
    int penetration;
    float damage;
    float radius = COMMON_BULLET_RADIUS;
    sf::CircleShape* circle;
    bool exlpode = false;
    sf::Time timer;
    Scale<float> ExplosionRadius = {1, 12, 1};
    Bullet::Type type;
    bool todel = false;

    Bullet() {}
    Bullet(float x, float y, float vx, float vy, sf::Color clr, int penetr, float dmg, float time, Bullet::Type t = Bullet::Common) {
        PosX = x; PosY = y; dx = vx; dy = vy;
        color = clr;
        penetration = penetr;
        damage = dmg;
        circle = new sf::CircleShape();
        timer = sf::seconds(time);
        type = t;
        UpdateCircleShape();
    }

    void UpdateCircleShape() {
        circle->setRadius(radius);
        circle->setFillColor(color);
    }
    
    sf::Rect<float> getRect() { return {PosX, PosY, dx, dy}; }
    sf::Vector2f getPosition() { return {PosX, PosY}; }
    void setPosition(sf::Vector2f &v) { PosX = v.x; PosY = v.y; }
    void setPosition(float x, float y) { PosX = x; PosY = y; }

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        target.draw(*circle);
    }

    void move(vvr& wallsRect, sf::Clock* clock) {
        if (dx == 0 && dy == 0) return;
        sf::Vector2i res = WillCollisionWithWalls(wallsRect, PosX, PosY, radius, radius, dx, dy);
        dx *= res.x;
        dy *= res.y;
        switch (type) {
            case Bullet::Bubble:
                PosX += dx * (timer - clock->getElapsedTime()).asSeconds();
                PosY += dy * (timer - clock->getElapsedTime()).asSeconds();
                if (timer < clock->getElapsedTime()) { dy = 0; dx = 0; exlpode = true; }
                if (exlpode && !todel) {
                    if (ExplosionRadius.fromTop() > 0) {
                        ExplosionRadius += 1.f / 5;
                        circle->setFillColor(circle->getFillColor() - sf::Color(0, 0, 0, 4));
                        circle->setRadius(radius * ExplosionRadius.cur);
                        PosX -= radius / 5;
                        PosY -= radius / 5;
                    } else todel = true;
                }
                break;
            case Bullet::Common:
                PosX += dx;
                PosY += dy;
        }
        circle->setPosition(PosX, PosY);
    }
};

using vB = std::vector<Bullet>;
vB Bullets(0);

sf::Packet& operator<<(sf::Packet& packet, Bullet& b) {
    return packet << b.PosX << b.PosY << b.dx << b.dy << b.color << b.damage << b.type;
}
sf::Packet& operator>>(sf::Packet& packet, Bullet& b) {
    packet >> b.PosX >> b.PosY >> b.dx >> b.dy >> b.color >> b.damage;
    int t; packet >> t; b.type = Bullet::Type(t);
    return packet;
}
