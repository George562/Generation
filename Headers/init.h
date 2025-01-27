#pragma once
#include "Utility/tools.h"
#include "Abstracts/scale.h"
#include "CollisionShapes/collisionRect.h"
#include "CollisionShapes/collisionCircle.h"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <vector>
#include <map>
#include <set>
#include <ctime>

namespace faction {
    using Type = sf::Uint8;
    enum : Type {
        Player,
        Enemy
    };

    std::map<Type, std::set<Type>> Friendship{
        {Player, {Player}},
        {Enemy, {Enemy}}
    };

    bool friends(Type& left, Type& right) {
        return Friendship[left].count(right);
    }
};

namespace packetStates {
    sf::Uint8 curState;
    enum : sf::Uint8 {
        Disconnect,       // client send to host
        PlayersAmount,    // to connecting client
        PlayerConnect,    // host sent to clients when someone connected
        PlayerDisconnect, // host sent to clients when someone disconnected
        Labyrinth,        // for LabirintData
        PlayerPos,        // send from client to host than host sending others clients
        SetPos,           // for set pos of player, because PlayerPos just say positions of others players
        ChatEvent,        // Any new message from chat
        Shooting,         // Someone shoot
        FirstConnect,     // first connect of new players
        UseInteractable,  // When client use some interactable item
        EnemiesPos,       // send Enemies position
        EnemyDie,         // send when Enemy die
        AllPlayerDie      // when all players die
    };
};

namespace inventoryPage {
    using Type = sf::Uint8;
    enum : Type {
        Items,
        Weapons,
        Stats,
        PageCount,
    };
}

float scw = sf::VideoMode::getDesktopMode().width; // screen width
float sch = sf::VideoMode::getDesktopMode().height; // screen height

std::string errStr = "Error. This shouldn't happen...\nSeems like someone's shit at programming!";

float scwScale = scw / 1920.f;
float schScale = sch / 1080.f;

constexpr int size = 480, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
constexpr float ScaleParam = float(miniSize) / float(size);
int START_N = 9, START_M = 6;
float WallMinSize = size / 8, WallMaxSize = size;

constexpr float oneOverSixty = 1.f / 60.f;

int completedLevels = 0;
int curLevel = 1;

bool playerCanDash;
bool playerMakingADash = false;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View HUDView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});

sf::View InventoryView({0, 0, (float)scw , (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View ShopStockView;
sf::View ShopPlayerInvView;

sf::Clock* GameClock;
sf::Time GameTime, TimeSinceLastFrame;

namespace Textures {
//////////////////////////////////////////////////////////// Textures
    sf::Texture Distorted,
                DistortedScientist,
                Enemy,
                Boss,
                Player,
                Portal,
                WallG,
                WallV,
                XButton,
                InfoLogo,
                Box,
                floor,
                floorFork,
                Fire,
                Architect,
                Noise,
//////////////////////////////////////////////////////////// Effect Textures
                Eff_HPRegen,
                Eff_Burn,
//////////////////////////////////////////////////////////// Panel Tetures
                GreenPanel,
                GreenPanelPushed,
                RedPanel,
                RedPanelPushed,
                BluePanel,
                BluePanelPushed,
                YellowPanel,
                YellowPanelPushed,
                GradientFrame,
                GradientFrameAlpha,
                ItemPanel,
                Frame4,
//////////////////////////////////////////////////////////// Button Tetures
                BlueButton,
                BlueButtonPushed,
                CyanButton,
                CyanButtonPushed,
                GreenButton,
                GreenButtonPushed,
                GreyButton,
                GreyButtonPushed,
                MagentaButton,
                MagentaButtonPushed,
                RedButton,
                RedButtonPushed,
                YellowButton,
                YellowButtonPushed,
                CheckButton,

                SettingButton,
                SettingButtonPushed,
                EscapeButton,
                EscapeButtonPushed,
                EncyclopediaButton,
                EncyclopediaButtonPushed,

                NPCDialogueFrame_Square,
                NPCDialogueFrame_Wide,

                GridBG,

                INVISIBLE,
                PH_gun,
                PH_CochGen,
                PH_FormFactor,
                PH_Converter,
                PH_Targeting,
                PH_Pistol,
                PH_Shotgun,
                PH_Rifle
                ;
}

void loadTextures() {
    Textures::Distorted                .loadFromFile("sources/textures/Distorted.png");
    Textures::DistortedScientist       .loadFromFile("sources/textures/DistortedScientist.png");
    Textures::Enemy                    .create(size / 4.f, size / 4.f);
    Textures::Boss                     .create(size, size);
    Textures::Player                   .loadFromFile("sources/textures/Player.png");
    Textures::Portal                   .create(size, size);
    Textures::WallG                    .loadFromFile("sources/textures/wallG.png");
    Textures::WallV                    .loadFromFile("sources/textures/wallV.png");
    Textures::XButton                  .loadFromFile("sources/textures/XButton.png");
    Textures::InfoLogo                 .loadFromFile("sources/textures/infoLogo.png");
    Textures::Box                      .loadFromFile("sources/textures/Box.png");
    Textures::floor                    .loadFromFile("sources/textures/floor.png");
    Textures::floorFork                .loadFromFile("sources/textures/fork.png");
    Textures::Fire                     .create(140.f, 175.f);
    Textures::Architect                .loadFromFile("sources/textures/Architect.png");
    Textures::Noise                    .loadFromFile("sources/textures/noise.png");

    Textures::Eff_HPRegen              .loadFromFile("sources/textures/effects/HPRegenEffect.png");
    Textures::Eff_Burn                 .loadFromFile("sources/textures/effects/BurnEffect.png");

    Textures::GreenPanel               .loadFromFile("sources/textures/Panels/GreenPanel.png");
    Textures::GreenPanelPushed         .loadFromFile("sources/textures/Panels/GreenPanelPushed.png");
    Textures::RedPanel                 .loadFromFile("sources/textures/Panels/RedPanel.png");
    Textures::RedPanelPushed           .loadFromFile("sources/textures/Panels/RedPanelPushed.png");
    Textures::BluePanel                .loadFromFile("sources/textures/Panels/BluePanel.png");
    Textures::BluePanelPushed          .loadFromFile("sources/textures/Panels/BluePanelPushed.png");
    Textures::YellowPanel              .loadFromFile("sources/textures/Panels/YellowPanel.png");
    Textures::YellowPanelPushed        .loadFromFile("sources/textures/Panels/YellowPanelPushed.png");
    Textures::GradientFrame            .loadFromFile("sources/textures/Panels/GradientFrame.png");
    Textures::GradientFrameAlpha       .loadFromFile("sources/textures/Panels/GradientFrameAlpha.png");
    Textures::ItemPanel                .loadFromFile("sources/textures/Panels/ItemPanel.png");
    Textures::Frame4                   .loadFromFile("sources/textures/Panels/frame4.png");

    Textures::BlueButton               .loadFromFile("sources/textures/Buttons/BlueButton.png");
    Textures::BlueButtonPushed         .loadFromFile("sources/textures/Buttons/BlueButtonPushed.png");
    Textures::CyanButton               .loadFromFile("sources/textures/Buttons/CyanButton.png");
    Textures::CyanButtonPushed         .loadFromFile("sources/textures/Buttons/CyanButtonPushed.png");
    Textures::GreenButton              .loadFromFile("sources/textures/Buttons/GreenButton.png");
    Textures::GreenButtonPushed        .loadFromFile("sources/textures/Buttons/GreenButtonPushed.png");
    Textures::GreyButton               .loadFromFile("sources/textures/Buttons/GreyButton.png");
    Textures::GreyButtonPushed         .loadFromFile("sources/textures/Buttons/GreyButtonPushed.png");
    Textures::MagentaButton            .loadFromFile("sources/textures/Buttons/MagentaButton.png");
    Textures::MagentaButtonPushed      .loadFromFile("sources/textures/Buttons/MagentaButtonPushed.png");
    Textures::RedButton                .loadFromFile("sources/textures/Buttons/RedButton.png");
    Textures::RedButtonPushed          .loadFromFile("sources/textures/Buttons/RedButtonPushed.png");
    Textures::YellowButton             .loadFromFile("sources/textures/Buttons/YellowButton.png");
    Textures::YellowButtonPushed       .loadFromFile("sources/textures/Buttons/YellowButtonPushed.png");
    Textures::CheckButton              .loadFromFile("sources/textures/Buttons/CheckButton.png");

    Textures::SettingButton            .loadFromFile("sources/textures/Buttons/SettingButton.png");
    Textures::SettingButtonPushed      .loadFromFile("sources/textures/Buttons/SettingButtonPushed.png");
    Textures::EscapeButton             .loadFromFile("sources/textures/Buttons/EscapeButton.png");
    Textures::EscapeButtonPushed       .loadFromFile("sources/textures/Buttons/EscapeButtonPushed.png");
    Textures::EncyclopediaButton       .loadFromFile("sources/textures/Buttons/EncyclopediaButton.png");
    Textures::EncyclopediaButtonPushed .loadFromFile("sources/textures/Buttons/EncyclopediaButtonPushed.png");

    Textures::GridBG                   .loadFromFile("sources/textures/Panels/GridBackground.png");

    Textures::INVISIBLE                .create(size, size);

    Textures::PH_CochGen               .loadFromFile("sources/textures/Placeholders/CochGen.png");
    Textures::PH_FormFactor            .loadFromFile("sources/textures/Placeholders/FormFactor.png");
    Textures::PH_Converter             .loadFromFile("sources/textures/Placeholders/Converter.png");
    Textures::PH_Targeting             .loadFromFile("sources/textures/Placeholders/Targeting.png");
    Textures::PH_Pistol                .loadFromFile("sources/textures/Placeholders/Pistol.png");
}

namespace Shaders {
//////////////////////////////////////////////////////////// Shaders
    sf::Shader Flashlight,
               Player,
               Portal,
               Portal2,
               PickupItem,
               Architect,
               FloatingUp,  // text moving up
               Distortion1, // linear left move
               Distortion2, // circle move
               Outline,
               Bullet,
               Fire,
               WaveMix,
               Enemy,
               Boss,
               b_w
               ;
}

void loadShaders() {
    Shaders::Flashlight  .loadFromFile("sources/shaders/flashlight/flashlight.vert", "sources/shaders/flashlight/flashlight.frag" );
    Shaders::Player      .loadFromFile("sources/shaders/player/player.vert",         "sources/shaders/player/player.frag"         );
    Shaders::Portal      .loadFromFile("sources/shaders/portal/portal.vert",         "sources/shaders/portal/portal.frag"         );
    Shaders::Portal2     .loadFromFile("sources/shaders/portal2/portal2.vert",       "sources/shaders/portal2/portal2.frag"       );
    Shaders::PickupItem  .loadFromFile("sources/shaders/pickupItem/pickupItem.vert", "sources/shaders/pickupItem/pickupItem.frag" );
    Shaders::Architect   .loadFromFile("sources/shaders/Architect/Architect.vert",   "sources/shaders/Architect/Architect.frag"   );
    Shaders::FloatingUp  .loadFromFile("sources/shaders/FloatingUp/FloatingUp.vert", "sources/shaders/FloatingUp/FloatingUp.frag" );
    Shaders::Distortion1 .loadFromFile("sources/shaders/distorted1/distorted1.vert", "sources/shaders/distorted1/distorted1.frag" );
    Shaders::Distortion2 .loadFromFile("sources/shaders/distorted2/distorted2.vert", "sources/shaders/distorted2/distorted2.frag" );
    Shaders::Outline     .loadFromFile("sources/shaders/outline/outline.vert",       "sources/shaders/outline/outline.frag"       );
    Shaders::Bullet      .loadFromFile("sources/shaders/bullet/bullet.vert",         "sources/shaders/bullet/bullet.frag"         );
    Shaders::Fire        .loadFromFile("sources/shaders/fire/fire.vert",             "sources/shaders/fire/fire.frag"             );
    Shaders::WaveMix     .loadFromFile("sources/shaders/waveMix/waveMix.vert",       "sources/shaders/waveMix/waveMix.frag"       );
    Shaders::Enemy       .loadFromFile("sources/shaders/enemy/enemy.vert",           "sources/shaders/enemy/enemy.frag"           );
    Shaders::Boss        .loadFromFile("sources/shaders/boss/boss.vert",             "sources/shaders/boss/boss.frag"             );
    Shaders::b_w         .loadFromFile("sources/shaders/b&w/b&w.vert",               "sources/shaders/b&w/b&w.frag"               );
}

namespace Musics {
//////////////////////////////////////////////////////////// Musics
    sf::Music MainMenu,
              Fight1, Fight2
              ;
}

void loadMusics() {
    Musics::MainMenu .openFromFile("sources/music/MainMenu.wav");
    Musics::Fight1   .openFromFile("sources/music/Fight1.flac");
    Musics::Fight2   .openFromFile("sources/music/Fight2.flac");
}

namespace SoundBuffers {
//////////////////////////////////////////////////////////// SoundBuffers
    sf::SoundBuffer Shoot, Shoot2,
                    Engine
                    ;
}

void loadSoundBuffers() {
    SoundBuffers::Shoot  .loadFromFile("sources/music/Shoot.flac");
    SoundBuffers::Shoot2 .loadFromFile("sources/music/Shoot2.flac");
    SoundBuffers::Engine .loadFromFile("sources/music/Engine.flac");
}

namespace GameSettings {
//////////////////////////////////////////////////////////// GameSettings
    float soundVolume = 1.0f;
}