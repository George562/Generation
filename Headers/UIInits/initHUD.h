#pragma once
#include "../UI/bar.h"
#include "../UI/Frame.h"
#include "../UI/panel.h"
#include "../UI/PolygonButton.h"
#include "../UI/Slot.h"
#include "../UI/tempText.h"
#include "../Systems/effect.h"
#include "../Entities/player.h"

namespace HUD {
    void initHUD(Player& player, std::vector<Weapon*>& Weapons);
    void drawHUD(sf::RenderWindow& window, Player& player, std::vector<Weapon*>& Weapons);
    void drawEffects(sf::RenderWindow& window, Player& player);
    void updateHUDUI(Player& player);
    void loadDescriptions();
    void displayDescription(DescriptionID::Type id);

    bool EscapeMenuActivated = false, IsDrawHUD = true, isDrawSettings = false, showFPS = false;

    std::vector<sf::Drawable*> InterfaceStuff;
    std::vector<TempText*> TempTextsOnScreen, MessageText;
    std::vector<Frame*> effectIcons(Effects::EffectCount);
    std::vector<PlacedText*> effectIconsTimers(Effects::EffectCount);

//////////////////////////////////////////////////////////// FPS Stuff
    sf::Clock ClockFPS; int FPSCounter;
    PlacedText TextFPS;

//////////////////////////////////////////////////////////// Panels
    Panel ListOfPlayers("listOfPlayers");

//////////////////////////////////////////////////////////// Buttons
    RectButton EscapeButton("exitBtn", UI::L, UI::R, { 0, 0 });
    RectButton SettingsButton("settingsBtn", UI::center, UI::center, { 0, 0 });
    RectButton EncyclopediaButton("encyclopediaBtn", UI::R, UI::L, { 0, 0 });
    RectButton FPSButton("settings_FPSBtn");

//////////////////////////////////////////////////////////// HUDStuff
    Frame HUDFrame("HUD", { 0, 0, scw, sch });
    Bar<float> HPBar("HPBar", UI::TR, UI::TR, { 360, 50 }), MPBar("MPBar", UI::BL, UI::TL, { 240, 50 });
    std::vector<Bar<float>*> AmmoBars;
    std::vector<PlacedText*> WeaponNameTexts;
    PlacedText ReloadWeaponText;
    Frame XButtonSprite("xBtn", UI::L, UI::BL, { 0, 0 });
    Frame InfoLogoSprite("infoBtn", UI::TL, UI::BL, { 0, 0 });

    std::map<DescriptionID::Type, std::string> interactibleDescriptions;
    PlacedText DescriptionText("descTxt", UI::center, UI::center, FontString("", 32));
    bool showDescriptions;
    Animation coinSprite("upg_coinAnim", UI::BR, UI::BR);
    PlacedText playerCoinAmount("plCoin_amount", UI::L, UI::R);
}

void HUD::initHUD(Player& player, std::vector<Weapon*>& Weapons) {
    ListOfPlayers.setTexture(Textures::GradientFrameAlpha);

    EscapeButton.setTexture(Textures::EscapeButton, Textures::EscapeButtonPushed, UI::texture);
    EscapeButton.setHitboxPoints({ EscapeButton.getLeftTop(), EscapeButton.getRightTop(),
                                    EscapeButton.getRightBottom(), EscapeButton.getLeftBottom() });

    SettingsButton.setTexture(Textures::SettingButton, Textures::SettingButtonPushed, UI::texture);
    SettingsButton.setHitboxPoints({ SettingsButton.getLeftTop(), SettingsButton.getRightTop(),
                                        SettingsButton.getRightBottom(), SettingsButton.getLeftBottom() });

    EncyclopediaButton.setTexture(Textures::EncyclopediaButton, Textures::EncyclopediaButtonPushed, UI::texture);
    EncyclopediaButton.setHitboxPoints({ EncyclopediaButton.getLeftTop(), EncyclopediaButton.getRightTop(),
                                            EncyclopediaButton.getRightBottom(), EncyclopediaButton.getLeftBottom() });

    ListOfPlayers.text.setCharacterSize(60);

    SettingsButton.moveToAnchor(&HUDFrame, { 0, sch / 4 });
    EscapeButton.moveToAnchor(&SettingsButton, { -10, 0 });
    EncyclopediaButton.moveToAnchor(&SettingsButton, { 10, 0 });

    ListOfPlayers.moveToAnchor(&HUDFrame, { 0, -sch / 4 });

    HPBar.setValue(player.Health);
    HPBar.setFontString(FontString("", 36));
    HPBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);
    HPBar.parentTo(&HUDFrame, true, { -30, 20 });

    MPBar.setValue(player.Mana);
    MPBar.setFontString(FontString("", 32));
    MPBar.setColors(CommonColors::barWall, sf::Color(0, 0, 192, 160), CommonColors::barBG);
    MPBar.parentTo(&HPBar, true);

    for (int i = 0; i < Weapons.size(); i++) {
        AmmoBars.push_back(new Bar<float>("AmmoBar_" + Weapons[i]->Name, UI::TL, UI::BL, { 160, 50 }));
        AmmoBars[i]->setColors(CommonColors::barWall, sf::Color(128, 128, 128, 160), CommonColors::barBG);
    }
    AmmoBars.back()->setAnchor(UI::BL);
    AmmoBars.back()->parentTo(&HUDFrame, true, { 50, -20 });
    for (int i = AmmoBars.size() - 2; i >= 0; i--) {
        AmmoBars[i]->parentTo(AmmoBars[i + 1], true, { 0, -20 });
    }

    for (int i = 0; i < Weapons.size(); i++) {
        WeaponNameTexts.push_back(new PlacedText("weapName" + i + 1, UI::R, UI::L, FontString(Weapons[i]->Name, 36)));
        WeaponNameTexts[i]->setFillColor(sf::Color(25, 192, 25, 160));
        WeaponNameTexts[i]->setOutlineColor(sf::Color::Black);
        WeaponNameTexts[i]->parentTo(AmmoBars[i], true, { 35, 0 });
    }

    ReloadWeaponText.setFillColor(sf::Color(255, 20, 20));
    ReloadWeaponText.setCharacterSize(100);

    XButtonSprite.setTexture(Textures::XButton, UI::texture);
    XButtonSprite.setScale(2.f, 2.f);
    XButtonSprite.parentTo(&HUDFrame, true, { 0, -20 });

    InfoLogoSprite.setTexture(Textures::InfoLogo, UI::texture);
    InfoLogoSprite.setScale(2.f, 2.f);
    InfoLogoSprite.parentTo(&XButtonSprite, true, { 0, -10 });

    for (int i = 0; i < Effects::EffectCount; i++) {
        effectIconsTimers[i] = new PlacedText();
        effectIcons[i] = new Frame("effect_" + i, UI::none, UI::none, (sf::Vector2f)Textures::Eff_HPRegen.getSize() / 2.f);
    }
    effectIcons[2]->setTexture(Textures::Eff_HPRegen, UI::element);
    effectIcons[3]->setTexture(Textures::Eff_Burn, UI::element);

    coinSprite.setScale({ 0.5, 0.5 });
    coinSprite.setPosition({ playerCoinAmount.getRight() + 20, playerCoinAmount.getTop() - 50 });
    coinSprite.setAnimation(*itemTexture[ItemID::coin], itemTextureFrameAmount[ItemID::coin],
                            1, itemTextureDuration[ItemID::coin]);
    coinSprite.play();
    coinSprite.parentTo(&HUDFrame, true, { -20, -10 });

    playerCoinAmount.setFontString(FontString("", 50, sf::Color(200, 200, 200)));
    playerCoinAmount.parentTo(&coinSprite, true, { -10, 0 });
    playerCoinAmount.setOutlineColor(sf::Color::Black);
    playerCoinAmount.setOutlineThickness(2.f);

    FPSButton.setTexture(Textures::CheckButton, Textures::CheckButton, UI::texture);
    FPSButton.setHitboxPoints({ FPSButton.getLeftTop(), FPSButton.getRightTop(),
                                FPSButton.getRightBottom(), FPSButton.getLeftBottom() });
    FPSButton.setPosition(scw / 2.f - FPSButton.getSize().x / 2.f, sch / 2.f - FPSButton.getSize().y / 2.f);
    FPSButton.setWord(FontString("Show FPS", 40));
    FPSButton.sprite.setColor(showFPS ? sf::Color::Green : sf::Color::Red);
    FPSButton.ButtonText.setFillColor(showFPS ? sf::Color::Black : sf::Color::White);
    FPSButton.setFunction([] {
        showFPS ^= true;
        FPSButton.sprite.setColor(showFPS ? sf::Color::Green : sf::Color::Red);
        FPSButton.ButtonText.setFillColor(showFPS ? sf::Color::Black : sf::Color::White);
    });
}

void HUD::updateHUDUI(Player& player) {
    playerCoinAmount.setFontString(FontString(std::to_string(player.inventory.money), 50, sf::Color(200, 200, 200)));
    playerCoinAmount.parentTo(&coinSprite, true, { -10, 0 });
}

void HUD::loadDescriptions() {
    std::ifstream descFile("sources/texts/descriptions.json");
    try {
        nlohmann::json j = nlohmann::json::parse(descFile);
        interactibleDescriptions[DescriptionID::portal]        = j["portal"]        .template get<std::string>();
        interactibleDescriptions[DescriptionID::box]           = j["box"]           .template get<std::string>();
        interactibleDescriptions[DescriptionID::shopSector]    = j["shopSector"]    .template get<std::string>();
        interactibleDescriptions[DescriptionID::upgradeSector] = j["upgradeSector"] .template get<std::string>();
        interactibleDescriptions[DescriptionID::artifact]      = j["artifact"]      .template get<std::string>();
        interactibleDescriptions[DescriptionID::fire]          = j["fire"]          .template get<std::string>();
    } catch(const std::exception& e) {
        for (DescriptionID::Type i = DescriptionID::portal; i < DescriptionID::DescriptionCount; i++) {
            interactibleDescriptions[i] = "Error loading description";
        }
    }
    descFile.close();
}