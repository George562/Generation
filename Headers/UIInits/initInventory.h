#pragma once
#include "../UI/Bar.h"
#include "../UI/Frame.h"
#include "../UI/Panel.h"
#include "../UI/PolygonButton.h"
#include "../UI/Slot.h"
#include "../UI/TempText.h"
#include "../UI/PlacedText.h"
#include "../Entities/player.h"
#include "../Systems/Inventory.h"
#include "../Systems/Weapon.h"
#include "../Utility/Tools.h"
#include "../Utility/VectorTools.h"

namespace inventoryInterface {
    bool isDrawInventory = false;

    std::vector<bool> doInventoryUpdate(inventoryPage::PageCount);
    inventoryPage::Type activePage = inventoryPage::Items;
    std::vector<sf::Drawable*> commonElements; // These elements appear on every page
    std::vector<std::vector<sf::Drawable*>> pageElements; // These elements only appear on certain pages

    Frame inventoryFrame("InventoryFrame", { 0, 0, scw, sch });
    Frame itemsFrame("PageFrame_items", { 0, 0, scw, sch });
    Frame weaponsFrame("PageFrame_weapons", { 0, 0, scw, sch });
    Frame statsFrame("PageFrame_stats", { 0, 0, scw, sch });

    RectButton itemsButton("inv_itemsBtn", UI::BL, UI::BL, { 0.333f * scw, 0.1f * sch });
    RectButton weaponsButton("inv_weaponsBtn", UI::R, UI::L, { 0.333f * scw, 0.1f * sch });
    RectButton statsButton("inv_statsBtn", UI::R, UI::L, { 0.333f * scw, 0.1f * sch });

    ButtonsHolder frameButtonsHolder({&itemsButton, &weaponsButton, &statsButton});

    RectButton backButton("inv_backBtn", UI::TL, UI::TL, { 300, 150 });

    // ITEM INVENTORY ELEMENTS
    int prevItemTypeCount = 0;
    bool isItemDescDrawn = false;

    Frame itemListBG("itemListBG", UI::center, UI::center, { 0.8f * scw, 0.5f * sch });

    Animation coinSprite("inv_coinAnim", { 0, 0 });
    ItemSlot coinSlot;        // Special slot, so that it can be conveniently used other than in the inventory

    std::vector<ItemSlot> itemSlotsElements; // Elements that comprise an inventory slot - the background texture and the amount text

    Frame statsPlayerImage("statsPlImg", UI::center, UI::L, { 0, 0 });
    Bar<float> statsHPBar("statsHPBar", UI::R, UI::L, { 400, 60 });
    Bar<float> statsMPBar("statsMPBar", UI::R, UI::L, { 400, 60 });

    PlacedText statsHPText("statsHPText", UI::TL, UI::TL),
        statsMPText("statsMPText", UI::TL, UI::TL),
        statsArmorText("statsArmorText", UI::TL, UI::TL),
        statsHPRegenText("statsHPRegenText", UI::TL, UI::TL),
        statsMPRegenText("statsMPRegenText", UI::TL, UI::TL),
        statsCompletedLevelsText("statsComplLvlText", UI::TL, UI::TL),
        statsCurLevelsText("statsCurLvlText", UI::TL, UI::TL);

    PlacedText itemDescText;
    ItemID::Type prevItemDescID;
}

void setUpgradeFunctions();
void updateUpgradeShopStats();

namespace upgradeInterface {
    bool isDrawUpgradeInterface = false;
    std::vector<sf::Drawable*> UIElements;

    Frame BG("upg_BG", { 0, 0, scw, sch });
    Frame weaponImg("upg_weapImg", UI::B, UI::T, { 0, 0 });
    Panel weaponDescPanel("upg_weapDescPanel", UI::T, UI::T, { 0.5f * scw - 50.f, sch * 0.15f });

    PlacedText manaStStat("manaSt_Stat", UI::T, UI::T);
    PlacedText relSpStat("relSp_Stat", UI::B, UI::T);

    PlacedText tthStat("tth_Stat", UI::T, UI::T);
    PlacedText ttdStat("ttd_Stat", UI::B, UI::T);

    PlacedText dmgStat("dmg_Stat", UI::T, UI::T);
    PlacedText frStat("fr_Stat", UI::B, UI::T);
    PlacedText msStat("ms_Stat", UI::B, UI::T);

    PlacedText velStat("vel_Stat", UI::T, UI::T);
    PlacedText scatStat("scat_Stat", UI::B, UI::T);

    RectButton backButton("upg_backBtn", UI::TL, UI::TL, { 250, 50 });
    PolygonButton switchGunLBtn("upg_lGunBtn", UI::L, UI::R, { 0, 0 });
    PolygonButton switchGunRBtn("upg_rGunBtn", UI::R, UI::L, { 0, 0 });


    Frame genStats("genStats_Frame", { 0.1f * scw, 0.5f * sch, scw / 6.f, scw / 6.f });
    PolygonButton generatorBtn("upg_genBtn", UI::B, UI::B, { 0, 0 });
    sf::Color genColor = sf::Color(192, 192, 255, 255);

    Frame ffaStats("ffaStats_Frame", { 0.3f * scw, 0.5f * sch, scw / 6.f, scw / 6.f });
    PolygonButton formFactorBtn("upg_ffaBtn", UI::B, UI::B, { 0, 0 });
    sf::Color ffaColor = sf::Color(255, 255, 192, 255);

    Frame convStats("convStats_Frame", { 0.534f * scw, 0.5f * sch, scw / 6.f, scw / 6.f });
    PolygonButton converterBtn("upg_conBtn", UI::B, UI::B, { 0, 0 });
    sf::Color convColor = sf::Color(192, 255, 192, 255);

    Frame targStats("targStats_Frame", { 0.734f * scw, 0.5f * sch, scw / 6.f, scw / 6.f });
    PolygonButton targetingBtn("upg_tarBtn", UI::B, UI::B, { 0, 0 });
    sf::Color targColor = sf::Color(255, 192, 192, 255);


    // UPGRADE SHOP COMPONENT CHOICE ELEMENTS
    std::vector<sf::Drawable*> choiceUIElements;
    Frame choiceBG("choiceUI_BG", { 0, 0, scw, sch });
    Frame choiceComp("choiceUI_comp", UI::center, UI::center, { 0.75f * scw, 0.5f * sch });
    Panel choiceMessage("choiceUI_msg", UI::T, UI::B, { 0.5f * scw, 0.125f * sch });
    Frame choiceCompImg("choiceUI_img", UI::R, UI::R, { 0.25f * scw, 0.25f * sch });
    std::vector<UIElement*> compUpgPages;
    std::vector<std::vector<RectButton*>> compUpgBtns;
    std::vector<std::vector<PlacedText*>> compUpgCosts;
    std::vector<std::vector<PlacedText*>> compUpgStats;
    std::vector<std::vector<PlacedText*>> compUpgCount;
    Animation coinSprite("upg_coinAnim", UI::BR, UI::BR);
    PlacedText playerCoinAmount("plCoin_amount", UI::L, UI::R);

    bool isChoosingComponent = false;
    int compType = 0;

    void openComponentUpgrade(int type) {
        isChoosingComponent = true;
        removeUI(&playerCoinAmount, choiceUIElements);
        removeUI(compUpgPages[compType], choiceUIElements);
        compType = type;
        addUI(&choiceBG, choiceUIElements);
        addUI(compUpgPages[compType], choiceUIElements);
        std::string compName;
        sf::Texture* compTexture = &Textures::INVISIBLE;;
        switch (type) {
            case 0:
                compName = "Coch Generator";
                compTexture = &Textures::PH_CochGen;
                break;
            case 1:
                compName = "Form Factor";
                compTexture = &Textures::PH_FormFactor;
                break;
            case 2:
                compName = "Converter";
                compTexture = &Textures::PH_Converter;
                break;
            case 3:
                compName = "Targeting";
                compTexture = &Textures::PH_Targeting;
                break;
            default:
                compName = errStr;
        }
        if (inventoryInterface::isDrawInventory) {
            choiceMessage.setFontString(FontString("You can only upgrade at a special upgrade shop.",
                                                   40, CommonColors::warning));
            removeUI(&playerCoinAmount, choiceUIElements);
        } else {
            choiceMessage.setFontString(FontString("Choose a " + compName + " upgrade   (press RMB to close)",
                                                   40, CommonColors::text));
            addUI(&playerCoinAmount, choiceUIElements);
        }
        choiceCompImg.setTexture(*compTexture, UI::TextureResize::texture);
        choiceCompImg.moveToAnchor(&choiceComp, { -50, 0 });
    }
}

void openUpgradeShop();

void initInventory(Player* player) {
    {
        using namespace inventoryInterface;

        pageElements.resize(inventoryPage::PageCount);
        itemSlotsElements.resize(ItemID::ItemCount, ItemSlot());

        inventoryFrame.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setWord(FontString("Back", 52));
        backButton.setFunction([]() { isDrawInventory = false; });
        backButton.parentTo(&inventoryFrame, true);

        itemsButton.setTexture(Textures::YellowButton, Textures::YellowButtonPushed);
        itemsButton.setWord(FontString("Items", 32));
        itemsButton.setFunction([]() {
            activePage = inventoryPage::Items;
            addUI(&itemsFrame, pageElements[activePage]);
        });
        itemsButton.parentTo(&inventoryFrame, true);

        weaponsButton.setTexture(Textures::RedButton, Textures::RedButtonPushed);
        weaponsButton.setWord(FontString("Weapons", 32));
        weaponsButton.setFunction([]() {
            activePage = inventoryPage::Weapons;
            addUI(&weaponsFrame, pageElements[activePage]);
            addUI(&upgradeInterface::BG, pageElements[activePage]);
            removeUI(&upgradeInterface::BG, pageElements[activePage], false);
            removeUI(&upgradeInterface::backButton, pageElements[activePage], false);
            openUpgradeShop();
        });
        weaponsButton.parentTo(&itemsButton, true);

        statsButton.setTexture(Textures::GreyButton, Textures::GreyButtonPushed);
        statsButton.setWord(FontString("Stats", 32));
        statsButton.setFunction([]() {
            activePage = inventoryPage::Stats;
            addUI(&statsFrame, pageElements[activePage]);
        });
        statsButton.parentTo(&weaponsButton, true);

        itemListBG.setTexture(Textures::GradientFrameAlpha);
        itemListBG.parentTo(&itemsFrame, true);

        statsPlayerImage.setTexture(Textures::Player, UI::texture);
        statsPlayerImage.parentTo(&statsFrame, true, { 200, 0 });

        int fontSize = 40;
        float yOffset = sch / 10;
        statsHPText.setFontString(FontString("Health", fontSize));
        statsHPText.parentTo(&statsFrame, true, { 200, 200 });
        statsHPBar.setValue(player->Health);
        statsHPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 255), CommonColors::barBG);
        statsHPBar.parentTo(&statsHPText, true, { 300, 0 });

        statsMPText.setFontString(FontString("Mana", fontSize));
        statsMPText.parentTo(&statsHPText, true, { 0, yOffset });
        statsMPBar.setValue(player->Mana);
        statsMPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 255), CommonColors::barBG);
        statsMPBar.parentTo(&statsMPText);
        statsMPBar.setPosition(statsHPBar.getPosition() + sf::Vector2f{ 0, yOffset });

        statsHPRegenText.setFontString(FontString("Health regen: " + floatToString(player->HealthRecovery), fontSize));
        statsHPRegenText.parentTo(&statsMPText, true, { 0, yOffset });

        statsMPRegenText.setFontString(FontString("Mana regen: " + floatToString(player->ManaRecovery), fontSize));
        statsMPRegenText.parentTo(&statsHPRegenText, true, { 0, yOffset });

        statsArmorText.setFontString(FontString("Armor: " + floatToString(player->Armor.cur), fontSize));
        statsArmorText.parentTo(&statsMPRegenText, true, { 0, yOffset });

        statsCompletedLevelsText.setFontString(FontString("Completed Levels: " + std::to_string(completedLevels), fontSize));
        statsCompletedLevelsText.parentTo(&statsArmorText, true, { 0, yOffset });

        statsCurLevelsText.setFontString(FontString("Current Levels: " + std::to_string(curLevel), fontSize));
        statsCurLevelsText.parentTo(&statsCompletedLevelsText, true, { 0, yOffset });

        coinSprite.parentTo(&itemsFrame);
        coinSprite.setPosition(backButton.getRightTop() + sf::Vector2f(200, 25));
        coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                1, itemTextureDuration[ItemID::coin]);
        coinSprite.play();

        coinSlot.init("coinSlot");
        coinSlot.amountText->setAnchors(UI::R, UI::L);
        coinSlot.amountText->setFontString(FontString("0", 50));
        coinSlot.amountText->parentTo(&coinSprite, true, { 50, 0 });
        doInventoryUpdate[inventoryPage::Items] = true;
        doInventoryUpdate[inventoryPage::Weapons] = true;
        doInventoryUpdate[inventoryPage::Stats] = true;

        addUI(&inventoryFrame, commonElements);

        itemDescText.setOutlineThickness(2);
        itemDescText.setOutlineColor(sf::Color::Black);
    }
}

void initUpgradeUI();

void initUpgradeShop() {
    {
        using namespace upgradeInterface;
        BG.setTexture(Textures::GridBG);

        backButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed);
        backButton.setWord(FontString("Back", 36));
        backButton.setFunction([]() { isDrawUpgradeInterface = false; });
        backButton.parentTo(&BG, true);

        weaponDescPanel.setTexture(Textures::GradientFrameAlpha);
        weaponDescPanel.setSpriteColor(sf::Color::Black);
        weaponDescPanel.setCharacterSize(30);
        weaponDescPanel.parentTo(&BG, true, { 0, 75 });

        manaStStat.setFontString(FontString("Mana storage", 30));
        relSpStat.setFontString(FontString("Reload Speed", 30));
        tthStat.setFontString(FontString("Time To Holster", 30));
        ttdStat.setFontString(FontString("Time To Dispatch", 30));
        dmgStat.setFontString(FontString("Damage", 30));
        msStat.setFontString(FontString("Bullet per shot", 30));
        frStat.setFontString(FontString("Rate of fire", 30));
        velStat.setFontString(FontString("Bullet velocity", 30));
        scatStat.setFontString(FontString("Scatter", 30));

        weaponImg.setTexture(Textures::PH_Pistol, UI::texture);
        weaponImg.parentTo(&weaponDescPanel, true, { 0, 25 });

        switchGunLBtn.setHitboxPoints(std::vector<sf::Vector2f>{{50, 100}, { 50, 0 }, { 0, 50 }});
        switchGunLBtn.setShape(sf::Color::Black, sf::Color::White, 2);
        switchGunLBtn.enableShape(true);
        switchGunLBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunLBtn.parentTo(&weaponImg, true, { -50, 0 });

        switchGunRBtn.setHitboxPoints(std::vector<sf::Vector2f>{{0, 100}, { 0, 0 }, { 50, 50 }});
        switchGunRBtn.setShape(sf::Color::Black, sf::Color::White, 2);
        switchGunRBtn.enableShape(true);
        switchGunRBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        switchGunRBtn.parentTo(&weaponImg, true, { 50, 0 });

        choiceComp.setTexture(Textures::GradientFrameAlpha);
        choiceComp.parentTo(&choiceBG, true);

        sf::Texture* fadeTexture = new sf::Texture();
        sf::Image fadeTexturePixels = sf::Image();
        fadeTexturePixels.create(scw, sch, sf::Color(255, 255, 255, 128));
        for (int i = 0; i < scw; i++)
            for (int j = 0; j < sch; j++)
                if (i >= choiceComp.getLeft() && i <= choiceComp.getRight() &&
                    j >= choiceComp.getTop() && j <= choiceComp.getBottom()) {
                    float gradVal = 25 * (((int)(i - choiceComp.getLeft() + j - choiceComp.getTop()) / (255)) % 25);
                    fadeTexturePixels.setPixel(i, j, sf::Color(gradVal, gradVal, gradVal, 255));
                }
        fadeTexture->create(scw, sch);
        fadeTexture->update(fadeTexturePixels);
        choiceBG.setTexture(*fadeTexture);

        choiceMessage.setTexture(Textures::GradientFrame);
        choiceMessage.setFontString(FontString("Choose a component upgrade", 40));
        choiceMessage.parentTo(&choiceComp, true, { 0, -100 });

        choiceCompImg.setTexture(Textures::INVISIBLE);
        choiceCompImg.parentTo(&choiceComp, true, { -50, 0 });

        sf::Vector2f statSpacing(0, sch / 34.f);
        genStats.setTexture(Textures::GradientFrame);
        genStats.setSpriteColor(genColor);
        genStats.parentTo(&BG);

        generatorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        generatorBtn.setHitboxPoints(CommonShapes::starShape);
        generatorBtn.hitbox.setScale({2. / 3., 2. / 3.});
        generatorBtn.enableShape(true);
        generatorBtn.setFunction([]() { openComponentUpgrade(0); });
        std::vector<float> innerColor = RGBtoHSV(genColor);
        float hueShift = 20, satShift = -0.1, valShift = -0.2;
        innerColor[0] += hueShift; innerColor[1] += satShift; innerColor[2] += valShift;
        generatorBtn.setShape(HSVtoRGB(innerColor), genColor, 3);
        generatorBtn.parentTo(&genStats, true, -statSpacing / 2.f);

        manaStStat.parentTo(&genStats, true, statSpacing);
        relSpStat.parentTo(&manaStStat, true, statSpacing);

        ffaStats.setTexture(Textures::GradientFrame);
        ffaStats.setSpriteColor(ffaColor);
        ffaStats.parentTo(&BG);

        formFactorBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        formFactorBtn.setHitboxPoints(CommonShapes::rectShape);
        formFactorBtn.hitbox.setScale({2. / 3., 2. / 3.});
        formFactorBtn.enableShape(true);
        formFactorBtn.setFunction([]() { openComponentUpgrade(1); });
        innerColor = RGBtoHSV(ffaColor);
        innerColor[0] += hueShift; innerColor[1] += satShift; innerColor[2] += valShift;
        formFactorBtn.setShape(HSVtoRGB(innerColor), ffaColor, 3);
        formFactorBtn.parentTo(&ffaStats, true, -statSpacing / 2.f);

        tthStat.parentTo(&ffaStats, true, statSpacing);
        ttdStat.parentTo(&tthStat, true, statSpacing);

        convStats.setTexture(Textures::GradientFrame);
        convStats.setSpriteColor(convColor);
        convStats.parentTo(&BG);

        converterBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        converterBtn.setHitboxPoints(CommonShapes::triangleShape);
        converterBtn.hitbox.setScale({2. / 3., 2. / 3.});
        converterBtn.enableShape(true);
        converterBtn.setFunction([]() { openComponentUpgrade(2); });
        innerColor = RGBtoHSV(convColor);
        innerColor[0] += hueShift; innerColor[1] += satShift; innerColor[2] += valShift;
        converterBtn.setShape(HSVtoRGB(innerColor), convColor, 3);
        converterBtn.parentTo(&convStats, true, -statSpacing / 2.f);

        dmgStat.parentTo(&convStats, true, statSpacing);
        msStat.parentTo(&dmgStat, true, statSpacing);
        frStat.parentTo(&msStat, true, statSpacing);

        targStats.setTexture(Textures::GradientFrame);
        targStats.setSpriteColor(targColor);
        targStats.parentTo(&BG);

        targetingBtn.setTexture(Textures::INVISIBLE, Textures::INVISIBLE);
        std::vector<sf::Vector2f> rotatedFrustum = RotateOn(DegToRad(90), CommonShapes::frustumShape);
        targetingBtn.setHitboxPoints(Translate({ 0, 150 }, rotatedFrustum));
        targetingBtn.hitbox.setScale({2. / 3., 2. / 3.});
        targetingBtn.enableShape(true);
        targetingBtn.setFunction([]() { openComponentUpgrade(3); });
        innerColor = RGBtoHSV(targColor);
        innerColor[0] += hueShift; innerColor[1] += satShift; innerColor[2] += valShift;
        targetingBtn.setShape(HSVtoRGB(innerColor), targColor, 3);
        targetingBtn.parentTo(&targStats, true, -statSpacing / 2.f);

        velStat.parentTo(&targStats, true, statSpacing);
        scatStat.parentTo(&velStat, true, statSpacing);

        coinSprite.setScale({ 0.5, 0.5 });
        coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                                1, itemTextureDuration[ItemID::coin]);
        coinSprite.play();
        coinSprite.moveToAnchor(&BG, { -20, -10 });

        playerCoinAmount.setFontString(FontString("", 50, sf::Color(200, 200, 200)));
        playerCoinAmount.parentTo(&coinSprite, true, { -10, 0 });
        playerCoinAmount.setOutlineColor(sf::Color::Black);
        playerCoinAmount.setOutlineThickness(2.f);

        initUpgradeUI();
    }
}

void initUpgradeUI() {
    {
        using namespace upgradeInterface;

        int btnTextSize = 24;
        sf::Vector2f costOffset = sf::Vector2f(300, -25);
        sf::Vector2f countOffset = sf::Vector2f(0, -24);
        sf::Vector2f statOffset = sf::Vector2f(0, 0);

        compUpgBtns.resize(4);
        compUpgCosts.resize(4);
        compUpgStats.resize(4);
        compUpgCount.resize(4);

        compUpgPages.push_back(new Frame("upgPage_CGen", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_FFac", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_Conv", choiceComp.getRect()));
        compUpgPages.push_back(new Frame("upgPage_Targ", choiceComp.getRect()));

        compUpgBtns[0].push_back(new RectButton("upg_MMSBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Mana storage", btnTextSize))
        );
        compUpgBtns[0][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[0][0]->parentTo(compUpgPages[0], true, { 50, 50 });

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][0]->setPosition(compUpgBtns[0][0]->getCenter() + costOffset);
        compUpgStats[0][0]->setPosition(compUpgBtns[0][0]->getRightTop() + statOffset);
        compUpgCount[0][0]->setPosition(compUpgBtns[0][0]->getRightBottom() + countOffset);


        compUpgBtns[0].push_back(new RectButton("upg_RSBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Reload Speed", btnTextSize))
        );
        compUpgBtns[0][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[0][1]->parentTo(compUpgBtns[0][0], true, { 0, 50 });

        compUpgCosts[0].push_back(new PlacedText());
        compUpgStats[0].push_back(new PlacedText());
        compUpgCount[0].push_back(new PlacedText());
        compUpgCosts[0][1]->setPosition(compUpgBtns[0][1]->getCenter() + costOffset);
        compUpgStats[0][1]->setPosition(compUpgBtns[0][1]->getRightTop() + statOffset);
        compUpgCount[0][1]->setPosition(compUpgBtns[0][1]->getRightBottom() + countOffset);


        compUpgBtns[1].push_back(new RectButton("upg_TTHBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Time To\nHolster", btnTextSize))
        );
        compUpgBtns[1][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[1][0]->parentTo(compUpgPages[1], true, { 50, 50 });

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][0]->setPosition(compUpgBtns[1][0]->getCenter() + costOffset);
        compUpgStats[1][0]->setPosition(compUpgBtns[1][0]->getRightTop() + statOffset);
        compUpgCount[1][0]->setPosition(compUpgBtns[1][0]->getRightBottom() + countOffset);


        compUpgBtns[1].push_back(new RectButton("upg_TTDBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Time To\nDispatch", btnTextSize))
        );
        compUpgBtns[1][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[1][1]->parentTo(compUpgBtns[1][0], true, { 0, 50 });

        compUpgCosts[1].push_back(new PlacedText());
        compUpgStats[1].push_back(new PlacedText());
        compUpgCount[1].push_back(new PlacedText());
        compUpgCosts[1][1]->setPosition(compUpgBtns[1][1]->getCenter() + costOffset);
        compUpgStats[1][1]->setPosition(compUpgBtns[1][1]->getRightTop() + statOffset);
        compUpgCount[1][1]->setPosition(compUpgBtns[1][1]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_FRBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Fire Rate", btnTextSize))
        );
        compUpgBtns[2][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[2][0]->parentTo(compUpgPages[2], true, { 50, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][0]->setPosition(compUpgBtns[2][0]->getCenter() + costOffset);
        compUpgStats[2][0]->setPosition(compUpgBtns[2][0]->getRightTop() + statOffset);
        compUpgCount[2][0]->setPosition(compUpgBtns[2][0]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_MCBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Mana Cost", btnTextSize))
        );
        compUpgBtns[2][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[2][1]->parentTo(compUpgBtns[2][0], true, { 0, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][1]->setPosition(compUpgBtns[2][1]->getCenter() + costOffset);
        compUpgStats[2][1]->setPosition(compUpgBtns[2][1]->getRightTop() + statOffset);
        compUpgCount[2][1]->setPosition(compUpgBtns[2][1]->getRightBottom() + countOffset);


        compUpgBtns[2].push_back(new RectButton("upg_MSBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Multishot", btnTextSize))
        );
        compUpgBtns[2][2]->setTexture(Textures::RedButton, Textures::RedButtonPushed);
        compUpgBtns[2][2]->parentTo(compUpgBtns[2][1], true, { 0, 50 });

        compUpgCosts[2].push_back(new PlacedText());
        compUpgStats[2].push_back(new PlacedText());
        compUpgCount[2].push_back(new PlacedText());
        compUpgCosts[2][2]->setPosition(compUpgBtns[2][2]->getCenter() + costOffset);
        compUpgStats[2][2]->setPosition(compUpgBtns[2][2]->getRightTop() + statOffset);
        compUpgCount[2][2]->setPosition(compUpgBtns[2][2]->getRightBottom() + countOffset);

        compUpgBtns[3].push_back(new RectButton("upg_BVBtn",
                                                UI::TL, UI::TL, { 250, 100 },
                                                FontString("Bullet Velocity", btnTextSize))
        );
        compUpgBtns[3][0]->setTexture(Textures::BlueButton, Textures::BlueButtonPushed);
        compUpgBtns[3][0]->parentTo(compUpgPages[3], true, { 50, 50 });

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][0]->setPosition(compUpgBtns[3][0]->getCenter() + costOffset);
        compUpgStats[3][0]->setPosition(compUpgBtns[3][0]->getRightTop() + statOffset);
        compUpgCount[3][0]->setPosition(compUpgBtns[3][0]->getRightBottom() + countOffset);

        compUpgBtns[3].push_back(new RectButton("upg_SCBtn",
                                                UI::BL, UI::TL, { 250, 100 },
                                                FontString("Scatter", btnTextSize))
        );
        compUpgBtns[3][1]->setTexture(Textures::GreenButton, Textures::GreenButtonPushed);
        compUpgBtns[3][1]->parentTo(compUpgBtns[3][0], true, { 0, 50 });

        compUpgCosts[3].push_back(new PlacedText());
        compUpgStats[3].push_back(new PlacedText());
        compUpgCount[3].push_back(new PlacedText());
        compUpgCosts[3][1]->setPosition(compUpgBtns[3][1]->getCenter() + costOffset);
        compUpgStats[3][1]->setPosition(compUpgBtns[3][1]->getRightTop() + statOffset);
        compUpgCount[3][1]->setPosition(compUpgBtns[3][1]->getRightBottom() + countOffset);
    }
}