#include "TitleScene.h"
#include "GameScene.h"
#include "InfoScene.h"

USING_NS_CC;
const float IMAGE_SCALE = 0.5f;
const float IMAGE_SCALE_LOGO = 0.8f;

Scene* TitleScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}
TitleScene::TitleScene()
{
}
TitleScene::~TitleScene()
{
}
bool TitleScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    //Dierctorを取り出し
    auto director = Director::getInstance();
    //画像サイズ取り出し
    auto winSize = director->getWinSize();

    //ロゴの指定
    auto titleLogo = Sprite::create("title_logo.png");
    titleLogo->setScale(IMAGE_SCALE_LOGO);
    titleLogo->setPosition(Vec2(
    		(titleLogo->getContentSize().width * IMAGE_SCALE_LOGO) / 2.0 + 30,
    		winSize.height - (titleLogo->getContentSize().height * IMAGE_SCALE_LOGO)/2.0 - 30));
	this->addChild(titleLogo);

    //Startボタン
	auto startButton = MenuItemImage::create(
			"title_btn_start_01.png",
			"title_btn_start_02.png",
			[](Ref* ref){
				auto scene = GameScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);
			});

    //Infoボタン
	auto infoButton = MenuItemImage::create(
			"title_btn_info_01.png",
			"title_btn_info_02.png",
			[](Ref* ref){
				auto scene = InfoScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);
			});
	startButton->setScale(IMAGE_SCALE);
	infoButton->setScale(IMAGE_SCALE);

	//２つのボタンからメニューを作成する
	auto menu = Menu::create(startButton, infoButton, NULL);
	//ボタンを縦に並べる
	menu->alignItemsVerticallyWithPadding(15);
	menu->setPosition(Vec2(
			(startButton->getContentSize().width * IMAGE_SCALE) / 2.0 + 30,
			(startButton->getContentSize().height * IMAGE_SCALE)  + 30));
	this->addChild(menu);


    //ロゴの追加
//    auto logo = Sprite::create("title_logo.png");
//    logo->setPosition(Vec2(winSize.width / 2.0, winSize.height - 150));
//    this->addChild(logo);



    return true;

}
