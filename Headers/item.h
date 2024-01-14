#pragma once
#include "interactible.h"

namespace ItemID {
    using Type = sf::Uint8;
    enum : Type {
        gunParts,
        repairKit,
        keyCard,
        protectiveSuit,
        generic,
        medkit,
        regenDrug,
        coin,
        NONE
    };
}

//////////////////////////////////////////////////////////// Item Animation params
std::map<ItemID::Type, sf::Texture*> itemTextureName {
    {ItemID::gunParts, new sf::Texture},
    {ItemID::repairKit, new sf::Texture},
    {ItemID::keyCard, new sf::Texture},
    {ItemID::protectiveSuit, new sf::Texture},
    {ItemID::regenDrug, new sf::Texture},
    {ItemID::coin, new sf::Texture}
};

std::map<ItemID::Type, int> itemTextureFrameAmount {
    {ItemID::generic, 1},
    {ItemID::medkit, 1},
    {ItemID::regenDrug, 1},
    {ItemID::coin, 4}
};

std::map<ItemID::Type, sf::Time> itemTextureDuration {
    {ItemID::generic, sf::seconds(1)},
    {ItemID::medkit, sf::seconds(1)},
    {ItemID::regenDrug, sf::seconds(1)},
    {ItemID::coin, sf::seconds(1)}
};

std::map<ItemID::Type, sf::Shader *> itemTextureShader {
    {ItemID::generic, &PickupItemShader},
    {ItemID::medkit, &PickupItemShader},
    {ItemID::regenDrug, &PickupItemShader},
    {ItemID::coin, &PickupItemShader}
};

//////////////////////////////////////////////////////////// Pickup Item Animation params
std::map<ItemID::Type, sf::Texture*> pickupItemTextureName {
    {ItemID::generic, new sf::Texture},
    {ItemID::medkit, new sf::Texture},
    {ItemID::regenDrug, new sf::Texture},
    {ItemID::coin, new sf::Texture}
};

std::map<ItemID::Type, int> pickupItemTextureFrameAmount {
    {ItemID::generic, 1},
    {ItemID::medkit, 1},
    {ItemID::regenDrug, 1},
    {ItemID::coin, 4}
};

std::map<ItemID::Type, sf::Time> pickupItemTextureDuration {
    {ItemID::generic, sf::seconds(1)},
    {ItemID::medkit, sf::seconds(1)},
    {ItemID::regenDrug, sf::seconds(1)},
    {ItemID::coin, sf::seconds(1)}
};

std::map<ItemID::Type, sf::Shader *> pickupItemTextureShader {
    {ItemID::generic, &MapShader},
    {ItemID::medkit, &MapShader},
    {ItemID::regenDrug, &MapShader},
    {ItemID::coin, &MapShader}
};

void loadItemTextures() {
    // itemTextureName[ItemID::gunParts]       ->loadFromFile("sources/textures/gunPartsItem.png");
    // itemTextureName[ItemID::repairKit]      ->loadFromFile("sources/textures/repairKitItem.png");
    // itemTextureName[ItemID::keyCard]        ->loadFromFile("sources/textures/keyCardItem.png");
    // itemTextureName[ItemID::protectiveSuit] ->loadFromFile("sources/textures/protectiveSuitItem.png");
    itemTextureName[ItemID::regenDrug]      ->loadFromFile("sources/textures/regenDrugItem.png");
    itemTextureName[ItemID::coin]           ->loadFromFile("sources/textures/coin.png");

    // pickupItemTextureName[ItemID::generic]   ->loadFromFile("sources/textures/genericPickup.png");
    // pickupItemTextureName[ItemID::medkit]    ->loadFromFile("sources/textures/medkitPickup.png");
    pickupItemTextureName[ItemID::regenDrug] ->loadFromFile("sources/textures/regenDrugPickup.png");
    pickupItemTextureName[ItemID::coin]      ->loadFromFile("sources/textures/coin.png");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Item : public Interactable {
public:
    ItemID::Type id;
    int amount = 0;
    bool pickupable = true;
    bool isDropable = true;
    bool isEquippable = false;
    bool isKeyItem = false;
    bool isInInventory = true;

    Item(ItemID::Type, int, bool = true, bool = true, bool = true, bool = false, bool = false);

    bool isActivated(Circle&, sf::Event&) override;
    bool CanBeActivated(Circle&) override;
    void dropTo(sf::Vector2f);
    void picked(); // call when you pick up
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Item::Item(ItemID::Type _id, int _amount, bool _pickapable, bool _isInInventory, bool _isDropable, bool _isKeyItem, bool _isEquippable) {
    id            = _id;
    amount        = _amount;
    pickupable    = _pickapable;
    isDropable    = _isDropable;
    isInInventory = _isInInventory;
    isEquippable  = _isEquippable;
    isKeyItem     = _isKeyItem;

    setAnimation(*itemTextureName[id], itemTextureFrameAmount[id], 1, itemTextureDuration[id], itemTextureShader[id]);
}

bool Item::CanBeActivated(Circle& player) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

bool Item::isActivated(Circle& player, sf::Event& event) {
    if (pickupable && !isInInventory)
        return intersect(player);
    return false;
}

void Item::dropTo(sf::Vector2f pos) {
    setAnimation(*pickupItemTextureName[id], pickupItemTextureFrameAmount[id], 1, pickupItemTextureDuration[id], pickupItemTextureShader[id]);
    setPosition(pos);
    isInInventory = false;
}

void Item::picked() {
    animation->setShader(itemTextureShader[id]);
    isInInventory = true;
}

void operator +=(Item& sumTo, Item& sumItem) {
    if (sumTo.id == sumItem.id) {
        sumTo.amount += sumItem.amount;
    }
}