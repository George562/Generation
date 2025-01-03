#pragma once

namespace Effects {
    using Type = sf::Uint8;
    enum : Type {
        Damage,
        Heal,
        HPRegen,
        Burn,
        EffectCount,
    };
};

class Effect {
public:
    Effects::Type type;
    std::vector<float> parameters;
    bool active = false;

    sf::Clock* customTickClock = nullptr;
    sf::Time howLongToExist;
    sf::Time customTick;

    Effect(Effects::Type type, std::vector<float> parameters, sf::Time howLongToExist, sf::Time customTick=sf::Time::Zero) {
        this->type = type;
        this->parameters = parameters;
        this->howLongToExist = GameTime + howLongToExist;
        if (customTick != sf::Time::Zero) {
            this->customTick = customTick;
            this->customTickClock = new sf::Clock();
        }
    }

    ~Effect() {
        if (customTickClock) {
            delete customTickClock;
        }
    }
};