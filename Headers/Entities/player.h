#pragma once
#include "creature.h"

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Player : public Creature {
public:
    Player();
    void move(Location*) override;
	void updateLook() const override;
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void to_json(json& j, const Player& p) {
	j["Name"] = p.Name.getString().text.toAnsiString();
	j["Health"] = p.Health;
	j["HealthRecovery"] = p.HealthRecovery;
	j["Mana"] = p.Mana;
	j["ManaRecovery"] = p.ManaRecovery;
	j["Inventory"] = json({});
	j["Money"] = p.inventory.money;

	for (Item* item : p.inventory.items) {
		j["Inventory"][itemName[item->id]] = item->amount;
	}
}

void from_json(const json& j, Player& p) {
	p.Name.setString(j["Name"].template get<std::string>());
	p.Health = j["Health"].template get<Scale<float>>();
	p.HealthRecovery = j["HealthRecovery"].template get<float>();
	p.Mana = j["Mana"].template get<Scale<float>>();
	p.ManaRecovery = j["ManaRecovery"].template get<float>();
	p.inventory.money = j["Money"].template get<int>();
	for (int id = 0; id < ItemID::ItemCount; id++) {
		if (j["Inventory"].contains(itemName[id])) {
			Item* newItem = new Item(id, j["Inventory"][itemName[id]].template get<int>());
			p.inventory.addItem(newItem);
		}
	}
}

Player::Player() : Creature("Player", faction::Player) {
	Health = { {0.f, 20.f, 20.f} }; HealthRecovery = Health.top / 5;
	Mana = { {0.f, 10.f, 10.f} }; ManaRecovery = Mana.top / 5;
    Armor = { {0.f, 0.f, 0.f} };
    hitbox.setRadius(80.f);
    Velocity = {0.f, 0.f}; MaxVelocity = 700.f;
    Acceleration = MaxVelocity * 20.f;
    Name.setOutlineColor(sf::Color::Green);
    Name.setFillColor(sf::Color::Black);
}

void Player::move(Location* location) {
    sf::Vector2f direction;
	if (makeADash) {
		direction = normalize(sf::Vector2f(sf::Mouse::getPosition()) - sf::Vector2f(scw / 2.f, sch / 2.f));
	} else {
		direction.x = sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        direction.y = sf::Keyboard::isKeyPressed(sf::Keyboard::S) - sf::Keyboard::isKeyPressed(sf::Keyboard::W);
	}

    VelocityBuff = 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        VelocityBuff *= 2;

    setTarget(hitbox.getCenter() + direction * MaxVelocity * VelocityBuff);
    Creature::move(location);
}

void Player::updateLook() const {
	sf::Vector2f dir = sf::Vector2f(sf::Mouse::getPosition()) - (hitbox.getCenter() - GameView.getCenter() + GameView.getSize() / 2.f);
	if (dir.x == 0.f && dir.y == 0.f) return;
	dir = normalize(dir);

	float s = cross(dir, lookDirection), delta = 4.f * M_PI * TimeSinceLastFrame.asSeconds();
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

sf::Packet& operator<<(sf::Packet& packet, Player& a) {
    return packet << a.hitbox.getCenter();
}
sf::Packet& operator>>(sf::Packet& packet, Player& a) {
    sf::Vector2f v; packet >> v;
    a.hitbox.setCenter(v);
    return packet;
}
