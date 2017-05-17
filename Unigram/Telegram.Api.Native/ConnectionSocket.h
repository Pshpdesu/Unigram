#pragma once
#include <vector>
#include <string>
#include <Winsock2.h>
#include <wrl.h>
#include "WSAEvent.h"
#include "EventObject.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

namespace Telegram
{
	namespace Api
	{
		namespace Native
		{

			struct EventObjectEventContext;

			class ConnectionSocket abstract : virtual EventObjectT<EventTraits::WaitTraits>
			{
			public:
				ConnectionSocket();
				~ConnectionSocket();

			protected:
				inline SOCKET GetSocket() const
				{
					return m_socket;
				}

				inline HANDLE GetSocketEvent() const
				{
					return m_socketEvent.Get();
				}

				HRESULT ConnectSocket(_In_ std::wstring const& address, UINT16 port, boolean ipv6);
				HRESULT DisconnectSocket();
				HRESULT SendData(_In_reads_(length) BYTE const* buffer, UINT32 length);
				HRESULT OnEvent(_In_ PTP_CALLBACK_INSTANCE callbackInstance);

				virtual HRESULT OnSocketCreated() = 0;
				virtual HRESULT OnSocketConnected() = 0;
				virtual HRESULT OnDataReceived(_In_reads_(length) BYTE const* buffer, UINT32 length) = 0;
				virtual HRESULT OnSocketDisconnected() = 0;
				virtual HRESULT OnSocketClosed(int wsaError) = 0;

			private:
				HRESULT CloseSocket(int wsaError, BYTE flags);
				HRESULT GetLastErrorAndCloseSocket(BYTE flags);

				SOCKET m_socket;
				WSAEvent m_socketEvent;
				std::vector<BYTE> m_sendBuffer;
				std::vector<BYTE> m_receiveBuffer;
			};

		}
	}
}