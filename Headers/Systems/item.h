#pragma once
#include "../LevelSystem/interactible.h"
#include "../init.h"

namespace ItemID {
    using Type = sf::Uint8;
    enum : Type {
        regenDrug,
        coin,
        dasher,
        ItemCount
    };
}

std::map<ItemID::Type, std::string> itemName {
    {ItemID::regenDrug, "Regen drug"},
    {ItemID::coin,      "Coin"},
    {ItemID::dasher,    "Dasher"},
};

std::map<ItemID::Type, std::string> itemDesc { // Descriptions of items
    {ItemID::regenDrug, "Substance for stabilizing the state of dimension of an object"}, // Gives +1 HP/s for the next 10 seconds
    {ItemID::coin,      "Moneh :D"},
    {ItemID::dasher,    "A device that allows you to perform a dash by holding down the Shift key after double-clicking"},
};

//////////////////////////////////////////////////////////// Item Animation params
std::map<ItemID::Type, sf::Texture*> itemTexture {
    {ItemID::regenDrug, new sf::Texture},
    {ItemID::coin,      new sf::Texture},
    {ItemID::dasher,    new sf::Texture},
};

std::map<ItemID::Type, int> itemTextureFrameAmount {
    {ItemID::regenDrug, 1},
    {ItemID::coin,      4},
    {ItemID::dasher,    1},
};

std::map<ItemID::Type, sf::Time> itemTextureDuration {
    {ItemID::regenDrug, sf::Time::Zero},
    {ItemID::coin,      sf::seconds(0.75)},
    {ItemID::dasher,    sf::Time::Zero},
};

std::map<ItemID::Type, sf::Shader *> itemTextureShader {
    {ItemID::regenDrug, &Shaders::PickupItem},
    {ItemID::coin,      &Shaders::PickupItem},
    {ItemID::dasher,    &Shaders::PickupItem},
};

//////////////////////////////////////////////////////////// Pickup Item Animation params
std::map<ItemID::Type, sf::Texture*> pickupItemTexture {
    {ItemID::regenDrug, new sf::Texture},
    {ItemID::coin,      new sf::Texture},
    {ItemID::dasher,    new sf::Texture}
};

std::map<ItemID::Type, int> pickupItemTextureFrameAmount {
    {ItemID::regenDrug, 1},
    {ItemID::coin,      4},
    {ItemID::dasher,    1},
};

std::map<ItemID::Type, sf::Time> pickupItemTextureDuration {
    {ItemID::regenDrug, sf::Time::Zero},
    {ItemID::coin,      sf::Time::Zero},
    {ItemID::dasher,    sf::Time::Zero},
};

std::map<ItemID::Type, sf::Shader *> pickupItemTextureShader {
    {ItemID::regenDrug, &Shaders::Flashlight},
    {ItemID::coin,      &Shaders::Flashlight},
    {ItemID::dasher,    &Shaders::Flashlight},
};

void loadItemTextures() {
    itemTexture[ItemID::regenDrug]       ->loadFromFile("sources/textures/regenDrugItem.png");
    itemTexture[ItemID::coin]            ->loadFromFile("sources/textures/coin.png");
    itemTexture[ItemID::dasher]          ->loadFromFile("sources/textures/dasher.png");

    pickupItemTexture[ItemID::regenDrug] ->loadFromFile("sources/textures/regenDrugPickup.png");
    pickupItemTexture[ItemID::coin]      ->loadFromFile("sources/textures/coin.png");
    pickupItemTexture[ItemID::dasher]    ->loadFromFile("sources/textures/dasher.png");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Item : public Interactable {
public:
    ItemID::Type id;
    int amount = 0; // -1 if an item is infinite
    bool pickupable = true;
    bool isDropable = true;
    bool isComponent = false;
    bool isKeyItem = false;
    bool isInInventory = true;

    Item(ItemID::Type, int, bool = true, bool = true, bool = true, bool = false, bool = false);
    Item(const Item& item);

    bool isActivated(CollisionCircle&, sf::Event&) override;
    bool CanBeActivated(CollisionCircle&) override;
    void dropTo(sf::Vector2f);
    void picked(); // call when you pick up
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Item::Item(ItemID::Type _id, int _amount, bool _pickupable, bool _isInInventory, bool _isDropable, bool _isKeyItem, bool _isComponent) {
    id            = _id;
    amount        = _amount;
    pickupable    = _pickupable;
    isDropable    = _isDropable;
    isInInventory = _isInInventory;
    isComponent   = _isComponent;
    isKeyItem     = _isKeyItem;
    if (itemTextureDuration[id] != sf::Time::Zero) {
        setAnimation(*itemTexture[id], itemTextureFrameAmount[id], 1, itemTextureDuration[id], itemTextureShader[id]);
    } else {
        setAnimation(*itemTexture[id], itemTextureShader[id]);
    }
}

Item::Item(const Item& item) {
    id            = item.id;
    amount        = item.amount;
    pickupable    = item.pickupable;
    isDropable    = item.isDropable;
    isInInventory = item.isInInventory;
    isComponent   = item.isComponent;
    isKeyItem     = item.isKeyItem;
    if (itemTextureDuration[id] != sf::Time::Zero) {
        setAnimation(*itemTexture[id], itemTextureFrameAmount[id], 1, itemTextureDuration[id], itemTextureShader[id]);
    } else {
        setAnimation(*itemTexture[id], itemTextureShader[id]);
    }
}

bool Item::CanBeActivated(CollisionCircle& player) {
    return pickupable && !isInInventory && hitbox.intersect(player);
}

bool Item::isActivated(CollisionCircle& player, sf::Event& event) {
    if (pickupable && !isInInventory)
        return hitbox.intersect(player);
    if (isInInventory) {
        if ((event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) &&
           (event.mouseButton.button == sf::Mouse::Button::Right || event.mouseButton.button == sf::Mouse::Button::Left))
            return hitbox.contains(event.mouseButton.x, event.mouseButton.y);
        if (event.type == sf::Event::MouseMoved)
            return hitbox.contains(event.mouseMove.x, event.mouseMove.y);
        return false;
    }
    return false;
}

void Item::dropTo(sf::Vector2f pos) {
    if (pickupItemTextureDuration[id] != sf::Time::Zero) {
        setAnimation(*pickupItemTexture[id], pickupItemTextureFrameAmount[id], 1, pickupItemTextureDuration[id], pickupItemTextureShader[id]);
    } else {
        setAnimation(*pickupItemTexture[id], pickupItemTextureShader[id]);
    }
    setPosition(pos);
    isInInventory = false;
}

void Item::picked() {
    animation->setShader(itemTextureShader[id]);
    isInInventory = true;
}
