#pragma once
#include "../../SFML-2.5.1/include/SFML/System.hpp"
#include <cmath>

float dot(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.x + a.y * b.y;
}

float cross(sf::Vector2f a, sf::Vector2f b) {
    return a.x * b.y - a.y * b.x;
}

float distance(sf::Vector2f a, sf::Vector2f b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

float length(sf::Vector2f v) {
    return std::hypot(v.x, v.y);
}

sf::Vector2f normalize(sf::Vector2f v) {
    if (std::isnan(v.x) || std::isnan(v.y))
        return sf::Vector2f(0, 0);
    if (length(v) == 0)
        return v;
    return v / length(v);
}
// clamp by vector coordinates
sf::Vector2f clamp(sf::Vector2f v, sf::Vector2f min, sf::Vector2f max) {
    return sf::Vector2f(std::clamp(v.x, min.x, max.x), std::clamp(v.y, min.y, max.y));
}
// vector length clamp
sf::Vector2f clamp(sf::Vector2f v, float min, float max) {
    float len = length(v);
    return len < min ? (normalize(v) * min) : (len > max ? normalize(v) * max : v);
}

float DegToRad(float deg) {
    return deg * M_PI / 180.f;
}

void RotateOn(float phi, float& x, float& y) {
    float oldX = x, OldY = y;
    x =   oldX * cos(phi) + OldY * sin(phi);
    y = - oldX * sin(phi) + OldY * cos(phi);
}
sf::Vector2f RotateOn(float phi, sf::Vector2f a) {
    sf::Vector2f newA;
    newA.x =   a.x * cos(phi) + a.y * sin(phi);
    newA.y = - a.x * sin(phi) + a.y * cos(phi); // FIXME:
    return newA;
}
std::vector<sf::Vector2f> RotateOn(float phi, std::vector<sf::Vector2f> vec) {
    std::vector<sf::Vector2f> newVec;
    for (int i = 0; i < vec.size(); i++)
        newVec.push_back(RotateOn(phi, vec[i]));
    return newVec;
}

std::vector<sf::Vector2f> Translate(sf::Vector2f shiftBy, std::vector<sf::Vector2f>& vec) {
    std::vector<sf::Vector2f> newVec;
    for (int i = 0; i < vec.size(); i++)
        newVec.push_back(vec[i] + shiftBy);
    return newVec;
}

void RotateAround(float phi, float& x, float& y, float& X, float& Y) {
    float oldX = x, OldY = y;
    x =   (oldX - X) * cos(phi) + (OldY - Y) * sin(phi) + X;
    y = - (oldX - X) * sin(phi) + (OldY - Y) * cos(phi) + Y;
}
sf::Vector2f RotateAround(float phi, sf::Vector2f& a, float& X, float& Y) {
    sf::Vector2f newA;
    newA.x =   (a.x - X) * cos(phi) + (a.y - Y) * sin(phi) + X;
    newA.y = - (a.x - X) * sin(phi) + (a.y - Y) * cos(phi) + Y;
    return newA;
}