#include "GameLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace CocosDenshion;

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

    // add score labels
    _player1ScoreLabel = LabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(Point{
        _screenSize.width - 60,
        _screenSize.height * 0.5 - 80
    });
    _player1ScoreLabel->setRotation(90);
    addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = LabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(Point{
        _screenSize.width - 60,
        _screenSize.height * 0.5 + 80
    });
    _player2ScoreLabel->setRotation(90);
    addChild(_player2ScoreLabel);

    // create the main loop!
    schedule(schedule_selector(GameLayer::update));

    return true;
}

void GameLayer::update(float dt) {
    Point ballNextPosition = _ball->getNextPosition();
    Point ballVector = _ball->getVector();
    
    ballVector = ballVector * 0.98f;
    ballNextPosition.x += ballVector.x;
    ballNextPosition.y += ballVector.y;

    // since we will not be using the sqrt function to find distance because
    // it is costly, then we have to square everything.
    float squared_radii = pow(_player1->radius() + _ball->radius(), 2);
    for (const auto& player : _players) {
        auto playerNextPosition = player->getNextPosition();
        auto playerVector = player->getVector();

        auto diffx = ballNextPosition.x - player->getPositionX();
        auto diffy = ballNextPosition.y - player->getPositionY();

        auto distance1 = pow(diffx, 2) + pow(diffy, 2);
        
        auto distance2 =
            pow(_ball->getPositionX() - playerNextPosition.x, 2) +
            pow(_ball->getPositionY() - playerNextPosition.y, 2);
        
        if (distance1 <= squared_radii || distance2 <= squared_radii) {
            auto mag_ball = pow(ballVector.x, 2) + pow(ballVector.y, 2);
            auto mag_player = pow(playerVector.x, 2) + pow(playerVector.y, 2);
            auto force = sqrt(mag_ball + mag_player);
            auto angle = atan2(diffy, diffx);
            
            ballVector.x = force * cos(angle);
            ballVector.y = force * sin(angle);
            
            ballNextPosition.x = playerNextPosition.x + (player->radius() + _ball->radius() + force) * cos(angle);
            ballNextPosition.y = playerNextPosition.y + (player->radius() + _ball->radius() + force) * sin(angle);

            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
         }
    }
    
    if (ballNextPosition.x < _ball->radius()) {
        ballNextPosition.x = _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }

    if (ballNextPosition.x > _screenSize.width - _ball->radius()) {
        ballNextPosition.x = _screenSize.width - _ball->radius();
        ballVector.x *= -0.8f;
        SimpleAudioEngine::getInstance()->playEffect("hit.wav");
    }

    if (ballNextPosition.y < _ball->radius()) {
        if (_ball->getPositionX() < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
            _ball->getPositionX() > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y = _ball->radius();
            ballVector.y *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }
    
    if (ballNextPosition.y > _screenSize.height - _ball->radius()) {
        if (_ball->getPositionX() < _screenSize.width * 0.5f - GOAL_WIDTH * 0.5f ||
            _ball->getPositionX() > _screenSize.width * 0.5f + GOAL_WIDTH * 0.5f) {
            ballNextPosition.y = _screenSize.height - _ball->radius();
            ballVector.y *= -0.8f;
            SimpleAudioEngine::getInstance()->playEffect("hit.wav");
        }
    }

    _ball->setVector(ballVector);
    _ball->setNextPosition(ballNextPosition);

    // check for goal
    if (ballNextPosition.y < -_ball->radius() * 2) {
        playerScore(2);
    }
    
    if (ballNextPosition.y > _screenSize.height + _ball->radius() * 2) {
        playerScore(1);
    }

    _player1->setPosition(_player1->getNextPosition());
    _player2->setPosition(_player2->getNextPosition());
    _ball->setPosition(_ball->getNextPosition());
}

void GameLayer::playerScore(int player) {
    SimpleAudioEngine::getInstance()->playEffect("score.wav");
    _ball->setVector(Point::ZERO);
    
    char buffer[10];
    
    if (player == 1) {
        _player1Score++;
        sprintf(buffer, "%i", _player1Score);
        _player1ScoreLabel->setString(buffer);
        _ball->setNextPosition(Point{
            _screenSize.width * 0.5f,
            _screenSize.height * 0.5f + _ball->radius() * 2
        });
    } else {
        _player2Score++;
        sprintf(buffer, "%i", _player2Score);
        _player2ScoreLabel->setString(buffer);
        _ball->setNextPosition(Point{
            _screenSize.width * 0.5f,
            _screenSize.height * 0.5f - _ball->radius() * 2
        });
    }

    // move players to there original position
    _player1->setPosition(Point{
        _screenSize.width * 0.5f,
        _player1->radius() * 2
    });

    _player2->setPosition(Point{
        _screenSize.width * 0.5f,
        _screenSize.height - _player2->radius() * 2
    });
    
    // clear touch information
    _player1->setTouch(nullptr);
    _player2->setTouch(nullptr);
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
                if (p->getPositionY() < _screenSize.height * 0.5f) {
                    if (nextPosition.y > _screenSize.height * 0.5f - p->radius()) {
                        nextPosition.y = _screenSize.height * 0.5f - p->radius();
                    }
                } else {
                    if (nextPosition.y < _screenSize.height * 0.5f + p->radius()) {
                        nextPosition.y = _screenSize.height * 0.5f + p->radius();
                    }
                }

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
