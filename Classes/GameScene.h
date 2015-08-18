#ifndef __KawazCatch__GameScene__
#define __KawazCatch__GameScene__

#include "cocos2d.h"
//#include <random>

class GameScene :public cocos2d::Layer
{
protected:
	GameScene();
	virtual ~GameScene();
	bool init() override;

public:
    ///////////////////////////////////
    /// ゲームの状態を表します
    enum class GameState {
    	PLAYING,
    	END
    };
    ///////////////////////////////////
    /// ボタンのタイプを表します
    enum class ButtonType
    {
    	UP,
    	DOWN,
    	LEFT,
    	RIGHT,
    	A,
    	B
    };

    static cocos2d::Scene* createScene();
	CREATE_FUNC(GameScene);
    void update(float dt);

    //開始時間
    CC_SYNTHESIZE(double, _startTime, StartTime);
    //経過時間
    CC_SYNTHESIZE(double, _second, Second);
    //ゲームステータス
    CC_SYNTHESIZE(GameState, _state, State);
    //ボタンイメージリスト実体
    CC_SYNTHESIZE(cocos2d::Vector<cocos2d::Sprite *>, _buttonImageList, ButtonImageList);
    //経過時間ラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _secondLabel, SecondLabel);
    //ハイスコアラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _highScoreLabel, HighScoreLabel);
    //ゲーム状態ラベル
    CC_SYNTHESIZE_RETAIN(cocos2d::Label *, _gameStateLabel, GameStateLabel);


private:
    //計測
    double getSec();
    /**
     * ゲームが開始された時に呼ぶメソッド
     */
    void onStartGame();
    /**
     * ゲームが終了した時に呼ぶメソッド
     */
    void onEndGame(bool isHigh);
    /**
     * ゲームをリセットするメソッド
     */
    void onResetGame();
    /**
     * タイトルに戻るメソッド
     */
    void onMenuGame();

    /**
     * ボタンをラベルに表示
     */
    void setButtonImage(ButtonType type);

    /**
     * チェック
     */
    bool check();

    /**
     * リスト削除メソッド
     */
    void deleteButtonLbl();
};

#endif /* defined(__KawazCatch__GameScene__) */
