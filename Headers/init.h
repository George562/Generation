#pragma once
#include "../SFML-2.5.1/include/SFML/Network.hpp"
#include "../SFML-2.5.1/include/SFML/Graphics.hpp"
#include "../SFML-2.5.1/include/SFML/Audio.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <ctime>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include "scale.h"
#include "rect.h"
#include "circle.h"
#include "animation.h"
#define M_PI       3.14159265358979323846   // pi

#define ActivationButton sf::Keyboard::X

using Point = sf::Vector2i;

using str = std::string;
using vb = std::vector<bool>;
using vvb = std::vector<vb>;

Point dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

namespace Fraction {
    enum Fraction : sf::Uint8 {
        Player,
        Enemy
    };
    std::unordered_map<Fraction, std::unordered_set<Fraction>> Friendship{
        {Player, {Player}},
        {Enemy, {Enemy}}
    };
};

namespace screens {
    enum screens : sf::Uint8 {  // the types of screen
        MainRoom,
        SetIP,
        Dungeon
    };
};

namespace pacetStates {
    enum pacetStates : sf::Uint8 {
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

int scw = 1920; // screen width
int sch = 1080; // screen height

int size = 480, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
float ScaleParam = float(miniSize) / float(size);
int START_N = 10, START_M = 10;
float WallMinSize = size / 8, WallMaxSize = size;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});

bool Rect::intersect(Circle& circle) {
    if (circle.PosY < PosY) {   // Если сверху
        if (circle.PosX < PosX)         // Если в левом углу
            return distance(getPosition(), circle.getPosition()) <= circle.Radius;
        if (circle.PosX > getRight())   // Если в правом углу
            return distance(getRightTop(), circle.getPosition()) <= circle.Radius;
        return PosY - circle.PosY <= circle.Radius; // Если посередине
    }
    if (circle.PosY > getBottom()) {    // Если снизу
        if (circle.PosX < PosX)             // Если в левом углу
            return distance(getLeftBottom(), circle.getPosition()) <= circle.Radius;
        if (circle.PosX > getRight())       // Если в правом углу
            return distance(getRightBottom(), circle.getPosition()) <= circle.Radius;
        return circle.PosY - getBottom() <= circle.Radius; // Если посередине
    }
    
    if (circle.PosX < PosX)   // Если слева
        return PosX - circle.PosX <= circle.Radius;
    if (circle.PosX > getRight())   // Если справа
        return circle.PosX - getRight() <= circle.Radius;
 
    return true; // внутри
}

bool Circle::intersect(Rect& rect) {
    return rect.intersect(*this);
}