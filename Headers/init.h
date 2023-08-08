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
#include "scale.h"
#include "rect.h"
#define M_PI       3.14159265358979323846   // pi

#define ActivationButton sf::Keyboard::X

using Point = sf::Vector2i;

using str = std::string;
using vb = std::vector<bool>;
using vvb = std::vector<vb>;

Point dirs[] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

namespace screens {
    enum screens {  // the types of screen
        MainRoom,
        SetIP,
        Dungeon
    };
};

int scw = sf::VideoMode::getDesktopMode().width; // screen width
int sch = sf::VideoMode::getDesktopMode().height; // screen height

int size = 540, miniSize = 50; // map is matrix n x m cells with size of one; minisize for minimap
int START_N = 10, START_M = 10;
float WallMinSize = size / 8, WallMaxSize = size;

sf::Vector2f CameraPos, miniCameraPos;

sf::View GameView({0, 0, (float)scw, (float)sch});
sf::View InterfaceView({0, 0, (float)scw, (float)sch});
sf::View MiniMapView({0, 0, (float)scw, (float)sch});