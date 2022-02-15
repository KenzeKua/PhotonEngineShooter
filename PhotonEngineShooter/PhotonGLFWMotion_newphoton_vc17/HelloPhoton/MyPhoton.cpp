#include "MyPhoton.h"
#include <iostream>
#include "Application.h"

// Kenze's ID: 9d4ccadb-bb50-416b-a840-482ee37f78e8
// Victor's ID: ca1729c5-d695-4c91-b708-963bcf94fca6
// Chan Zhe's ID: 88eab4cf-e716-4fd5-a3cf-109ca086f23f

static const ExitGames::Common::JString appId = L"9d4ccadb-bb50-416b-a840-482ee37f78e8"; // set your app id here
static const ExitGames::Common::JString appVersion = L"1.0";
static const ExitGames::Common::JString PLAYER_NAME = L"Kenze";

//LoadBalancing::Listener& listener, const Common::JString& applicationID, 
//const Common::JString& appVersion, nByte connectionProtocol=Photon::ConnectionProtocol::DEFAULT, bool autoLobbyStats=false, nByte regionSelectionMode=RegionSelectionMode::DEFAULT

// Functions
MyPhoton::MyPhoton() : mLoadBalancingClient(*this, appId, appVersion, ExitGames::Photon::ConnectionProtocol::TCP, false, ExitGames::LoadBalancing::RegionSelectionMode::SELECT)
{
}

void MyPhoton::connect(void)
{
	std::wcout<<"Connecting..."<<std::endl;
	mLoadBalancingClient.connect(ExitGames::LoadBalancing::AuthenticationValues(), PLAYER_NAME);
}

void MyPhoton::disconnect(void)
{
	mLoadBalancingClient.disconnect();
}

void MyPhoton::sendMyData(float xpos, float ypos, float xvel, float yvel, float xaccel, float yaccel, float MouseX, float MouseY)
{
	float data[8];
	data[0] = xpos;
	data[1] = ypos;
	data[2] = xvel;
	data[3] = yvel;
	data[4] = xaccel;
	data[5] = yaccel;
	data[6] = MouseX;
	data[7] = MouseY;

	mLoadBalancingClient.opRaiseEvent(true, data, 8, 1);
}
void MyPhoton::SendShootBullet(bool Shoot)
{
	mLoadBalancingClient.opRaiseEvent(true, Shoot, 2);
}
void MyPhoton::SendShootMissile(bool Shoot)
{
	mLoadBalancingClient.opRaiseEvent(true, Shoot, 3);
}

void MyPhoton::run(void)
{
	mLoadBalancingClient.service();
}

// protocol implementations

void MyPhoton::debugReturn(int debugLevel, const ExitGames::Common::JString& string)
{
}

void MyPhoton::connectionErrorReturn(int errorCode)
{
	std::wcout<<"Connection error return : "<<errorCode<<std::endl;
}

void MyPhoton::clientErrorReturn(int errorCode)
{
	std::wcout<<"Client error return : "<<errorCode<<std::endl;
}

void MyPhoton::warningReturn(int warningCode)
{
	std::wcout<<"Warning return : "<<warningCode<<std::endl;
}

void MyPhoton::serverErrorReturn(int errorCode)
{
	std::wcout<<"Server error return : "<<errorCode<<std::endl;
}

void MyPhoton::joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int> &playernrs, const ExitGames::LoadBalancing::Player& player)
{
	std::wcout << "Player " << playerNr << "(" << player.getName() << ") joined\n";
	
	Application::getInstance().SetMyPlayerNumber(playerNr);
	Application::getInstance().SetTotalPlayer(playernrs.getSize());

	if(playernrs.getSize() >= 2)
		Application::getInstance().sendMyData();
}

void MyPhoton::leaveRoomEventAction(int playerNr, bool isInactive)
{
	std::wcout << "Player " << playerNr << " left. Inactivity : "<< isInactive << std::endl;
}

void MyPhoton::customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object& eventContent)
{
	if (eventCode == 1)
	{
		float* data = ExitGames::Common::ValueObject<float*>(eventContent).getDataCopy();
		if (data)
		{
			Application::getInstance().OnReceivedOpponentData(playerNr, data);
		}
		else
			std::cout << "Invalid data" << std::endl;
	}
	else if (eventCode == 2)
	{
		bool Shoot = ExitGames::Common::ValueObject<bool>(eventContent).getDataCopy();
		if (Shoot)
		{
			Application::getInstance().ReceiveEnemyShoot(playerNr, eventCode);
		}
	}
	else if (eventCode == 3)
	{
		bool Shoot = ExitGames::Common::ValueObject<bool>(eventContent).getDataCopy();
		if (Shoot)
		{
			Application::getInstance().ReceiveEnemyShoot(playerNr, eventCode);
		}
	}
}

void MyPhoton::connectReturn(int errorCode, const ExitGames::Common::JString& errorString, const ExitGames::Common::JString& cluster)
{
	if(errorCode)
		std::wcout<<"Failed to connect : "<<errorString.cstr()<<std::endl;
	else
	{
		std::wcout<<"Connected to Photon Server."<<std::endl;
		
		std::wcout<<"Trying to join a random room..."<<std::endl;
		mLoadBalancingClient.opJoinRandomRoom();
	}
}

void MyPhoton::disconnectReturn(void)
{
	std::wcout<<"Disconnect return"<<std::endl;
}

void MyPhoton::createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"Create room return"<<std::endl;
}

void MyPhoton::joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"Join or create room return"<<std::endl;
}

void MyPhoton::joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"Join room return"<<std::endl;
}

void MyPhoton::joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable& /*gameProperties*/, const ExitGames::Common::Hashtable& /*playerProperties*/, int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"Join random room return"<<std::endl;

	if(errorCode)
	{
		std::wcout<<"Failed to join a random room : "<<errorCode<<", "<<errorString.cstr()<<std::endl;

		if(errorCode == 32760) //no match found error code
		{
			std::wcout<<"Creating a room..."<<std::endl;
			//try to create a room and wait;
			ExitGames::Common::JString name;
			name += GETTIMEMS();
			mLoadBalancingClient.opCreateRoom(name, ExitGames::LoadBalancing::RoomOptions().setMaxPlayers(8));
		}

		return;
	}
}

void MyPhoton::leaveRoomReturn(int errorCode, const ExitGames::Common::JString& errorString)
{
	std::wcout<<"Leave room return. Error code : " << errorCode << "Error : " << errorString << std::endl;
}

void MyPhoton::joinLobbyReturn(void)
{
	std::wcout<<"Join lobby return"<<std::endl;
}

void MyPhoton::leaveLobbyReturn(void)
{
	std::wcout<<"Leave lobby return"<<std::endl;
}

void MyPhoton::onLobbyStatsResponse(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"On lobby stats response"<<std::endl;
}

void MyPhoton::onLobbyStatsUpdate(const ExitGames::Common::JVector<ExitGames::LoadBalancing::LobbyStatsResponse>& lobbyStats)
{
	std::wcout<<"On lobby stats update"<<std::endl;
}

void MyPhoton::onAvailableRegions(const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegions, const ExitGames::Common::JVector<ExitGames::Common::JString>& availableRegionServers)
{
	//std::wcout<<availableRegions[0]<<std::endl;
	//std::wcout<<availableRegions[1]<<std::endl;
	//mLoadBalancingClient.selectRegion(availableRegions[0]);
	mLoadBalancingClient.selectRegion("asia");
}
