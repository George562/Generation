#pragma once
#include "creature.h"

std::vector<std::vector<sf::Vector2f>> TheWayToPlayer;

////////////////////////////////////////////////////////////
// Enemy
class Enemy : public Creature {
public:
    Enemy(std::string name) : Creature(name, faction::Enemy) {}
    ~Enemy() { if (CurWeapon) { delete CurWeapon; } }
    void updateLook() const override {
        sf::Vector2f dir = this->target - hitbox.getCenter();
        if (dir.x == 0.f && dir.y == 0.f) return;
        dir = normalize(dir);

        float s = cross(dir, lookDirection), delta = M_PI * TimeSinceLastFrame.asSeconds();
        if (s >= sin(delta)) {
            lookDirection = RotateOn(delta, lookDirection);
        } else if (s <= sin(-delta)) {
            lookDirection = RotateOn(-delta, lookDirection);
        } else if (distance(dir, lookDirection) > 1.5f) {
            lookDirection = RotateOn(delta, lookDirection);
        } else {
            lookDirection = dir;
        }
    }
};
////////////////////////////////////////////////////////////

// DistortedScientist
class DistortedScientist : public Enemy {
public:
    DistortedScientist() : Enemy("Distorted Scientist") {
		Health = { {0, 10 + 4.f * curLevel, 10 + 4.f * curLevel} };
        HealthRecovery = 2.5;
		Mana = { {0, 10, 10} };
        ManaRecovery = 1;
        Armor  = { {0, 100, 100} };
        Velocity = {0, 0}; MaxVelocity = 400.f;
        Acceleration = MaxVelocity * 10.f;
        hitbox.setRadius(60.f);
        CurWeapon = new Pistol();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        setAnimation(Textures::DistortedScientist);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;
        Creature::move(location);
    }

    void shift(sf::Vector2f shift) override { Velocity += normalize(shift) * 4.f; }
    void shift(float x, float y) override { Velocity += normalize(sf::Vector2f(x, y)) * 4.f; }
};

// Distorted
class Distorted : public Enemy {
public:
    Distorted() : Enemy("Distorted") {
		Health = { {0, 5 + 2.5f * curLevel, 5 + 2.5f * curLevel} };
        HealthRecovery = 0.5;
		Mana = { {0, 20, 20} };
        ManaRecovery = 2.5;
		Armor = { {0, 25, 25} };
        Velocity = {0, 0}; MaxVelocity = 700.f;
        Acceleration = MaxVelocity * 17.5f;
        CurWeapon = new Pistol();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        hitbox.setRadius(60.f);
        setAnimation(Textures::Distorted, &Shaders::Distortion1);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    void move(Location* location) override {
        VelocityBuff = 1;
        Creature::move(location);
    }

    void shift(sf::Vector2f shift) override { Velocity += normalize(shift) * 4.f; }
    void shift(float x, float y) override { Velocity += normalize(sf::Vector2f(x, y)) * 4.f; }
};

// AngularBody
class AngularBody : public Enemy {
public:
    float angles;
    AngularBody(float angles) : Enemy("Angular body") {
        this->angles = angles;
		Health = { {0, 5 + 2.5f * curLevel, 5 + 2.5f * curLevel} };
        HealthRecovery = 0.5;
		Mana = { {0, 20, 20} };
        ManaRecovery = 2.5;
		Armor = { {0, 25, 25} };
        Velocity = {0, 0}; MaxVelocity = 700.f;
        Acceleration = MaxVelocity * 17.5f;
        CurWeapon = new Pistol();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        hitbox.setRadius(size / 8.f);
        setAnimation(Textures::Enemy, &Shaders::Enemy);
        hitbox.setRadius(size / 8.f * 0.8f);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        Shaders::Enemy.setUniform("direction", this->lookDirection);
        Shaders::Enemy.setUniform("angles", angles);
        Creature::draw(target, states);
    }
};

// Boss
class Boss : public Enemy {
public:
    Boss() : Enemy("Big Bad Boss") {
		Health = { {0, 15 + 2.5f * curLevel, 15 + 2.5f * curLevel} };
        HealthRecovery = 0.5;
		Mana = { {0, 20, 20} };
        ManaRecovery = 20.5;
		Armor = { {0, 25, 25} };
        Velocity = {0, 0}; MaxVelocity = 700.f;
        Acceleration = MaxVelocity * 17.5f;
        CurWeapon = new BigBadBossWeapon();
        CurWeapon->ManaStorage.top = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaStorage.cur = 100.f * CurWeapon->ManaCostOfBullet;
        CurWeapon->ManaCostOfBullet.stats[CurWeapon->ManaCostOfBullet.curLevel] += curLevel;

        hitbox.setRadius(size / 2.);
        setAnimation(Textures::Boss, &Shaders::Boss);
        hitbox.setRadius(size * 0.37 / 1.75);

        addItem(new Item(ItemID::regenDrug, 1));
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        Shaders::Boss.setUniform("direction", this->lookDirection);
        Creature::draw(target, states);
    }
};

sf::Packet& operator<<(sf::Packet& packet, Enemy* e) {
    return packet << e->Name.getText() << e->hitbox.getCenter();
}
sf::Packet& operator>>(sf::Packet& packet, Enemy* e) {
    sf::Vector2f v; packet >> v;
    e->hitbox.setCenter(v);
    return packet;
}