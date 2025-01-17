#pragma once
#include <vector>
#include "../Utility/VectorTools.h"
#include "../Utility/commonShapes.h"
#include <algorithm>

class CollisionShape {
protected:
    std::vector<sf::Vector2f> points;
    size_t pointCount;
    sf::Vector2f position, size;
    virtual void updateShape() {
        if (pointCount > 0) {
            position = points[0];
            for (size_t i = 1; i < pointCount; i++) {
                position.x = std::min(position.x, points[i].x);
                position.y = std::min(position.y, points[i].y);
            }
            size = points[0] - position;
            for (size_t i = 1; i < pointCount; i++) {
                size.x = std::max(size.x, points[i].x - position.x);
                size.y = std::max(size.y, points[i].y - position.y);
            }
        }
    }
public:
    CollisionShape() : pointCount(0) {}
    CollisionShape(size_t count) : pointCount(count) { points.resize(count); }
    CollisionShape(std::vector<sf::Vector2f> points) : points(points), pointCount(points.size()) {
        updateShape();
    }

    virtual void setPoints(std::vector<sf::Vector2f> points) {
        this->points = points;
        pointCount = points.size();
        updateShape();
    }

    virtual sf::Vector2f getPosition() const { return position; };
    virtual void setPosition(float x, float y) { setPosition(sf::Vector2f(x, y)); }
    virtual void setPosition(sf::Vector2f newPosition) {
        for (size_t i = 0; i < pointCount; i++) {
            points[i] += newPosition - position;
        }
        position = newPosition;
    }

    virtual sf::Vector2f getCenter() const { return position + size / 2.f; }
    virtual void setCenter(float x, float y) { setPosition(x - size.x / 2.f, y - size.y / 2.f); }
    virtual void setCenter(sf::Vector2f v)   { setCenter(v.x, v.y); }

    virtual sf::Vector2f getSize() const { return size; }
    virtual void setSize(float w, float h) {
        if (size.x == 0 || size.y == 0) throw "You cannot set the size before you set the points.";
        for (size_t i = 0; i < pointCount; i++) {
            points[i].x = (points[i].x - position.x) * w / size.x + position.x;
            points[i].y = (points[i].y - position.y) * h / size.y + position.y;
        }
        size.x = w;
        size.y = h;
    }
    void setSize(sf::Vector2f size) { setSize(size.x, size.y); }

    void setScale(float x, float y) { setSize(size.x * x, size.y * y); }
    void setScale(sf::Vector2f scale) { setScale(scale.x, scale.y); }

    virtual void move(float x, float y)  { setPosition(position.x + x, position.y + y); }
    virtual void move(sf::Vector2f v)    { setPosition(position + v); }

    void setPoint(size_t index, sf::Vector2f point) { points[index] = point; updateShape(); }
    sf::Vector2f getPoint(size_t index) const { return points[index]; }
    std::vector<sf::Vector2f> getPoints() const { return points; }

    std::size_t getPointCount() { return pointCount; }

    virtual sf::FloatRect getBounds() const { return sf::FloatRect(position, size); }

    bool contains(float x, float y) const { return contains(sf::Vector2f(x, y)); }
    bool contains(sf::Vector2f point) const {
        if (!getBounds().contains(point)) {
            return false;
        }
        bool s = false;
        sf::Vector2f e, w;
        for(int i = 0, j = pointCount - 1; i < pointCount; j = i, i++) {
            e = points[j] - points[i];
            w =     point - points[i];
            if( (points[i].y <= point.y && point.y < points[j].y && cross(e, w) > 0) ||
               !(points[i].y <= point.y || point.y < points[j].y || cross(e, w) > 0))
                s = !s;
        }
        return s;
    }

    bool intersect(CollisionShape& shape) {
        if (!getBounds().intersects(shape.getBounds())) {
            return false;
        }
        for (int i = 0; i < shape.pointCount; i++) {
            if (contains(shape.points[i])) {
                return true;
            }
        }
        sf::Vector2f a, b, ab, c, d, cd, intersectionPoint;
        for (int i = 0; i < pointCount; i++) {
            a = points[i]; b = points[(i + 1) % pointCount]; ab = a - b;
            for (int j = 0; j < shape.pointCount; j++) {
                c = shape.points[j]; d = shape.points[(j + 1) % shape.pointCount]; cd = c - d;
                if (ab.x * cd.y - ab.y * cd.x == 0) {
                    if ((ab.y == 0 && std::abs(a.y - c.y) < 0.01 && std::min(a.x, b.x) <= std::max(c.x, d.x) && std::min(c.x, d.x) <= std::max(a.x, b.x)) ||
                       (ab.x == 0 && std::abs(a.x - c.x) < 0.01 && std::min(a.y, b.y) <= std::max(c.y, d.y) && std::min(c.y, d.y) <= std::max(a.y, b.y))) {
                        return true;
                    }
                }
                intersectionPoint = sf::Vector2f((cross(a, b) * cd.x - ab.x * cross(c, d)) / (ab.x * cd.y - ab.y * cd.x),
                                                 (cross(a, b) * cd.y - ab.y * cross(c, d)) / (ab.x * cd.y - ab.y * cd.x));
                if (std::min(a.x, b.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(a.x, b.x) + 0.01 &&
                    std::min(a.y, b.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(a.y, b.y) + 0.01 &&
                    std::min(c.x, d.x) - 0.01 <= intersectionPoint.x && intersectionPoint.x <= std::max(c.x, d.x) + 0.01 &&
                    std::min(c.y, d.y) - 0.01 <= intersectionPoint.y && intersectionPoint.y <= std::max(c.y, d.y) + 0.01) {
                    return true;
                }
            }
        }
        return false;
    }

    float distance(float x, float y) const {
        return distance(sf::Vector2f(x, y));
    }
    float distance(sf::Vector2f point) const {
        float s = 1., d = dot(point - points[0], point - points[0]);
        sf::Vector2f e, w, b;
        for(int i = 0, j = pointCount - 1; i < pointCount; j = i, i++) {
            e = points[j] - points[i];
            w =     point - points[i];
            b = w - e * std::clamp( dot(w, e) / dot(e, e), 0.f, 1.f );
            d = std::min(d, dot(b,b));
            if( (points[i].y <= point.y && point.y < points[j].y && cross(e, w) > 0) ||
               !(points[i].y <= point.y || point.y < points[j].y || cross(e, w) > 0))
                s *= -1.;
        }
        return s * std::sqrt(d);
    }
};

std::vector<CollisionShape*> CollisionShapes;