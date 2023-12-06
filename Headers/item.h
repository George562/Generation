#pragma once
#include "interactible.h"

enum ItemID : sf::Uint8 {
    gunParts,
    repairKit,
    keyCard,
    protectiveSuit,
    generic,
    medkit,
    regenDrug,
    coin
};

//////////////////////////////////////////////////////////// Item Animation params
std::map<ItemID, sf::Texture*> itemTextureName {
    {gunParts, new sf::Texture},
    {repairKit, new sf::Texture},
    {keyCard, new sf::Texture},
    {protectiveSuit, new sf::Texture},
    {regenDrug, new sf::Texture},
    {coin, new sf::Texture}
};

std::map<ItemID, int> ItemTextureFrameAmount {
    {generic, 1},
    {medkit, 1},
    {regenDrug, 1},
    {coin, 4}
};

std::map<ItemID, sf::Time> ItemTextureDuration {
    {generic, sf::seconds(1)},
    {medkit, sf::seconds(1)},
    {regenDrug, sf::seconds(1)},
    {coin, sf::seconds(1)}
};

std::map<ItemID, sf::Shader *> ItemTextureShader {
    {generic, &MapShader},
    {medkit, &MapShader},
    {regenDrug, &MapShader},
    {coin, &MapShader}
};

//////////////////////////////////////////////////////////// Pickup Item Animation params
std::map<ItemID, sf::Texture*> pickupItemTextureName {
    {generic, new sf::Texture},
    {medkit, new sf::Texture},
    {regenDrug, new sf::Texture},
    {coin, new sf::Texture}
};

std::map<ItemID, int> pickupItemTextureFrameAmount {
    {generic, 1},
    {medkit, 1},
    {regenDrug, 1},
    {coin, 4}
};

std::map<ItemID, sf::Time> pickupItemTextureDuration {
    {generic, sf::seconds(1)},
    {medkit, sf::seconds(1)},
    {regenDrug, sf::seconds(1)},
    {coin, sf::seconds(1)}
};

std::map<ItemID, sf::Shader *> pickupItemTextureShader {
    {generic, &MapShader},
    {medkit, &MapShader},
    {regenDrug, &MapShader},
    {coin, &MapShader}
};

void loadItemTextures() {
    // itemTextureName[gunParts]       ->loadFromFile("sources/textures/gunPartsItem.png");
    // itemTextureName[repairKit]      ->loadFromFile("sources/textures/repairKitItem.png");
    // itemTextureName[keyCard]        ->loadFromFile("sources/textures/keyCardItem.png");
    // itemTextureName[protectiveSuit] ->loadFromFile("sources/textures/protectiveSuitItem.png");
    itemTextureName[regenDrug]      ->loadFromFile("sources/textures/regenDrugItem.png");
    itemTextureName[coin]           ->loadFromFile("sources/textures/coin.png");

    // pickupItemTextureName[generic]   ->loadFromFile("sources/textures/genericPickup.png");
    // pickupItemTextureName[medkit]    ->loadFromFile("sources/textures/medkitPickup.png");
    pickupItemTextureName[regenDrug] ->loadFromFile("sources/textures/regenDrugPickup.png");
    pickupItemTextureName[coin]      ->loadFromFile("sources/textures/coin.png");
}

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

#pragma pack(push, 1)
class Item : public Interactable {
public:
    ItemID id;
    int amount = 0;
    bool pickupable = true;
    bool isDropable = true;
    bool isEquippable = false;
    bool isKeyItem = false;
    bool isInInventory = true;

    Item(ItemID, int, bool = true, bool = true, bool = true, bool = false, bool = false);

    void setPosition(sf::Vector2f);
    bool isActivated(Circle&, sf::Event&) override;
    bool CanBeActivated(Circle&) override;
    void dropTo(sf::Vector2f);
};
#pragma pack(pop)

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

Item::Item(ItemID _id, int _amount, bool _pickapable, bool _isInInventory, bool _isDropable, bool _isKeyItem, bool _isEquippable) {
    id            = _id;
    amount        = _amount;
    pickupable    = _pickapable;
    isDropable    = _isDropable;
    isInInventory = _isInInventory;
    isEquippable  = _isEquippable;
    isKeyItem     = _isKeyItem;

    setAnimation(*itemTextureName[id], ItemTextureFrameAmount[id], 1, ItemTextureDuration[id], ItemTextureShader[id]);
}

void Item::setPosition(sf::Vector2f v) {
    PosX = v.x; PosY = v.y;
    animation->setPosition(v);
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

void operator +=(Item& sumTo, Item& sumItem) {
    if (sumTo.id == sumItem.id) {
        sumTo.amount += sumItem.amount;
    }
}