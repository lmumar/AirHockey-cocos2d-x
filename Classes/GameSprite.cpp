//
//  GameSprite.cpp
//  AirHockey
//
//  Created by Lord Norlan Mumar on 4/18/14.
//
//

#include "GameSprite.h"

GameSprite::GameSprite() : _nextPosition(cocos2d::Point::ZERO) {
}

bool GameSprite::initWithFile(const std::string& filename) {
    return cocos2d::Sprite::initWithFile(filename);
}

GameSprite::~GameSprite() {
}

void GameSprite::setPosition(const cocos2d::Point &pos) {
    cocos2d::Sprite::setPosition(pos);
    if (!_nextPosition.equals(pos)) {
        _nextPosition = pos;
    }
}

GameSprite* GameSprite::gameSpriteWithFile(const std::string& filename) {
    auto sprite = new GameSprite();
    if (sprite && sprite->initWithFile(filename)) {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

