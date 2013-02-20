#include "InputMonitor.h"
#include <libkern/OSAtomic.h>

static inline bool	operator== (const REFIID& iid1, const REFIID& iid2)
{ 
	return CFEqual(&iid1, &iid2);
}

// Monitor the properties on Switcher Inputs.
// In this sample app we're only interested in changes to the Long Name property to update the PopupButton list

InputMonitor::InputMonitor(IBMDSwitcherInput* input, testApp* uiDelegate) : mInput(input), mUiDelegate(uiDelegate), mRefCount(1)
{
	mInput->AddRef();
	mInput->AddCallback(this);
}

InputMonitor::~InputMonitor()
{
	mInput->RemoveCallback(this);
	mInput->Release();
}

HRESULT STDMETHODCALLTYPE InputMonitor::QueryInterface(REFIID iid, LPVOID *ppv)
{
	if (!ppv)
		return E_POINTER;
	
	if (iid == IID_IBMDSwitcherInputCallback)
	{
		*ppv = static_cast<IBMDSwitcherInputCallback*>(this);
		AddRef();
		return S_OK;
	}
	
	if (CFEqual(&iid, IUnknownUUID))
	{
		*ppv = static_cast<IUnknown*>(this);
		AddRef();
		return S_OK;
	}
	
	*ppv = NULL;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE InputMonitor::AddRef(void)
{
	return ::OSAtomicIncrement32(&mRefCount);
}

ULONG STDMETHODCALLTYPE InputMonitor::Release(void)
{
	int newCount = ::OSAtomicDecrement32(&mRefCount);
	if (newCount == 0)
		delete this;
	return newCount;
}

HRESULT InputMonitor::PropertyChanged(BMDSwitcherInputPropertyId propertyId)
{
	switch (propertyId)
	{
		case bmdSwitcherInputPropertyIdLongName:
		
		default:	// ignore other property changes not used for this sample app
			break;
	}
	
	return S_OK;
}

IBMDSwitcherInput* InputMonitor::input() 
{ 
	return mInput; 
}




