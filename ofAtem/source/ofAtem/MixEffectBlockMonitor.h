#ifndef MIXEFFECTBLOCKMONITOR_H
#define MIXEFFECTBLOCKMONITOR_H

//#include "MixEffectBlockMonitor.h"
#include "BMDSwitcherAPI.h"
#include <list>

class testApp;
#include "testApp.h"




// Callback class for monitoring property changes on a mix effect block.
class MixEffectBlockMonitor : public IBMDSwitcherMixEffectBlockCallback{
	
public:
	MixEffectBlockMonitor(testApp* uiDelegate) : mUiDelegate(uiDelegate), mRefCount(1) {};
	
	
protected:
	virtual ~MixEffectBlockMonitor(){};
	
	
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);
	HRESULT PropertyChanged(BMDSwitcherMixEffectBlockPropertyId propertyId);
	
	
private:
	testApp*		mUiDelegate;
	int				mRefCount;
};



#endif