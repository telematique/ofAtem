// (c) 2012 telematique
// www.telematique.de
// telematique@web.de


#include "testApp.h"

#include <stdio.h>
#include <iostream>

using namespace std;


//--------------------------------------------------------------
void testApp::setup(){
	inputCount = 1;
	connectionStatus = 0;
	ofBackground(40,40,40);
	XML.loadFile("settings.xml");
	sender.setup(HOST, PORT);
	oscSendIp = XML.getValue("oscSendIp", "default", 0);
	atemIp = XML.getValue("atemIp", "default", 0);
	oscSendPort = XML.getValue("oscSendPort", 0, 0);
	oscReceivePort = XML.getValue("oscReceivePort", 0, 0);
	receiver.setup(oscReceivePort);
	senderInternal.setup("127.0.0.1", oscReceivePort);
	current_msg_string = 0;
	initSwitch();
	setIP();
}

void testApp::draw(){
	
	ofSetColor( ofColor(0, 0, 0 )); 
	if ( connectionStatus == 1 ){
		ofSetColor( ofColor(0, 255, 0 )); 
	}
	else {
		ofSetColor( ofColor(255, 0, 0 )); 
	}
	ofRect(295,6,20,20);
	ofSetColor( ofColor(211, 211, 211 )); 
	ofDrawBitmapStringHighlight( "ofAtem 0.0.1 by telematique", 10 , 20, ofColor(211, 211, 211 ) , ofColor(0, 0, 0 ) );
	ofDrawBitmapString( infoMessage, 10 , 45);
	ofDrawBitmapString( oscAddress, 10 , 60);
}


void testApp::update(){
	// osc 
	while(receiver.hasWaitingMessages()){
		// get the next osc message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
		oscAddress = m.getAddress();
		
		
		//////////   get functions   ///////////
		
		if (oscAddress == "/atem/getInputNames"){
			updatePopupButtonItems();
		}
		
		if (oscAddress == "/atem/getProrgamInputId"){
			int theSourceId;
			theSourceId = getProgramInput();
			
		}
		
		if (oscAddress == "/atem/getPreviewInputId"){
			int theSourceId;
			theSourceId = getPreviewInput();
			
		}
		
		if (oscAddress == "/atem/getTransitionFrames"){
			getTransitionFrames();
			
		}
		
		if (oscAddress == "/atem/getFTBFrames"){
			getFTBFrames();
			
		}
		
		if (oscAddress == "/atem/getFTBRate"){
			getFTBRate();
			
		}
		
		if (oscAddress == "/atem/isInTransition"){
			isInTransition();
			
		}
		
		if (oscAddress == "/atem/getAuxCount"){
			getAuxCount();
		}
		
		if (oscAddress == "/atem/getAuxSource"){
			oscMessageInt = m.getArgAsInt32(0);
			getAuxSource(oscMessageInt);
		}
		
		if (oscAddress == "/atem/getAutoRate"){
			getAutoRate();
		}
		
		
		//////////   set functions   ///////////
		
		if (oscAddress == "/atem/cut"){
			mMixEffectBlock->PerformCut();
		}
		
		if (oscAddress == "/atem/auto"){
			mMixEffectBlock->PerformAutoTransition();
		}
		
		if (oscAddress == "/atem/fadeToblack"){
			mMixEffectBlock->PerformFadeToBlack();
		}
		
		if (oscAddress == "/atem/mix"){
			oscMessageFloat = m.getArgAsFloat(0);
			sliderChanged(oscMessageFloat);
		}
		
		if (oscAddress == "/atem/program"){
			oscMessageInt = m.getArgAsInt32(0);
			programPopupChanged(oscMessageInt);
		}
		
		if (oscAddress == "/atem/preview"){
			oscMessageInt = m.getArgAsInt32(0);
			previewPopupChanged(oscMessageInt);
		}
		
		if (oscAddress == "/atem/FTBRate"){
			oscMessageInt = m.getArgAsInt32(0);
			mMixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdFadeToBlackRate, oscMessageInt);
		}
		
		if (oscAddress == "/atem/FTBRate"){
			oscMessageInt = m.getArgAsInt32(0);
			mMixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdFadeToBlackRate, oscMessageInt);
		}
		
		if (oscAddress == "/atem/auxSource"){
			oscMessageInt = m.getArgAsInt32(0);
			oscMessageInt2 = m.getArgAsInt32(1);
			mSwitcherInputAuxList[oscMessageInt]->SetInputSource(oscMessageInt2);
		}
		
		if (oscAddress == "/atem/transitionStyle"){
			oscMessageString = m.getArgAsString(0);
			setTransitionStyle(oscMessageString);
		}
		if (oscAddress == "/atem/setAutoRate"){
			oscMessageInt = m.getArgAsInt32(0);
			setAutoRate(oscMessageInt);
		}
		
		
		oscAddress = "osc: " + oscAddress ;
	}
}


void testApp::exit(){
	cout << "APP: STOP" << endl;
	switcherDisconnected();
	mSwitcherDiscovery = NULL;
	
}

//////////////  ATEM classes   ////////////////


void testApp::initSwitch()
{
	mSwitcherDiscovery = NULL;
	mSwitcher = NULL;
	mMixEffectBlock = NULL;
	
	mSwitcherMonitor = new SwitcherMonitor(this);
	mMixEffectBlockMonitor = new MixEffectBlockMonitor(this);
	
	this->mSwitcherDiscovery = CreateBMDSwitcherDiscoveryInstance();
	
	if (! mSwitcherDiscovery)
	{
		infoMessage = "atem status: Could not create Switcher Discovery Instance.\nATEM Switcher Software may not be installed.";
		cout << "Could not create Switcher Discovery Instance.\nATEM Switcher Software may not be installed." << endl;
	}
	
	
	switcherDisconnected();
}


void testApp::setIP()
{
	BMDSwitcherConnectToFailure			failReason;
	
	CFStringRef cf;
	cf = CFStringCreateWithCString(kCFAllocatorDefault,atemIp.c_str(),kCFStringEncodingMacRoman);
	
	HRESULT hr = mSwitcherDiscovery->ConnectTo(cf, &mSwitcher, &failReason);
	
	if (SUCCEEDED(hr))
	{
		connectionStatus = 1;
		cout << "NET CONNECTION OK" << endl;
		infoMessage = "atem status: NET CONNECTION OK";
		switcherConnected();
	}
	else
		
	{
		connectionStatus = 0;
		cout << "NET CONNECTION ERROR: ";
		CFStringRef* reason;
		switch (failReason)
		{
			case bmdSwitcherConnectToFailureNoResponse:
				cout << "No response from Switcher"<< endl;
				infoMessage = "atem status: No response from Switcher";
				break;
			case bmdSwitcherConnectToFailureIncompatibleFirmware:
				cout << "Switcher has incompatible firmware" << endl;
				infoMessage = "atem status: Switcher has incompatible firmware";
				break;
			default:
				cout << "Connection failed for unknown reason"<< endl;
				infoMessage = "atem status: Connection failed for unknown reason";
		}
	}
}


void testApp::switcherDisconnected()
{
	// cleanup resources created when switcher was connected
	for (std::list<InputMonitor*>::iterator it = mInputMonitors.begin(); it != mInputMonitors.end(); ++it)
	{
		(*it)->Release();
	}
	mInputMonitors.clear();
	
	if (mMixEffectBlock)
	{
		mMixEffectBlock->RemoveCallback(mMixEffectBlockMonitor);
		mMixEffectBlock->Release();
		mMixEffectBlock = NULL;
	}
	
	if (mSwitcher)
	{
		mSwitcher->RemoveCallback(mSwitcherMonitor);
		
		mSwitcher->Release();
		mSwitcher = NULL;
	}
	
	while (mSwitcherInputAuxList.size())
	{
		mSwitcherInputAuxList.back()->Release();
		mSwitcherInputAuxList.pop_back();
	}
}


void testApp::switcherConnected()
{
	HRESULT result;
	IBMDSwitcherMixEffectBlockIterator* iterator = NULL;
	IBMDSwitcherInputIterator* inputIterator = NULL;
	
	CFStringRef productName;
	CFStringRef ID;
	
	if (FAILED(mSwitcher->GetString(bmdSwitcherPropertyIdProductName, (CFStringRef*)&productName)))
	{
		cout << "Could not get switcher product name" << endl;
		return;
	}
	
	cout << "PRODUCT NAME: " << getCStringFromCFString(productName) << endl;
	
	ofxOscMessage m;
	m.setAddress("atem/product/");
	m.addStringArg(getCStringFromCFString(productName));
	sender.sendMessage(m);
	
	BMDSwitcherInputId id;
	mSwitcher->AddCallback(mSwitcherMonitor);
	
	result = mSwitcher->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator);
	if (SUCCEEDED(result))
	{
		IBMDSwitcherInput* input = NULL;
		while (S_OK == inputIterator->Next(&input))
		{
			InputMonitor* inputMonitor = new InputMonitor(input, this);
			IBMDSwitcherInputAux* auxObj;
			result = input->QueryInterface(IID_IBMDSwitcherInputAux, (void**)&auxObj);
			input->GetInputId(&id);
			inputCount = inputCount++;
			if (SUCCEEDED(result))
			{
				BMDSwitcherInputId auxId;
				result = auxObj->GetInputSource(&auxId);
				mSwitcherInputAuxList.push_back(auxObj);
			}
			input->Release();
			mInputMonitors.push_back(inputMonitor);
		}
		inputIterator->Release();
		inputIterator = NULL;
	}
	
	result = mSwitcher->CreateIterator(IID_IBMDSwitcherMixEffectBlockIterator, (void**)&iterator);
	if (FAILED(result))
	{
		cout << "Could not create IBMDSwitcherMixEffectBlockIterator iterator" << endl;
		goto finish;
	}
	
	if (S_OK != iterator->Next(&mMixEffectBlock))
	{
		cout << "Could not get the first IBMDSwitcherMixEffectBlock" << endl;
		goto finish;
	}
	
	mMixEffectBlock->AddCallback(mMixEffectBlockMonitor);
	
finish:
	if (iterator)
		iterator->Release();
	// updatePopupButtonItems();
}


void testApp::updatePopupButtonItems()

{
	ofxOscMessage m;
	m.setAddress("/atem/input/");
	m.addStringArg("begin");
	sender.sendMessage(m);
	
	HRESULT result;
	IBMDSwitcherInputIterator* inputIterator = NULL;
	IBMDSwitcherInput* input = NULL;
	
	result = mSwitcher->CreateIterator(IID_IBMDSwitcherInputIterator, (void**)&inputIterator);
	if (FAILED(result))
	{
		cout <<"Could not create IBMDSwitcherInputIterator iterator"<< endl;
		return;
	}
	usleep(100000);
	while (S_OK == inputIterator->Next(&input))
	{
		CFStringRef name;
		BMDSwitcherInputId id;
		
		input->GetInputId(&id);
		input->GetString(bmdSwitcherInputPropertyIdLongName, (CFStringRef*)&name);
		ofxOscMessage m;
		m.setAddress("/atem/input/");
		m.addIntArg(id);
		m.addStringArg(getCStringFromCFString(name));
		sender.sendMessage(m);
		input->Release();
	}
	m.clear();
	m.setAddress("/atem/input/");
	m.addStringArg("end");
	sender.sendMessage(m);
	
	inputIterator->Release();
	
	updateProgramButtonSelection();
	updatePreviewButtonSelection();
}

void testApp::updateTransitionFramesTextField()
{
	int64_t framesRemaining;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdTransitionFramesRemaining, &framesRemaining);
}


void testApp::updateFTBFramesTextField()
{
	int64_t framesRemaining;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdFadeToBlackFramesRemaining, &framesRemaining);
}

void testApp::updateProgramButtonSelection()
{
	BMDSwitcherInputId	programId;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, &programId);
}


void testApp::getProgramInputFromexternal(){
	getProgramInput();
}


int testApp::getProgramInput()
{
	BMDSwitcherInputId	programId;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, &programId);
	sendOscInt("/atem/programInputID", programId);
	return programId;
}



int testApp::getPreviewInput(){
	BMDSwitcherInputId	previewId;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, &previewId);
	sendOscInt("/atem/previewInputID", previewId);
	return previewId;
}

float testApp::getTransitionPosition()
{
	double position;
	mMixEffectBlock->GetFloat(bmdSwitcherMixEffectBlockPropertyIdTransitionPosition, &position);
	ofxOscMessage m;
	m.setAddress("/atem/mix");
	m.addFloatArg(position);
	sender.sendMessage(m);
	return position;
}

int64_t testApp::getTransitionFrames()
{
	int64_t framesRemaining;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdTransitionFramesRemaining, &framesRemaining);
	sendOscInt("/atem/tfr", framesRemaining);
	return framesRemaining;
}

int64_t testApp::getFTBFrames()
{
	int64_t framesRemaining;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdFadeToBlackFramesRemaining, &framesRemaining);
	sendOscInt("/atem/ftbfr", framesRemaining);
	return framesRemaining;
	
}

int64_t testApp::getFTBRate()
{
	int64_t blackRate;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdFadeToBlackRate, &blackRate);
	sendOscInt("/atem/ftbrate", blackRate);
	return blackRate;
	
}

bool testApp::isInTransition()
{
	bool inTransition;
	mMixEffectBlock->GetFlag(bmdSwitcherMixEffectBlockPropertyIdInTransition, &inTransition);
	sendOscInt("/atem/isInTransition", inTransition);
	return inTransition;
}

int testApp::getAuxCount(){
	sendOscInt("/atem/auxCount", mSwitcherInputAuxList.size());
	return mSwitcherInputAuxList.size();
}

int testApp::getAuxSource(int auxNum){
	BMDSwitcherInputId auxId;
	mSwitcherInputAuxList[auxNum]->GetInputSource(&auxId);
	ofxOscMessage m;
	m.setAddress("/atem/auxSource");
	m.addIntArg(auxNum);
	m.addIntArg(auxId);
	sender.sendMessage(m);
}

void testApp::updatePreviewButtonSelection()
{
	BMDSwitcherInputId	previewId;
	BMDSwitcherInputId id;
	mMixEffectBlock->GetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, &previewId);
}

void testApp::programPopupChanged(int myID)
{
	if (myID < inputCount){
		mMixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdProgramInput, myID);
	}
}

void testApp::previewPopupChanged(int myID)
{
	if (myID < inputCount){
		mMixEffectBlock->SetInt(bmdSwitcherMixEffectBlockPropertyIdPreviewInput, myID);
	}
}

void testApp::sliderChanged(float myPos){
	mMixEffectBlock->SetFloat(bmdSwitcherMixEffectBlockPropertyIdTransitionPosition, myPos);
}


void testApp::setTransitionStyle(string style){
	HRESULT result;
	REFIID transitionStyleID = IID_IBMDSwitcherTransitionParameters;
	IBMDSwitcherTransitionParameters* mTransitionStyleParameters=NULL;
	result = mMixEffectBlock->QueryInterface(transitionStyleID, (void**)&mTransitionStyleParameters);
	if (SUCCEEDED(result))
	{
		if (style == "mix"){
			mTransitionStyleParameters->SetNextTransitionStyle(bmdSwitcherTransitionStyleMix);
		}
		if (style == "dip"){
			mTransitionStyleParameters->SetNextTransitionStyle(bmdSwitcherTransitionStyleDip);
		}
		if (style == "wipe"){
			mTransitionStyleParameters->SetNextTransitionStyle(bmdSwitcherTransitionStyleWipe);
		}
		if (style == "sting"){
			mTransitionStyleParameters->SetNextTransitionStyle(bmdSwitcherTransitionStyleStinger);
		}
		if (style == "dve"){
			mTransitionStyleParameters->SetNextTransitionStyle(bmdSwitcherTransitionStyleDVE);
		}
	}
}

void testApp::getAutoRate(){
	HRESULT result;
	IBMDSwitcherTransitionMixParameters* mTransitionMixParameters=NULL;
	result = mMixEffectBlock->QueryInterface(IID_IBMDSwitcherTransitionMixParameters, (void**)&mTransitionMixParameters);
	if (SUCCEEDED(result))
	{
		uint32_t frames;
		mTransitionMixParameters->GetRate(&frames);
		sendOscInt("/atem/autoRate", frames);
	}
}

void testApp::setAutoRate(uint32_t frames){
	HRESULT result;
	IBMDSwitcherTransitionMixParameters* mTransitionMixParameters=NULL;
	result = mMixEffectBlock->QueryInterface(IID_IBMDSwitcherTransitionMixParameters, (void**)&mTransitionMixParameters);
	if (SUCCEEDED(result))
	{
		mTransitionMixParameters->SetRate(frames);
	}
}

//////////////  TOOLS  ///////////////////

void testApp::sendOscInt(string oscMsg, int oscArg){
	ofxOscMessage m;
	m.setAddress(oscMsg);
	m.addIntArg(oscArg);
	sender.sendMessage(m);
}

void testApp::sendOscFloat(string oscMsg, float oscArg){
	ofxOscMessage m;
	m.setAddress(oscMsg);
	m.addFloatArg(oscArg);
	sender.sendMessage(m);
}

void testApp::sendOscString(string oscMsg, string oscArg){
	ofxOscMessage m;
	m.setAddress(oscMsg);
	m.addStringArg(oscArg);
	sender.sendMessage(m);
}

string testApp::getCStringFromCFString(CFStringRef myCFStringRef){
	CFRange rangeToProcess;
	const char *bytes;
	bytes = CFStringGetCStringPtr(myCFStringRef, kCFStringEncodingMacRoman);
	if (bytes == NULL) {
		char localBuffer[10];
		Boolean success;
		success = CFStringGetCString(myCFStringRef, localBuffer, 10, kCFStringEncodingMacRoman);
	}
	return bytes;
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 
	
}