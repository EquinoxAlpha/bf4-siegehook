#include "hooking.h"
#include "common.h"

#include <d3d11.h>

namespace hooking {
	void hkCopySubresourceRegion(ID3D11DeviceContext* pContext,
		ID3D11Resource* pDstResource,
		UINT            DstSubresource,
		UINT            DstX,
		UINT            DstY,
		UINT            DstZ,
		ID3D11Resource* pSrcResource,
		UINT            SrcSubresource,
		const D3D11_BOX* pSrcBox) {

		void* ret = _ReturnAddress();
		
		if (reinterpret_cast<DWORD_PTR>(ret) == OFFSET_PBSSRETURN) {
			logger.log("PB screenshot initiated");
			globals.g_punkbuster_2 = true;
			Sleep(100);
			inline_copysubresourceregion.call<void>(pContext, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
			Sleep(10);
			globals.g_punkbuster_2 = false;
			return;
		}

		inline_copysubresourceregion.call<void>(pContext, pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
	}
}