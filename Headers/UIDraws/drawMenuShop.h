#pragma once
#include "../UIInits/initMenuShop.h"

void drawShop(sf::RenderWindow& window, Player& player) {
    window.setView(InterfaceView);
    {
        using namespace MenuShop;
        updateShopUI(player);

        for (sf::Drawable*& elem : UIElements)
            window.draw(*elem);

        window.setView(ShopStockView);
        for (Item*& item : shop.soldItems.items) {
            if (slotsElements[item->id].isInitialized) {
                window.draw(slotsElements[item->id], stockTransform);
                window.draw(*item, stockTransform);
            }
        }

        window.setView(ShopPlayerInvView);
        for (Item*& item : player.inventory.items) {
            if (playerSlotsElements[item->id].isInitialized) {
                window.draw(playerSlotsElements[item->id], playerInvTransform);
                window.draw(*item, playerInvTransform);
            }
        }

    }
    window.setView(HUDView);
}