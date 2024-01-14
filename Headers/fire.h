#pragma once

#include "rect.h"
#include "animation.h"
#include "init.h"

class Fire : public Rect, public sf::Drawable {
public:    
    Animation* animation = nullptr;
    // fire propagation
    int tacts;

    Fire* descendant1 = nullptr;
    Fire* descendant2 = nullptr;

    Fire(float PosX, float PosY, float Width, float Height, int tacts) {
        this->PosX = PosX;
        this->PosY = PosY;
        this->Width = Width;
        this->Height = Height;
        this->tacts = tacts;
        setAnimation(FireTexture, 1, 1, sf::seconds(1), &MapShader);
    }

    void Propagation() {
        double dist = 3*Width/2;
        double angle = (rand() % 361) * M_PI / 180;
        descendant1 = new Fire(PosX + dist*std::sin(angle), PosY + dist*std::cos(angle), Width, Height, 1000);
        //descendant1 = new Fire(PosX, PosY, Width, Height, tacts);
        angle = (rand() % 361) * M_PI / 180;
        descendant2 = new Fire(PosX + dist*std::sin(angle), PosY + dist*std::cos(angle), Width, Height, 1000);
        //descendant2 = new Fire(PosX + 200.f, PosY + 200.f, Width, Height, tacts);
    }

    void setAnimation(sf::Texture& texture, int FrameAmount, int maxLevel, sf::Time duration, sf::Shader *shader = nullptr) {
        if (animation != nullptr) {
            delete animation;
        }
        animation = new Animation(texture, FrameAmount, maxLevel, duration, shader);
        animation->setSize({Width, Height});
        animation->setOrigin(animation->getLocalSize() / 2.f);
        animation->play();
    };

    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const {
        if (animation != nullptr) {
            animation->setPosition(getPosition());
            target.draw(*animation, states);
        }
    };
};