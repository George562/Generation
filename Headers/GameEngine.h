#pragma once
#include "Entities/Enemy.h"
#include "Entities/Player.h"
#include "Multiplayer/Chat.h"
#include "UI/PolygonButton.h"
#include "UI/Panel.h"
#include "UI/Bar.h"
#include "UI/tempText.h"
#include "UI/Slot.h"
#include "UI/Frame.h"
#include "Systems/Effect.h"
#include "Systems/Shop.h"
#include "UIInits/initInventory.h"
#include "UIInits/initMenuShop.h"
#include "UIInits/initHUD.h"
#include "UIDraws/drawInventory.h"
#include "UIDraws/drawMenuShop.h"
#include "UIDraws/drawHUD.h"
#include <regex>


//////////////////////////////////////////////////////////// Settings of the game
bool IsDrawMinimap = true;
bool MiniMapHoldOnPlayer = true;


//////////////////////////////////////////////////////////// Stuff for work with system and screen
sf::ContextSettings settings;


sf::RenderWindow window(sf::VideoMode(scw, sch), "multigame", sf::Style::Fullscreen, settings);
sf::RenderTexture preRenderTexture, outlineRenderTexture;
sf::Sprite preRenderSprite, outlineRenderSprite;
float MiniMapZoom = 1.f;
bool MiniMapActivated;
std::vector<sf::Drawable*> DrawableStuff; // references to objects that exist somewhere
std::vector<Interactable*> InteractableStuff; // references to objects that exist somewhere
Interactable* CurInteractable;

std::vector<Item*> PickupStuff;


//////////////////////////////////////////////////////////// Players
Player player;
std::vector<Player> ConnectedPlayers;

//////////////////////////////////////////////////////////// Weapons
Pistol pistol;
Shotgun shotgun;
Rifle rifle;
std::vector<Weapon*> Weapons = {
    &pistol,
    &shotgun,
    &rifle,
};
Scale<int> CurWeapon;

//////////////////////////////////////////////////////////// DrawableStuff
sf::Sprite WallRect;
std::vector<TempText*> TempTextsOnGround, DamageText;
Bar<float> EnemyHealthBar;
sf::Sprite FLoorTileSprite;
sf::Sprite FloorForkSprite;
sf::Sprite undergroundBG;
sf::Image icon;

//////////////////////////////////////////////////////////// Online tools
sf::TcpListener listener;
sf::Packet ReceivePacket, SendPacket;
std::vector<sf::TcpSocket*> clients;
sf::SocketSelector selector;
std::string ClientState, IPOfHost, MyIP, sPacketData;
sf::TcpSocket MySocket; // this computer socket
sf::Int32 ComputerID, i32PacketData;
sf::Vector2f V2fPacketData;
sf::Mutex mutexOnSend, mutexOnDataChange;
bool ClientFuncRun, HostFuncRun;
bool Connecting = false;
std::regex regexOfIP("\\d+.\\d+.\\d+.\\d+");


//////////////////////////////////////////////////////////// Interactables
Interactable portal(DescriptionID::portal),
             shopSector(DescriptionID::shopSector),
             upgradeSector(DescriptionID::upgradeSector),
             portal2(DescriptionID::portal);
std::vector<Interactable*> listOfBox,
                           listOfArtifact,
                           listOfFire;


//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle, MMEnemyCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect, MMPuddleRect, MMArtifact;


//////////////////////////////////////////////////////////// Locations
Location* CurLocation = nullptr;
Location LabyrinthLocation, MainMenuLocation;

void loadLocations() {
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");
}


//////////////////////////////////////////////////////////// Other stuff
Chat chat(scw, sch);
sf::Vector2i MouseBuffer;
std::vector<Enemy*> Enemies;


//////////////////////////////////////////////////////////// functions

//---------------------------- INITS
void init();
void initMinimap();
void initScripts();
//----------------------------


//---------------------------- DRAW FUNCTIONS
void draw();
void drawFloor();
void drawWalls();
void drawMiniMap();
//----------------------------


//---------------------------- UI UPDATERS/CREATORS
void updateHUDUI();
void updateInventoryUI();
void updateShopUI();
void updateUpgradeInterfaceUI();
//----------------------------


//---------------------------- EVENT HANDLERS
void EventHandler();
void inventoryHandler(sf::Event&);
void shopHandler(sf::Event&);
void upgradeInterfaceHandler(sf::Event&);
//----------------------------


//---------------------------- LEVEL GENERATION FUNCTIONS
void LevelGenerate(int, int);
void LoadMainMenu();

void setInteractable(Interactable*&);
void setBox(Interactable*&);
void setArtifact(Interactable*&);
void setFire(Interactable*&);
//----------------------------


//---------------------------- GAME STATE FUNCTIONS
void updateBullets();
void EnemyDie(int i);
void updateEnemies();
void updateUpgradeShopStats();
void openUpgradeShop();

bool useItem(Item*&);

void processEffects();
void updateEffects(Creature*);
void applyEffect(Creature&, Effect*);
void clearEffect(Creature&, Effect*);
//----------------------------


//---------------------------- GAME GRAPHICS FUNCTIONS
void updateShaders();
//----------------------------


//---------------------------- HELPER FUNCTIONS
bool CanSomethingBeActivated();
template <class T> void updateCostsText(PlacedText*, Upgradable<T>*, int);
template <class T> void updateCountText(PlacedText* counterText, Upgradable<T>* stat);
template <class T> void updateStatsText(PlacedText* statText, Upgradable<T>* stat);
template <class T> void upgradeStat(int, Upgradable<T>*,
                                    PlacedText* costText = nullptr,
                                    PlacedText* = nullptr,
                                    PlacedText* = nullptr);
void setUpgradeFunctions();
void updateUpgradeTexts();
void addMessageText(std::string s, sf::Color fillColor, sf::Color outlineColor = sf::Color::White);

void saveGame();
void loadSave();
//----------------------------


void MainLoop(); // SELF-EXPLANATORY


//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect();
void ClientDisconnect(int);
void SelfDisconnect();
void SendToClients(sf::Packet&, int = -1);
void funcOfHost();
void funcOfClient();
void sendSendPacket();


//////////////////////////////////////////////////////////// Threads
sf::Thread HostTread(funcOfHost);
sf::Thread ClientTread(funcOfClient);


//////////////////////////////////////////////////////////// functions realizations

//============================================================================================== INITS
void init() {
    setlocale(LC_ALL, "rus");

    window.setVerticalSyncEnabled(true);
    settings.antialiasingLevel = 8;
    window.setView(GameView);

    preRenderTexture.create(scw, sch);
    preRenderSprite.setTexture(preRenderTexture.getTexture());

    outlineRenderTexture.create(scw, sch);
    outlineRenderSprite.setTexture(outlineRenderTexture.getTexture());

    MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);
    GameClock = new sf::Clock;
    GameTime = GameClock->getElapsedTime();
    TimeSinceLastFrame = sf::Time::Zero;

    loadLocations();
    loadTextures();
    loadItemTextures();
    loadFonts();
    loadShaders();
    loadMusics();
    loadSoundBuffers();
    loadSave();
    loadDescriptions();

    icon.loadFromFile("sources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    playerCanDash = player.inventory.find(ItemID::dasher) != -1;

    Musics::MainMenu.setLoop(true);
    Musics::MainMenu.setVolume(0);
    Musics::Fight1.setVolume(0);
    Musics::Fight2.setVolume(0);

    for (Weapon*& w: Weapons) w->ShootSound.setRelativeToListener(true);

    portal.setAnimation(Textures::Portal, &Shaders::Portal2);
    portal.animation->setSpriteColor(sf::Color(0, 0, 255, 255));
    portal.setSize(140.f, 270.f);

    portal2.setAnimation(Textures::Portal, &Shaders::Portal2);
    portal2.animation->setSpriteColor(sf::Color(255, 128, 0, 255));
    portal2.setSize(140.f, 250.f);

    player.setAnimation(Textures::Player, &Shaders::Player);

    shopSector.setAnimation(Textures::INVISIBLE);
    shopSector.setPosition(0, 2 * size);

    upgradeSector.setAnimation(Textures::INVISIBLE);
    upgradeSector.setPosition(6 * size, 2 * size);

    Shaders::Flashlight.setUniform("uResolution", sf::Vector2f(scw, sch));
    Shaders::Flashlight.setUniform("u_playerRadius", player.hitbox.getRadius());

    Shaders::Distortion1.setUniform("noise_png", Textures::Noise);

    Shaders::Distortion2.setUniform("noise_png", Textures::Noise);

    Shaders::Portal2.setUniform("noise_png", Textures::Noise);

    Shaders::Outline.setUniform("uResolution", sf::Vector2f(scw, sch));

    Shaders::Bullet.setUniform("noise_png", Textures::Noise);

    Shaders::Fire.setUniform("noise_png", Textures::Noise);

    CurWeapon.looped = true;

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getPublicAddress().toString();
    std::cout << "LocalAddress: " << MySocket.getRemoteAddress().getLocalAddress().toString() << "\n";
    std::cout << "PublicAddress: " << MyIP << '\n';

    CurWeapon = { {0, (int)Weapons.size() - 1, 0} };
    player.ChangeWeapon(Weapons[CurWeapon.cur]);

    EnemyHealthBar.setColors(CommonColors::barWall, sf::Color(192, 0, 0, 160), CommonColors::barBG);
    EnemyHealthBar.setSize(125.f, 15.f);
    EnemyHealthBar.setWallWidth(1);
    EnemyHealthBar.ShowText = false;

    FLoorTileSprite.setScale(5.f, 5.f);
    FLoorTileSprite.setTexture(Textures::floor);

    FloorForkSprite.setTexture(Textures::floorFork);
    FloorForkSprite.setScale(15.f / 61.f, 15.f / 61.f); // det scale to make it 240x240 px

    undergroundBG.setTexture(Textures::Noise);
    undergroundBG.setPosition(0, 0);
    undergroundBG.setScale(scw / undergroundBG.getLocalBounds().width, sch / undergroundBG.getLocalBounds().height);

    initHUD(&player, &Weapons);
    initInventory(&player);
    initUpgradeShop();
    initShop(&player);
    initMinimap();
    initScripts();

    chat.addLine("/? - info");

    LoadMainMenu();
}

void initMinimap() {
    MMPlayerCircle.setRadius(9);
    MMPlayerCircle.setFillColor(sf::Color(0, 180, 0));
    MMPlayerCircle.setOrigin(MMPlayerCircle.getRadius(), MMPlayerCircle.getRadius());

    MMEnemyCircle.setRadius(9);
    MMEnemyCircle.setFillColor(sf::Color(180, 0, 0));
    MMEnemyCircle.setOrigin(MMEnemyCircle.getRadius(), MMEnemyCircle.getRadius());

    MMPortalRect.setSize(portal.hitbox.getSize() * ScaleParam);
    MMPortalRect.setFillColor(sf::Color(200, 0, 200, 200));

    MMBoxRect.setSize(sf::Vector2f(105.f, 117.f) * ScaleParam);
    MMBoxRect.setFillColor(sf::Color(252, 108, 24, 200));

    MMArtifact.setSize(sf::Vector2f(150.f, 105.f) * ScaleParam);
    MMArtifact.setFillColor(sf::Color::White);
}

void initScripts() {
    upgradeInterface::switchGunLBtn.setFunction([](){
        CurWeapon.cur = (CurWeapon.cur - 1 + Weapons.size()) % Weapons.size();
        player.ChangeWeapon(Weapons[CurWeapon.cur]);
    });
    upgradeInterface::switchGunRBtn.setFunction([](){
        CurWeapon.cur = (CurWeapon.cur + 1) % Weapons.size();
        player.ChangeWeapon(Weapons[CurWeapon.cur]);
    });

    {
        using namespace MenuShop;
        shop.setFunction([]() {
            if (selectedItem != nullptr) {
                if (player.inventory.money >= shop.itemPrices[selectedItem->id]) {
                    Item* boughtItem = new Item(*selectedItem);
                    boughtItem->amount = 1;
                    player.addItem(boughtItem);
                    player.inventory.money -= shop.itemPrices[selectedItem->id];
                    selectedItem->amount--;

                    playerCoinsText.setString(std::to_string(player.inventory.money));
                    playerCoinsSprite.moveToAnchor(&MenuShop::playerCoinsText, { 25, -10 });
                    shop.soldItems.removeItem(selectedItem, false);
                    NPCText.setString("Thank you for buying a " + stringLower(itemName[selectedItem->id]) + "!");

                    if (!shop.soldItems.find(selectedItem)) {
                        slotsElements[selectedItem->id].erase();
                        itemSprite.setTexture(Textures::INVISIBLE);
                        selectedItem = nullptr;
                        removeUI(&itemSlot, UIElements);
                    }
                } else {
                    NPCText.setString("Sorry, but you cannot afford a " + stringLower(itemName[selectedItem->id]) + ".");
                }
            }
        });
    }

    HUD::EscapeButton.setFunction([]() {
        if (ClientFuncRun)    chat.UseCommand("/disconnect");
        else if (HostFuncRun) chat.UseCommand("/server off");
        if (CurLocation == &MainMenuLocation) {
            Musics::MainMenu.pause();
            window.close();
        } else {
            LoadMainMenu();
            saveGame();
        }
    });

    HUD::SettingsButton.setFunction([]() {
        saveGame();
        addMessageText("You don't have to press this button. It's not done yet.", sf::Color(170, 50, 50), sf::Color(150, 150, 150));
    });

    HUD::EncyclopediaButton.setFunction([]() {
        saveGame();
        addMessageText("You don't have to press this button. It's not done yet.", sf::Color(170, 50, 50), sf::Color(150, 150, 150));
    });

    shopSector.setFunction([](Interactable* i) {
        {
            using namespace MenuShop;
            isDrawShop = true;
            NPCText.setString(textWrap("Hello! Welcome to our shop! "
                                       "Do not mind the eldritch-geometric fuckery outside :)", 94));
            NPCText.moveToAnchor(&NPCName, { 100, 0 });
            addUI(&BG, UIElements);
            if (!selectedItem)
                removeUI(&itemSlot, UIElements);
        }
    });

    upgradeSector.setFunction([](Interactable* i) {
        upgradeInterface::isDrawUpgradeInterface = true;
        openUpgradeShop();
    });

    portal.setFunction([](Interactable* i) {
        if (ClientFuncRun) {
            addMessageText("Host must start next level", sf::Color::Black);
        } else {
            clearVectorOfPointers(PickupStuff);
            clearVectorOfPointers(Bullets);

            DrawableStuff.clear();
            HUD::InterfaceStuff.clear();
            InteractableStuff.clear();

            player.hitbox.setCenter((START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size);

            MiniMapActivated = false;
            HUD::EscapeMenuActivated = false;

            MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
            MiniMapView.setCenter(player.getCenter() * ScaleParam);

            Musics::MainMenu.pause();
            if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
                Musics::Fight1.play();
            }
            if (CurLocation != &LabyrinthLocation) {
                CurLocation = &LabyrinthLocation;
            }
            LevelGenerate(START_N + curLevel, START_M + curLevel * 2);
            // FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);

            DrawableStuff.push_back(&player);
            DrawableStuff.push_back(&portal);

            addUI(&HUD::HUDFrame, HUD::InterfaceStuff);
            for (int i = 0; i < HUD::WeaponNameTexts.size(); i++) {
                HUD::InterfaceStuff.push_back(HUD::WeaponNameTexts[i]);
            }
            HUD::InterfaceStuff.push_back(&chat);

            saveGame();

            if (HostFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::Labyrinth << CurLocation;
                SendPacket << (sf::Int32)Enemies.size() << Enemies;
                SendPacket << (sf::Int32)InteractableStuff.size() << InteractableStuff;
                SendPacket << &portal << player;
                sendSendPacket();
                mutexOnSend.unlock();
                for (Player& p : ConnectedPlayers) {
                    DrawableStuff.push_back(&p);
                }
            }
        }
    });

    portal2.setFunction([](Interactable* i) {
        addMessageText("We're not sure where it ends.\n    You shouldn't go there.", sf::Color(200, 0, 200));
    });

    chat.SetCommand("/?", []{
        chat.addLine("/? - info");
        chat.addLine("/server on - start server");
        chat.addLine("/server off - close server");
        chat.addLine("/connect - connect to other");
        chat.addLine("/disconnect - disconnect from server");
    });
    chat.SetCommand("/server on", []{
        if (!HostFuncRun && !ClientFuncRun) {
            if (CurLocation == &MainMenuLocation) {
                listener.listen(53000);
                selector.add(listener);
                HUD::ListOfPlayers.setString(MyIP);
                ComputerID = 0;
                HostFuncRun = true;
                HostTread.launch();
                chat.addLine("Server is running! Your IP: " + MyIP);
            } else {
                chat.addLine("You must run serer at start location");
            }
        } else {
            chat.addLine("Server is already running! Your IP: " + MyIP);
        }
    });
    chat.SetCommand("/server off", []{
        if (HostFuncRun) {
            mutexOnSend.lock();
            SendPacket << packetStates::Disconnect;
            sendSendPacket();
            SendPacket.clear();
            mutexOnSend.unlock();
            clients.clear();
            selector.clear();
            listener.close();
            HostFuncRun = false;
            for (Player& p : ConnectedPlayers) {
                DeleteFromVector(DrawableStuff, (sf::Drawable*)&p);
            }
            ConnectedPlayers.clear();
            chat.addLine("Server is closed!");
        } else {
            chat.addLine("Server is not running!");
        }
    });
    chat.SetCommand("/connect", []{
        if (HostFuncRun) {
            chat.addLine("Cannot connect to server while running server!");
        } else if (ClientFuncRun) {
            chat.addLine("input IP of host");
        } else {
            if (CurLocation == &MainMenuLocation) {
                Connecting = true;
                chat.addLine("input IP of host");
            } else {
                chat.addLine("You must connect at start location");
            }
        }
    });
    chat.SetCommand("/disconnect", []{
        if (ClientFuncRun) {
            mutexOnSend.lock();
            SendPacket << packetStates::Disconnect;
            sendSendPacket();
            mutexOnSend.unlock();
            SelfDisconnect();
        }
    });
}
//==============================================================================================



//============================================================================================== DRAW FUNCTIONS
void draw() {
    mutexOnDataChange.lock();
    window.clear(sf::Color::Transparent);
    updateShaders();
    if (inventoryInterface::isDrawInventory) {
        drawInventory(window, &player);
    } else if (MenuShop::isDrawShop) {
        drawShop(window, &player);
    } else if (upgradeInterface::isDrawUpgradeInterface) {
        drawUpgradeInterface(window);
    } else {
        preRenderTexture.clear(sf::Color::Transparent);
        preRenderTexture.setView(HUDView);
        preRenderTexture.draw(undergroundBG, &Shaders::Distortion1);

        preRenderTexture.setView(GameView);
        outlineRenderTexture.setView(GameView);
        outlineRenderSprite.setPosition(GameView.getCenter() - GameView.getSize() / 2.f);

        drawFloor();
        drawWalls();

        for (sf::Drawable*& d : DrawableStuff) {
            if (d == CurInteractable) {
                outlineRenderTexture.clear(sf::Color::Transparent);
                outlineRenderTexture.draw(*d); outlineRenderTexture.display();
                preRenderTexture.draw(outlineRenderSprite, &Shaders::Outline);
            } else {
                preRenderTexture.draw(*d);
            }
        }

        for (Enemy*& enemy : Enemies) {
            EnemyHealthBar.setPosition(enemy->getCenter() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, enemy->hitbox.getRadius() + 50.f));
            EnemyHealthBar.setValue(enemy->Health);
            preRenderTexture.draw(EnemyHealthBar);
        }

        for (Player& p : ConnectedPlayers) {
            EnemyHealthBar.setPosition(p.getCenter() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, p.hitbox.getRadius() + 50.f));
            EnemyHealthBar.setValue(p.Health);
            preRenderTexture.draw(EnemyHealthBar);
        }

        preRenderTexture.display();

        window.setView(HUDView);
        window.draw(preRenderSprite, &Shaders::Flashlight);

        preRenderTexture.clear(sf::Color::Transparent);
        for (int i = 0; i < Bullets.size(); i++) {
            preRenderTexture.draw(*Bullets[i]);
        }
        preRenderTexture.display();
        window.draw(preRenderSprite, &Shaders::Bullet);

        window.setView(GameView);

        for (size_t i = 0; i < TempTextsOnGround.size(); i++) {
            if (GameTime < TempTextsOnGround[i]->howLongToExist) {
                window.draw(*TempTextsOnGround[i]);
            } else {
                DeletePointerFromVector(TempTextsOnGround, i--);
            }
        }

        for (size_t i = 0; i < DamageText.size(); i++) {
            if (GameTime < DamageText[i]->howLongToExist) {
                Shaders::FloatingUp.setUniform("uTime", DamageText[i]->localClock->getElapsedTime().asSeconds());
                window.draw(*DamageText[i], &Shaders::FloatingUp);
            } else {
                DeletePointerFromVector(DamageText, i--);
            }
        }

        if (player.CurWeapon != nullptr && player.CurWeapon->ManaStorage.toBottom() < player.CurWeapon->ManaCostOfBullet) {
            window.setView(HUDView);
            window.draw(HUD::ReloadWeaponText);
            window.setView(GameView);
        }

        if (IsDrawMinimap) {
            drawMiniMap();
        }
        if (HUD::IsDrawHUD) {
            drawHUD(window, &player, &Weapons);
        }
    }
    window.display();
    mutexOnDataChange.unlock();
}

void drawFloor() {
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                FLoorTileSprite.setPosition(size * j, size * i);
                preRenderTexture.draw(FLoorTileSprite);
                if (CurLocation->getPassagesAmount(j, i) > 3) {
                    FloorForkSprite.setPosition(size * j + 120, size * i + 120);
                    preRenderTexture.draw(FloorForkSprite);
                }
            }
        }
    }
}

sf::Vector2f CameraPos;
CollisionRect CameraRect({ 0, 0, scw, sch });
void drawWalls() {
    CameraPos = GameView.getCenter() - GameView.getSize() / 2.f;
    CameraRect.setPosition(CameraPos);
    for (int i = 0; i < CurLocation->wallsRect.size(); i++) {
        CurLocation->SeenWalls[i] = CurLocation->SeenWalls[i] || CameraRect.intersect(CurLocation->wallsRect[i]);
        WallRect.setPosition(CurLocation->wallsRect[i].getPosition());
        WallRect.setTexture((CurLocation->wallsRect[i].getSize().y == WallMaxSize) ? Textures::WallV : Textures::WallG, true);
        preRenderTexture.draw(WallRect);
    }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MiniMapView.setCenter(player.getCenter() * ScaleParam);
        }
    }

    // draw walls
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->SeenWalls.size(); i++) {
        if (CurLocation->SeenWalls[i]) {
            if (CurLocation->wallsRect[i].getSize().y == WallMaxSize) { // |
                sf::Vector2f offset(WallMinSize / 2., 0.);
                line[0] = sf::Vertex((CurLocation->wallsRect[i].getPoint(0) + offset) * ScaleParam, sf::Color::White);
                line[1] = sf::Vertex((CurLocation->wallsRect[i].getPoint(3) + offset) * ScaleParam, sf::Color::White);
            } else { // -
                sf::Vector2f offset(0., WallMinSize / 2.);
                line[0] = sf::Vertex((CurLocation->wallsRect[i].getPoint(0) + offset) * ScaleParam, sf::Color::White);
                line[1] = sf::Vertex((CurLocation->wallsRect[i].getPoint(1) + offset) * ScaleParam, sf::Color::White);
            }
            window.draw(line);
        }
    }

    MMPortalRect.setPosition(portal.hitbox.getPosition() * ScaleParam);
    window.draw(MMPortalRect);

    for (Interactable*& i : listOfBox) {
        MMBoxRect.setPosition(i->hitbox.getPosition() * ScaleParam);
        window.draw(MMBoxRect);
    }

    for (Interactable*& i : listOfArtifact) {
        MMArtifact.setPosition(i->hitbox.getPosition() * ScaleParam);
        window.draw(MMArtifact);
    }

    for (Enemy*& enemy : Enemies) {
        MMEnemyCircle.setPosition(enemy->hitbox.getPosition() * ScaleParam);
        window.draw(MMEnemyCircle);
    }

    // draw players
    if (ClientFuncRun || HostFuncRun) {
        for (Player& p : ConnectedPlayers) {
            MMPlayerCircle.setPosition(p.hitbox.getPosition() * ScaleParam);
            window.draw(MMPlayerCircle);
        }
    }
    MMPlayerCircle.setPosition(player.getCenter() * ScaleParam);
    window.draw(MMPlayerCircle);
    window.setView(GameView);
}
//==============================================================================================



//============================================================================================== UI UPDATERS/CREATORS
void updateHUDUI() {
    HUD::playerCoinAmount.setFontString(FontString(std::to_string(player.inventory.money), 50, sf::Color(200, 200, 200)));
    HUD::playerCoinAmount.parentTo(&HUD::coinSprite, true, { -10, 0 });
}

void updateInventoryUI() {
    {
        using namespace inventoryInterface;

        if (doInventoryUpdate[inventoryPage::Items]) {
            int slotNumber = 0;
            unsigned int offsetLarge = 200;
            unsigned int offsetSmall = 50;
            unsigned int slotSize = 150;
            for (Item*& drawnItem : player.inventory.items) {
                drawnItem->animation->setScale({ 0.75, 0.75 });

                float itemX = (slotNumber % 6) * offsetLarge +
                              itemListBG.getPosition().x + offsetSmall;
                float itemY = (slotNumber / 6) * offsetLarge +
                              itemListBG.getPosition().y + offsetSmall;

                ItemSlot* slot = &itemSlotsElements[drawnItem->id];
                if (!slot->isInitialized) {
                    slot->init("inv_ItemIDSlot" + drawnItem->id);
                }
                slot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));
                slot->setSize({ slotSize, slotSize });
                slot->setTexture(Textures::ItemPanel, UI::element);
                slot->setPosition(itemX, itemY);

                drawnItem->animation->moveToAnchor(slot, UI::center, UI::center);

                slotNumber++;
            }

            coinSlot.background->setTexture(Textures::INVISIBLE);
            coinSlot.amountText->setString(std::to_string(player.inventory.money));

            doInventoryUpdate[inventoryPage::Items] = false;
        }
        statsHPRegenText.setString("Health regen: " + floatToString(player.HealthRecovery));
        statsMPRegenText.setString("Mana regen: " + floatToString(player.ManaRecovery));
        statsArmorText.setString("Armor: " + floatToString(player.Armor.cur));
        statsCompletedLevelsText.setString("Completed Levels: " + std::to_string(completedLevels));
        statsCurLevelsText.setString("Current Level: " + std::to_string(curLevel));
        updateUpgradeInterfaceUI();
    }
}

void updateShopUI() {
    {
        using namespace MenuShop;
        int slotNumber = 0;
        unsigned int offsetLarge = 200;
        unsigned int offsetSmall = 50;
        unsigned int slotSize = 100;
        for (Item*& drawnItem : shop.soldItems.items) {
            drawnItem->animation->setScale({ 0.5, 0.5 });

            float itemX = (slotNumber % 5) * offsetLarge + offsetSmall;
            float itemY = (slotNumber / 5) * offsetLarge + offsetSmall;

            ShopSlot* slot = &slotsElements[drawnItem->id];
            if (!slotsElements[drawnItem->id].isInitialized) {
                slot->init("mShop_ItemIDSlot" + drawnItem->id);
            }
            slot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));
            slot->setSize({ slotSize, slotSize });
            slot->setTexture(Textures::ItemPanel, UI::element);
            slot->setPosition(itemX, itemY);

            drawnItem->animation->moveToAnchor(slot, UI::center, UI::center);


            PlacedText& itemPriceText = *slot->priceText;
            itemPriceText.setFontString(
                FontString(std::to_string(shop.itemPrices[drawnItem->id]) + " C", 20)
            );

            slotNumber++;
        }

        slotNumber = 0;
        for (Item*& drawnItem : player.inventory.items) {
            drawnItem->animation->setScale({ 0.5, 0.5 });

            float itemX = (slotNumber % 3) * offsetLarge + offsetSmall;
            float itemY = (slotNumber / 3) * offsetLarge + offsetSmall;

            ShopSlot* pslot = &playerSlotsElements[drawnItem->id];
            if (!pslot->isInitialized) {
                pslot->init("mShop_PlItemIDSlot" + drawnItem->id);
            }
            pslot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));

            pslot->priceText->setFontString(
                FontString(std::to_string(shop.itemPrices[drawnItem->id]) + " C", 20)
            );

            pslot->setSize({ slotSize, slotSize });
            pslot->setTexture(Textures::ItemPanel, UI::element);
            pslot->setPosition(itemX, itemY);

            drawnItem->animation->moveToAnchor(pslot, UI::center, UI::center);

            slotNumber++;
        }
    }
}

void updateUpgradeInterfaceUI() {
    {
        using namespace upgradeInterface;
        updateUpgradeTexts();
        updateUpgradeShopStats();
        upgradeInterface::playerCoinAmount.setFontString(FontString(std::to_string(player.inventory.money), 50, sf::Color(200, 200, 200)));
        upgradeInterface::playerCoinAmount.parentTo(&upgradeInterface::coinSprite, true, { -10, 0 });
    }
}
//==============================================================================================

sf::Clock SHiftClickTime;
//============================================================================================== EVENT HANDLERS
void EventHandler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            if (ClientFuncRun)    chat.UseCommand("/disconnect");
            else if (HostFuncRun) chat.UseCommand("/server off");
            Musics::MainMenu.pause();
            window.close();
        }
        if (Connecting) {
            if (keyPressed(event, sf::Keyboard::Escape)) {
                Connecting = false;
            }
        }
        if (chat.InputText(event)) {
            if (keyPressed(event, sf::Keyboard::Enter)) {
                if (Connecting && CurLocation == &MainMenuLocation) {
                    IPOfHost = chat.Last();
                    if (!std::regex_match(IPOfHost, regexOfIP)) continue;
                    chat.addLine("connecting...");
                    if (MySocket.connect(IPOfHost, 53000, sf::milliseconds(300)) == sf::Socket::Done) {
                        selector.add(MySocket);
                        if (selector.wait(sf::seconds(1)) && selector.isReady(MySocket) && MySocket.receive(ReceivePacket) == sf::Socket::Done) {
                            ReceivePacket >> ComputerID;
                            std::cout << "My ID = " << ComputerID << '\n';

                            for (int i = 0; i < ComputerID; i++) {
                                ReceivePacket >> sPacketData;
                                HUD::ListOfPlayers.addWord(sPacketData);
                                ConnectedPlayers.push_back(*(new Player()));
                                ConnectedPlayers.back().setAnimation(Textures::Player, &Shaders::Player);
                                DrawableStuff.push_back(&(ConnectedPlayers.back()));
                                std::cout << sPacketData << " connected\n";
                            }

                            for (int i = 0; i < ComputerID; i++) {
                                ReceivePacket >> ConnectedPlayers[i] >> sPacketData;
                                ConnectedPlayers[i].Name.setString(sPacketData);
                                ReceivePacket >> ConnectedPlayers[i].Health >> ConnectedPlayers[i].HealthRecovery;
                                std::cout << sPacketData << '\n';
                            }
                            SendPacket << packetStates::FirstConnect << player.Name.getText();
                            SendPacket << player.Health << player.HealthRecovery;

                            ClientFuncRun = true;
                            sendSendPacket();
                            ClientTread.launch();
                            Connecting = false;

                            chat.addLine("done");
                        } else {
                            chat.addLine("Failed to connect to server");
                            SelfDisconnect();
                        }
                    } else {
                        chat.addLine("Failed to connect to server");
                    }
                }
                if (ClientFuncRun || HostFuncRun) {
                    mutexOnSend.lock();
                    SendPacket << packetStates::ChatEvent << chat.Last();
                    sendSendPacket();
                    mutexOnSend.unlock();
                }
            }
        } else if (HUD::EscapeMenuActivated) {
            HUD::EscapeButton.isActivated(event);
            HUD::SettingsButton.isActivated(event);
            HUD::EncyclopediaButton.isActivated(event);
            if (keyPressed(event, sf::Keyboard::Escape)) {
                HUD::EscapeMenuActivated = false;
            }
        } else if (inventoryInterface::isDrawInventory) {
            inventoryHandler(event);
        } else if (MenuShop::isDrawShop) {
            shopHandler(event);
        } else if (upgradeInterface::isDrawUpgradeInterface) {
            upgradeInterfaceHandler(event);
        } else {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    if (MiniMapActivated) {
                        MiniMapActivated = false;
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                        continue;
                    } else if (HUD::showDescriptions) {
                        HUD::showDescriptions = false;
                        continue;
                    } else {
                        HUD::EscapeMenuActivated = true;
                    }
                }
                if (event.key.code == sf::Keyboard::R && player.isAlive()) {
                    player.CurWeapon->HolsterAction();
                }
                if (event.key.code == sf::Keyboard::Tab) {
                    MiniMapActivated = !MiniMapActivated;
                    if (MiniMapActivated) {
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                    } else {
                        MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                    }
                }
                if (event.key.code == sf::Keyboard::Space) {
                    if (MiniMapActivated) {
                        MiniMapHoldOnPlayer = !MiniMapHoldOnPlayer;
                    }
                }
                if (event.key.code == sf::Keyboard::H) {
                    int index = player.inventory.find(ItemID::regenDrug);
                    if (index != -1 && useItem(player.inventory.items[index])) {
                        if (player.inventory.items[index]->amount <= 0) {
                            player.inventory.removeItem(player.inventory.items[index], false);
                        }
                        inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
                    }
                }
                if (event.key.code == sf::Keyboard::E) {
                    {
                        using namespace inventoryInterface;
                        isDrawInventory = true;
                        frameButtonsHolder.activateButton(activePage);
                        doInventoryUpdate[activePage] = true;
                    }
                }
                if (event.key.code == sf::Keyboard::LShift && player.isAlive()) {
                    player.makeADash = SHiftClickTime.restart().asSeconds() < 0.2f && playerCanDash;
                    playerMakingADash = player.makeADash;
                }
                if (sf::Keyboard::Num1 <= event.key.code && event.key.code <= sf::Keyboard::Num3) {
                    if (!MiniMapActivated) {
                        CurWeapon = event.key.code - sf::Keyboard::Num1;
                        player.ChangeWeapon(Weapons[CurWeapon.cur]);

                        std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                        HUD::ReloadWeaponText.setString(reloadStr);
                        HUD::ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                    }
                }
            }
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::LShift) {
                    playerMakingADash = false;
                }
            }
            if (event.type == sf::Event::MouseWheelMoved) {
                if (MiniMapActivated) {
                    if (event.mouseWheel.delta < 0) {
                        MiniMapView.zoom(1.1f);
                        MiniMapZoom *= 1.1f;
                    } else {
                        MiniMapView.zoom(1.f / 1.1f);
                        MiniMapZoom /= 1.1f;
                    }
                }
            }

            if (player.isAlive()) {
                mutexOnDataChange.lock();
                for (Interactable*& x : InteractableStuff) {
                    if (x->CanBeActivated(player.hitbox)) {
                        x->isActivated(player.hitbox, event);
                        break;
                    }
                }

                for (int i = 0; i < PickupStuff.size(); i++) {
                    if (PickupStuff[i]->CanBeActivated(player.hitbox)) {
                        if (PickupStuff[i]->isActivated(player.hitbox, event)) {
                            player.addItem(PickupStuff[i]);
                            inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
                            DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(PickupStuff[i]));
                            DeletePointerFromVector(PickupStuff, i--);
                        }
                    }
                }
                mutexOnDataChange.unlock();

                if (player.CurWeapon != nullptr && !MiniMapActivated) {
                    player.CurWeapon->Update(event);
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (!MiniMapActivated) {
                    CurWeapon -= (int)event.mouseWheelScroll.delta;
                    player.ChangeWeapon(Weapons[CurWeapon.cur]);

                    std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                    HUD::ReloadWeaponText.setString(reloadStr);
                    HUD::ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                }
            }
        }
    }
}

void inventoryHandler(sf::Event& event) {
    {
        using namespace inventoryInterface;

        if (keyPressed(event, sf::Keyboard::E) || keyPressed(event, sf::Keyboard::Escape)) {
            backButton.buttonFunction();
            return;
        }

        backButton.isActivated(event);
        frameButtonsHolder.isButtonsActive(event);

        if ((mouseButtonPressed(event, sf::Mouse::Left) && upgradeInterface::switchGunLBtn.isActivated(event)) || keyPressed(event, sf::Keyboard::Left)) {
            upgradeInterface::switchGunLBtn.buttonFunction();
            setUpgradeFunctions();
            updateUpgradeShopStats();
        }
        if ((mouseButtonPressed(event, sf::Mouse::Left) && upgradeInterface::switchGunRBtn.isActivated(event)) || keyPressed(event, sf::Keyboard::Right)) {
            upgradeInterface::switchGunRBtn.buttonFunction();
            setUpgradeFunctions();
            updateUpgradeShopStats();
        }

        bool isAnythingHovered = false;
        int itemTypeCount = 0;
        if (activePage == inventoryPage::Items) {
            for (Item*& item : player.inventory.items) {
                if (itemSlotsElements[item->id].isInitialized &&
                    itemSlotsElements[item->id].contains(sf::Vector2f(sf::Mouse::getPosition()))) {
                    if (item->id != prevItemDescID) {
                        prevItemDescID = ItemID::ItemCount;
                        isItemDescDrawn = false;
                    }
                    isAnythingHovered = true;
                    if (mouseButtonPressed(event, sf::Mouse::Right)) {
                        isItemDescDrawn = true;
                        itemDescText.setString(itemDesc[item->id]);
                        prevItemDescID = item->id;
                    }
                    if (mouseButtonPressed(event, sf::Mouse::Left) && useItem(item)) {
                        if (item->amount <= 0) {
                            itemTypeCount--;
                            isItemDescDrawn = false;
                            player.inventory.removeItem(item, false);
                        }
                        doInventoryUpdate[inventoryPage::Items] = true;
                    }
                }
                itemTypeCount++;
            }
            prevItemTypeCount = itemTypeCount;
            if (!isAnythingHovered) isItemDescDrawn = false;
        }
        if (activePage == inventoryPage::Weapons) {
            for (int i = 0; i < upgradeInterface::compUpgBtns.size(); i++) {
                for (int j = 0; j < upgradeInterface::compUpgBtns[i].size(); j++)
                    upgradeInterface::compUpgBtns[i][j]->setSpriteColor(sf::Color(128, 128, 128));
            }
        }
    }
}

void shopHandler(sf::Event& event) {
    {
        using namespace MenuShop;
        if (keyPressed(event, sf::Keyboard::Escape)) {
            backButton.buttonFunction();
            return;
        }
        if (backButton.isActivated(event)) return;
        if (buyButton.isActivated(event)) {
            playerCanDash = player.inventory.find(ItemID::dasher) != -1;
        }

        if (mouseButtonPressed(event, sf::Mouse::Left)) {

            window.setView(ShopStockView);
            sf::Vector2f viewPos = stockTransform.getInverse().
                transformPoint(window.mapPixelToCoords(sf::Mouse::getPosition()));
                for (Item*& item : shop.soldItems.items) {
                    if (slotsElements[item->id].isInitialized && slotsElements[item->id].contains(viewPos)) {

                        selectedItem = item;
                        itemSlot.priceText->setString(std::string(std::to_string(shop.itemPrices[item->id]) + " C"));
                        itemCoinsSprite.moveToAnchor((itemSlot.priceText),
                                                      itemSlot.getPosition() + sf::Vector2f(10, -15));
                        itemStatsText.setString(textWrap(itemDesc[item->id], 62));
                        itemSprite.setTexture(*itemTexture[item->id]);
                        itemSprite.moveToAnchor(&itemSlot);

                        addUI(&itemSlot, UIElements);
                    }
                }
        }
    }
    window.setView(InterfaceView);
}

void upgradeInterfaceHandler(sf::Event& event) {
    {
        using namespace upgradeInterface;

        if (keyPressed(event, sf::Keyboard::Escape)) {
            if (!isChoosingComponent)
                backButton.buttonFunction();
            else isChoosingComponent = false;
            return;
        }

        if (!isChoosingComponent) {
            if ((mouseButtonPressed(event, sf::Mouse::Left) && switchGunLBtn.isActivated(event)) || keyPressed(event, sf::Keyboard::Left)) {
                switchGunLBtn.buttonFunction();
                setUpgradeFunctions();
                updateUpgradeShopStats();
            }
            if ((mouseButtonPressed(event, sf::Mouse::Left) && switchGunRBtn.isActivated(event)) || keyPressed(event, sf::Keyboard::Right)) {
                switchGunRBtn.buttonFunction();
                setUpgradeFunctions();
                updateUpgradeShopStats();
            }
            backButton.isActivated(event);
            generatorBtn.isActivated(event);
            formFactorBtn.isActivated(event);
            converterBtn.isActivated(event);
            targetingBtn.isActivated(event);
        }

        if (mouseButtonPressed(event, sf::Mouse::Right) && isChoosingComponent)
            isChoosingComponent = false;

        if (!inventoryInterface::isDrawInventory && isChoosingComponent) {
            for (RectButton*& btn : compUpgBtns[compType]) {
                if (btn->isActivated(event))
                    updateUpgradeShopStats();
            }
        }
    }
}
//==============================================================================================



//============================================================================================== LEVEL GENERATION FUNCTIONS
void setInteractable(Interactable*& x) {
    if      (x->descriptionID == DescriptionID::box     ) { listOfBox.push_back(x);      setBox(x);      }
    else if (x->descriptionID == DescriptionID::artifact) { listOfArtifact.push_back(x); setArtifact(x); }
    else if (x->descriptionID == DescriptionID::fire    ) { listOfFire.push_back(x);     setFire(x);     }
}

void setBox(Interactable*& box) {
    box->setAnimation(Textures::Box);
    box->setSize(105.f, 117.f);
    box->setFunction([](Interactable* i) {
        if (player.Mana.cur >= 20) {
            player.Mana -= 20.f;

            std::rand(); int r = 1 + std::rand() % 5;
            addMessageText("Money + " + std::to_string(r), sf::Color(255, 170, 29), sf::Color(120, 120, 120));

            player.inventory.money += r;
            inventoryInterface::doInventoryUpdate[inventoryPage::Items] = true;
            DeleteFromVector(listOfBox, i);
            DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
            DeleteFromVector(InteractableStuff, i);

            if (ClientFuncRun || HostFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::UseInteractable << ComputerID << i << player.Health << player.HealthRecovery;
                sendSendPacket();
                mutexOnSend.unlock();
            }
            delete i;
        } else {
            addMessageText("Not enough Mana: " + std::to_string((int)player.Mana.cur) + "/20", sf::Color(255, 0, 0));
        }
    });
}

std::vector<void (*)()> artifactEffects = {
    []{ player.Health.top += 2;       },
    []{ player.Mana.top += 1;         },
    []{ player.HealthRecovery += 0.4; },
    []{ player.ManaRecovery += 0.1;   }
};
std::vector<std::string> artifactText = {
    "Health limit +2",
    "Mana limit +1",
    "Health Recovery +0.4",
    "Mana Recovery +0.2"
};
std::vector<sf::Color> artifactColors = {
    sf::Color(250, 50, 50),
    sf::Color(50, 50, 250),
    sf::Color(250, 80, 80),
    sf::Color(80, 80, 250)
};
void setArtifact(Interactable*& artifact) {
    artifact->setAnimation(Textures::Architect, &Shaders::Architect);
    artifact->setSize(150.f, 150.f);
    artifact->setFunction([](Interactable* i) {
        std::rand(); int r = std::rand() % artifactEffects.size();
        artifactEffects[r]();

        addMessageText(artifactText[r], artifactColors[r]);
        DeleteFromVector(listOfArtifact, i);
        DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
        DeleteFromVector(InteractableStuff, i);

        if (ClientFuncRun || HostFuncRun) {
            mutexOnSend.lock();
            SendPacket << packetStates::UseInteractable << ComputerID << i << player.Health << player.HealthRecovery;
            sendSendPacket();
            mutexOnSend.unlock();
        }
        delete i;
    });
}

void setFire(Interactable*& fire) {
    fire->setAnimation(Textures::Fire, &Shaders::Fire);
    fire->setSize(70.f, 70.f);
}

void placeOnMap(Interactable* i) {
    InteractableStuff.push_back(i);
    DrawableStuff.push_back(i);
}
void placeOnMap(Interactable*& i, float x, float y) {
    i->setPosition(x, y);
    placeOnMap(i);
}
void placeOnMap(Interactable*& i, sf::Vector2f v) {
    placeOnMap(i, v.x, v.y);
}
void placeOnMap(Interactable*& i, int& m, int& n) {
    int x, y;
    sf::Vector2f pos(sf::Vector2i(std::rand(), std::rand()) % (-sf::Vector2i(i->hitbox.getSize() + WallMinSize) + size));
    do {
        x = std::rand() % m;
        y = std::rand() % n;
    } while (!LabyrinthLocation.EnableTiles[y][x]);
    placeOnMap(i, sf::Vector2f(x, y) * (float)size + WallMinSize + pos);
}

void LevelGenerate(int n, int m) {
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);

    LabyrinthLocation.GenerateLocation(n, m, player.getCenter() / float(size));

    portal.setCenter(player.getCenter());

    clearVectorOfPointers(listOfBox);
    clearVectorOfPointers(listOfArtifact);
    clearVectorOfPointers(listOfFire);
    CollisionShapes.clear();
    for (CollisionShape &wall: LabyrinthLocation.wallsRect) {
        CollisionShapes.push_back(&wall);
    }
    Interactable* x;
    for (int i = 0; i < 10; i++) { x = new Interactable(DescriptionID::box);      setInteractable(x); placeOnMap(x, m, n); }
    for (int i = 0; i < 10; i++) { x = new Interactable(DescriptionID::artifact); setInteractable(x); placeOnMap(x, m, n); }
    for (int i = 0; i < 0; i++)  { x = new Interactable(DescriptionID::fire);     setInteractable(x); placeOnMap(x, m, n); }


    clearVectorOfPointers(Enemies);
    Enemies.push_back(new Boss());
    Enemies.back()->atTarget = true;
    Enemies.back()->targetMode = TargetMode::wander;
    Enemies.back()->passiveWait = sf::seconds(GameTime.asSeconds() + std::rand() % 5);
    int amountOfEveryEnemiesOnLevel = curLevel + (curLevel > completedLevels ? 4 : 2);
    for (int i = 0; i < amountOfEveryEnemiesOnLevel; i++) {
        Enemies.push_back(new AngularBody(3 + rand() % 4));
        Enemies.back()->atTarget = true;
        Enemies.back()->targetMode = TargetMode::wander;
        Enemies.back()->passiveWait = sf::seconds(GameTime.asSeconds() + std::rand() % 5);
        // Enemies.push_back(new Distorted());
    }
    for (Enemy*& enemy : Enemies) DrawableStuff.push_back(enemy);

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->hitbox.setCenter(sf::Vector2f((std::rand() % m) + 0.5f, (std::rand() % n) + 0.5f) * (float)size);
            Enemies[i]->setTarget(Enemies[i]->getCenter());
        } while (!LabyrinthLocation.EnableTiles[(int)Enemies[i]->hitbox.getPosition().y / size][(int)Enemies[i]->hitbox.getPosition().x / size] ||
                 distance(Enemies[i]->hitbox.getPosition(), player.getCenter()) < size * 3);
    }
}

void LoadMainMenu() {
    HUD::EscapeMenuActivated = false;
    HUD::ListOfPlayers.clearText();
    clearVectorOfPointers(Bullets);
    clearVectorOfPointers(Enemies);
    clearVectorOfPointers(TempTextsOnGround);
    clearVectorOfPointers(HUD::TempTextsOnScreen);
    clearVectorOfPointers(DamageText);
    clearVectorOfPointers(HUD::MessageText);
    clearVectorOfPointers(listOfBox);
    clearVectorOfPointers(listOfArtifact);
    clearVectorOfPointers(listOfFire);
    clearVectorOfPointers(PickupStuff);
    CollisionShapes.clear();
    DrawableStuff.clear();
    InteractableStuff.clear();

    for (Effect *effect : player.effects) {
        clearEffect(player, effect);
    }
    player.CurWeapon->lock = true;

    CurLocation = &MainMenuLocation;
    for (CollisionShape &wall : CurLocation->wallsRect) {
        CollisionShapes.push_back(&wall);
    }
    curLevel = 0;

    player.hitbox.setCenter(3.5f * size, 2.5f * size);
    for (Player& p: ConnectedPlayers) {
        p.hitbox.setCenter(3.5f * size, 2.5f * size);
    }
    // FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);

    portal.setPosition(1612.5, 1545);
    portal2.setPosition(1612.5 - size * 1.5, 1545);

    CurLocation->FindEnableTilesFrom(player.getCenter() / (float)size);

    // Set cameras
    GameView.setCenter(player.getCenter());
    MiniMapView.setCenter(player.getCenter() * ScaleParam);
    HUDView.setCenter({ scw / 2.f, sch / 2.f });

    Musics::Fight1.stop();
    Musics::Fight2.stop();
    Musics::MainMenu.play();

    HUD::InterfaceStuff.clear();
    HUD::InterfaceStuff.push_back(&chat);
    addUI(&HUD::HUDFrame, HUD::InterfaceStuff);

    InteractableStuff.push_back(&shopSector);
    InteractableStuff.push_back(&upgradeSector);

    placeOnMap(&portal);
    placeOnMap(&portal2);

    Item* newItem = new Item(ItemID::regenDrug, 1);
    newItem->setAnimation(*itemTexture[ItemID::regenDrug]);
    PickupStuff.push_back(newItem);
    DrawableStuff.push_back(PickupStuff[0]);
    PickupStuff[0]->dropTo(player.getCenter() + sf::Vector2f(100, 100));

    Interactable* x = new Interactable(DescriptionID::box);
    setInteractable(x);
    x->setPosition(1912.5, 1545);
    placeOnMap(x);

    x = new Interactable(DescriptionID::artifact);
    setInteractable(x);
    x->setPosition(1312.5, 1545);
    placeOnMap(x);

    DrawableStuff.push_back(&player);
    for (Player& p: ConnectedPlayers) {
        DrawableStuff.push_back(&p);
    }
}
//==============================================================================================



//============================================================================================== GAME STATE FUNCTIONS
void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++) {
        if (Bullets[i]->penetration < 0 || Bullets[i]->todel) {
            DeletePointerFromVector(Bullets, i--);
        } else {
            Bullets[i]->move(CurLocation);
            if (!faction::friends(Bullets[i]->fromWho, player.faction)) {
                if (player.hitbox.intersect(Bullets[i]->hitbox)) {
                    player.getDamage(Bullets[i]->damage);
                    Bullets[i]->penetration--;
                }
                for (Player& p: ConnectedPlayers) {
                    if (p.hitbox.intersect(Bullets[i]->hitbox)) {
                        p.getDamage(Bullets[i]->damage);
                        Bullets[i]->penetration--;
                    }
                }
            } else {
                for (Enemy*& enemy : Enemies) {
                    if (!faction::friends(Bullets[i]->fromWho, enemy->faction) && enemy->hitbox.intersect(Bullets[i]->hitbox)) {
                        enemy->getDamage(Bullets[i]->damage);
                        Bullets[i]->penetration--;
                        TempText* tempText = new TempText(sf::seconds(1.5f));
                        tempText->setCharacterSize(30);
                        tempText->setOutlineColor(sf::Color::White);
                        tempText->setOutlineThickness(3);
                        tempText->setString(std::to_string(int(Bullets[i]->damage)));
                        tempText->setFillColor(sf::Color(250, 50, 50, 200));
                        tempText->setCenter(enemy->hitbox.getPosition());
                        DamageText.push_back(tempText);
                        break;
                    }
                }
            }
        }
    }
    newBullets.clear();
}

void EnemyDie(int i) {
    if (HostFuncRun) {
        mutexOnSend.lock();
        SendPacket << packetStates::EnemyDie << i;
        sendSendPacket();
        mutexOnSend.unlock();
    }
    mutexOnDataChange.lock();
    if (Enemies[i]->dropInventory) {
        for (Item*& item : Enemies[i]->inventory.items) {
            item->dropTo(Enemies[i]->hitbox.getPosition());

            PickupStuff.push_back(item);
            DrawableStuff.push_back(item);
        }
    }
    Enemies[i]->inventory.items.clear();

    DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(Enemies[i]));
    DeletePointerFromVector(Enemies, i);
    if (Enemies.size() == 0) {
        if (!player.isAlive()) {
            player.Health.cur = player.Health.top;
            DrawableStuff.push_back(&player);
        }
        for (Player& p: ConnectedPlayers) {
            if (!p.isAlive()) {
                p.Health.cur = p.Health.top;
                DrawableStuff.push_back(&p);
            }
        }

        TempText* enemiesKilledText = new TempText(sf::seconds(7));
        enemiesKilledText->setCharacterSize(40);
        enemiesKilledText->setString("      All enemies cleared!\nPortal to the next area has now opened.");
        enemiesKilledText->setCenter(scw / 2.0f, sch / 3.0f);
        HUD::TempTextsOnScreen.push_back(enemiesKilledText);

        if (!in(InteractableStuff, (Interactable*)&portal)) {
            InteractableStuff.push_back(&portal);
        }
        curLevel++;
        completedLevels = std::max(curLevel, completedLevels);
    }
    mutexOnDataChange.unlock();
}

sf::Vector2f chooseCellCenter(Enemy*& cr, sf::Vector2f lastTarget) {
    sf::Vector2f targetCenter = cr->getCenter();
    sf::Vector2f lastTargetCenter = cr->lastTarget;
    float halfStep = size / 2;
    targetCenter.x = halfStep + size * round(targetCenter.x / size);
    targetCenter.y = halfStep + size * round(targetCenter.y / size);
    int axis = std::rand() % 2 == 0 ? 1 : -1;
    int dirX = 0, dirY = 0;
    if (axis == -1)
        dirX = std::rand() % 2 == 0 ? 1 : -1;
    else
        dirY = std::rand() % 2 == 0 ? 1 : -1;
    return targetCenter + sf::Vector2f(dirX * size, dirY * size);
}

void updateEnemies() {
    for (int i = 0; i < Enemies.size(); i++) {
        if (!Enemies[i]->isAlive()) {
            EnemyDie(i--);
        } else {
            std::vector<sf::Vector2f> centers;
            if (player.isAlive() &&
                ExistDirectWay(Enemies[i]->hitbox, player.getCenter()) &&
                (length(Enemies[i]->getCenter() - player.getCenter()) <= 3 * size ||
                 Enemies[i]->targetMode > TargetMode::wander)) {
                centers.push_back(player.getCenter());
                // std::cout << "Enemy \"" << Enemies[i]->Name.getText() << "\" has found a direct way to the player\n";
            }
            mutexOnDataChange.lock();
            for (Player& p: ConnectedPlayers) {
                if (p.isAlive() && ExistDirectWay(Enemies[i]->hitbox, p.getCenter()))
                    centers.push_back(p.getCenter());
            }
            mutexOnDataChange.unlock();
            if (centers.size() > 0) {
                // std::cout << Enemies[i]->Name.getText() << " takes action: TARGET FOUND. ATTACKING.\n";
                if (Enemies[i]->targetMode <= TargetMode::search)
                    Enemies[i]->targetMode = TargetMode::pursuit;
                Enemies[i]->VelocityBuff = 1.0;
                Enemies[i]->shootTarget = centers[0];
                for (int j = 1; j < centers.size(); j++)
                    if (distance(Enemies[i]->getCenter(), centers[j]) < distance(Enemies[i]->getCenter(), Enemies[i]->target)) {
                        Enemies[i]->shootTarget = centers[j];
                        Enemies[i]->passiveWait = sf::seconds(GameTime.asSeconds() + 2 * distance(Enemies[i]->getCenter(), centers[j]) / Enemies[i]->MaxVelocity);
                    }
                sf::Vector2f playerTargetPos = Enemies[i]->shootTarget;
                float attackArea = size / 4.;
                float targetEnemyDistDiff = length(Enemies[i]->getCenter() - playerTargetPos);
                float distRange = 3 / 2 * size;
                switch (Enemies[i]->targetMode)
                {
                    case TargetMode::pursuit:
                        // EnemyType if-else is Placeholder.
                        // Will be changed to a proper Enemy class behaviour fuinction
                        if (Enemies[i]->enemyType == EnemyType::minibossBullethell) {
                            Enemies[i]->setTarget(Enemies[i]->shootTarget);
                            Enemies[i]->CurWeapon->Shoot(Enemies[i]->hitbox, Enemies[i]->shootTarget - Enemies[i]->getCenter(), Enemies[i]->faction);
                        }
                        else {
                            if (abs(targetEnemyDistDiff - distRange) <= attackArea / 4) {
                                Enemies[i]->targetMode = TargetMode::fight;
                                Enemies[i]->passiveWait = GameTime;
                            } else {
                                float posCoef = distRange / targetEnemyDistDiff;
                                sf::Vector2f targetPoint = posCoef * Enemies[i]->getCenter() + (1 - posCoef) * playerTargetPos;
                                Enemies[i]->setTarget(targetPoint);
                            }
                        }
                        break;
                    case TargetMode::fight:
                        Enemies[i]->CurWeapon->Shoot(Enemies[i]->hitbox, Enemies[i]->shootTarget - Enemies[i]->getCenter(), Enemies[i]->faction);
                        if (length(Enemies[i]->getCenter() - Enemies[i]->target) <= attackArea / 4) {
                            Enemies[i]->setTarget(Enemies[i]->getCenter());
                        }
                        if (GameTime > Enemies[i]->passiveWait &&
                            (targetEnemyDistDiff - distRange > attackArea * 2 ||
                            targetEnemyDistDiff <= attackArea / 2)) {
                            Enemies[i]->targetMode = TargetMode::pursuit;
                        }
                        if (GameTime > Enemies[i]->passiveWait) {
                            float increment = size / 8;
                            sf::Vector2f randomOffset = increment * centerRandVector(4);
                            Enemies[i]->setTarget(Enemies[i]->target + randomOffset);
                            Enemies[i]->passiveWait = sf::seconds(GameTime.asSeconds() + 1. / 3.);
                        }
                        break;
                }
            }
            else {
                float timeToTarget;
                if (Enemies[i]->targetMode == TargetMode::fight)
                    Enemies[i]->targetMode = TargetMode::pursuit;
                switch (Enemies[i]->targetMode)
                {
                    case TargetMode::sleep:
                        break;
                    case TargetMode::wander:
                        if (GameTime >= Enemies[i]->passiveWait) {
                            Enemies[i]->lastTarget = Enemies[i]->target;
                            sf::Vector2f newTarget;
                            do {
                                newTarget = chooseCellCenter(Enemies[i], Enemies[i]->lastTarget);
                            } while (newTarget == Enemies[i]->lastTarget);
                            Enemies[i]->setTarget(newTarget);
                            Enemies[i]->passiveWait = sf::seconds(GameTime.asSeconds() + std::rand() % 5 + 3);
                            Enemies[i]->targetMode = TargetMode::wander;
                            // std::cout << Enemies[i]->Name.getText() << " takes action: wander to " << Enemies[i]->target << "\n";
                            Enemies[i]->VelocityBuff = 0.35;
                        }
                        break;
                    case TargetMode::pursuit:
                        Enemies[i]->setTarget(Enemies[i]->shootTarget);
                        if (GameTime >= Enemies[i]->passiveWait || length(Enemies[i]->Velocity) <= 2) {
                            Enemies[i]->setTarget(Enemies[i]->getCenter());
                        }
                        if (Enemies[i]->atTarget) {
                            Enemies[i]->targetMode = TargetMode::search;
                            Enemies[i]->passiveWait = sf::seconds(GameTime.asSeconds() + std::rand() % 15 + 15);
                            // std::cout << Enemies[i]->Name.getText() << " takes action: searching for target out of sight until " << Enemies[i]->passiveWait.asSeconds() << "\n";
                        }
                        break;
                    case TargetMode::search:
                        if (Enemies[i]->atTarget || GameTime >= Enemies[i]->timeUntilNextSearch || length(Enemies[i]->Velocity) <= 2) {
                            sf::Vector2f newTarget;
                            float targetX = std::rand() % (2 * size) - size, targetY = std::rand() % (2 * size) - size;
                            if (abs(targetX) < abs(targetY))
                                newTarget = sf::Vector2f(targetX, 0);
                            else newTarget = sf::Vector2f(0, targetY);
                            Enemies[i]->setTarget(Enemies[i]->getCenter() + newTarget);
                            timeToTarget = distance(Enemies[i]->getCenter(), Enemies[i]->target) / Enemies[i]->MaxVelocity;
                            Enemies[i]->timeUntilNextSearch = sf::seconds(GameTime.asSeconds() + timeToTarget);
                            // std::cout << Enemies[i]->Name.getText() << " takes action: searching for target at " << newTarget
                            //           << ". Time left to search: " << Enemies[i]->passiveWait.asSeconds() << "\n";
                        }
                        if (GameTime >= Enemies[i]->passiveWait) {
                            Enemies[i]->targetMode = TargetMode::wander;
                            Enemies[i]->passiveWait = sf::seconds(GameTime.asSeconds() + timeToTarget);
                            Enemies[i]->setTarget(Enemies[i]->getCenter());
                            // std::cout << Enemies[i]->Name.getText() << " takes action: wandering due to not finding target until " << Enemies[i]->passiveWait.asSeconds() << "\n";
                        }
                        break;
                    default:
                        break;
                }
            }
            Enemies[i]->move(CurLocation);
            Enemies[i]->UpdateState();
            Enemies[i]->CurWeapon->lock = false;
            Enemies[i]->CurWeapon->holstered = true;
            Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
            Enemies[i]->CurWeapon->holstered = false;
        }
    }
}

void updateUpgradeShopStats() {
    {
        using namespace upgradeInterface;
        std::string descString = player.CurWeapon->Name + '\n';
        descString += weaponDescString[player.CurWeapon->Name];
        weaponDescPanel.setString(descString);

        manaStStat.setString("Mana storage: " + floatToString(player.CurWeapon->MaxManaStorage));
        relSpStat.setString("Reload Speed: " + floatToString(player.CurWeapon->ReloadSpeed.getStat()) + " mana/sec");
        manaStStat.updateAnchor({ 0, sch / 34.f });
        relSpStat.updateAnchor({ 0, sch / 34.f });

        tthStat.setString("Time To Holster: " + floatToString(player.CurWeapon->TimeToHolster.getStat().asSeconds()) + " sec");
        ttdStat.setString("Time To Dispatch: " + floatToString(player.CurWeapon->TimeToDispatch.getStat().asSeconds()) + " sec");
        tthStat.updateAnchor({ 0, sch / 34.f });
        ttdStat.updateAnchor({ 0, sch / 34.f });

        dmgStat.setString("Damage: " + floatToString(player.CurWeapon->ManaCostOfBullet));
        msStat.setString("Bullet per shot: " + std::to_string(player.CurWeapon->Multishot));
        frStat.setString("Rate of fire: " + floatToString(1 / player.CurWeapon->FireRate.getStat().asSeconds()) + " shots/sec");
        dmgStat.updateAnchor({ 0, sch / 34.f });
        msStat.updateAnchor({ 0, sch / 34.f });
        frStat.updateAnchor({ 0, sch / 34.f });

        velStat.setString("Bullet velocity: " + floatToString(player.CurWeapon->BulletVelocity));
        scatStat.setString("Scatter: " + floatToString(player.CurWeapon->Scatter) + " deg");
        velStat.updateAnchor({ 0, sch / 34.f });
        scatStat.updateAnchor({ 0, sch / 34.f });

        if (player.CurWeapon == &pistol)
            weaponImg.setTexture(Textures::PH_Pistol, UI::texture);
        if (player.CurWeapon == &shotgun)
            weaponImg.setTexture(Textures::PH_Shotgun, UI::texture);
        if (player.CurWeapon == &rifle)
            weaponImg.setTexture(Textures::PH_Rifle, UI::texture);
    }
}

void setUpgradeFunctions() {
    {
        using namespace upgradeInterface;

        compUpgBtns[0][0]->setFunction([]() {
            upgradeStat(50, &player.CurWeapon->MaxManaStorage,
            compUpgCosts[0][0], compUpgStats[0][0], compUpgCount[0][0]);
            if (!pistol.MaxManaStorage.maxed())
                pistol.ManaStorage.top = pistol.MaxManaStorage;
        });

        compUpgBtns[0][1]->setFunction([]() {
            upgradeStat(70, &player.CurWeapon->ReloadSpeed,
            compUpgCosts[0][1], compUpgStats[0][1], compUpgCount[0][1]);
        });

        compUpgBtns[1][0]->setFunction([]() {
            upgradeStat(25, &player.CurWeapon->TimeToHolster,
            compUpgCosts[1][0], compUpgStats[1][0], compUpgCount[1][0]);
        });

        compUpgBtns[1][1]->setFunction([]() {
            upgradeStat(25, &player.CurWeapon->TimeToDispatch,
            compUpgCosts[1][1], compUpgStats[1][1], compUpgCount[1][1]);
        });

        compUpgBtns[2][0]->setFunction([]() {
            upgradeStat(35, &player.CurWeapon->FireRate,
            compUpgCosts[2][0], compUpgStats[2][0], compUpgCount[2][0]);
        });

        compUpgBtns[2][1]->setFunction([]() {
            upgradeStat(80, &player.CurWeapon->ManaCostOfBullet,
            compUpgCosts[2][1], compUpgStats[2][1], compUpgCount[2][1]);
        });

        compUpgBtns[2][2]->setFunction([]() {
            upgradeStat(80, &player.CurWeapon->Multishot,
            compUpgCosts[2][2], compUpgStats[2][2], compUpgCount[2][2]);
        });

        compUpgBtns[3][0]->setFunction([]() {
            upgradeStat(65, &player.CurWeapon->BulletVelocity,
            compUpgCosts[3][0], compUpgStats[3][0], compUpgCount[3][0]);
        });

        compUpgBtns[3][1]->setFunction([]() {
            upgradeStat(65, &player.CurWeapon->Scatter,
            compUpgCosts[3][1], compUpgStats[3][1], compUpgCount[3][1]);
        });
    }
}

void updateUpgradeTexts() {
    {
        using namespace upgradeInterface;
        updateCostsText(compUpgCosts[0][0], &player.CurWeapon->MaxManaStorage, 50);
        updateStatsText(compUpgStats[0][0], &player.CurWeapon->MaxManaStorage);
        updateCountText(compUpgCount[0][0], &player.CurWeapon->MaxManaStorage);

        updateCostsText(compUpgCosts[0][1], &player.CurWeapon->ReloadSpeed, 70);
        updateStatsText(compUpgStats[0][1], &player.CurWeapon->ReloadSpeed);
        updateCountText(compUpgCount[0][1], &player.CurWeapon->ReloadSpeed);

        updateCostsText(compUpgCosts[1][0], &player.CurWeapon->TimeToHolster, 25);
        updateStatsText(compUpgStats[1][0], &player.CurWeapon->TimeToHolster);
        updateCountText(compUpgCount[1][0], &player.CurWeapon->TimeToHolster);

        updateCostsText(compUpgCosts[1][1], &player.CurWeapon->TimeToDispatch, 25);
        updateStatsText(compUpgStats[1][1], &player.CurWeapon->TimeToDispatch);
        updateCountText(compUpgCount[1][1], &player.CurWeapon->TimeToDispatch);

        updateCostsText(compUpgCosts[2][0], &player.CurWeapon->FireRate, 35);
        updateStatsText(compUpgStats[2][0], &player.CurWeapon->FireRate);
        updateCountText(compUpgCount[2][0], &player.CurWeapon->FireRate);

        updateCostsText(compUpgCosts[2][1], &player.CurWeapon->ManaCostOfBullet, 80);
        updateStatsText(compUpgStats[2][1], &player.CurWeapon->ManaCostOfBullet);
        updateCountText(compUpgCount[2][1], &player.CurWeapon->ManaCostOfBullet);

        updateCostsText(compUpgCosts[2][2], &player.CurWeapon->Multishot, 80);
        updateStatsText(compUpgStats[2][2], &player.CurWeapon->Multishot);
        updateCountText(compUpgCount[2][2], &player.CurWeapon->Multishot);

        updateCostsText(compUpgCosts[3][0], &player.CurWeapon->BulletVelocity, 65);
        updateStatsText(compUpgStats[3][0], &player.CurWeapon->BulletVelocity);
        updateCountText(compUpgCount[3][0], &player.CurWeapon->BulletVelocity);

        updateCostsText(compUpgCosts[3][1], &player.CurWeapon->Scatter, 65);
        updateStatsText(compUpgStats[3][1], &player.CurWeapon->Scatter);
        updateCountText(compUpgCount[3][1], &player.CurWeapon->Scatter);
    }
}

void addMessageText(std::string s, sf::Color fillColor, sf::Color outlineColor) {
    TempText* tempText = new TempText(sf::seconds(2.5f));
    tempText->setCharacterSize(50);
    tempText->setOutlineColor(outlineColor);
    tempText->setOutlineThickness(3);
    tempText->setString(s);
    tempText->setFillColor(fillColor);
    tempText->setCenter(scw / 2.f, sch / 2.f - 165.f);
    HUD::MessageText.push_back(tempText);
}

void openUpgradeShop() {
    {
        using namespace upgradeInterface;
        initUpgradeUI();
        setUpgradeFunctions();
        updateUpgradeShopStats();
        addUI(&BG, UIElements);
        for (int i = 0; i < compUpgBtns.size(); i++)
            for (int j = 0; j < compUpgBtns[i].size(); j++)
                compUpgBtns[i][j]->setSpriteColor(sf::Color::White);
    }
}

void processEffects() {
    updateEffects(&player);
    for (Enemy*& enemy : Enemies)
        updateEffects(enemy);
}

void updateEffects(Creature* creature) {
    std::vector<Effect*>& effectVec = creature->effects;
    for (int i = 0; i < effectVec.size(); i++) {
        if (GameTime > effectVec[i]->howLongToExist) {
            clearEffect(*creature, effectVec[i]);
            DeletePointerFromVector(effectVec, i--);
        } else {
            switch (effectVec[i]->type) {
                case Effects::Damage:
                    creature->getDamage(effectVec[i]->parameters[0] * TimeSinceLastFrame.asSeconds());
                    break;
                case Effects::Heal:
                    creature->getDamage(-effectVec[i]->parameters[0] * TimeSinceLastFrame.asSeconds());
                    break;
                case Effects::HPRegen:
                    if (!effectVec[i]->active) {
                        creature->HealthRecovery += effectVec[i]->parameters[0];
                        effectVec[i]->active = true;
                    }
                    break;
                case Effects::Burn:
                    if (!effectVec[i]->active) {
                        creature->getDamage(effectVec[i]->parameters[0]);
                        creature->HealthRecoveryActive = false;
                        effectVec[i]->active = true;
                    }
                    if (effectVec[i]->customTickClock->getElapsedTime() >= effectVec[i]->customTick) {
                        creature->getDamage(effectVec[i]->parameters[0]);
                        creature->HealthRecoveryActive = false;
                        effectVec[i]->customTickClock->restart();
                    }
                    break;
                default:
                    break;
            }
        }
    }
}

void applyEffect(Creature& owner, Effect* effect) {
    switch (effect->type) {
        case Effects::Burn:
            if (owner.effectStacks[effect->type] < 1) {
                owner.effects.push_back(effect);
                owner.effectStacks[effect->type] = 1;
            }
            break;

        case Effects::HPRegen:
            owner.effects.push_back(effect);
            owner.effectStacks[effect->type] += 1;
            break;

        case Effects::Heal:
            owner.effects.push_back(effect);
            break;

        case Effects::Damage:
            owner.effects.push_back(effect);
            break;

        default:
            owner.effects.push_back(effect);
            owner.effectStacks[effect->type] += 1;
            break;
    }
}

void clearEffect(Creature& owner, Effect* effect) {
    switch (effect->type) {
        case Effects::HPRegen:
            owner.HealthRecovery -= effect->parameters[0];
            break;
        case Effects::Burn:
            owner.HealthRecoveryActive = true;
            owner.effectStacks[effect->type] -= 1;
            break;
        default:
            owner.effectStacks[effect->type] -= 1;
            break;
    }
}

bool useItem(Item*& item) {
    if (item->amount <= 0)
        return false;
    item->amount--;
    switch (item->id) {
        case ItemID::regenDrug:
            applyEffect(player, new Effect(Effects::HPRegen, std::vector<float>{1.0f}, sf::seconds(10.f)));
            return true;
        default:
            return false;
    }
}
//==============================================================================================



//============================================================================================== GAME GRAPHICS FUNCTIONS
void updateShaders() {
    sf::Vector2f uMouse(sf::Mouse::getPosition());
    float uTime = GameTime.asSeconds();
    sf::Vector2f uPlayerPosition(player.getCenter() - GameView.getCenter() + GameView.getSize() / 2.f);

    Shaders::Flashlight.setUniform("uMouse", player.lookDirection + uPlayerPosition);
    Shaders::Flashlight.setUniform("uPlayerPosition", uPlayerPosition);

    Shaders::Player.setUniform("direction", player.lookDirection);

    Shaders::Portal.setUniform("uTime", uTime);

    Shaders::Portal2.setUniform("uTime", uTime);

    Shaders::Architect.setUniform("uTime", uTime);

    Shaders::Distortion1.setUniform("uTime", uTime);
    Shaders::Distortion2.setUniform("uTime", uTime);

    Shaders::Bullet.setUniform("uTime", uTime);

    Shaders::Fire.setUniform("uTime", uTime);

    Shaders::WaveMix.setUniform("uTime", uTime);

    Shaders::Boss.setUniform("uTime", uTime);
}
//==============================================================================================



//============================================================================================== HELPER FUNCTIONS
bool CanSomethingBeActivated() {
    CurInteractable = nullptr;
    for (Interactable*& x : InteractableStuff) {
        if (x->CanBeActivated(player.hitbox)) {
            CurInteractable = x;
            return true;
        }
    }
    HUD::showDescriptions = false;
    return false;
}

template <class T>
void updateCostsText(PlacedText* costText, Upgradable<T>* stat, int cost) {
    if (stat->maxed()) costText->setString(FontString("Maxed out", 50, sf::Color::Green));
    else costText->setString(FontString("Cost of upgrade: " + std::to_string(cost * (1 + stat->curLevel)), 50));
}

template <class T>
void updateCountText(PlacedText* counterText, Upgradable<T>* stat) {
    if (stat->maxed()) counterText->setString(FontString(std::to_string(stat->maxLevel) + " / " +
                                                         std::to_string(stat->maxLevel), 24, sf::Color::Green));
    else counterText->setString(FontString(std::to_string(stat->curLevel + 1) + " / " +
                                           std::to_string(stat->maxLevel), 24));
}

template <class T>
void updateStatsText(PlacedText* statText, Upgradable<T>* stat) {
    if constexpr (std::is_same_v<T, sf::Time>) {
        if (stat->maxed()) statText->setString(FontString(floatToString(stat->getStat().asSeconds()), 24, sf::Color::Green));
        else statText->setString(FontString(floatToString(stat->getStat().asSeconds()) + " -> " +
                                            floatToString(stat->stats[stat->curLevel + 1].asSeconds()), 24));
    } else if constexpr (std::is_same_v<T, float>) {
        if (stat->maxed()) statText->setString(FontString(floatToString(stat->getStat()), 24, sf::Color::Green));
        else statText->setString(FontString(floatToString(stat->getStat()) + " -> " +
                                            floatToString(stat->stats[stat->curLevel + 1]), 24));
    } else {
        if (stat->maxed()) statText->setString(FontString(std::to_string(stat->getStat()), 24, sf::Color::Green));
        else statText->setString(FontString(std::to_string(stat->getStat()) + " -> " +
                                            std::to_string(stat->stats[stat->curLevel + 1]), 24));
    }
}

template <class T>
void upgradeStat(int cost, Upgradable<T>* stat, PlacedText* costText, PlacedText* statText, PlacedText* counterText) {
    if (!stat->maxed() && player.inventory.money >= cost * (1 + stat->curLevel)) {
        player.inventory.money -= cost * (1 + stat->curLevel);
        stat->upgrade(1);
    }
    if (costText != nullptr) updateCostsText(costText, stat, cost);
    if (statText != nullptr) updateStatsText(statText, stat);
    if (counterText != nullptr) updateCountText(counterText, stat);
}

void saveGame() {
    std::ofstream fileToSave("sources/saves/save.json");
    json j;
    j["Player"] = player;
    j["Pistol"] = pistol;
    j["Shotgun"] = shotgun;
    j["Rifle"] = rifle;

    fileToSave << j.dump(4);

    fileToSave.close();
}

void loadSave() {
    std::ifstream saveFile("sources/saves/save.json");
    if (!saveFile.is_open()) {
        std::rand();
        player.Name.setString("Employee " + std::to_string(1 + (size_t(std::rand()) * 8645) % 999));
    } else {
        nlohmann::json j = nlohmann::json::parse(saveFile);
        j.at("Player").get_to<Player>(player);
        j.at("Pistol").get_to<Pistol>(pistol);
        j.at("Shotgun").get_to<Shotgun>(shotgun);
        j.at("Rifle").get_to<Rifle>(rifle);
    }
    saveFile.close();
}
//==============================================================================================



//============================================================================================== MAIN LOOP
void MainLoop() {
    while (window.isOpen()) {

        mutexOnDataChange.lock();
        bool someoneAlive = false;
        if (player.isAlive()) {
            player.UpdateState();
            someoneAlive = true;
        }
        for (Player& p: ConnectedPlayers) {
            if (p.isAlive()) {
                p.UpdateState();
                someoneAlive = true;
            }
        }
        if (!someoneAlive) {
            if (HostFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::AllPlayerDie;
                sendSendPacket();
                mutexOnSend.unlock();
            }
            if (!ClientFuncRun) {
                LoadMainMenu();
                player.Health.cur = player.Health.top;
                for (Player& p: ConnectedPlayers) {
                    p.Health.cur = p.Health.top;
                }
            }
        }
        mutexOnDataChange.unlock();

        if (!ClientFuncRun) {
            updateEnemies();
            if (HostFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::EnemiesPos << Enemies;
                sendSendPacket();
                mutexOnSend.unlock();
            }
        }

        if (CurLocation == &LabyrinthLocation) {
            if (Musics::Fight1.getDuration() - Musics::Fight1.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight2.play();
            }
            if (Musics::Fight2.getDuration() - Musics::Fight2.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight1.play();
            }
        }
        if (!window.hasFocus()) {
            mutexOnDataChange.lock();
            if (player.CurWeapon != nullptr && player.isAlive()) {
                player.CurWeapon->Shoot(player.hitbox, player.lookDirection, player.faction);
            }
            for (Weapon*& weapon : Weapons)
                if (weapon->holstered) weapon->Reload(player.Mana);

            if (newBullets.size() > 0 && (HostFuncRun || ClientFuncRun)) {
                mutexOnSend.lock();
                SendPacket << packetStates::Shooting << (sf::Int32)(newBullets.size());
                for (int i = 0; i < newBullets.size(); i++) {
                    SendPacket << *newBullets[i];
                }
                sendSendPacket();
                mutexOnSend.unlock();
            }
            updateBullets();
            mutexOnDataChange.unlock();

            sf::Event event;
            while (window.pollEvent(event)) {}

            if (HostFuncRun || ClientFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::PlayerPos << player;
                if (HostFuncRun) SendPacket << ConnectedPlayers;
                sendSendPacket();
                mutexOnSend.unlock();
            }
        } else {
            if (!chat.inputted && !inventoryInterface::isDrawInventory && !MenuShop::isDrawShop) {
                if (player.isAlive()) {
                    player.move(CurLocation);
                    GameView.setCenter(player.getCenter() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i(scw, sch) / 2) / 8));
                    sf::Listener::setPosition(player.getCenter().x, player.getCenter().y, 50.f);
                }
                if (!ClientFuncRun) {
                    mutexOnDataChange.lock();
                    std::vector<sf::Vector2f> centers;
                    for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                        if (i != ComputerID) {
                            if (ConnectedPlayers[i - k].isAlive()) {
                                centers.push_back(ConnectedPlayers[i - k].getCenter());
                            }
                        } else if (player.isAlive()) {
                            centers.push_back(player.getCenter());
                            k++;
                        }
                    }
                    mutexOnDataChange.unlock();
                    // FindAllWaysTo(CurLocation, centers, TheWayToPlayer);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && player.isAlive()) {
                    player.makeADash = playerMakingADash && playerCanDash;
                }
            }

            mutexOnDataChange.lock();
            if (player.CurWeapon != nullptr && player.isAlive()) {
                player.CurWeapon->Shoot(player.hitbox, player.lookDirection, player.faction);
            }
            for (Weapon*& weapon : Weapons)
                if (weapon->holstered) weapon->Reload(player.Mana);

            if (newBullets.size() > 0 && (HostFuncRun || ClientFuncRun)) {
                mutexOnSend.lock();
                SendPacket << packetStates::Shooting << (sf::Int32)(newBullets.size());
                for (int i = 0; i < newBullets.size(); i++) {
                    SendPacket << *newBullets[i];
                }
                sendSendPacket();
                mutexOnSend.unlock();
            }

            updateBullets();
            mutexOnDataChange.unlock();

            if (HostFuncRun || ClientFuncRun) {
                mutexOnSend.lock();
                SendPacket << packetStates::PlayerPos << player;
                if (HostFuncRun) SendPacket << ConnectedPlayers;
                sendSendPacket();
                mutexOnSend.unlock();
            }

            if (MiniMapActivated) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    MiniMapView.move(-sf::Vector2f(sf::Mouse::getPosition() - MouseBuffer) * MiniMapZoom);
                }
            }
            MouseBuffer = sf::Mouse::getPosition();
        }

        draw();

        for (int i = 0; i < listOfFire.size(); i++) {
            if (listOfFire[i]->hitbox.intersect(player.hitbox))
                applyEffect(player, new Effect(Effects::Burn, std::vector<float>{5.f}, sf::seconds(5.f), sf::seconds(1.f)));
        }
        processEffects();

        if (CanSomethingBeActivated()) {
            addUI(&HUD::XButtonSprite, HUD::InterfaceStuff);
            addUI(&HUD::InfoLogoSprite, HUD::InterfaceStuff);
        } else {
            removeUI(&HUD::XButtonSprite, HUD::InterfaceStuff);
            removeUI(&HUD::InfoLogoSprite, HUD::InterfaceStuff);
        }

        EventHandler();

        TimeSinceLastFrame = GameClock->getElapsedTime() - GameTime;
        GameTime = GameClock->getElapsedTime();
    }
    saveGame();
}


//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect() {
    sf::TcpSocket* client = new sf::TcpSocket;
    if (listener.accept(*client) == sf::Socket::Done && CurLocation == &MainMenuLocation) {
        std::string ConnectedClientIP = (*client).getRemoteAddress().toString();

        for (int i = 0; i < clients.size(); i++) {
            if (ConnectedClientIP == clients[i]->getRemoteAddress().toString()) {
                ClientDisconnect(i);
            }
        }

        std::cout << ConnectedClientIP << " connected\n";
        mutexOnDataChange.lock();
        HUD::ListOfPlayers.addWord(ConnectedClientIP);
        mutexOnDataChange.unlock();
        std::cout << "list of players:\n" << HUD::ListOfPlayers.text.getText() << '\n';

        clients.push_back(client);
        selector.add(*client);

        mutexOnSend.lock();
        SendPacket << (sf::Int32)(ConnectedPlayers.size() + 1);
        for (int i = 0; i < ConnectedPlayers.size() + 1; i++) {
            SendPacket << HUD::ListOfPlayers[i];
        }

        std::cout << "amount players = " << ConnectedPlayers.size() + 1 << '\n';
        SendPacket << player << player.Name.getText() << player.Health << player.HealthRecovery;
        for (Player& p : ConnectedPlayers) {
            SendPacket << p << p.Name.getText() << p.Health << p.HealthRecovery;
        }

        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "SendPacket was sended\n";
        } else {
            std::cout << "SendPacket didn't sended\n";
        }

        SendPacket.clear();

        SendPacket << packetStates::Shooting << (sf::Int32)Bullets.size();
        std::cout << "bullets: " << Bullets.size() << "\n";
        for (int i = 0; i < Bullets.size(); i++) {
            SendPacket << *Bullets[i];
        }

        if (client->send(SendPacket) == sf::Socket::Done) {
            std::cout << "bullets was sended\n";
        } else {
            std::cout << "bullets didn't sended\n";
        }
        SendPacket.clear();
        mutexOnSend.unlock();

        mutexOnDataChange.lock();
        ConnectedPlayers.push_back(*(new Player()));
        ConnectedPlayers.back().setAnimation(Textures::Player, &Shaders::Player);
        DrawableStuff.push_back(&(ConnectedPlayers.back()));
        mutexOnDataChange.unlock();
    } else {
        delete client;
        std::cout << "Error: Unable to accept connection\n";
    }
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    std::cout << (*clients[i]).getRemoteAddress().toString() << " disconnected; number = " << i << "\n";
    DeletePointerFromVector(clients, i);

    mutexOnDataChange.lock();
    DeleteFromVector(DrawableStuff, (sf::Drawable*)&ConnectedPlayers[i]);
    ConnectedPlayers.erase(ConnectedPlayers.begin() + i);
    HUD::ListOfPlayers.removeWord(i);
    mutexOnDataChange.unlock();

    std::cout << "amount of clients = " << clients.size() << "\n";
    mutexOnSend.lock();
    SendPacket << packetStates::PlayerDisconnect << i;
    sendSendPacket();
    mutexOnSend.unlock();
}

void SelfDisconnect() {
    mutexOnDataChange.lock();
    std::cout << "SelfDisconnect\n";
    ComputerID = 0;
    LoadMainMenu();
    MySocket.disconnect();
    selector.clear();
    ClientFuncRun = false;
    for (Player& p : ConnectedPlayers) {
        DeleteFromVector(DrawableStuff, (sf::Drawable*)&p);
    }
    ConnectedPlayers.clear();
    mutexOnDataChange.unlock();
}

void SendToClients(sf::Packet& pac, int ExceptOf) {
    for (int i = 0; i < clients.size(); i++) {
        if (i != ExceptOf && clients[i]->send(pac) != sf::Socket::Done) {}
    }
    SendPacket.clear();
}

void funcOfHost() {
    std::cout << "Server turn on\n";
    while (HostFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(listener)) {
                ClientConnect();
            }
            for (int i = 0; i < clients.size(); i++) {
                if (selector.isReady(*clients[i]) && clients[i]->receive(ReceivePacket) == sf::Socket::Done) {
                    while (!ReceivePacket.endOfPacket()) {
                        ReceivePacket >> packetStates::curState;
                        switch (packetStates::curState) {
                            case packetStates::FirstConnect:
                                ReceivePacket >> sPacketData;
                                mutexOnDataChange.lock();
                                ReceivePacket >> ConnectedPlayers[i].Health >> ConnectedPlayers[i].HealthRecovery;
                                ConnectedPlayers[i].Name.setString(sPacketData);
                                mutexOnDataChange.unlock();
                                std::cout << "Connected " << sPacketData << " whith ID:" << i + 1 << '\n';
                                mutexOnSend.lock();
                                SendPacket << packetStates::PlayerConnect << sPacketData << player.Health << player.HealthRecovery;
                                SendToClients(SendPacket, i);
                                mutexOnSend.unlock();
                                break;
                            case packetStates::Disconnect:
                                std::cout << "client disconected\n";
                                ClientDisconnect(i--);
                                break;
                            case packetStates::PlayerPos:
                                mutexOnDataChange.lock();
                                ReceivePacket >> ConnectedPlayers[i];
                                mutexOnDataChange.unlock();
                                break;
                            case packetStates::ChatEvent:
                                ReceivePacket >> sPacketData;
                                chat.addLine(sPacketData);
                                mutexOnSend.lock();
                                SendPacket << packetStates::ChatEvent << sPacketData;
                                SendToClients(SendPacket, i);
                                mutexOnSend.unlock();
                                break;
                            case packetStates::Shooting:
                                ReceivePacket >> i32PacketData;
                                mutexOnDataChange.lock();
                                mutexOnSend.lock();
                                SendPacket << packetStates::Shooting << i32PacketData;
                                for (int j = 0; j < i32PacketData; j++) {
                                    Bullets.push_back(new Bullet());
                                    ReceivePacket >> *(Bullets.back());
                                    SendPacket << *(Bullets.back());
                                }
                                SendToClients(SendPacket, i);
                                mutexOnSend.unlock();
                                mutexOnDataChange.unlock();
                                break;
                            case packetStates::UseInteractable: {
                                DescriptionID::Type id;
                                ReceivePacket >> i32PacketData >> id >> V2fPacketData; i32PacketData--;
                                ReceivePacket >> ConnectedPlayers[i32PacketData].Health >> ConnectedPlayers[i32PacketData].HealthRecovery;
                                Interactable* x1 = nullptr;
                                mutexOnDataChange.lock();
                                for (Interactable*& x2: InteractableStuff) {
                                    if (x2->descriptionID == id && x2->hitbox.getPosition() == V2fPacketData) {
                                        x1 = x2;
                                        break;
                                    }
                                }
                                if (x1) {
                                    if      (id == DescriptionID::box     ) DeleteFromVector(listOfBox,      x1);
                                    else if (id == DescriptionID::artifact) DeleteFromVector(listOfArtifact, x1);
                                    DeleteFromVector(DrawableStuff, (sf::Drawable*)x1);
                                    DeleteFromVector(InteractableStuff, x1);
                                    delete x1;
                                }
                                mutexOnDataChange.unlock();
                                mutexOnSend.lock();
                                SendPacket << packetStates::UseInteractable << i32PacketData << id << V2fPacketData;
                                SendPacket << ConnectedPlayers[i32PacketData].Health << ConnectedPlayers[i32PacketData].HealthRecovery;
                                SendToClients(SendPacket, i32PacketData);
                                mutexOnSend.unlock();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "Server turn off\n";
}

void funcOfClient() {
    while (ClientFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(MySocket) && MySocket.receive(ReceivePacket) == sf::Socket::Done) {
                while (!ReceivePacket.endOfPacket()) {
                    ReceivePacket >> packetStates::curState;
                    switch (packetStates::curState) {
                        case packetStates::Disconnect:
                            SelfDisconnect();
                            break;
                        case packetStates::PlayerConnect:
                            ReceivePacket >> sPacketData;
                            mutexOnDataChange.lock();
                            HUD::ListOfPlayers.addWord(sPacketData);
                            ConnectedPlayers.push_back(*(new Player()));
                            ConnectedPlayers.back().setAnimation(Textures::Player, &Shaders::Player);
                            ConnectedPlayers.back().Name.setString(sPacketData);
                            DrawableStuff.push_back(&(ConnectedPlayers.back()));
                            mutexOnDataChange.unlock();
                            std::cout << sPacketData + " connected\n";
                            break;
                        case packetStates::PlayerDisconnect:
                            int index;
                            ReceivePacket >> index;
                            std::cout << std::string(HUD::ListOfPlayers[index]) << " disconnected\n";
                            mutexOnDataChange.lock();
                            if (index < ComputerID) ComputerID--;
                            HUD::ListOfPlayers.removeWord(index);
                            ConnectedPlayers.erase(ConnectedPlayers.begin() + index);
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::Labyrinth:
                            mutexOnDataChange.lock();
                            clearVectorOfPointers(PickupStuff);
                            clearVectorOfPointers(Bullets);

                            DrawableStuff.clear();
                            HUD::InterfaceStuff.clear();
                            InteractableStuff.clear();

                            MiniMapActivated = false;
                            HUD::EscapeMenuActivated = false;

                            MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                            MiniMapView.setCenter(player.getCenter() * ScaleParam);

                            Musics::MainMenu.pause();
                            if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
                                Musics::Fight1.play();
                            }

                            if (CurLocation != &LabyrinthLocation) {
                                CurLocation = &LabyrinthLocation;
                                CollisionShapes.clear();
                                for (CollisionShape &wall : CurLocation->wallsRect) {
                                    CollisionShapes.push_back(&wall);
                                }
                            } else {
                                completedLevels = std::max(curLevel, completedLevels);
                                curLevel++;
                            }
                            MiniMapView.zoom(1 / MiniMapZoom);
                            MiniMapZoom = std::pow(1.1, -10);
                            MiniMapView.zoom(MiniMapZoom);
                            ReceivePacket >> &LabyrinthLocation;
                            // FindAllWaysTo(CurLocation, player.getCenter(), TheWayToPlayer);
                            ReceivePacket >> i32PacketData; clearVectorOfPointers(Enemies);
                            for (int i = 0; i < i32PacketData; i++) {
                                ReceivePacket >> sPacketData;
                                if (sPacketData == "Distorted Scientist") Enemies.push_back(new DistortedScientist());
                                else if (sPacketData == "Distorted")      Enemies.push_back(new Distorted());
                                ReceivePacket >> Enemies[i];
                                DrawableStuff.push_back(Enemies[i]);
                            }
                            ReceivePacket >> i32PacketData;
                            clearVectorOfPointers(listOfBox);
                            clearVectorOfPointers(listOfArtifact);
                            clearVectorOfPointers(listOfFire);
                            for (int i = 0; i < i32PacketData; i++) {
                                Interactable* x = new Interactable();
                                ReceivePacket >> x;
                                setInteractable(x);
                                placeOnMap(x);
                            }
                            ReceivePacket >> &portal >> player;

                            CurLocation->FindEnableTilesFrom(player.getCenter() / (float)size);

                            placeOnMap(&portal);

                            addUI(&HUD::HUDFrame, HUD::InterfaceStuff);
                            for (int i = 0; i < HUD::WeaponNameTexts.size(); i++) {
                                HUD::InterfaceStuff.push_back(HUD::WeaponNameTexts[i]);
                            }
                            HUD::InterfaceStuff.push_back(&chat);
                            DrawableStuff.push_back(&player);
                            for (Player& p : ConnectedPlayers) {
                                DrawableStuff.push_back(&p);
                            }
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::PlayerPos:
                            mutexOnDataChange.lock();
                            for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                                if (i != ComputerID) {
                                    ReceivePacket >> ConnectedPlayers[i - k];
                                } else {
                                    sf::Vector2i tempPoint;
                                    ReceivePacket >> tempPoint;
                                    k++;
                                }
                            }
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::SetPos:
                            mutexOnDataChange.lock();
                            for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                                if (i != ComputerID) {
                                    ReceivePacket >> ConnectedPlayers[i - k];
                                } else {
                                    ReceivePacket >> player;
                                    k++;
                                }
                            }
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::ChatEvent:
                            ReceivePacket >> sPacketData;
                            chat.addLine(sPacketData);
                            break;
                        case packetStates::Shooting:
                            ReceivePacket >> i32PacketData;
                            mutexOnDataChange.lock();
                            for (int i = 0; i < i32PacketData; i++) {
                                Bullets.push_back(new Bullet());
                                ReceivePacket >> *(Bullets.back());
                            }
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::UseInteractable: {
                            DescriptionID::Type id;
                            ReceivePacket >> i32PacketData >> id >> V2fPacketData;
                            i32PacketData -= i32PacketData > ComputerID;
                            Interactable* x1 = nullptr;
                            mutexOnDataChange.lock();
                            ReceivePacket >> ConnectedPlayers[i32PacketData].Health >> ConnectedPlayers[i32PacketData].HealthRecovery;
                            for (Interactable*& x2: InteractableStuff) {
                                if (x2->descriptionID == id && x2->hitbox.getPosition() == V2fPacketData) {
                                    x1 = x2;
                                    break;
                                }
                            }
                            if (x1) {
                                if      (id == DescriptionID::box     ) DeleteFromVector(listOfBox,      x1);
                                else if (id == DescriptionID::artifact) DeleteFromVector(listOfArtifact, x1);
                                DeleteFromVector(DrawableStuff, (sf::Drawable*)x1);
                                DeleteFromVector(InteractableStuff, x1);
                                delete x1;
                            }
                            mutexOnDataChange.unlock();
                            break;
                        }
                        case packetStates::EnemiesPos:
                            mutexOnDataChange.lock();
                            for (Enemy*& enemy: Enemies) {
                                ReceivePacket >> sPacketData;
                                ReceivePacket >> enemy;
                            }
                            mutexOnDataChange.unlock();
                            break;
                        case packetStates::EnemyDie:
                            ReceivePacket >> i32PacketData;
                            EnemyDie(i32PacketData);
                            break;
                        case packetStates::AllPlayerDie:
                            mutexOnDataChange.lock();
                            LoadMainMenu();
                            player.Health.cur = player.Health.top;
                            for (Player& p: ConnectedPlayers) {
                                p.Health.cur = p.Health.top;
                            }
                            mutexOnDataChange.unlock();
                            break;
                    }
                }
            }
        }
    }
}

void sendSendPacket() {
    if (HostFuncRun) {
        SendToClients(SendPacket);
    } else if (ClientFuncRun) {
        MySocket.send(SendPacket);
    }
    SendPacket.clear();
}