
#include "SpyHelper.h"

CSpyHelper& CSpyHelper::sharedHelper()
{
	static CSpyHelper spyHelper;
	return spyHelper;
}



void CSpyHelper::findNodeByPoint(int x, int y,vector<CCNode*>& vecNode)
{
	CCPoint touchPt = CCEGLView::sharedOpenGLView()->screenPt2TouchPt(ccp(x,y));

	CCScene *pRootNode = CCDirector::sharedDirector()->getRunningScene();
	if(NULL==pRootNode)
		return ;

	if (touchPt.x < 0 || touchPt.x > CCDirector::sharedDirector()->getWinSize().width ||
		touchPt.y < 0 || touchPt.y > CCDirector::sharedDirector()->getWinSize().height)
		return ;

	getNodeAtPos(pRootNode, touchPt.x, touchPt.y,vecNode);
	return;
}

void CSpyHelper::getNodeAtPos(CCNode* rootNode, int x, int y,vector<CCNode*>& vecNode)
{
	// CCLabelBMFont and CCMenuItem's children needn't be selected
// 	if (! dynamic_cast<CCLabelBMFont*>(rootNode) &&
// 		! dynamic_cast<CCMenuItem*>(rootNode))
// 	{
	CCArray* childrenArray = rootNode->getChildren();
	if (childrenArray && childrenArray->count() > 0)
	{
		CCNode *pChildNode = NULL, *retNode = NULL;
		for (int i = childrenArray->count()-1; i >= 0; i--)
		{
			pChildNode = (CCNode*) childrenArray->objectAtIndex(i);

			if (! pChildNode)
			{
				CCLog("a child node has been destroyed unexpectedly");
				break;
			}

			getNodeAtPos(pChildNode, x, y,vecNode);
// 			if (retNode)
// 				return retNode;
		}
	}
//	}

	CCPoint touchLocation = CCDirector::sharedDirector()->convertToGL(CCPoint(x,y));
	CCPoint local = rootNode->convertToNodeSpace(touchLocation);
	CCRect boxRect=CCRectMake(rootNode->getPositionX() - rootNode->getContentSize().width * rootNode->getAnchorPoint().x,
		rootNode->getPositionY() - rootNode->getContentSize().height * rootNode->getAnchorPoint().y,
		rootNode->getContentSize().width, rootNode->getContentSize().height);
	boxRect.origin = CCPointZero;
	if (boxRect.containsPoint(local))
	{
		CCSprite *pSprite = dynamic_cast<CCSprite*>(rootNode);
		if (pSprite)
			vecNode.push_back(rootNode);
	}
		
/*
	bool ret = rootNode->boundingBox().containsPoint(CCDirector::sharedDirector()->convertToGL(CCPoint(x,y)));
	float WinSizewidth = CCDirector::sharedDirector()->getWinSize().width;
	float WinSizeheight = CCDirector::sharedDirector()->getWinSize().height;
	if(ret && rootNode->boundingBox().size.width<WinSizewidth && rootNode->boundingBox().size.height<WinSizeheight)
		vecNode.push_back(rootNode);
*/

////////************
// 	CCPoint point;
// 	CCSize size = rootNode->getContentSize();
// 	point = rootNode->convertToNodeSpace(CCPoint(x, y));
// 
// 	// is the cursor under rootNode ?
// 	if (point.x > 0 && point.y > 0 && point.x < size.width && point.y < size.height)
// 		vecNode.push_back(rootNode);
////*****************
	return ;
}

CCLayer* CSpyHelper::getLayerByNode(CCNode* node)
{
	if (NULL==node)
		return NULL;

	CCLayer *layer = dynamic_cast<CCLayer*>(node);
	if (layer)
		return layer;
	return getLayerByNode(node->getParent());
}

CCScene* CSpyHelper::getSceneByNode(CCNode* node)
{
	if (NULL==node)
		return NULL;

	CCScene *pScene = dynamic_cast<CCScene*>(node);
	if (pScene)
		return pScene;
	return getSceneByNode(node->getParent());
}

string CSpyHelper::getNodeClassName(CCNode* node)
{
	if(!node)
		return "";
	string strClassName = typeid( *node ).name();
	string strkey="class";
	int pos=strClassName.find(strkey.c_str());
	strClassName=strClassName.substr(pos+strkey.length()+1,strClassName.length());
	return strClassName;
}

void Split(const std::string &strText, std::vector<std::string> &vecString, const char *pszSplitChar)
{
	if (!pszSplitChar) {
		return;
	}


	int nSplitCharLen = strlen(pszSplitChar);
	if (0 == nSplitCharLen) {
		return;
	}

	vecString.clear();
	char szDestribe[1024] = { 0 };
	strncpy(szDestribe, strText.c_str(), sizeof(szDestribe) - 1);

	const char *pBeg = szDestribe;
	const char *pPos = strstr(pBeg, pszSplitChar);

	while (pPos) {
		char szTemp[256] = { 0 };

		strncpy(szTemp, pBeg, __min((size_t)(pPos - pBeg), sizeof(szTemp) - 1));
		vecString.push_back(szTemp);
		pBeg = pPos + nSplitCharLen;
		pPos = strstr(pPos + nSplitCharLen, pszSplitChar);
	}

	if (pBeg!=NULL&&strcmp(pBeg,"")!=0)
	{
		vecString.push_back(pBeg);
	}
}

bool TestVecStringEmpty(std::vector<std::string> &vecString)
{
	bool breturn=false;
	int size=vecString.size();
	for(int i=0;i<size;++i)
	{
		if(vecString[i].empty())	
		{
			breturn = true;
			break;
		}
	}
	return breturn;		
}