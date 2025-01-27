#pragma once
#include "../UIInits/initInventory.h"

void upgradeInterface::drawChoosingComponent(sf::RenderWindow& window) {
    float listBGVal = 128 * std::pow(std::sin(GameTime.asSeconds()), 2);
    if (isChoosingComponent) {
        switch (compType) {
            case 0:
                choiceComp.setSpriteColor(sf::Color(0, 0, listBGVal));
                break;
            case 1:
                choiceComp.setSpriteColor(sf::Color(listBGVal, listBGVal, 0));
                break;
            case 2:
                choiceComp.setSpriteColor(sf::Color(0, listBGVal, 0));
                break;
            case 3:
                choiceComp.setSpriteColor(sf::Color(listBGVal, 0, 0));
                break;
        }
        for (sf::Drawable* elem : choiceUIElements)
            window.draw(*elem);

        for (sf::Drawable* elem : compUpgCosts[compType])
            window.draw(*elem);
        for (sf::Drawable* elem : compUpgStats[compType])
            window.draw(*elem);
        for (sf::Drawable* elem : compUpgCount[compType])
            window.draw(*elem);
    }
}

void inventoryInterface::drawInventory(sf::RenderWindow& window, Player& player) {
    window.setView(InventoryView);
    updateInventoryUI(player);

    for (sf::Drawable*& elem : commonElements)
        window.draw(*elem);
    switch (activePage) {
        case inventoryPage::Items:
            for (sf::Drawable*& elem : pageElements[activePage])
                window.draw(*elem);

            for (Item*& item : player.inventory.items) {
                window.draw(itemSlotsElements[item->id]);
                window.draw(*item);
            }

            if (isItemDescDrawn) {
                itemDescText.setPosition(sf::Mouse::getPosition(window).x + 100, sf::Mouse::getPosition(window).y);
                window.draw(itemDescText);
            }
            break;

        case inventoryPage::Weapons:
            for (sf::Drawable*& elem : pageElements[activePage])
                window.draw(*elem);
            break;

        case inventoryPage::Stats:
            for (sf::Drawable*& elem : pageElements[inventoryPage::Stats])
                window.draw(*elem);
            break;

        default:
            break;
    }
    window.setView(HUDView);
}

void upgradeInterface::drawUpgradeInterface(sf::RenderWindow& window, Player& player) {
    window.setView(InterfaceView);
    updateUpgradeInterfaceUI(player);
    for (sf::Drawable*& elem : UIElements) // not working
        window.draw(*elem);

    drawChoosingComponent(window);

    window.draw(coinSprite);
    window.draw(playerCoinAmount);
    window.setView(HUDView);
}