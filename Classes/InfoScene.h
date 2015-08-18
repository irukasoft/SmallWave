#ifndef __SmallWave__InfoScene__
#define __SmallWave__InfoScene__

#include "cocos2d.h"
//#include <random>

class InfoScene :public cocos2d::Layer
{
protected:
	InfoScene();
	virtual ~InfoScene();
	bool init() override;

public:
	static cocos2d::Scene* createScene();
	CREATE_FUNC(InfoScene);

};

#endif /* defined(__SmallWave__InfoScene__) */
