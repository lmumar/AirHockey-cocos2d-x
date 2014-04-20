//
//  GameSprite.h
//  AirHockey
//
//  Created by Lord Norlan Mumar on 4/18/14.
//
//

#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__

#include "cocos2d.h"

class GameSprite : public cocos2d::Sprite {
public:
    GameSprite();
    ~GameSprite();

    CC_SYNTHESIZE(cocos2d::Point, _nextPosition, NextPosition);
    CC_SYNTHESIZE(cocos2d::Point, _vector, Vector);
    CC_SYNTHESIZE(cocos2d::Touch*, _touch, Touch);

    static GameSprite* gameSpriteWithFile(const std::string& filename);

    virtual void setPosition(const cocos2d::Point& pos) override;
    
    inline float radius() const { return getTexture()->getContentSize().width * 0.5f; }

    virtual bool initWithFile(const std::string& filename) override;
};

#endif
