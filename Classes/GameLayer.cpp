#include "GameLayer.h"

USING_NS_CC;

Scene* GameLayer::createScene()
{
    auto scene = Scene::create();
    auto layer = GameLayer::create();

    scene->addChild(layer);

    return scene;
}

bool GameLayer::init()
{
    if ( !Layer::init() ) return false;

    _screenSize = cocos2d::Director::getInstance()->getWinSize();

    cocos2d::Sprite *court = cocos2d::Sprite::create("court.png");
    court->setPosition(cocos2d::Point{
        _screenSize.width * 0.5f,
        _screenSize.height * 0.5f
    });
    addChild(court);

    _player1 = GameSprite::gameSpriteWithFile("mallet.png");
    _player1->setPosition(cocos2d::Point{
        _screenSize.width * 0.5f,
        _player1->radius() * 2
    });
    addChild(_player1);

    _player2 = GameSprite::gameSpriteWithFile("mallet.png");
    _player2->setPosition(cocos2d::Point{
        _screenSize.width * 0.5f,
        _screenSize.height - _player2->radius() * 2
    });
    addChild(_player2);

    _players.pushBack(_player1);
    _players.pushBack(_player2);

    _ball = GameSprite::gameSpriteWithFile("puck.png");
    _ball->setPosition(cocos2d::Point{
        _screenSize.width * 0.5f,
        _screenSize.height * 0.5f - _ball->radius() * 2
    });
    addChild(_ball);

    // create the main loop!
    schedule(schedule_selector(GameLayer::update));

    return true;
}

void GameLayer::update(float dt) {
//    Point ballNextPosition = _ball->getNextPosition();
//    Point ballVector = _ball->getVector();
//    ballVector = ballVector * 0.98f;
//
//    ballNextPosition.x += ballVector.x;
//    ballNextPosition.y += ballVector.y;
//
//    // since we will not be using the sqrt function to find distance because
//    // it is costly, then we have to square everything.
//    float squared_radii = std::pow(_player1->radius() + _ball->radius(), 2);
//    for (const auto& player : _players) {
//        auto playerNextPosition = player->getNextPosition();
//        auto playerVector = player->getVector();
//
//        auto distance1 =
//            std::pow(ballNextPosition.x - player->getPositionX(), 2) +
//            std::pow(ballNextPosition.y - player->getPositionY(), 2);
//        
//        auto distance2 =
//            std::pow(_ball->getPositionX() - playerNextPosition.x, 2) +
//            std::pow(_ball->getPositionY() - playerNextPosition.y, 2);
//        
//        if (distance1 <= squared_radii || distance2 <= squared_radii) {
//            // houston we have a collision...
//        }
//    }
    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
}

void GameLayer::onEnter() {
    cocos2d::Layer::onEnter();

    auto listener = cocos2d::EventListenerTouchAllAtOnce::create();
    
    listener->onTouchesBegan = CC_CALLBACK_2(GameLayer::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(GameLayer::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(GameLayer::onTouchesEnded, this);
    
    auto dispatcher = cocos2d::Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameLayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) {
    cocos2d::Point tap;
    for (const auto& touch : touches) {
        tap = touch->getLocation();
        for (const auto& p : _players) {
            if (p->getBoundingBox().containsPoint(tap)) {
                p->setTouch(touch);
            }
        }
    }
}

void GameLayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) {
    cocos2d::Point tap;
    for (const auto& touch : touches) {
        tap = touch->getLocation();
        for (const auto& p : _players) {
            if (p->getTouch() == touch) {
                Point nextPosition = tap;
                // keep player inside screen
                if (nextPosition.x < p->radius() ) {
                    nextPosition.x = p->radius();
                }

                if (nextPosition.x > _screenSize.width - p->radius()) {
                    nextPosition.x = _screenSize.width - p->radius();
                }
                
                if (nextPosition.y < p->radius()) {
                    nextPosition.y = p->radius();
                }
                
                if (nextPosition.y > _screenSize.height - p->radius()) {
                    nextPosition.y = _screenSize.height - p->radius();
                }
                
                // keep player inside it's court
                // NOTE: why do we only need to check player's y position is less
                // than screen's height? How about when the player's y position is
                // greater than the screen's height?
//                if (p->getPositionY() < _screenSize.height * 0.5f) {
//                    if (nextPosition.y > _screenSize.height * 0.5f - p->radius()) {
//                        nextPosition.y = _screenSize.height * 0.5f - p->radius();
//                    } else {
//                        if (nextPosition.y < _screenSize.height + p->radius()) {
//                            nextPosition.y = _screenSize.height + p->radius();
//                        }
//                    }
//                }

                p->setNextPosition(nextPosition);
                p->setVector(cocos2d::Point{
                    tap.x - p->getPositionX(),
                    tap.y - p->getPositionY()
                });
            }
        }
    }
}

void GameLayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event) {
    for (const auto& touch : touches) {
        for (const auto& p : _players) {
            if (p->getTouch() == touch) {
                p->setTouch(nullptr);
                p->setVector(cocos2d::Point::ZERO);
            }
        }
    }
}
