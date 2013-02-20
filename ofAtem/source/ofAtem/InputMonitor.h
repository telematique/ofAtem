#ifndef INPUTMONITOR_H
#define INPUTMONITOR_H

#include "BMDSwitcherAPI.h"
#include "testApp.h"


class InputMonitor : public IBMDSwitcherInputCallback {
	
public:
	InputMonitor(IBMDSwitcherInput* input, testApp* uiDelegate);

	
protected:
	~InputMonitor();
	
public:
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
	ULONG STDMETHODCALLTYPE AddRef(void);
	ULONG STDMETHODCALLTYPE Release(void);
	HRESULT PropertyChanged(BMDSwitcherInputPropertyId propertyId);
	IBMDSwitcherInput* input();
	
private:
	IBMDSwitcherInput*			mInput;
	testApp*					mUiDelegate;
	int							mRefCount;
};

#endif




