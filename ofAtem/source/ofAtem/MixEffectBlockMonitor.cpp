#include "MixEffectBlockMonitor.h"
#include <libkern/OSAtomic.h>

#include <stdint.h>

// Callback class for monitoring property changes on a mix effect block.


static inline bool	operator== (const REFIID& iid1, const REFIID& iid2)
{ 
	return CFEqual(&iid1, &iid2);
}

HRESULT STDMETHODCALLTYPE MixEffectBlockMonitor::QueryInterface(REFIID iid, LPVOID *ppv)
{
	if (!ppv)
		return E_POINTER;
	
	if (iid == IID_IBMDSwitcherMixEffectBlockCallback)
	{
		*ppv = static_cast<IBMDSwitcherMixEffectBlockCallback*>(this);
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

ULONG STDMETHODCALLTYPE MixEffectBlockMonitor::AddRef(void)
{
	return ::OSAtomicIncrement32(&mRefCount);
}

ULONG STDMETHODCALLTYPE MixEffectBlockMonitor::Release(void)
{
	int newCount = ::OSAtomicDecrement32(&mRefCount);
	if (newCount == 0)
		delete this;
	return newCount;
}

HRESULT MixEffectBlockMonitor::PropertyChanged(BMDSwitcherMixEffectBlockPropertyId propertyId)
{
	switch (propertyId)
	{
		case bmdSwitcherMixEffectBlockPropertyIdProgramInput:
			mUiDelegate->getProgramInput();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdPreviewInput:
			mUiDelegate->getPreviewInput();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdInTransition:
			mUiDelegate->isInTransition();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdTransitionPosition:
			mUiDelegate->getTransitionPosition();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdTransitionFramesRemaining:
			mUiDelegate->getTransitionFrames();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdFadeToBlackFramesRemaining:
			mUiDelegate->getFTBFrames();
			break;
		case bmdSwitcherMixEffectBlockPropertyIdFadeToBlackRate:
			mUiDelegate->getFTBRate();
			break;
		default:	// ignore other property changes not used for this sample app
			break;
	}
	return S_OK;
}


