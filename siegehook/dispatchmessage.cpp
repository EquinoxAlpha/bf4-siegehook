#include "hooking.h"
#include "sdk.h"
#include "common.h"

namespace hooking {
	void __fastcall hkDispatchMessage(MessageManager* pMessageManagerImpl, Message* pMessage) {
		auto type = pMessage->m_Type;

		if (type == VeniceNetworkRequestScreenshotMessage::Type() ||
			type == VeniceNetworkRequestFrontBufferScreenshotMessage::Type() ||
			type == VeniceNetworkRequestFrontBufferScreenshot2Message::Type()
			) {
			auto t = (VeniceNetworkRequestScreenshotMessage*)pMessage;

			logger.log(xorstr_("FairFight Screenshot initiated: %s ( %dx%d )\n"), pMessage->GetType()->m_InfoData->m_Name, t->width, t->height);

			globals.g_fairfight = true;
			Sleep(100);
			globals.g_fairfight = false;
		}
		
		inline_dispatchmessage.call<void>(pMessageManagerImpl, pMessage);
	}
}