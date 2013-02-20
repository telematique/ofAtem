#include "SwitcherMonitor.h"
#include <libkern/OSAtomic.h>

static inline bool	operator== (const REFIID& iid1, const REFIID& iid2)
{ 
	return CFEqual(&iid1, &iid2);
}

HRESULT STDMETHODCALLTYPE SwitcherMonitor::QueryInterface(REFIID iid, LPVOID *ppv)
{
	if (!ppv)
		return E_POINTER;
	
	if (iid == IID_IBMDSwitcherCallback)
	{
		*ppv = static_cast<IBMDSwitcherCallback*>(this);
		AddRef();
		cout << "SWITCH connected" << endl;
		return S_OK;
	}
	
	if (CFEqual(&iid, IUnknownUUID))
	{
		*ppv = static_cast<IUnknown*>(this);
		AddRef();
		cout << "SWITCH connected" << endl;
		return S_OK;
	}
	cout << "ERROR SWITCH disconnected" << endl;
	*ppv = NULL;
	return E_NOINTERFACE;
}

ULONG STDMETHODCALLTYPE SwitcherMonitor::AddRef(void)
{
	return ::OSAtomicIncrement32(&mRefCount);
}

ULONG STDMETHODCALLTYPE SwitcherMonitor::Release(void)
{
	int newCount = ::OSAtomicDecrement32(&mRefCount);
	if (newCount == 0)
		delete this;
	return newCount;
}

HRESULT STDMETHODCALLTYPE	SwitcherMonitor::PropertyChanged(BMDSwitcherPropertyId propId) { 
	return S_OK; 
}

HRESULT STDMETHODCALLTYPE	SwitcherMonitor::Disconnected(void)
{
	return S_OK;
}


