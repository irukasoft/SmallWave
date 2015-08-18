#include "GameScene.h"
#include "TitleScene.h"

USING_NS_CC;

//イメージの全体的な倍率(px)
const float IMAGE_SCALE = 0.6f;
const float IMAGE_SCALE2 = 0.5f;
const float IMAGE_SCALE_LBL = 0.5f;
const int MAX_BUTTON_SIZE = 10;
const int MAX_TIME = 30;
// ハイスコア格納用のキー
const char* HIGHSCORE_KEY = "highscore_key";
//答え合わせ用
const GameScene::ButtonType KOTAE_LIST[] = {
		GameScene::ButtonType::UP,
		GameScene::ButtonType::UP,
		GameScene::ButtonType::DOWN,
		GameScene::ButtonType::DOWN,
		GameScene::ButtonType::LEFT,
		GameScene::ButtonType::RIGHT,
		GameScene::ButtonType::LEFT,
		GameScene::ButtonType::RIGHT,
		GameScene::ButtonType::B,
		GameScene::ButtonType::A
};
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}
GameScene::GameScene()
: _second(0)
, _state(GameState::END)
, _secondLabel(NULL)
, _gameStateLabel(NULL)
, _buttonImageList(NULL)
, _highScoreLabel(NULL)
{
}
GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(_secondLabel);
	CC_SAFE_RELEASE_NULL(_gameStateLabel);
	CC_SAFE_RELEASE_NULL(_highScoreLabel);
}
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    //////////////////////
    //TODO 背景の追加
//    auto background = LayerColor::create(Color4B::GRAY, winSize.width, winSize.height);
//    this->addChild(background);

    //////////////////////
    //ボタンの設定
    //upボタン
	auto upButton = MenuItemImage::create("main_btn_up_01.png","main_btn_up_02.png",
			[this](Ref* ref){
				if (_state == GameState::END){
					onStartGame();
				}
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::UP);
					check();
				}
			});
    //downボタン
	auto downButton = MenuItemImage::create("main_btn_down_01.png","main_btn_down_02.png",
			[this](Ref* ref){
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::DOWN);
					check();
				}
			});
    //leftボタン
	auto leftButton = MenuItemImage::create("main_btn_left_01.png","main_btn_left_02.png",
			[this](Ref* ref){
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::LEFT);
					check();
				}
			});
    //rightボタン
	auto rightButton = MenuItemImage::create("main_btn_right_01.png","main_btn_right_02.png",
			[this](Ref* ref){
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::RIGHT);
					check();
				}
			});
    //aボタン
	auto aButton = MenuItemImage::create("main_btn_a_01.png","main_btn_a_02.png",
			[this](Ref* ref){
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::A);
					if (check()){
						onEndGame(true);
					}
				}
			});
    //bボタン
	auto bButton = MenuItemImage::create("main_btn_b_01.png","main_btn_b_02.png",
			[this](Ref* ref){
				if (_state == GameState::PLAYING){
					setButtonImage(ButtonType::B);
					check();
				}
			});
    //startボタン
	auto menuButton = MenuItemImage::create("main_btn_menu_01.png","main_btn_menu_02.png",
			[this](Ref* ref){
				onMenuGame();
			});
    //resetボタン
	auto resetButton = MenuItemImage::create("main_btn_reset_01.png","main_btn_reset_02.png",
			[this](Ref* ref){
				onResetGame();
			});

    //////////////////////
	//画面表示の調整

	//サイズ変更
	upButton->setScale(IMAGE_SCALE);
	downButton->setScale(IMAGE_SCALE);
	leftButton->setScale(IMAGE_SCALE);
	rightButton->setScale(IMAGE_SCALE);
	aButton->setScale(IMAGE_SCALE);
	bButton->setScale(IMAGE_SCALE);
	menuButton->setScale(IMAGE_SCALE2);
	resetButton->setScale(IMAGE_SCALE2);
	//メニューを作成する
	auto upDownMenu = Menu::create(upButton, downButton, NULL);
	auto leftRightMenu = Menu::create(leftButton, rightButton, NULL);
	auto baMenu = Menu::create(bButton, aButton, NULL);
	auto menuMenu = Menu::create(resetButton, menuButton, NULL);

	//padding
	upDownMenu->alignItemsVerticallyWithPadding(1);
	leftRightMenu->alignItemsHorizontallyWithPadding(1);
	baMenu->alignItemsHorizontallyWithPadding(15);
	menuMenu->alignItemsVerticallyWithPadding(10);

	//センターを指定してそこから左右に配置
	auto centerPosition = Vec2(winSize.width / 2.0, winSize.height /2.0);
	auto menuPosition = Vec2(
			centerPosition.x,
			10 * IMAGE_SCALE + menuButton->getContentSize().height + 5);
	auto zyuziPosition  = Vec2(
			centerPosition.x
				- (rightButton->getContentSize().width * IMAGE_SCALE)
				- ((menuButton->getContentSize().width * IMAGE_SCALE2) / 2 )
				- ((menuButton->getContentSize().width * IMAGE_SCALE2) / 3 ),//マージン
			10 * IMAGE_SCALE + (downButton->getContentSize().height * IMAGE_SCALE ) + 10 );
	auto abPosition = Vec2(
			centerPosition.x
				+ ((bButton->getContentSize().width * IMAGE_SCALE * 2) + 15) /2
				+ ((menuButton->getContentSize().width * IMAGE_SCALE2) / 2 )
				+ ((menuButton->getContentSize().width * IMAGE_SCALE2) / 3 ),//マージン
			10 * IMAGE_SCALE + menuButton->getContentSize().height + 5);
	menuMenu->setPosition(menuPosition);
	upDownMenu->setPosition(zyuziPosition);
	leftRightMenu->setPosition(zyuziPosition);
	baMenu->setPosition(abPosition);

    ///////////////////////////////////////////
    //タイマーラベルの追加
    auto secondLabel = Label::createWithSystemFont(
    		StringUtils::format("%.3lf", _second),
    		"Marker Felt",
    		40);
    this->setSecondLabel(secondLabel);
    secondLabel->setColor(Color3B::YELLOW);
    secondLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    secondLabel->enableOutline(Color4B::BLACK, 1.5);
    secondLabel->setPosition(Vec2(
    		abPosition.x,
    		winSize.height / 2.0 + 60));
    this->addChild(secondLabel);
    ///////////////////////////////////////////
    //ハイスコアラベルの追加
    auto highScore = UserDefault::getInstance()->getDoubleForKey(HIGHSCORE_KEY);

    auto highScoreLabel = Label::createWithSystemFont(
    		StringUtils::format("%.3lf", highScore),
    		"Marker Felt",
    		40);
    this->setHighScoreLabel(highScoreLabel);
    highScoreLabel->setColor(Color3B::YELLOW);
    highScoreLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    highScoreLabel->enableOutline(Color4B::BLACK, 1.5);
    highScoreLabel->setPosition(Vec2(
    		abPosition.x,
    		winSize.height / 2.0));
    this->addChild(_highScoreLabel);
    ///////////////////////////////////////////
    //ゲーム状態ラベルの追加
    auto gameStateLabel = Label::createWithSystemFont(
    		"---",
    		"Marker Felt",
    		40);
    this->setGameStateLabel(gameStateLabel);
    gameStateLabel->setColor(Color3B::YELLOW);
    gameStateLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    gameStateLabel->enableOutline(Color4B::BLACK, 1.5);
    gameStateLabel->setPosition(Vec2(
    		abPosition.x - (40 * 3.1),
    		winSize.height / 2.0 + 60));
    this->addChild(gameStateLabel);

    ///////////////////////////////////////////
    //ハイスコア文字の追加
    auto highScoreStgLabel = Label::createWithSystemFont(
    		"HIGH SCORE",
    		"Marker Felt",
    		40);
    highScoreStgLabel->setColor(Color3B::YELLOW);
    highScoreStgLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    highScoreStgLabel->enableOutline(Color4B::BLACK, 1.5);
    highScoreStgLabel->setPosition(Vec2(
    		abPosition.x - (40 * 4.6),
    		winSize.height / 2.0));
    this->addChild(highScoreStgLabel);


	//画面に追加
	this->addChild(upDownMenu);
	this->addChild(leftRightMenu);
	this->addChild(baMenu);
	this->addChild(menuMenu);

    //////////////////////
    //updateを毎フレーム実行するように登録
    this->scheduleUpdate();

    return true;
}

/**
 * 時間計測メソッド
 */
double GameScene::getSec()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

/**
 * ゲームスタートメソッド
 */
void GameScene::onStartGame(){
	_state = GameState::PLAYING;	//ゲーム中ステータスへ
	_startTime = getSec();			//開始時刻をセット
	_second = 0;					//経過時間をリセット
	_gameStateLabel->setString("START");//ラベルをスタートへ
	//スタートするとラベルログをクリア
	deleteButtonLbl();
}

/**
 * ゲームエンドメソッド
 */
void GameScene::onEndGame(bool isHigh){
	_state = GameState::END;		//ゲームENDステータスへ
	_gameStateLabel->setString("STOP");//ラベルを終了へ

	//ハイスコアを更新していた場合は上書きする
	if (isHigh){
		auto highScore = UserDefault::getInstance()->getDoubleForKey(HIGHSCORE_KEY);
		if (highScore <= 0.0) {
			//初回は必ず保存
			log ("その１");
	    	UserDefault::getInstance()->setDoubleForKey(HIGHSCORE_KEY, _second);
	        _highScoreLabel->setString(StringUtils::format("%.3lf", _second));
		}else if (_second > 0.0 && _second < highScore) {
	    	//更新
			log ("その２");
	    	UserDefault::getInstance()->setDoubleForKey(HIGHSCORE_KEY, _second);
	        _highScoreLabel->setString(StringUtils::format("%.3lf", _second));
	    }

	}
}

/**
 * ゲームリセット
 */
void GameScene::onResetGame(){
	onEndGame(false);
	_second = 0;
    _secondLabel->setString(StringUtils::format("%.3lf", _second));
    _gameStateLabel->setString("---");
	deleteButtonLbl();
}

/**
 * メニューに戻る
 */
void GameScene::onMenuGame(){
	onEndGame(false);
	deleteButtonLbl();
	auto scene = TitleScene::createScene();
	auto transition = TransitionFade::create(0.5, scene);
	Director::getInstance()->replaceScene(transition);
}

/**
 *
 */
void GameScene::setButtonImage(ButtonType type){
	auto winSize = Director::getInstance()->getWinSize();
	//サイズが１０コを超えていた場合は先頭から削除
	if (_buttonImageList.size() >= MAX_BUTTON_SIZE) {
		auto tmpObject = _buttonImageList.front();
		tmpObject->removeFromParent();
		_buttonImageList.eraseObject(tmpObject);
	}

	if (_buttonImageList.size() > 0) {
		int count = _buttonImageList.size();
    	log("===========");
		for (auto &it : _buttonImageList){
			//移動する
			it->setPosition(Vec2(
					 winSize.width
					 	 - 100
					 	 - (count * it->getContentSize().width * IMAGE_SCALE_LBL),
					 winSize.height - 100));
	    	log("count %d at (%f,%f)",count, it->getPosition().x,it->getPosition().y);
	    	count--;
		}
	}
	//現在押されたボタンを追加
	std::string filename;
	int filetag;
	switch(type){
	 case ButtonType::UP:
		 filename = "main_lbl_up.png";
		 filetag = static_cast<int>(ButtonType::UP);
		 break;
	 case ButtonType::DOWN:
		 filename = "main_lbl_down.png";
		 filetag = static_cast<int>(ButtonType::DOWN);
		 break;
	 case ButtonType::LEFT:
		 filename = "main_lbl_left.png";
		 filetag = static_cast<int>(ButtonType::LEFT);
		 break;
	 case ButtonType::RIGHT:
		 filename = "main_lbl_right.png";
		 filetag = static_cast<int>(ButtonType::RIGHT);
		 break;
	 case ButtonType::A:
		 filename = "main_lbl_a.png";
		 filetag = static_cast<int>(ButtonType::A);
		 break;
	 case ButtonType::B:
		 filename = "main_lbl_b.png";
		 filetag = static_cast<int>(ButtonType::B);
		 break;
	 }
	 auto lblButton = Sprite::create(filename);
	 lblButton->setPosition(Vec2(
			 winSize.width - 100,
			 winSize.height - 100));
	 lblButton->setScale(IMAGE_SCALE_LBL);
	 lblButton->setTag(filetag);
	 this->addChild(lblButton);
	 _buttonImageList.pushBack(lblButton);

}


//判定処理
bool GameScene::check(){
	int count = 0;
	for (auto &it : _buttonImageList){
		ButtonType type = static_cast<ButtonType>(it->getTag());
		if (KOTAE_LIST[count] != type) {
			//不適合
			break;
		 }
		count++;
	}
	if (count == MAX_BUTTON_SIZE) {
		return true;
	}
	return false;
}

/**
 * ラベル削除メソッド
 */
void GameScene::deleteButtonLbl(){
	int size = _buttonImageList.size();
	for( int i=0; i<size; i++ ) {
		auto tmpObject = _buttonImageList.front();
		tmpObject->removeFromParent();
		_buttonImageList.eraseObject(tmpObject);
	}
}

////////////////////
/**
 * 毎フレーム処理
 **/
void GameScene::update(float dt){
	if (_state == GameState::PLAYING){
		//ゲームがスタートしていたらタイム計測
		_second = getSec() - _startTime;
	    _secondLabel->setString(StringUtils::format("%.3lf", _second));

	    //タイムアウト計測
	    if (_second > MAX_TIME) {
	    	onEndGame(false);
	    }

	}
}
