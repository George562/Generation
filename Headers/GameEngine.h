#pragma once
#include "Entities/enemy.h"
#include "Entities/player.h"
#include "Multiplayer/chat.h"
#include "UI/PolygonButton.h"
#include "UI/panel.h"
#include "UI/bar.h"
#include "UI/tempText.h"
#include "UI/Slot.h"
#include "UI/Frame.h"
#include "Systems/effect.h"
#include "Systems/shop.h"
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
sf::Mutex mutex, mutexOnDraw;
bool ClientFuncRun, HostFuncRun;
bool Connecting = false;
std::regex regexOfIP("\\d+.\\d+.\\d+.\\d+");


//////////////////////////////////////////////////////////// Interactables
Interactable portal(DescriptionID::portal),
             puddle(DescriptionID::puddle),
             shopSector(DescriptionID::shopSector),
             upgradeSector(DescriptionID::upgradeSector);
std::vector<Interactable*> listOfBox,
                           listOfArtifact,
                           listOfFire;


//////////////////////////////////////////////////////////// MiniMapStuff
sf::CircleShape MMPlayerCircle, MMEnemyCircle; // MM - MiniMap prefix
sf::RectangleShape MMPortalRect, MMBoxRect, MMPuddleRect, MMArtifact;


//////////////////////////////////////////////////////////// Locations
Location* CurLocation = nullptr;
Location LabyrinthLocation, WaitingRoomLoaction, MainMenuLocation;

void loadLocations() {
    WaitingRoomLoaction.LoadFromFile("sources/locations/WaitingRoom.txt");
    MainMenuLocation.LoadFromFile("sources/locations/MainMenu.txt");
}


//////////////////////////////////////////////////////////// Chat
Chat chat(scw, sch);


//////////////////////////////////////////////////////////// Other stuff
sf::Vector2i MouseBuffer;


//////////////////////////////////////////////////////////// Enemies
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

void setBox(Interactable*&);
void setArtifact(Interactable*&);
void setFire(Interactable*&);
//----------------------------


//---------------------------- GAME STATE FUNCTIONS
void updateBullets();
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
    coutClock = new sf::Clock;

    loadLocations();
    loadTextures();
    loadItemTextures();
    loadFonts();
    loadShaders();
    loadMusics();
    loadSave();
    loadDescriptions();

    icon.loadFromFile("sources/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    playerCanDashing = player.inventory.find(ItemID::dasher) != -1;

    Musics::MainMenu.setLoop(true);
    Musics::MainMenu.setVolume(5);
    Musics::Fight1.setVolume(5);
    Musics::Fight2.setVolume(5);

    portal.setAnimation(Textures::Portal, 9, 1, sf::seconds(1), &Shaders::Portal);
    portal.setSize(170.f, 320.f);
    player.setAnimation(Textures::Player, &Shaders::Player);
    puddle.setAnimation(Textures::Puddle);
    puddle.setSize(90.f, 90.f);
    shopSector.setAnimation(Textures::INVISIBLE);
    shopSector.setPosition(0, 2 * size);

    upgradeSector.setAnimation(Textures::INVISIBLE);
    upgradeSector.setPosition(6 * size, 2 * size);

    Shaders::Flashlight.setUniform("uResolution", sf::Vector2f(scw, sch));
    Shaders::Flashlight.setUniform("u_playerRadius", player.hitbox.getRadius());

    Shaders::Distortion1.setUniform("noise_png", Textures::Noise);

    Shaders::Distortion2.setUniform("noise_png", Textures::Noise);

    Shaders::Outline.setUniform("uResolution", sf::Vector2f(scw, sch));

    Shaders::Bullet.setUniform("noise_png", Textures::Noise);

    Shaders::Fire.setUniform("noise_png", Textures::Noise);

    {
        using namespace HUD;
        ListOfPlayers.setTexture(Textures::GradientFrameAlpha);

        EscapeButton.setTexture(Textures::RedPanel, Textures::RedPanelPushed, UI::texture);
        EscapeButton.setHitboxPoints({ EscapeButton.getLeftTop(), EscapeButton.getRightTop(),
                                       EscapeButton.getRightBottom(), EscapeButton.getLeftBottom() });
    }

    CurWeapon.looped = true;

    listener.setBlocking(false);
    MyIP = MySocket.getRemoteAddress().getPublicAddress().toString();
    std::cout << "LocalAddress: " << MySocket.getRemoteAddress().getLocalAddress().toString() << "\n";
    std::cout << "PublicAddress: " << MyIP << '\n';

    CurWeapon = { {0, (int)Weapons.size() - 1, 0} };

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

    MMPuddleRect.setSize(puddle.hitbox.getSize() * ScaleParam);
    MMPuddleRect.setFillColor(sf::Color(0, 0, 255, 200));

    MMArtifact.setSize(sf::Vector2f(150.f, 105.f) * ScaleParam);
    MMArtifact.setFillColor(sf::Color::White);
}

void initScripts() {
    {
        using namespace upgradeInterface;
        switchGunLBtn.setFunction([](){
            CurWeapon.cur = (CurWeapon.cur - 1 + Weapons.size()) % Weapons.size();
            player.CurWeapon = Weapons[CurWeapon.cur];
        });
        switchGunRBtn.setFunction([](){
            CurWeapon.cur = (CurWeapon.cur + 1) % Weapons.size();
            player.CurWeapon = Weapons[CurWeapon.cur];
        });
    }

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

                    playerCoinsText.setString("You have: " + std::to_string(player.inventory.money));
                    playerCoinsSprite.parentTo(&MenuShop::playerCoinsText, true, { 25, -10 });
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
                NPCText.parentTo(&NPCTextFrame, true);
            }
        });
    }
    HUD::EscapeButton.setFunction([]() {
        HUD::EscapeMenuActivated = false;
        HUD::ListOfPlayers.clearText();
        clearVectorOfPointer(Bullets);
        clearVectorOfPointer(Enemies);
        clearVectorOfPointer(TempTextsOnGround);
        clearVectorOfPointer(HUD::TempTextsOnScreen);
        clearVectorOfPointer(DamageText);
        clearVectorOfPointer(HUD::MessageText);
        clearVectorOfPointer(listOfBox);
        clearVectorOfPointer(listOfArtifact);
        clearVectorOfPointer(listOfFire);
        clearVectorOfPointer(PickupStuff);
        player.CurWeapon->lock = true;
        LoadMainMenu();
        saveGame();
    });

    chat.SetCommand("/?", []{
        chat.addLine("/? - info");
        chat.addLine("/server on - start server");
        chat.addLine("/server off - close server");
        chat.addLine("/connect - connect to other");
        chat.addLine("/disconnect - disconnect from server");
    });
    chat.SetCommand("/server on", []{
        if (!HostFuncRun) {
            listener.listen(53000);
            selector.add(listener);
            HUD::ListOfPlayers.setString(MyIP);
            ComputerID = 0;
            HostFuncRun = true;
            HostTread.launch();
            chat.addLine("Server is running! Your IP: " + MyIP);
        } else {
            chat.addLine("Server is already running! Your IP: " + MyIP);
        }
    });
    chat.SetCommand("/server off", []{
        if (HostFuncRun) {
            mutex.lock();
            SendPacket << packetStates::Disconnect;
            SendToClients(SendPacket);
            SendPacket.clear();
            mutex.unlock();
            clients.clear();
            selector.clear();
            listener.close();
            HostFuncRun = false;
            for (int i = 0; i < ConnectedPlayers.size(); i++) {
                DeleteFromVector(DrawableStuff, (sf::Drawable*)&ConnectedPlayers[i]);
            }
            ConnectedPlayers.clear();
            chat.addLine("Server is closed!");
        } else {
            chat.addLine("Server is not running!");
        }
    });
    chat.SetCommand("/connect", []{
        if (!ClientFuncRun) {
            Connecting = true;
            chat.addLine("input IP of host");
        }
    });
    chat.SetCommand("/disconnect", []{
        if (ClientFuncRun) {
            SelfDisconnect();
        }
    });
}
//==============================================================================================



//============================================================================================== DRAW FUNCTIONS
void draw() {
    mutexOnDraw.lock();
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
            EnemyHealthBar.setPosition(enemy->hitbox.getCenter() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, enemy->hitbox.getRadius() + 50.f));
            EnemyHealthBar.setValue(enemy->Health);
            preRenderTexture.draw(EnemyHealthBar);
        }

        for (Player& p : ConnectedPlayers) {
            EnemyHealthBar.setPosition(p.hitbox.getCenter() - sf::Vector2f(EnemyHealthBar.getSize().x / 2.f, p.hitbox.getRadius() + 50.f));
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
            if (TempTextsOnGround[i]->localClock->getElapsedTime() < TempTextsOnGround[i]->howLongToExist) {
                window.draw(*TempTextsOnGround[i]);
            } else {
                DeletePointerFromVector(TempTextsOnGround, i--);
            }
        }

        for (size_t i = 0; i < DamageText.size(); i++) {
            if (DamageText[i]->localClock->getElapsedTime() < DamageText[i]->howLongToExist) {
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
    mutexOnDraw.unlock();
}

void drawFloor() {
    for (int i = 0; i < CurLocation->n; i++) {
        for (int j = 0; j < CurLocation->m; j++) {
            if (CurLocation->EnableTiles[i][j]) {
                FLoorTileSprite.setPosition(size * j, size * i);
                preRenderTexture.draw(FLoorTileSprite);
                if (CurLocation->getPassagesAmount(j, i) > 2) {
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
    for (int i = std::max(0, 2 * int((CameraPos.y - WallMinSize / 2) / size));
         i <= std::min(int(CurLocation->walls.size() - 1), 2 * int((CameraPos.y + sch + WallMinSize / 2) / size) + 1); i++) {
        for (int j = std::max(0, int(CameraPos.x / size));
             j <= std::min(int(CurLocation->walls[i].size() - 1), int((CameraPos.x + scw + WallMinSize) / size)); j++) {
            if (CurLocation->walls[i][j]) {
                CurLocation->SeenWalls[i][j] = CurLocation->SeenWalls[i][j] || CameraRect.intersect(CurLocation->wallsRect[i][j]);
                WallRect.setPosition(CurLocation->wallsRect[i][j].getPosition());
                WallRect.setTexture((i % 2 == 1) ? Textures::WallV : Textures::WallG, true);
                preRenderTexture.draw(WallRect);
            }
        }
    }
}

void drawMiniMap() {
    if (MiniMapHoldOnPlayer) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);
        }
    }

    // draw walls
    window.setView(MiniMapView);
    sf::VertexArray line(sf::Lines, 2);
    for (int i = 0; i < CurLocation->walls.size(); i++) {
        for (int j = 0; j < CurLocation->walls[i].size(); j++) {
            if (CurLocation->walls[i][j] && CurLocation->SeenWalls[i][j]) {
                if (i % 2 == 1) { // |
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i - 1) / 2), sf::Color::White);
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * (i + 1) / 2), sf::Color::White);
                } else { // -
                    line[0] = sf::Vertex(sf::Vector2f(miniSize * j, miniSize * i / 2));
                    line[1] = sf::Vertex(sf::Vector2f(miniSize * (j + 1), miniSize * i / 2));
                }
                window.draw(line);
            }
        }
    }

    MMPortalRect.setPosition(portal.hitbox.getPosition() * ScaleParam);
    window.draw(MMPortalRect);

    for (Interactable*& i : listOfBox) {
        MMBoxRect.setPosition(i->hitbox.getPosition() * ScaleParam);
        window.draw(MMBoxRect);
    }

    MMPuddleRect.setPosition(puddle.hitbox.getPosition() * ScaleParam);
    window.draw(MMPuddleRect);

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
    MMPlayerCircle.setPosition(player.hitbox.getCenter() * ScaleParam);
    window.draw(MMPlayerCircle);
    window.setView(GameView);
}
//==============================================================================================



//============================================================================================== UI UPDATERS/CREATORS
void updateInventoryUI() {
    {
        using namespace inventoryInterface;

        if (doInventoryUpdate[inventoryPage::Items]) {
            int slotNumber = 0;
            for (Item*& drawnItem : player.inventory.items) {
                drawnItem->animation->setScale({ 0.75, 0.75 });

                float itemX = (slotNumber % 6) * 200 + itemListBG.getPosition().x + 50;
                float itemY = (slotNumber / 6) * 200 + itemListBG.getPosition().y + 50;

                ItemSlot* slot = &itemSlotsElements[drawnItem->id];
                if (!slot->isInitialized) {
                    slot->init("inv_ItemIDSlot" + drawnItem->id);
                }
                slot->setSize({ 150, 150 });
                slot->setTexture(Textures::ItemPanel, UI::element);
                slot->setPosition(itemX, itemY);

                drawnItem->setPosition(itemX, itemY);

                slot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));

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
    }
}

void updateShopUI() {
    {
        using namespace MenuShop;
        int slotNumber = 0;
        for (Item*& drawnItem : shop.soldItems.items) {
            drawnItem->animation->setScale({ 0.5, 0.5 });

            float itemX = (slotNumber % 5) * 200 + 30;
            float itemY = (slotNumber / 5) * 200 + 20;

            ShopSlot* slot = &slotsElements[drawnItem->id];
            if (!slotsElements[drawnItem->id].isInitialized) {
                slot->init("mShop_ItemIDSlot" + drawnItem->id);
            }
            slot->setSize({ 100, 100 });
            slot->setTexture(Textures::ItemPanel, UI::element);
            slot->setPosition(itemX, itemY);

            drawnItem->setPosition(itemX, itemY);

            slot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));

            PlacedText& itemPriceText = *slot->priceText;
            itemPriceText.setFontString(
                FontString(std::to_string(shop.itemPrices[drawnItem->id]) + " C", 20)
            );

            slotNumber++;
        }

        slotNumber = 0;
        for (Item*& drawnItem : player.inventory.items) {
            drawnItem->animation->setScale({ 0.5, 0.5 });

            float itemX = (slotNumber % 3) * 200 + 30;
            float itemY = (slotNumber / 3) * 200 + 20;

            ShopSlot* pslot = &playerSlotsElements[drawnItem->id];
            if (!pslot->isInitialized) {
                pslot->init("mShop_PlItemIDSlot" + drawnItem->id);
            }
            pslot->setSize({ 100, 100 });
            pslot->setTexture(Textures::ItemPanel, UI::element);
            pslot->setPosition(itemX, itemY);

            drawnItem->setPosition(itemX, itemY);

            pslot->amountText->setFontString(FontString(std::to_string(drawnItem->amount), 20));

            pslot->priceText->setFontString(
                FontString(std::to_string(shop.itemPrices[drawnItem->id]) + " C", 20)
            );

            slotNumber++;
        }
    }
}

void updateUpgradeInterfaceUI() {
    {
        using namespace upgradeInterface;
        updateUpgradeTexts();
        updateUpgradeShopStats();
        playerCoinAmount.setFontString(FontString("You have: " + std::to_string(player.inventory.money),
                                                  50, sf::Color(200, 200, 200)));
        playerCoinAmount.parentTo(&choiceComp, true, { -100, -50 });
        coinSprite.parentTo(&playerCoinAmount, true);
    }
}
//==============================================================================================

sf::Clock SHiftClickTime;
//============================================================================================== EVENT HANDLERS
void EventHandler() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (Connecting) {
            if (keyPressed(event, sf::Keyboard::Escape)) {
                Connecting = false;
            }
        }
        if (chat.InputText(event)) {
            if (keyPressed(event, sf::Keyboard::Enter)) {
                if (Connecting) {
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
                                std::cout << sPacketData << '\n';
                            }
                        }

                        SendPacket << packetStates::FirstConnect << player.Name.getText();
                        SendPacket << player.Health << player.HealthRecovery;
                        MySocket.send(SendPacket);
                        SendPacket.clear();

                        ClientFuncRun = true;
                        ClientTread.launch();
                        Connecting = false;

                        chat.addLine("done");
                    } else {
                        chat.addLine("Failed to connect to server");
                    }
                }
                mutex.lock();
                SendPacket << packetStates::ChatEvent << chat.Last();
                if (HostFuncRun) {
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }
        } else if (HUD::EscapeMenuActivated) {
            HUD::EscapeButton.isActivated(event);
            if (keyPressed(event, sf::Keyboard::Escape)) {
                HUD::EscapeMenuActivated ^= true;
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
                    }
                    if (HUD::showDiscriptions) {
                        HUD::showDiscriptions = false;
                        continue;
                    }
                }
                if (event.key.code == sf::Keyboard::R) {
                    player.CurWeapon->HolsterAction();
                }
                if (event.key.code == sf::Keyboard::M) {
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
                if (event.key.code == sf::Keyboard::Tab) {
                    {
                        using namespace inventoryInterface;
                        isDrawInventory = true;
                        frameButtonsHolder.activateButton(activePage);
                        doInventoryUpdate[activePage] = true;
                    }
                }
                if (event.key.code == sf::Keyboard::LShift) {
                    player.makeADash = SHiftClickTime.restart().asSeconds() < 0.2f && playerCanDashing;
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

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (!MiniMapActivated) {
                    CurWeapon -= (int)event.mouseWheelScroll.delta;
                    player.ChangeWeapon(Weapons[CurWeapon.cur]);

                    std::string reloadStr = player.CurWeapon->Name + " is out of ammo!";
                    HUD::ReloadWeaponText.setString(reloadStr);
                    HUD::ReloadWeaponText.setCenter(sf::Vector2f(scw / 2, sch / 4));
                }
            }

            if (player.CurWeapon != nullptr && !MiniMapActivated) {
                player.CurWeapon->Update(event);
            }

            if (CurLocation == &MainMenuLocation) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        Musics::MainMenu.pause();
                        window.close();
                        HUD::EscapeButton.buttonFunction();
                        if (ClientFuncRun) {
                            SelfDisconnect();
                        } else if (HostFuncRun) {
                            chat.commands["/server off"];
                        }
                        return;
                    }
                }
            } else if (CurLocation == &LabyrinthLocation) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Escape) {
                        HUD::EscapeMenuActivated = true;
                    } else if (event.key.code == sf::Keyboard::H) {
                        player.hitbox.setCenter(size, size);
                        CurLocation = &WaitingRoomLoaction;
                    }
                }
            }
        }
    }
}

void inventoryHandler(sf::Event& event) {
    {
        using namespace inventoryInterface;

        if (keyPressed(event, sf::Keyboard::Escape)) {
            if (upgradeInterface::isChoosingComponent) {
                upgradeInterface::isChoosingComponent = false;
                return;
            }
            backButton.buttonFunction();
            return;
        }

        if (!upgradeInterface::isChoosingComponent) {
            backButton.isActivated(event);
            frameButtonsHolder.isButtonsActive(event);
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
            upgradeInterfaceHandler(event);
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
            playerCanDashing = player.inventory.find(ItemID::dasher) != -1;
        }

        if (mouseButtonPressed(event, sf::Mouse::Left)) {

            window.setView(ShopStockView);
            sf::Vector2f viewPos = stockTransform.getInverse().
                transformPoint(window.mapPixelToCoords(sf::Mouse::getPosition()));
                for (Item*& item : shop.soldItems.items) {
                    if (slotsElements[item->id].isInitialized && slotsElements[item->id].contains(viewPos)) {

                        selectedItem = item;
                        itemSlot.priceText->setString(std::string(std::to_string(shop.itemPrices[item->id]) + " C"));
                        itemSlot.priceText->setCenter(itemSlot.getCenter().x, itemSlot.priceText->getCenter().y);
                        itemCoinsSprite.parentTo(itemSlot.priceText, true, { 20, -15 });
                        itemCoinsSprite.parentTo(&itemSlot);
                        itemStatsText.setString(textWrap(itemDesc[item->id], 65));
                        itemSprite.setTexture(*itemTexture[item->id], UI::texture);
                        itemSprite.parentTo(&itemSlot, true);

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
void setBox(Interactable*& box) {
    box->setAnimation(Textures::Box);
    box->setSize(105.f, 117.f);
    box->descriptionID = DescriptionID::box;
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
                mutex.lock();
                SendPacket << packetStates::UseInteractable << ComputerID << DescriptionID::box << i;
                if (ClientFuncRun) MySocket.send(SendPacket);
                else               SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
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
    artifact->descriptionID = DescriptionID::artifact;
    artifact->setFunction([](Interactable* i) {
        std::rand(); int r = std::rand() % artifactEffects.size();
        artifactEffects[r]();
        
        addMessageText(artifactText[r], artifactColors[r]);
        DeleteFromVector(listOfArtifact, i);
        DeleteFromVector(DrawableStuff, (sf::Drawable*)i);
        DeleteFromVector(InteractableStuff, i);

        if (ClientFuncRun || HostFuncRun) {
            mutex.lock();
            SendPacket << packetStates::UseInteractable << ComputerID << DescriptionID::artifact << i;
            SendPacket << player.Health << player.HealthRecovery;
            if (ClientFuncRun) MySocket.send(SendPacket);
            else               SendToClients(SendPacket);
            SendPacket.clear();
            mutex.unlock();
        }
        delete i;
    });
}

void setFire(Interactable*& fire) {
    fire->setAnimation(Textures::Fire, &Shaders::Fire);
    fire->setSize(70.f, 70.f);
}

void placedOnMap(Interactable*& i, int& m, int& n) {
    int x, y, posX = WallMinSize + std::rand() % int(size - i->hitbox.getSize().x - WallMinSize),
              posY = WallMinSize + std::rand() % int(size - i->hitbox.getSize().y - WallMinSize);
    do {
        x = std::rand() % m;
        y = std::rand() % n;
    } while (!LabyrinthLocation.EnableTiles[y][x]);
    i->setPosition(sf::Vector2f(x, y) * (float)size + sf::Vector2f(posX, posY));

    InteractableStuff.push_back(i);
    DrawableStuff.push_back(i);
}
void placedOnMap(Interactable*& i, float x, float y) {
    i->setPosition(x, y);
    InteractableStuff.push_back(i);
    DrawableStuff.push_back(i);
}
void placedOnMap(Interactable*& i, sf::Vector2f v) {
    i->setPosition(v);
    InteractableStuff.push_back(i);
    DrawableStuff.push_back(i);
}
void placedOnMap(Interactable* i) {
    InteractableStuff.push_back(i);
    DrawableStuff.push_back(i);
}

void LevelGenerate(int n, int m) {
    MiniMapView.zoom(1 / MiniMapZoom);
    MiniMapZoom = std::pow(1.1, -10);
    MiniMapView.zoom(MiniMapZoom);

    LabyrinthLocation.GenerateLocation(n, m, player.hitbox.getCenter() / float(size));

    portal.setCenter(player.hitbox.getCenter());
    puddle.setCenter(player.hitbox.getCenter() + sf::Vector2f(size, size));

    clearVectorOfPointer(listOfBox);
    for (int i = 0; i < 10; i++) {
        listOfBox.push_back(new Interactable());
        setBox(listOfBox[i]);
        placedOnMap(listOfBox[i], m, n);
    }

    clearVectorOfPointer(listOfArtifact);
    for (int i = 0; i < 10; i++) {
        listOfArtifact.push_back(new Interactable());
        setArtifact(listOfArtifact[i]);
        placedOnMap(listOfArtifact[i], m, n);
    }

    clearVectorOfPointer(listOfFire);
    for (int i = 0; i < 2; i++) {
        listOfFire.push_back(new Interactable());
        setFire(listOfFire[i]);
        placedOnMap(listOfFire[i], m, n);
    }

    clearVectorOfPointer(Enemies);
    int amountOfEveryEnemiesOnLevel = curLevel > completedLevels ? 4 : 2;
    for (int i = 0; i < amountOfEveryEnemiesOnLevel; i++) {
        Enemies.push_back(new DistortedScientist());
        Enemies.push_back(new Distorted());
    }

    for (int i = 0; i < Enemies.size(); i++) {
        do {
            Enemies[i]->hitbox.setPosition(sf::Vector2f((std::rand() % m) + 0.5f, (std::rand() % n) + 0.5f) * (float)size);
        } while (!LabyrinthLocation.EnableTiles[(int)Enemies[i]->hitbox.getPosition().y / size][(int)Enemies[i]->hitbox.getPosition().x / size] ||
                 distance(Enemies[i]->hitbox.getPosition(), player.hitbox.getCenter()) < size * 3);
    }
}

void LoadMainMenu() {
    CurLocation = &MainMenuLocation;

    player.hitbox.setCenter(3.5f * size, 2.5f * size);
    FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);
    player.ChangeWeapon(Weapons[CurWeapon.cur]);

    portal.setPosition(1612.5, 1545);
    puddle.setPosition(1012.5, 1545);

    CurLocation->FindEnableTilesFrom(player.hitbox.getCenter() / (float)size);

    portal.setFunction([](Interactable* i) {
        if (ClientFuncRun) {
            addMessageText("Host must start next level", sf::Color::Black);
        } else {
            clearVectorOfPointer(PickupStuff);
            clearVectorOfPointer(Bullets);

            DrawableStuff.clear();
            HUD::InterfaceStuff.clear();
            InteractableStuff.clear();
            removeUI(&HUD::HUDFrame, HUD::InterfaceStuff);

            player.hitbox.setCenter((START_M / 2 + 0.5f) * size, (START_N / 2 + 0.5f) * size);

            MiniMapActivated = false;
            HUD::EscapeMenuActivated = false;

            MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
            MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);

            Musics::MainMenu.pause();
            if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
                Musics::Fight1.play();
            }
            if (CurLocation != &LabyrinthLocation) {
                CurLocation = &LabyrinthLocation;
            } else {
                completedLevels = std::max(curLevel, completedLevels);
                curLevel++;
            }
            LevelGenerate(START_N + curLevel, START_M + curLevel * 2);
            FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);

            DrawableStuff.push_back(&player);
            DrawableStuff.push_back(&portal);
            for (Enemy*& enemy : Enemies) {
                DrawableStuff.push_back(enemy);
            }

            addUI(&HUD::HUDFrame, HUD::InterfaceStuff);
            for (int i = 0; i < HUD::WeaponNameTexts.size(); i++) {
                HUD::InterfaceStuff.push_back(HUD::WeaponNameTexts[i]);
            }
            HUD::InterfaceStuff.push_back(&chat);

            placedOnMap(&puddle);
            saveGame();

            if (HostFuncRun) {
                mutex.lock();
                SendPacket << packetStates::Labyrinth << CurLocation;
                SendPacket << (sf::Int32)Enemies.size() << Enemies;
                SendPacket << (sf::Int32)listOfBox.size() << listOfBox;
                SendPacket << (sf::Int32)listOfArtifact.size() << listOfArtifact;
                SendPacket << (sf::Int32)listOfFire.size() << listOfFire;
                SendPacket << &portal << &puddle << player;
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
                for (Player& player : ConnectedPlayers) {
                    DrawableStuff.push_back(&player);
                }
            }
        }
    });

    puddle.setFunction([](Interactable* i) {
        player.getDamage(5.f);
    });

    shopSector.setFunction([](Interactable* i) {
        {
            using namespace MenuShop;
            isDrawShop = true;
            playerCoinsText.setString("You have: " + std::to_string(player.inventory.money));
            playerCoinsSprite.parentTo(&playerCoinsText, true, { 25, -10 });
            NPCText.setString(textWrap("Hello! Welcome to the \"We are literally standing near a phenomenon beyond our"
                                       " entire plane of existence in a literal motherfucking sense, that we have no fucking clue on"
                                       " how it fucking works why did we decide it was a good idea to station a shop exactly here\?!\?\?!\?!\?\" shop!", 94));
            NPCText.parentTo(&NPCTextFrame, true);
            addUI(&BG, UIElements);
            if (!selectedItem)
                removeUI(&itemSlot, UIElements);
        }
    });

    upgradeSector.setFunction([](Interactable* i) {
        upgradeInterface::isDrawUpgradeInterface = true;
        openUpgradeShop();
    });

    // Set cameras
    GameView.setCenter(player.hitbox.getCenter());
    MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);
    HUDView.setCenter({ scw / 2.f, sch / 2.f });

    Musics::Fight1.stop();
    Musics::Fight2.stop();
    Musics::MainMenu.play();

    DrawableStuff.clear();
    DrawableStuff.push_back(&player);

    HUD::InterfaceStuff.clear();
    HUD::InterfaceStuff.push_back(&chat);
    addUI(&HUD::HUDFrame, HUD::InterfaceStuff);

    InteractableStuff.clear();
    InteractableStuff.push_back(&shopSector);
    InteractableStuff.push_back(&upgradeSector);

    placedOnMap(&portal);
    placedOnMap(&puddle);

    Item* newItem = new Item(ItemID::regenDrug, 1);
    newItem->setAnimation(*itemTexture[ItemID::regenDrug]);
    PickupStuff.push_back(newItem);
    DrawableStuff.push_back(PickupStuff[0]);
    PickupStuff[0]->dropTo(player.hitbox.getCenter() + sf::Vector2f(100, 100));

    listOfBox.push_back(new Interactable()); setBox(listOfBox[0]);
    listOfBox[0]->setPosition(1912.5, 1545);
    placedOnMap(listOfBox[0]);

    listOfArtifact.push_back(new Interactable()); setArtifact(listOfArtifact[0]);
    listOfArtifact[0]->setPosition(1312.5, 1545);
    placedOnMap(listOfArtifact[0]);
}
//==============================================================================================



//============================================================================================== GAME STATE FUNCTIONS
void updateBullets() {
    for (int i = 0; i < Bullets.size(); i++) {
        if (Bullets[i]->penetration < 0 || Bullets[i]->todel) {
            DeletePointerFromVector(Bullets, i--);
        } else {
            Bullets[i]->move(CurLocation);
            if (!faction::friends(Bullets[i]->fromWho, player.faction) && player.hitbox.intersect(Bullets[i]->hitbox)) {
                player.getDamage(Bullets[i]->damage);
                Bullets[i]->penetration--;
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
}

void updateEnemies() {
    for (int i = 0; i < Enemies.size(); i++) {
        if (Enemies[i]->Health.toBottom() == 0) {
            if (Enemies[i]->dropInventory) {
                for (Item*& item : Enemies[i]->inventory.items) {
                    item->dropTo(Enemies[i]->hitbox.getPosition());

                    PickupStuff.push_back(item);
                    DrawableStuff.push_back(item);
                }
            }
            Enemies[i]->inventory.items.clear();

            DeleteFromVector(DrawableStuff, static_cast<sf::Drawable*>(Enemies[i]));
            DeletePointerFromVector(Enemies, i--);

            if (Enemies.size() == 0) {
                TempText* enemiesKilledText = new TempText(sf::seconds(10));
                enemiesKilledText->setCharacterSize(40);
                enemiesKilledText->setString("      All enemies cleared!\nPortal to the next area has now opened.");
                enemiesKilledText->setCenter(scw / 2.0f, sch / 3.0f);
                HUD::TempTextsOnScreen.push_back(enemiesKilledText);

                if (CurLocation == &LabyrinthLocation) {
                    if (!in(InteractableStuff, (Interactable*)&portal)) {
                        InteractableStuff.push_back(&portal);
                    }
                }
            }
        } else {
            sf::Vector2f bullet_path;
            for (int j = 0; j < Bullets.size(); j++) {
                if (!faction::friends(Bullets[j]->fromWho, Enemies[i]->faction)) {
                    bullet_path = Bullets[j]->hitbox.getCenter() + distance(Bullets[j]->hitbox.getCenter(), Enemies[i]->hitbox.getCenter()) * normalize(Bullets[j]->Velocity);
                    if (Enemies[i]->hitbox.contains(bullet_path)) {
                        sf::Vector2f toRotate90 = Bullets[j]->hitbox.getCenter() - Enemies[i]->hitbox.getCenter();
                        Enemies[i]->shift(sf::Vector2f(toRotate90.y, -toRotate90.x) * (random(i, i) < 0.5f ? 1.f : -1.f));
                        break;
                    }
                }
            }
            Enemies[i]->move(CurLocation);
            Enemies[i]->UpdateState();
            Enemies[i]->CurWeapon->lock = false;
            Enemies[i]->CurWeapon->Shoot(Enemies[i]->hitbox, player.hitbox.getCenter(), Enemies[i]->faction);
            Enemies[i]->CurWeapon->Reload(Enemies[i]->Mana);
        }
    }
}

void updateUpgradeShopStats() {
    {
        using namespace upgradeInterface;
        PlacedText& weaponDescText = weaponDescPanel.text;
        weaponDescText.setString(player.CurWeapon->Name + '\n');
        weaponDescText.addString(weaponDescString[player.CurWeapon->Name] + '\n');
        weaponDescText.addString("\nStats:");

        weaponDescText.addString("\nMana storage: " + floatToString(player.CurWeapon->MaxManaStorage));
        weaponDescText.addString("\t\t\tReload Speed: " + floatToString(player.CurWeapon->ReloadSpeed.getStat()) + " mana/sec");
        weaponDescText.addString("\n\nTime To Holster: " + floatToString(player.CurWeapon->TimeToHolster.getStat().asSeconds()) + " sec");
        weaponDescText.addString("\t\t\tTime To Dispatch: " + floatToString(player.CurWeapon->TimeToDispatch.getStat().asSeconds()) + " sec");
        weaponDescText.addString("\n\nDamage: " + floatToString(player.CurWeapon->ManaCostOfBullet));
        if (player.CurWeapon->Multishot != 1) {
            weaponDescText.addString("\t\t\tBullet per shot: " + std::to_string(player.CurWeapon->Multishot));
        }
        weaponDescText.addString("\t\t\tRate of fire: " + floatToString(1 / player.CurWeapon->FireRate.getStat().asSeconds()) + " shots/sec");
        weaponDescText.addString("\n\nVelocity of Bullet: " + floatToString(player.CurWeapon->BulletVelocity));
        weaponDescText.addString("\t\t\tScatter: " + floatToString(player.CurWeapon->Scatter) + " deg");

        weaponDescText.setCenter(weaponDescPanel.getCenter());

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
        if (effectVec[i]->howLongToExist <= sf::Time::Zero) {
            clearEffect(*creature, effectVec[i]);
            DeletePointerFromVector(effectVec, i--);
        } else {
            float t = std::min(effectVec[i]->localClock->restart().asSeconds(), effectVec[i]->howLongToExist.asSeconds());
            effectVec[i]->howLongToExist -= sf::seconds(t);
            switch (effectVec[i]->type) {
                case Effects::Damage:
                    creature->getDamage(effectVec[i]->parameters[0] * t);
                    break;
                case Effects::Heal:
                    creature->getDamage(-effectVec[i]->parameters[0] * t);
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
    float uTime = GameClock->getElapsedTime().asSeconds();
    sf::Vector2f uPlayerPosition(player.hitbox.getCenter() - GameView.getCenter() + GameView.getSize() / 2.f);

    Shaders::Flashlight.setUniform("uMouse", uMouse);
    Shaders::Flashlight.setUniform("uPlayerPosition", uPlayerPosition);

    Shaders::Portal.setUniform("uTime", uTime);

    Shaders::Architect.setUniform("uTime", uTime);

    Shaders::Distortion1.setUniform("uTime", uTime);
    Shaders::Distortion2.setUniform("uTime", uTime);

    Shaders::Bullet.setUniform("uTime", uTime);

    Shaders::Fire.setUniform("uTime", uTime);

    Shaders::WaveMix.setUniform("uTime", uTime);
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

        if (player.Health.toBottom() == 0) {
            HUD::EscapeButton.buttonFunction();
        }

        updateEnemies();
        player.UpdateState();

        if (CurLocation == &LabyrinthLocation) {
            if (Musics::Fight1.getDuration() - Musics::Fight1.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight2.play();
            }
            if (Musics::Fight2.getDuration() - Musics::Fight2.getPlayingOffset() < sf::seconds(0.3f)) {
                Musics::Fight1.play();
            }
        }
        if (!window.hasFocus()) {
            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player.hitbox, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
            }
            updateBullets();

            sf::Event event;
            while (window.pollEvent(event)) {}

            if (HostFuncRun) {
                mutex.lock();
                SendPacket << packetStates::PlayerPos << player << ConnectedPlayers;
                SendToClients(SendPacket);
                SendPacket.clear();
                mutex.unlock();
            }
        } else {
            if (!chat.inputted && !inventoryInterface::isDrawInventory && !MenuShop::isDrawShop) {
                player.move(CurLocation);
                GameView.setCenter(player.hitbox.getCenter() + static_cast<sf::Vector2f>((sf::Mouse::getPosition() - sf::Vector2i(scw, sch) / 2) / 8));
                std::vector<sf::Vector2f> centers;
                for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                    if (i != ComputerID) {
                        centers.push_back(ConnectedPlayers[i - k].hitbox.getCenter());
                    } else {
                        centers.push_back(player.hitbox.getCenter());
                        k++;
                    }
                }
                FindAllWaysTo(CurLocation, centers, TheWayToPlayer);
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                    player.makeADash = playerMakingADash && playerCanDashing;
                }
            }
            int wasBulletsSize = Bullets.size();

            if (player.CurWeapon != nullptr) {
                player.CurWeapon->Shoot(player.hitbox, window.mapPixelToCoords(sf::Mouse::getPosition()), player.faction);
            }
            for (Weapon*& weapon : Weapons)
                if (weapon->holstered) weapon->Reload(player.Mana);

            if (wasBulletsSize < Bullets.size() && (HostFuncRun || ClientFuncRun)) {
                mutex.lock();
                SendPacket << packetStates::Shooting << (sf::Int32)(Bullets.size() - wasBulletsSize);
                for (; wasBulletsSize < Bullets.size(); wasBulletsSize++) {
                    SendPacket << *Bullets[wasBulletsSize];
                }
                if (HostFuncRun) {
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }

            updateBullets();

            if (HostFuncRun || ClientFuncRun) {
                mutex.lock();
                SendPacket << packetStates::PlayerPos << player;
                if (HostFuncRun) {
                    SendPacket << ConnectedPlayers;
                    SendToClients(SendPacket);
                } else if (ClientFuncRun) {
                    MySocket.send(SendPacket);
                }
                SendPacket.clear();
                mutex.unlock();
            }

            if (MiniMapActivated) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    MiniMapView.move(-sf::Vector2f(sf::Mouse::getPosition() - MouseBuffer) * MiniMapZoom);
                }
            }
            MouseBuffer = sf::Mouse::getPosition();
        }

        draw();

        if (puddle.hitbox.intersect(player.hitbox))
            applyEffect(player, new Effect(Effects::Heal, std::vector<float>{30.f}, sf::seconds(1.5f)));

        for (int i = 0; i < listOfFire.size(); i++) {
            if (listOfFire[i]->hitbox.intersect(player.hitbox))
                applyEffect(player, new Effect(Effects::Burn, std::vector<float>{5.f}, sf::seconds(5.f), sf::seconds(1.f)));
        }
        processEffects();

        if (CanSomethingBeActivated()) {
            if (!in(HUD::InterfaceStuff, static_cast<sf::Drawable*>(&HUD::XButtonSprite))) {
                HUD::InterfaceStuff.push_back(&HUD::XButtonSprite);
            }
            if (!in(HUD::InterfaceStuff, static_cast<sf::Drawable*>(&HUD::InfoLogoSprite))) {
                HUD::InterfaceStuff.push_back(&HUD::InfoLogoSprite);
            }
        } else {
            DeleteFromVector(HUD::InterfaceStuff, static_cast<sf::Drawable*>(&HUD::XButtonSprite));
            DeleteFromVector(HUD::InterfaceStuff, static_cast<sf::Drawable*>(&HUD::InfoLogoSprite));
        }

        EventHandler();
    }
    saveGame();
}


//////////////////////////////////////////////////////////// Server-Client functions
void ClientConnect() {
    sf::TcpSocket* client = new sf::TcpSocket;
    if (listener.accept(*client) == sf::Socket::Done) {
        mutex.lock();

        std::string ConnectedClientIP = (*client).getRemoteAddress().toString();

        for (int i = 0; i < clients.size(); i++) {
            if (ConnectedClientIP == clients[i]->getRemoteAddress().toString()) {
                ClientDisconnect(i);
            }
        }

        std::cout << ConnectedClientIP << " connected\n";
        HUD::ListOfPlayers.addWord(ConnectedClientIP);
        std::cout << "list of players:\n" << HUD::ListOfPlayers.text.getText() << '\n';

        clients.push_back(client);
        selector.add(*client);

        SendPacket << (sf::Int32)(ConnectedPlayers.size() + 1);
        for (int i = 0; i < ConnectedPlayers.size() + 1; i++) {
            SendPacket << HUD::ListOfPlayers[i];
        }

        std::cout << "amount players = " << ConnectedPlayers.size() + 1 << '\n';
        SendPacket << player << player.Name.getText();
        for (int i = 0; i < ConnectedPlayers.size(); i++) {
            SendPacket << ConnectedPlayers[i] << ConnectedPlayers[i].Name.getText();
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
        client->send(SendPacket);

        SendPacket.clear();

        ConnectedPlayers.push_back(*(new Player()));
        ConnectedPlayers.back().setAnimation(Textures::Player, &Shaders::Player);
        DrawableStuff.push_back(&(ConnectedPlayers.back()));

        mutex.unlock();
    } else {
        delete client;
        std::cout << "Error: Unable to accept connection\n";
    }
}

void ClientDisconnect(int i) {
    selector.remove(*clients[i]);
    std::cout << (*clients[i]).getRemoteAddress().toString() << " disconnected; number = " << i << "\n";
    DeletePointerFromVector(clients, i);
    DeleteFromVector(DrawableStuff, (sf::Drawable*)&ConnectedPlayers[i]);
    ConnectedPlayers.erase(ConnectedPlayers.begin() + i);
    HUD::ListOfPlayers.removeWord(i);

    std::cout << "amount of clients = " << clients.size() << "\n";
    mutex.lock();
    SendPacket << packetStates::PlayerDisconnect << i;
    SendToClients(SendPacket);
    SendPacket.clear();
    mutex.unlock();
}

void SelfDisconnect() {
    std::cout << "SelfDisconnect\n";
    ClientFuncRun = false;
    LoadMainMenu();
    mutex.lock();
    SendPacket << packetStates::Disconnect;
    MySocket.send(SendPacket);
    SendPacket.clear();
    mutex.unlock();
    MySocket.disconnect();
    selector.clear();
    for (int i = 0; i < ConnectedPlayers.size(); i++) {
        DeleteFromVector(DrawableStuff, (sf::Drawable*)&ConnectedPlayers[i]);
    }
    ConnectedPlayers.clear();
}

void SendToClients(sf::Packet& pac, int ExceptOf) {
    for (int i = 0; i < clients.size(); i++) {
        if (i != ExceptOf && clients[i]->send(pac) != sf::Socket::Done) {}
    }
}

void funcOfHost() {
    std::cout << "Server turn on\n";
    while (HostFuncRun) {
        if (selector.wait(sf::seconds(1))) {
            if (selector.isReady(listener)) {
                ClientConnect();
            } else {
                for (int i = 0; i < clients.size(); i++) {
                    if (selector.isReady(*clients[i]) && clients[i]->receive(ReceivePacket) == sf::Socket::Done) {
                        while (!ReceivePacket.endOfPacket()) {
                            ReceivePacket >> packetStates::curState;
                            switch (packetStates::curState) {
                                case packetStates::FirstConnect:
                                    ReceivePacket >> sPacketData;
                                    ReceivePacket >> ConnectedPlayers[i].Health >> ConnectedPlayers[i].HealthRecovery;
                                    ConnectedPlayers[i].Name.setString(sPacketData);
                                    std::cout << "Connected " << sPacketData << " whith ID:" << i + 1 << '\n';
                                    mutex.lock();
                                    SendPacket << packetStates::PlayerConnect << sPacketData;
                                    SendPacket << player.Health << player.HealthRecovery;
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case packetStates::Disconnect:
                                    std::cout << "client disconected\n";
                                    ClientDisconnect(i--);
                                    break;
                                case packetStates::PlayerPos:
                                    ReceivePacket >> ConnectedPlayers[i];
                                    break;
                                case packetStates::ChatEvent:
                                    ReceivePacket >> sPacketData;
                                    chat.addLine(sPacketData);
                                    mutex.lock();
                                    SendPacket << packetStates::ChatEvent << sPacketData;
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case packetStates::Shooting:
                                    mutex.lock();
                                    ReceivePacket >> i32PacketData;
                                    SendPacket << packetStates::Shooting << i;
                                    for (sf::Int32 i = 0; i < i32PacketData; i++) {
                                        Bullets.push_back(new Bullet());
                                        ReceivePacket >> *(Bullets.back());
                                        SendPacket << Bullets.back();
                                    }
                                    SendToClients(SendPacket, i);
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                case packetStates::UseInteractable: {
                                    mutex.lock();
                                    DescriptionID::Type id;
                                    ReceivePacket >> i32PacketData >> id >> V2fPacketData;
                                    SendPacket << packetStates::UseInteractable << i32PacketData << id << V2fPacketData;
                                    Interactable* x1 = nullptr;
                                    for (Interactable*& x2: InteractableStuff) {
                                        if (x2->descriptionID == id && x2->hitbox.getCenter() == V2fPacketData) {
                                            if (id == DescriptionID::box) {
                                                DeleteFromVector(listOfBox, x2);
                                            } else if (id == DescriptionID::artifact) {
                                                DeleteFromVector(listOfArtifact, x2);
                                                ReceivePacket >> ConnectedPlayers[i32PacketData - 1].Health >> ConnectedPlayers[i32PacketData - 1].HealthRecovery;
                                                SendPacket << ConnectedPlayers[i32PacketData - 1].Health << ConnectedPlayers[i32PacketData - 1].HealthRecovery;
                                            }
                                            DeleteFromVector(DrawableStuff, (sf::Drawable*)x2);
                                            SendToClients(SendPacket, i32PacketData - 1);
                                            x1 = x2;
                                            break;
                                        }
                                    }
                                    if (x1) {
                                        DeleteFromVector(InteractableStuff, x1);
                                        delete x1;
                                    }
                                    SendPacket.clear();
                                    mutex.unlock();
                                    break;
                                }
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
                            mutex.lock();
                            ReceivePacket >> sPacketData;
                            HUD::ListOfPlayers.addWord(sPacketData);
                            ConnectedPlayers.push_back(*(new Player()));
                            ConnectedPlayers.back().setAnimation(Textures::Player, &Shaders::Player);
                            ConnectedPlayers.back().Name.setString(sPacketData);
                            DrawableStuff.push_back(&(ConnectedPlayers.back()));
                            std::cout << sPacketData + " connected\n";
                            mutex.unlock();
                            break;
                        case packetStates::PlayerDisconnect:
                            int index;
                            ReceivePacket >> index;
                            std::cout << std::string(HUD::ListOfPlayers[index]) << " disconnected\n";
                            if (index < ComputerID) ComputerID--;
                            HUD::ListOfPlayers.removeWord(index);
                            ConnectedPlayers.erase(ConnectedPlayers.begin() + index);
                            break;
                        case packetStates::Labyrinth:
                            mutexOnDraw.lock();
                            clearVectorOfPointer(PickupStuff);
                            clearVectorOfPointer(Bullets);

                            DrawableStuff.clear();
                            HUD::InterfaceStuff.clear();
                            InteractableStuff.clear();
                            removeUI(&HUD::HUDFrame, HUD::InterfaceStuff);

                            MiniMapActivated = false;
                            HUD::EscapeMenuActivated = false;

                            MiniMapView.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));
                            MiniMapView.setCenter(player.hitbox.getCenter() * ScaleParam);

                            Musics::MainMenu.pause();
                            if (Musics::Fight1.getStatus() != sf::Music::Playing && Musics::Fight2.getStatus() != sf::Music::Playing) {
                                Musics::Fight1.play();
                            }

                            if (CurLocation != &LabyrinthLocation) {
                                CurLocation = &LabyrinthLocation;
                            } else {
                                completedLevels = std::max(curLevel, completedLevels);
                                curLevel++;
                            }
                            MiniMapView.zoom(1 / MiniMapZoom);
                            MiniMapZoom = std::pow(1.1, -10);
                            MiniMapView.zoom(MiniMapZoom);
                            ReceivePacket >> &LabyrinthLocation;
                            FindAllWaysTo(CurLocation, player.hitbox.getCenter(), TheWayToPlayer);
                            ReceivePacket >> i32PacketData; clearVectorOfPointer(Enemies);
                            for (int i = 0; i < i32PacketData; i++) {
                                ReceivePacket >> sPacketData;
                                if (sPacketData == "Distorted Scientist") {
                                    Enemies.push_back(new DistortedScientist());
                                } else if (sPacketData == "Distorted") {
                                    Enemies.push_back(new Distorted());
                                }
                                ReceivePacket >> Enemies[i];
                                DrawableStuff.push_back(Enemies[i]);
                            }
                            ReceivePacket >> i32PacketData; clearVectorOfPointer(listOfBox);
                            for (int i = 0; i < i32PacketData; i++) {
                                listOfBox.push_back(new Interactable()); setBox(listOfBox[i]);
                                ReceivePacket >> listOfBox[i];
                                placedOnMap(listOfBox[i]);
                            }
                            ReceivePacket >> i32PacketData; clearVectorOfPointer(listOfArtifact);
                            for (int i = 0; i < i32PacketData; i++) {
                                listOfArtifact.push_back(new Interactable()); setArtifact(listOfArtifact[i]);
                                ReceivePacket >> listOfArtifact[i];
                                placedOnMap(listOfArtifact[i]);
                            }
                            ReceivePacket >> i32PacketData; clearVectorOfPointer(listOfFire);
                            for (int i = 0; i < i32PacketData; i++) {
                                listOfFire.push_back(new Interactable()); setFire(listOfFire[i]);
                                ReceivePacket >> listOfFire[i];
                                placedOnMap(listOfFire[i]);
                                std::cout << "listOfFire " << listOfFire[i]->hitbox.getCenter() << '\n';
                            }
                            ReceivePacket >> &portal >> &puddle >> player;

                            CurLocation->FindEnableTilesFrom(player.hitbox.getCenter() / (float)size);
                            
                            placedOnMap(&portal);
                            placedOnMap(&puddle);

                            addUI(&HUD::HUDFrame, HUD::InterfaceStuff);
                            for (int i = 0; i < HUD::WeaponNameTexts.size(); i++) {
                                HUD::InterfaceStuff.push_back(HUD::WeaponNameTexts[i]);
                            }
                            HUD::InterfaceStuff.push_back(&chat);
                            DrawableStuff.push_back(&player);
                            for (Player& player : ConnectedPlayers) {
                                DrawableStuff.push_back(&player);
                            }
                            mutexOnDraw.unlock();
                            break;
                        case packetStates::PlayerPos:
                            for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                                if (i != ComputerID) {
                                    ReceivePacket >> ConnectedPlayers[i - k];
                                } else {
                                    sf::Vector2i tempPoint;
                                    ReceivePacket >> tempPoint;
                                    k++;
                                }
                            }
                            break;
                        case packetStates::SetPos:
                            for (int i = 0, k = 0; i < ConnectedPlayers.size() + 1; i++) {
                                if (i != ComputerID) {
                                    ReceivePacket >> ConnectedPlayers[i - k];
                                } else {
                                    ReceivePacket >> player;
                                    k++;
                                }
                            }
                            break;
                        case packetStates::ChatEvent:
                            ReceivePacket >> sPacketData;
                            chat.addLine(sPacketData);
                            break;
                        case packetStates::Shooting:
                            ReceivePacket >> i32PacketData;
                            for (int i = 0; i < i32PacketData; i++) {
                                Bullets.push_back(new Bullet());
                                ReceivePacket >> *(Bullets.back());
                            }
                            std::cout << "bullet recieved\n";
                            break;
                        case packetStates::UseInteractable: {
                            mutex.lock();
                            DescriptionID::Type id;
                            ReceivePacket >> i32PacketData >> id >> V2fPacketData;
                            i32PacketData -= i32PacketData > ComputerID;
                            Interactable* x1 = nullptr;
                            for (Interactable*& x2: InteractableStuff) {
                                if (x2->descriptionID == id && x2->hitbox.getCenter() == V2fPacketData) {
                                    if (id == DescriptionID::box) {
                                        DeleteFromVector(listOfBox, x2);
                                    } else if (id == DescriptionID::artifact) {
                                        DeleteFromVector(listOfArtifact, x2);
                                        ReceivePacket >> ConnectedPlayers[i32PacketData].Health >> ConnectedPlayers[i32PacketData].HealthRecovery;
                                    }
                                    DeleteFromVector(DrawableStuff, (sf::Drawable*)x2);
                                    x1 = x2;
                                    break;
                                }
                            }
                            if (x1) {
                                DeleteFromVector(InteractableStuff, x1);
                                delete x1;
                            }
                            mutex.unlock();
                            break;
                        }
                    }
                }
            }
        }
    }
}
