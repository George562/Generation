#pragma once
#include "../Systems/Weapon.h"
#include "../Systems/Inventory.h"
#include "../Systems/Effect.h"
#include "../UI/PlacedText.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

enum TargetMode {
    sleep,          // Creature is completely inactive. Will react only to damage or special conditions
    wander,         // Creature is wandering about the map. Can see the player and go to other target modes
    search,         // Creature has lost the player out of sight and is actively searching for them
    pursuit,        // Creature is trying to get into a fighting position against the player
    fight           // Creature is actively attacking and obeying it's fighting behaviour
};

#pragma pack(push, 1)
class Creature : public sf::Drawable {
public:
    faction::Type faction;

    Scale<float> Health;
    bool HealthRecoveryActive = true;
    float HealthRecovery;
    Scale<float> Mana;
    bool ManaRecoveryActive = true;
    float ManaRecovery;
    Scale<float> Armor; // blocking damage = min(1, damage - armor)

    sf::Vector2f Velocity;
    float MaxVelocity;
    float VelocityBuff = 1.0;
    float Acceleration;
    sf::Vector2f target; // target point to move towards
    sf::Vector2f lastTarget; // lastTarget point. Needed to not constantly backtrack
    TargetMode targetMode = wander;
    bool atTarget = false;
    sf::Time passiveWait = sf::Time::Zero;          // A timer for keeping track of waits between different actions like switching modes, ai actions, etc
                                                    // Will change during battle or rest to not make the input reading unfair or make the enemy too dumb
    bool makeADash = false;
    mutable sf::Vector2f lookDirection = {1.f, 0.f};

    Weapon *CurWeapon = nullptr; // ref on exist weapon from Weapons

    mutable PlacedText Name;
    Animation *animation = nullptr;

    bool dropInventory;
    Inventory inventory;

    std::vector<Effect*> effects;
    std::vector<int> effectStacks;

    CollisionCircle hitbox;

    Creature(sf::String name, faction::Type f) {
        Name.setString(name);
        Name.setCharacterSize(25);
        Name.setOutlineThickness(2);
        Name.setOutlineColor(sf::Color::Red);
        Name.setFillColor(sf::Color::White);

        faction = f;

        animation = nullptr;
        dropInventory = true;

        effectStacks.assign(Effects::EffectCount, 0);
    }
    ~Creature() {
        if (animation) {
            delete animation;
        }
    }

    sf::Vector2f getCenter() const { return hitbox.getCenter(); }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        updateLook();
        if (animation != nullptr) {
            animation->setPosition(hitbox.getCenter());
            target.draw(*animation, states);
        }
        Name.setPosition(hitbox.getCenter().x - Name.Width / 2.f, hitbox.getPosition().y - Name.Height);
        target.draw(Name, states);
    }

    virtual void getDamage(float dmg) { Health -= dmg; }

    virtual void move(Location* location) {
        float ElapsedTimeAsSecond = std::min(TimeSinceLastFrame.asSeconds(), oneOverSixty);
        if (!makeADash && targetMode <= TargetMode::wander) {
            sf::Vector2f Difference = target - hitbox.getCenter() - Velocity;
            if (length(Difference) >= Acceleration * ElapsedTimeAsSecond) {
                sf::Vector2f Direction = sf::Vector2f(sign(Difference)) * Acceleration;
                Velocity += Direction * ElapsedTimeAsSecond;
            } else {    
                Velocity += Difference;
            }
        } else {
            Velocity = normalize(target - hitbox.getCenter()) * MaxVelocity * VelocityBuff;
            makeADash = false;
        }

        sf::Vector2i tempv = WillCollideWithWalls(hitbox, Velocity * ElapsedTimeAsSecond);

        if (tempv.x == -1) Velocity.x = 0;
        if (tempv.y == -1) Velocity.y = 0;
        hitbox.move(Velocity * ElapsedTimeAsSecond);
        atTarget = length(hitbox.getCenter() - target) < MaxVelocity / 4.;
    }

    virtual void shift(sf::Vector2f shift) {}
    virtual void shift(float x, float y) {}

    virtual void UpdateState() {
        Mana += ManaRecovery * ManaRecoveryActive * TimeSinceLastFrame.asSeconds();
        Health += HealthRecovery * HealthRecoveryActive * TimeSinceLastFrame.asSeconds();
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setAnimation(texture, FrameAmount, maxLevel, duration, shader);
        animation->setSize(hitbox.getSize());
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    }

    void setAnimation(sf::Texture& texture, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation("");
        animation->setTexture(texture, shader);
        animation->setSize(hitbox.getSize());
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    };

    void ChangeWeapon(Weapon* to) { CurWeapon = to; }

    virtual void setTarget(sf::Vector2f target) { this->target = target; }

    void addItem(Item* item) {
        inventory.addItem(item);
    }

    bool isAlive() {
        return Health.toBottom() > 0;
    }

    virtual void updateLook() const {}
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////
