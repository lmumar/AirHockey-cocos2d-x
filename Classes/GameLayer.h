#ifndef __GAMELAYER_SCENE_H__
#define __GAMELAYER_SCENE_H__

#include "cocos2d.h"
#include "GameSprite.h"

#define GOAL_WIDTH 400

class GameLayer : public cocos2d::Layer {
public:

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void playerScore(int player);
    
    void update(float dt);

    CREATE_FUNC(GameLayer);

    virtual void onEnter() override;

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event *unused_event);

private:

    GameSprite *_player1;
    GameSprite *_player2;
    GameSprite *_ball;
    
    cocos2d::LabelTTF *_player1ScoreLabel;
    cocos2d::LabelTTF *_player2ScoreLabel;
    
    cocos2d::Vector<GameSprite *> _players;
    
    cocos2d::Size _screenSize;
    
    int _player1Score = 0;
    int _player2Score = 0;
};

#endif
