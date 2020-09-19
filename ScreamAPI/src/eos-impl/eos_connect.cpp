#include "pch.h"
#include "eos-sdk/eos_connect.h"
#include "ScreamAPI.h"
#include "achievement_manager.h"


struct ConnectLoginContainer {
	void* originalClientData;
	EOS_Connect_OnLoginCallback originalCompletionDelegate;
};

void EOS_CALL ConnectLoginDelegate(const EOS_Connect_LoginCallbackInfo* Data) {
	auto container = reinterpret_cast<ConnectLoginContainer*>(Data->ClientData);

	// get non-const pointer to data
	auto modifiedData = const_cast<EOS_Connect_LoginCallbackInfo*>(Data);

	// Restore original client data
	modifiedData->ClientData = container->originalClientData;

	// Call original completion delegate with our modified data
	container->originalCompletionDelegate(modifiedData);

	// Free the heap
	delete container;

	AchievementManager::init();
}

EOS_DECLARE_FUNC(void) EOS_Connect_Login(EOS_HConnect Handle, const EOS_Connect_LoginOptions* Options, void* ClientData, const EOS_Connect_OnLoginCallback CompletionDelegate){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Connect_Login, __func__);
	auto container = new ConnectLoginContainer{ ClientData, CompletionDelegate }; // Don't forget to free the heap
	proxy(Handle, Options, container, ConnectLoginDelegate);
}

EOS_DECLARE_FUNC(EOS_ProductUserId) EOS_Connect_GetLoggedInUserByIndex(EOS_HConnect Handle, int32_t Index){
	Logger::debug(__func__);

	static auto proxy = ScreamAPI::proxyFunction(&EOS_Connect_GetLoggedInUserByIndex, __func__);
	return proxy(Handle, Index);
}