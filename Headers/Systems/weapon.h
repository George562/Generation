#pragma once
#include "../Entities/bullet.h"

#define M_PI_RAD M_PI / 180.f

////////////////////////////////////////////////////////////
// Weapon
#pragma pack(push, 1)
class Weapon {
public:
    sf::String Name;
    Scale<float> ManaStorage;
    sf::Clock* TimeFromLastShot = nullptr;
    sf::Clock* ReloadTimer = nullptr;
    float ReloadSpeed;
    sf::Time FireRate;
    int NumberOfBulletsPerShot;
    float ManaCostOfBullet;
    float damage;
    float BulletVelocity;
    float scatter; // at degree
    bool lock; // Bullets is like a stream and "lock" is blocking it stream

    Weapon() {}
    Weapon(sf::String name, float MaxManaStorage, float ManaCostOfBullet, float FireRate, float dmg) {
        Name = name;
        ManaStorage = {0, MaxManaStorage, MaxManaStorage};
        TimeFromLastShot = new sf::Clock();
        ReloadTimer = new sf::Clock();
        ReloadSpeed = 10.f;
        this->FireRate = sf::seconds(FireRate);
        NumberOfBulletsPerShot = 1;
        this->ManaCostOfBullet = ManaCostOfBullet;
        damage = dmg;
        lock = true;
    }
    virtual ~Weapon() {
        if (TimeFromLastShot) {
            delete TimeFromLastShot;
        }
        if (ReloadTimer) {
            delete ReloadTimer;
        }
    }

    virtual void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            lock = false;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    }

    virtual void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
    }

    virtual void Reload(Scale<float>& Mana) {
        float x = std::min(std::min(std::min(oneOverSixty, ReloadTimer->restart().asSeconds()) * ReloadSpeed, ManaStorage.fromTop()), Mana.toBottom());
        Mana -= x;
        ManaStorage += x;
    }
};
#pragma pack(pop)
////////////////////////////////////////////////////////////

// Pistol
class Pistol : public Weapon {
public:
    Pistol() : Weapon("Pistol", 10, 1, 0.4, 2) { BulletVelocity = 600; scatter = 20; }
};

// Revolver
class Revolver : public Weapon {
public:
    Revolver() : Weapon("Revolver", 6, 2, 0, 5) { BulletVelocity = 960; scatter = 10; }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        Weapon::Shoot(shooter, direction, f);
        lock = true;
    }
};

// Shotgun
class Shotgun : public Weapon {
public:
    Shotgun() : Weapon("Shotgun", 25, 5, 1 / 2.f, 3) { BulletVelocity = 600; NumberOfBulletsPerShot = 10;  scatter = 50; }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * scatter / 2.f, d) * BulletVelocity / len;
        for (int i = 0; i < NumberOfBulletsPerShot; i++, d = RotateOn(M_PI_RAD * scatter / (NumberOfBulletsPerShot - 1.f), d)) {
            sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
            Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        }
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
        lock = true;
    }
};

// Rifle
class Rifle : public Weapon {
public:
    Rifle() : Weapon("Rifle", 25, 1, 1 / 10.f, 1) { BulletVelocity = 960; scatter = 17; }
};

// Bubblegun
class Bubblegun : public Weapon {
public:
    Bubblegun() : Weapon("Bubblegun", 30, 3, 0.03, 4) { BulletVelocity = 540; NumberOfBulletsPerShot = 10;  scatter = 40; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            lock = false;
    }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;
        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = RotateOn(-M_PI_RAD * (rand() % (int)scatter - scatter / 2), d) * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage, COMMON_BULLET_PENETRATION, Bullet::Bubble, sf::seconds(1)));
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
        if (--NumberOfBulletsPerShot == 0) {
            NumberOfBulletsPerShot = 10;
            lock = true;
        }
    }
};

// Armageddon
class Armageddon : public Weapon {
public:
    Armageddon() : Weapon("Armageddon", 300, 0.1, 1.f / 16, 3) { BulletVelocity = 180; }
    void Update(sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            NumberOfBulletsPerShot = 0;
            lock = false;
        } if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            lock = true;
    }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(-M_PI * NumberOfBulletsPerShot) / 12, d);
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        ManaStorage -= ManaCostOfBullet;
        NumberOfBulletsPerShot++;
        TimeFromLastShot->restart();
    }
};

// Chaotic
class Chaotic : public Weapon {
public:
    Chaotic() : Weapon("Chaotic", 300, 0.1, 1.f / 16, 3) { BulletVelocity = 180; }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d{0, BulletVelocity};
        d = RotateOn(float(rand()), d);
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage));
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
    }
};

class FireHose : public Weapon {
public:
    FireHose() : Weapon("Fire hose", 100, 0, 1.f / 10, 0) { BulletVelocity = 1200; }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = d * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage, 0, Bullet::WaterParticle));
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
    }
    void Reload(Scale<float>& Mana) {}
};

class Flamethrower : public Weapon {
public:
    Flamethrower() : Weapon("Flamethrower", 100, 0, 1.f / 10, 1) { BulletVelocity = 1200; }
    void Shoot(CollisionCircle& shooter, sf::Vector2f direction, faction::Type f) {
        if (ManaStorage.toBottom() < ManaCostOfBullet) { lock = true; return; }
        if (lock || TimeFromLastShot->getElapsedTime() <= FireRate) return;

        sf::Vector2f d = direction - shooter.getCenter();
        float len = hypotf(d.x, d.y);
        if (len == 0) return;
        d = d * BulletVelocity / len;
        sf::Vector2f SpawnPoint(shooter.getCenter() + d * (shooter.getRadius() * 1.4f) / BulletVelocity);
        Bullets.push_back(new Bullet(f, SpawnPoint, d, damage, 0, Bullet::Type::FireParticle));
        ManaStorage -= ManaCostOfBullet;
        TimeFromLastShot->restart();
    }
    void Reload(Scale<float>& Mana) {}
};
