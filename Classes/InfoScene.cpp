#include "InfoScene.h"
#include "TitleScene.h"

USING_NS_CC;
const float IMAGE_SCALE = 0.5f;
const float IMAGE_SCALE_LOGO = 0.8f;

Scene* InfoScene::createScene()
{
    auto scene = Scene::create();
    auto layer = InfoScene::create();
    scene->addChild(layer);
    return scene;
}
InfoScene::InfoScene()
{
}
InfoScene::~InfoScene()
{
}
bool InfoScene::init()
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
    auto titleLogo = Sprite::create("info_logo.png");
    titleLogo->setScale(IMAGE_SCALE_LOGO);
    titleLogo->setPosition(Vec2(
    		(titleLogo->getContentSize().width * IMAGE_SCALE_LOGO) / 2.0 + 30,
    		winSize.height - (titleLogo->getContentSize().height * IMAGE_SCALE_LOGO)/2.0 - 30));
	this->addChild(titleLogo);

    //Menuボタン
	auto menuButton = MenuItemImage::create(
			"info_btn_menu_01.png",
			"info_btn_menu_02.png",
			[](Ref* ref){
				//もう一度遊ぶボタンを押した時の処理
				auto scene = TitleScene::createScene();
				auto transition = TransitionFade::create(0.5, scene);
				Director::getInstance()->replaceScene(transition);
			});
	menuButton->setScale(IMAGE_SCALE);
	auto menu = Menu::create(menuButton, NULL);

	menu->setPosition(Vec2(
			(menuButton->getContentSize().width * IMAGE_SCALE) / 2.0 + 30,
			(menuButton->getContentSize().height * IMAGE_SCALE) /2.0 + 30));
	this->addChild(menu);

    ///////////////////////////////////////////
    //説明文字の追加
	std::string infoMessage ="●ゲームの説明●\n";
	infoMessage +="\n";
	infoMessage +="このゲームはただ「↑ ↑ ↓ ↓ ← → ← → B A」と打ち込んで、タイムを計測するだけのアプリです\n";
	infoMessage +="ゲームのスタートは十字キーの「↑」ボタンを押すとスタートします。\n";
	infoMessage +="ゲームの終了はコマンドを最後まで打ち込むか、RESETボタンを押すことです。\n";
	infoMessage +="また３０秒をすぎるとタイムオーバーでストップします。\n";
	infoMessage +="\n";
	infoMessage +="ご意見があればストア本体にコメントお願いしますー\n";
	infoMessage +="こんなくだらない思いつきだけなアプリを・・・とか言わないでください^^;\n";
    auto messageLabel = Label::createWithSystemFont(
    		infoMessage,
    		"Marker Felt",
    		16);
    messageLabel->enableShadow(Color4B::BLACK, Size(0.5, 0.5), 3);
    messageLabel->enableOutline(Color4B::BLACK, 1.5);
    messageLabel->setPosition(Vec2(
    		winSize.width / 2.0,
    		winSize.height / 2.0));
	this->addChild(messageLabel);

	return true;

}
