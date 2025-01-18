#pragma once
#include "../init.h"
#include <queue>
#include <iostream>
#include <fstream>

using vb = std::vector<bool>;
using vvb = std::vector<std::vector<bool>>;

sf::Vector2i dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

std::vector<CollisionRect> wallsRect;
std::vector<bool> SeenWalls;

// namespace Side { может понадобиться, если нужно будет опрделять положение чего-то
//     using Type = sf::Uint8;
//     enum : Type {
//         Left,
//         Top,
//         Right,
//         Bottom
//     };
// }

////////////////////////////////////////////////////////////
// Class
////////////////////////////////////////////////////////////

class Location {
public:
    int n, m;
    vvb walls;
    vvb EnableTiles;
    size_t AmountOfEnableTiles;

    Location() { AmountOfEnableTiles = 0; }
    Location(int w, int h) { SetSize(w, h); }
    void SetSize(int w, int h);
    void GenerateLocation(int n, int m, sf::Vector2f RootPoint);
    void BuildWayFrom(sf::Vector2f);
    void WallGenerator(float probability);
    int getPassagesAmount(int x, int y);
    std::vector<sf::Vector2i> getPassages(int x, int y);
    void FindEnableTilesFrom(sf::Vector2f);
    void FillWallsRect();
    bool ExistDirectWay(sf::Vector2f from, sf::Vector2f to);
    bool LoadFromFile(std::string FileName);
    bool WriteToFile(std::string FileName);
    void ClearSeenWalls() {
        SeenWalls.assign(wallsRect.size(), false);
    }
};

////////////////////////////////////////////////////////////
// Realization
////////////////////////////////////////////////////////////

void Location::SetSize(int NewN, int NewM) {
    n = NewN;
    m = NewM;
    walls.assign(n * 2 + 1, vb(0));
    for (int i = 0; i < walls.size(); i++) walls[i].assign(m + (i % 2), false);
    EnableTiles.assign(n, vb(m, false));
    AmountOfEnableTiles = 0;
}

void Location::GenerateLocation(int n, int m, sf::Vector2f RootPoint) {
    SetSize(n, m);
    int CounterOfGenerations = 0;
    sf::Clock timer;
    do {
        WallGenerator(0.48);
        BuildWayFrom(RootPoint);
        CounterOfGenerations++;
    } while (AmountOfEnableTiles < float(n * m) * 0.3f || AmountOfEnableTiles > float(n * m) * 0.7f);
    FillWallsRect();
    ClearSeenWalls();
    std::cout << "Location was generated in " << timer.getElapsedTime().asSeconds() << " seconds with total number of generations = "
              << CounterOfGenerations << "; Count Of Enable Tiles = " << AmountOfEnableTiles << '\n';
}

void Location::BuildWayFrom(sf::Vector2f p) {
    FindEnableTilesFrom(p);
    sf::IntRect UsedAreaRect(0, 0, m, n);
    sf::Vector2i check;
    bool todel;
    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++) {
            if (!walls[i][j]) continue;

            todel = true;
            check = sf::Vector2i(j, i / 2);
            if (UsedAreaRect.contains(check) && EnableTiles[check.y][check.x])
                todel = false;
            check = (i % 2 == 0) ? sf::Vector2i(j, i / 2 - 1) : sf::Vector2i(j - 1, i / 2);
            if (UsedAreaRect.contains(check) && EnableTiles[check.y][check.x])
                todel = false;
            if (todel) walls[i][j] = false;
        }
}

void Location::WallGenerator(float probability) {
    walls[0].assign(m, true);

    for (int i = 1; i < walls.size() - 1; i++) {
        for (int j = 0; j < walls[i].size(); j++)
            walls[i][j] = (float(rand() % 100) / 100 < probability);
        if (i % 2 == 1) {
            walls[i][0] = true;
            walls[i][m] = true;
        }
    }

    walls.back().assign(m, true);
}

int Location::getPassagesAmount(int x, int y) {
    return !walls[y * 2 + 1][x] + !walls[y * 2 + 1][x + 1] + !walls[y * 2][x] + !walls[y * 2 + 2][x];
}
std::vector<sf::Vector2i> Location::getPassages(int x, int y) {
    std::vector<sf::Vector2i> passages;
    if (!walls[y * 2 + 1][x    ]) passages.push_back(sf::Vector2i(x    , y * 2 + 1)); // стена слева
    if (!walls[y * 2 + 1][x + 1]) passages.push_back(sf::Vector2i(x + 1, y * 2 + 1)); // стена справа
    if (!walls[y * 2    ][x    ]) passages.push_back(sf::Vector2i(x    , y * 2    )); // стена сверху
    if (!walls[y * 2 + 2][x    ]) passages.push_back(sf::Vector2i(x    , y * 2 + 2)); // стена снизу
    return passages;
}

void Location::FindEnableTilesFrom(sf::Vector2f p) {
    EnableTiles.assign(n, vb(m, false));
    AmountOfEnableTiles = 0;
    std::queue<sf::Vector2i> q; q.push(sf::Vector2i(p));
    sf::Vector2i cur, check;
    sf::IntRect UsedAreaRect(0, 0, m, n);
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (!UsedAreaRect.contains(cur) || EnableTiles[cur.y][cur.x]) continue;
        EnableTiles[cur.y][cur.x] = true;
        AmountOfEnableTiles++;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 1][check.x])
            q.push(check);
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 1][check.x + 1])
            q.push(check);
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2][check.x])
            q.push(check);
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check) && !EnableTiles[check.y][check.x] && !walls[check.y * 2 + 2][check.x])
            q.push(check);
    }
}

void Location::FillWallsRect() {
    wallsRect.clear();

    for (int i = 0; i < walls.size(); i++)
        for (int j = 0; j < walls[i].size(); j++)
            if (walls[i][j]) {
                if (i % 2 == 1) { // |
                    wallsRect.push_back(CollisionRect(size * j - WallMinSize / 2, size * i / 2 - WallMaxSize / 2, WallMinSize, WallMaxSize));
                } else { // -
                    wallsRect.push_back(CollisionRect(size * j, size * i / 2 - WallMinSize / 2, WallMaxSize, WallMinSize));
                }
            }
}

bool Location::LoadFromFile(std::string FileName) {
    std::ifstream file(FileName);
    if (!file.is_open()) return false;
    file >> n >> m;
    SetSize(n, m);
    for (int i = 0; i < walls.size(); i++) {
        for (int j = 0, t; j < walls[i].size(); j++) {
            file >> t;
            walls[i][j] = t;
        }
    }
    file.close();
    FillWallsRect();
    ClearSeenWalls();
    return true;
}

bool Location::WriteToFile(std::string FileName) {
    std::ofstream file(FileName);
    file << n << ' ' << m << '\n';
    for (int i = 0; i < walls.size(); i++) {
        for (int j = 0; j < walls[i].size(); j++)
            file << walls[i][j] << ' ';
        file << '\n';
    }
    file.close();
    return true;
}

////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////

sf::Packet& operator>>(sf::Packet& packet, Location* loc) {
    packet >> loc->n >> loc->m;
    std::cout << "n = " << loc->n << " m = " << loc->m << '\n';
    loc->SetSize(loc->n, loc->m);
    bool t;
    for (int i = 0; i < loc->walls.size(); i++) {
        for (int j = 0; j < loc->walls[i].size(); j++) {
            packet >> t;
            loc->walls[i][j] = t;
        }
    }
    loc->FillWallsRect();
    loc->ClearSeenWalls();
    return packet;
}

sf::Packet& operator<<(sf::Packet& packet, Location* loc) {
    packet << loc->n << loc->m;
    for (int i = 0; i < loc->walls.size(); i++)
        for (int j = 0; j < loc->walls[i].size(); j++)
            packet << loc->walls[i][j];
    return packet;
}

void FindTheWay(Location* where, sf::Vector2f from, sf::Vector2f to, std::vector<sf::Vector2f>& theWay) {
    std::vector<std::vector<sf::Vector2i>> place(where->n, std::vector<sf::Vector2i>(where->m, sf::Vector2i(-1, -1)));
    std::queue<sf::Vector2i> q; q.push(sf::Vector2i(from.x / size, from.y / size));
    sf::Vector2i cur, check;
    sf::IntRect UsedAreaRect(0, 0, where->m, where->n);
    vvb used(where->n, vb(where->m, false));
    while (!q.empty()) {
        cur = q.front(); q.pop();
        if (used[cur.y][cur.x]) continue;
        used[cur.y][cur.x] = true;
        if (sf::Vector2i(to / (float)size) == cur) break;
        check = cur + dirs[0]; // {1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 1][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[2]; // {-1, 0}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 1][check.x + 1]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[1]; // {0, 1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
        check = cur + dirs[3]; // {0, -1}
        if (UsedAreaRect.contains(check) && !used[check.y][check.x] && !where->walls[check.y * 2 + 2][check.x]) {
            q.push(check);
            place[check.y][check.x] = cur;
        }
    }

    theWay.clear();
    theWay.push_back(to);
    if (sf::Vector2i(to / (float)size) != cur) {
        std::cout << "Don't find the way\n";
        cur = {-1, -1};
    }
    for (; place[cur.y][cur.x] != sf::Vector2i(-1, -1); cur = place[cur.y][cur.x])
        theWay.push_back(((sf::Vector2f)place[cur.y][cur.x] + sf::Vector2f(0.5f, 0.5f)) * (float)size);
    if (theWay.size() > 1) theWay.pop_back();
}

bool ExistDirectWay(CollisionShape& shape, sf::Vector2f to) {
    sf::Vector2f dir = to - shape.getCenter();
    CollisionShape rect({
        {-shape.getSize().x / 2.f, 0.f        },
        { shape.getSize().x / 2.f, 0.f        },
        { shape.getSize().x / 2.f, length(dir)},
        {-shape.getSize().x / 2.f, length(dir)}
    });
    float phi = std::atan2(dir.x, dir.y);
    rect.setPoints(RotateOn(phi, rect.getPoints()));
    rect.move(shape.getCenter());
    for (CollisionShape*& obj: CollisionShapes) {
        if (obj != &shape && rect.intersect(*obj)) {
            return false;
        }
    }
    return true;
}

// {x = 1, y = -1} => collision at the y, up or down doesn't matter, because u know "dy" already
sf::Vector2i WillCollideWithWalls(CollisionShape& obj, sf::Vector2f Velocity) {
    sf::Vector2i res(1, 1);
    obj.move(0, Velocity.y);
    for (int i = 0; i < wallsRect.size(); i++) {
        if (wallsRect[i].intersect(obj)) {
            res.y = -1;
            break;
        }
    }
    obj.move(Velocity.x, -Velocity.y);
    for (int i = 0; i < wallsRect.size(); i++) {
        if (wallsRect[i].intersect(obj)) {
            res.x = -1;
            break;
        }
    }
    obj.move(-Velocity.x, 0);
    return res;  // if value of vector == -1 => there was collision
}

////////////////////////////////////////////////////////////
// perhaps unusable code, but usefull for future
////////////////////////////////////////////////////////////

// void Location::WallGenerator(float probability) {
    // auto gen = []() {
    //     thread_local std::mt19937 rng{std::random_device()()};
    //     thread_local std::uniform_real_distribution<float> distr(0, 100);
    //     return distr(rng);
    // };
    // if (walls.size() == 0) return;

    // walls[0].assign(m, true);
    // walls.back().assign(m, true);

    // sf::Thread* thr[7];
    // int from = 1, to = walls.size() / 8;
    // for (int i = 0; i < 7;) {
    //     thr[i] = new sf::Thread([&, to, from] {
    //         for (int i = from; i < to; i++) {
    //             for (int j = 0; j < walls[i].size(); j++)
    //                 walls[i][j] = (gen() / 100 < probability);
    //             if (i % 2 == 1)
    //                 walls[i][0] = walls[i][m] = true;
    //         }
    //     });
    //     thr[i++]->launch();
    //     from = i * walls.size() / 8; to = (i + 1) * walls.size() / 8;
    // }
    // for (int i = from; i < to - 1; i++) {
    //     for (int j = 0; j < walls[i].size(); j++)
    //         walls[i][j] = (gen() / 100 < probability);
    //     if (i % 2 == 1)
    //         walls[i][0] = walls[i][m] = true;
    // }
    // ClearSeenWalls();
    // for (int i = 0; i < 7; i++)
    //     thr[i]->wait();
    // FillWallsRect();
// }