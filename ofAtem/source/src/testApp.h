// (c) 2012 telematique
// www.telematique.de
// telematique@web.de


#pragma once

#include "ofxXmlSettings.h"
#include "ofxOsc.h"

#include "ofMain.h"

#include "BMDSwitcherAPI.h"
#include <list>

#include "MixEffectBlockMonitor.h"
#include "InputMonitor.h"
#include "SwitcherMonitor.h"



#define NUM_MSG_STRINGS 40
#define HOST "localhost"
#define PORT 3333


class MixEffectBlockMonitor;
class SwitcherMonitor;
class InputMonitor;
class MediaPlayerMonitor;

class SwitcherInputAux;


class testApp : public ofBaseApp{
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	// xml settings
	ofxXmlSettings XML;
	
	// osc 
	string oscSendIp;
	string atemIp;
	int oscReceivePort;
	int oscSendPort;
	ofxOscSender sender;
	ofxOscSender senderInternal;
	ofxOscReceiver receiver;
	ofTrueTypeFont font;
	int current_msg_string;
	string msg_strings[NUM_MSG_STRINGS];
	float timers[NUM_MSG_STRINGS];
	string oscAddress;
	string oscMessage;
	string infoMessage;
	string oscMessageString;
	float oscMessageFloat;
	int	oscMessageInt;
	int oscMessageInt2;
	
	
	/////////////////////////////////	
	///////////    ATEM    //////////
	/////////////////////////////////
	
	IBMDSwitcherDiscovery*		mSwitcherDiscovery;
	IBMDSwitcher*				mSwitcher;
	IBMDSwitcherMixEffectBlock*	mMixEffectBlock;
	

	
	MixEffectBlockMonitor*		mMixEffectBlockMonitor;
	SwitcherMonitor*			mSwitcherMonitor;
	std::list<InputMonitor*>	mInputMonitors;
	std::vector<IBMDSwitcherInputAux*>	mSwitcherInputAuxList;
	std::list<BMDSwitcherInputId*>	mInputIds;
	
	IBMDSwitcherTransitionParameters* mTransitionStyleParameters;
	
	bool						mMoveSliderDownwards;
	bool						mCurrentTransitionReachedHalfway;
	
	void setIP();
	void initSwitch();
	void switcherDisconnected();
	void switcherConnected();
	void updatePopupButtonItems();
	void updateTransitionFramesTextField();
	void updateFTBFramesTextField();
	void updateProgramButtonSelection();
	void getProgramInputFromexternal();
	void updatePreviewButtonSelection();
	void programPopupChanged(int myID);
	void previewPopupChanged(int myID);
	void sliderChanged(float myPos);
	
	int getProgramInput();
	int getPreviewInput();
	float getTransitionPosition();
	int64_t getTransitionFrames();
	int64_t getFTBFrames();
	int64_t getFTBRate();
	int getAuxCount();
	int getAuxSource(int auxNum);
	bool isInTransition();
	void setTransitionStyle(string style);
	int connectionStatus;
	int inputCount;
	void setAutoRate(uint32_t frames);
	void getAutoRate();
	void testGetFTBRate();

	//////////////////////////////
	/////////  TOOLS  ////////////
	//////////////////////////////
	string getCStringFromCFString(CFStringRef myString);
	void sendOscInt(string oscMsg, int oscArg);
	void sendOscFloat(string oscMsg, float oscArg);
	void sendOscString(string oscMsg, string oscArg);
private:
	
	

};






