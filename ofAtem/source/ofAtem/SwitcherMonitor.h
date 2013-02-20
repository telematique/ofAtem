#ifndef SWITCHERMONITOR_H
#define SWITCHERMONITOR_H

#include "BMDSwitcherAPI.h"
#include "testApp.h"


class SwitcherMonitor : public IBMDSwitcherCallback
{
public:
	SwitcherMonitor(testApp* uiDelegate) :	mUiDelegate(uiDelegate), mRefCount(1){};
	
protected:
	virtual ~SwitcherMonitor(){};
	
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
	
	ULONG STDMETHODCALLTYPE AddRef(void);
	
	ULONG STDMETHODCALLTYPE Release(void);
	
	// Switcher Property changes ignored by this sample app
	HRESULT STDMETHODCALLTYPE	PropertyChanged(BMDSwitcherPropertyId propId);	
	HRESULT STDMETHODCALLTYPE	Disconnected(void);
	
private:
	testApp*	mUiDelegate;
	int							mRefCount;
};

#endif