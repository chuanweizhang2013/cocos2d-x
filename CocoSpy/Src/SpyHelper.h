
#ifndef _SPY_H_
#define _SPY_H_

#include "cocos2d.h"
#include "CCDirector.h"
#include "jsoncpp/include/value.h"

USING_NS_CC;

using namespace std;
class CSpyHelper
{

public:
	static CSpyHelper& sharedHelper();
	void findNodeByPoint(int x, int y,vector<CCNode*>& vecNode);
	string getNodeClassName(CCNode* node);

	CCLayer* getLayerByNode(CCNode* node);
	CCScene* getSceneByNode(CCNode* node);

private:
	void getNodeAtPos(CCNode* rootNode, int x, int y,vector<CCNode*>& vecNode);
};

void Split(const std::string &strText, std::vector<std::string> &vecString, const char *pszSplitChar);

bool TestVecStringEmpty(std::vector<std::string> &vecString);

#endif