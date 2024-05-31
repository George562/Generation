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

namespace pacetStates {
    sf::Uint8 curState;
    enum : sf::Uint8 {
        disconnect,         // client send to host
        PlayersAmount,      // to connecting client
        PlayerConnect,      // host sent to clients when someone connected
        PlayerDisconnect,   // host sent to clients when someone disconnected
        Labyrinth,          // for LabirintData
        PlayerPos,          // send from client to host than host sending others clients
        SetPos,             // for set pos of player, because PlayerPos just say positions of others players
        ChatEvent,
        Shooting,
    };
};

namespace inventoryPage {
    using Type = sf::Uint8;
    enum : Type {
        Items,
        Arsenal,
        Equipables,
        Perks,
        Stats,
        NONE,
    };
}

constexpr int scw = 1920; // screen width
constexpr int sch = 1080; // screen height

constexpr int size = 480, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
constexpr float ScaleParam = float(miniSize) / float(size);
int START_N = 9, START_M = 6;
float WallMinSize = size / 8, WallMaxSize = size;

constexpr float oneOverSixty = 1.f / 60.f;

int completedLevels = 0;
int curLevel = 1;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});

sf::View InventoryView({0, 0, (float)scw, (float)sch});
sf::View ShopView({0, 0, (float)scw, (float)sch});
sf::View ShopStockView;
sf::View ShopPlayerInvView;

sf::Clock* GameClock;

namespace Textures {
//////////////////////////////////////////////////////////// Textures
    sf::Texture Distorted,
                DistortedScientist,
                Player,
                Portal,
                WallG,
                WallV,
                XButton,
                Box,
                floor,
                Puddle,
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
                SteelFrame,
                ItemPanel,
                Frame4,

                NPCDialogueFrame_Square,
                NPCDialogueFrame_Wide,

                ShopBG,
                ShopSectionFrame,
                ShopItemPhotoFrame,

                INVISIBLE,
                PH_gun
                ;
}

void loadTextures() {
    Textures::Distorted               .loadFromFile("sources/textures/Distorted.png");
    Textures::DistortedScientist      .loadFromFile("sources/textures/DistortedScientist.png");
    Textures::Player                  .loadFromFile("sources/textures/Player.png");
    Textures::Portal                  .loadFromFile("sources/textures/Portal.png");
    Textures::WallG                   .loadFromFile("sources/textures/wallG.png");
    Textures::WallV                   .loadFromFile("sources/textures/wallV.png");
    Textures::XButton                 .loadFromFile("sources/textures/XButton.png");
    Textures::Box                     .loadFromFile("sources/textures/Box.png");
    Textures::floor                   .loadFromFile("sources/textures/floor.png");
    Textures::Puddle                  .loadFromFile("sources/textures/Puddle.png");
    Textures::Fire                    .create(140.f, 175.f);
    Textures::Architect               .loadFromFile("sources/textures/Architect.png");
    Textures::Noise                   .loadFromFile("sources/textures/noise.png");

    Textures::Eff_HPRegen             .loadFromFile("sources/textures/effects/HPRegenEffect.png");
    Textures::Eff_Burn                .loadFromFile("sources/textures/effects/BurnEffect.png");

    Textures::GreenPanel              .loadFromFile("sources/textures/Panels/GreenPanel.png");
    Textures::GreenPanelPushed        .loadFromFile("sources/textures/Panels/GreenPanelPushed.png");
    Textures::RedPanel                .loadFromFile("sources/textures/Panels/RedPanel.png");
    Textures::RedPanelPushed          .loadFromFile("sources/textures/Panels/RedPanelPushed.png");
    Textures::BluePanel               .loadFromFile("sources/textures/Panels/BluePanel.png");
    Textures::BluePanelPushed         .loadFromFile("sources/textures/Panels/BluePanelPushed.png");
    Textures::YellowPanel             .loadFromFile("sources/textures/Panels/YellowPanel.png");
    Textures::YellowPanelPushed       .loadFromFile("sources/textures/Panels/YellowPanelPushed.png");
    Textures::SteelFrame              .loadFromFile("sources/textures/Panels/SteelFrame.png");
    Textures::ItemPanel               .loadFromFile("sources/textures/Panels/ItemPanel.png");
    Textures::Frame4                  .loadFromFile("sources/textures/Panels/frame4.png");

    Textures::NPCDialogueFrame_Square .loadFromFile("sources/textures/Panels/NPCDialogueFrame_Square.png");
    Textures::NPCDialogueFrame_Wide   .loadFromFile("sources/textures/Panels/NPCDialogueFrame_Wide.png");

    Textures::ShopBG                  .loadFromFile("sources/textures/Panels/ShopBG.png");
    Textures::ShopSectionFrame        .loadFromFile("sources/textures/Panels/ShopSectionFrame.png");
    Textures::ShopItemPhotoFrame      .loadFromFile("sources/textures/Panels/ShopItemPhotoFrame.png");

    Textures::INVISIBLE               .create(size, size);

    Textures::PH_gun                  .loadFromFile("sources/textures/Placeholders/kitchengun.png");
}

namespace Shaders {
//////////////////////////////////////////////////////////// Shaders
    sf::Shader Flashlight,
               Player,
               Portal,
               PickupItem,
               Architect,
               FloatingUp,  // text moving up
               Distortion1, // linear left move
               Distortion2, // circle move
               Outline,
               Bullet,
               Fire,
               WaveMix
               ;
}

void loadShaders() {
    Shaders::Flashlight  .loadFromFile("sources/shaders/flashlight/flashlight.vert", "sources/shaders/flashlight/flashlight.frag" );
    Shaders::Player      .loadFromFile("sources/shaders/player/player.vert",         "sources/shaders/player/player.frag"         );
    Shaders::Portal      .loadFromFile("sources/shaders/portal/portal.vert",         "sources/shaders/portal/portal.frag"         );
    Shaders::PickupItem  .loadFromFile("sources/shaders/pickupItem/pickupItem.vert", "sources/shaders/pickupItem/pickupItem.frag" );
    Shaders::Architect   .loadFromFile("sources/shaders/Architect/Architect.vert",   "sources/shaders/Architect/Architect.frag"   );
    Shaders::FloatingUp  .loadFromFile("sources/shaders/FloatingUp/FloatingUp.vert", "sources/shaders/FloatingUp/FloatingUp.frag" );
    Shaders::Distortion1 .loadFromFile("sources/shaders/distorted1/distorted1.vert", "sources/shaders/distorted1/distorted1.frag" );
    Shaders::Distortion2 .loadFromFile("sources/shaders/distorted2/distorted2.vert", "sources/shaders/distorted2/distorted2.frag" );
    Shaders::Outline     .loadFromFile("sources/shaders/outline/outline.vert",       "sources/shaders/outline/outline.frag"       );
    Shaders::Bullet      .loadFromFile("sources/shaders/bullet/bullet.vert",         "sources/shaders/bullet/bullet.frag"         );
    Shaders::Fire        .loadFromFile("sources/shaders/fire/fire.vert",             "sources/shaders/fire/fire.frag"             );
    Shaders::WaveMix     .loadFromFile("sources/shaders/waveMix/waveMix.vert",       "sources/shaders/waveMix/waveMix.frag"       );
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

float random(sf::Vector2f v) {
    return std::modf( ( 1.f + std::sin( v.x * 12.9898f + v.y * 78.233f ) ) * 43758.5453123f, nullptr);
}
float random(float x, float y) {
    return std::modf( ( 1.f + std::sin( x * 12.9898f + y * 78.233f ) ) * 43758.5453123f, nullptr);
}