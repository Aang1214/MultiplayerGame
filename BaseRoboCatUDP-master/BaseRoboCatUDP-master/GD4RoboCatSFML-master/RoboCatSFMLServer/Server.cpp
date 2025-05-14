
#include "RoboCatServerPCH.hpp"
#include <iostream>



//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	s_instance.reset(new Server());

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnServer::StaticCreate);

	InitNetworkManager();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg(2);
	if (!latencyString.empty())
	{
		latency = stof(latencyString);
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency(latency);
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{
	/*
	void CreateRandomMice(int inMouseCount)
	{
		Vector3 mouseMin(100.f, 100.f, 0.f);
		Vector3 mouseMax(1180.f, 620.f, 0.f);
		GameObjectPtr go;

		//make a mouse somewhere- where will these come from?
		for (int i = 0; i < inMouseCount; ++i)
		{
			
			go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
			Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
			go->SetLocation(mouseLocation);
		}
	}
	*/
	void CreateRandomMice(int inMouseCount)
	{/*
		Vector3 mouseMin(100.f, 100.f, 0.f);
		Vector3 mouseMax(1180.f, 620.f, 0.f);
		

		//make a mouse somewhere- where will these come from?
		for (int i = 0; i < inMouseCount; ++i)
		{
			
			MousePtr mouse = std::static_pointer_cast<Mouse>(GameObjectRegistry::sInstance->CreateGameObject('MOUS'));
			Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
			mouse->SetLocation(mouseLocation);
		}*/

		Vector3 mouseMin(100.f, 100.f, 0.f);
		Vector3 mouseMax(1820.f, 980.f, 0.f);

		// Center exclusion zone (400x400 box in the middle of a 1920x1080 screen)
		Vector3 centerMin(760.f, 340.f, 0.f);
		Vector3 centerMax(1160.f, 740.f, 0.f);

		GameObjectPtr go;

		int gridCols = static_cast<int>(ceilf(sqrtf(inMouseCount)));
		int gridRows = static_cast<int>(ceilf(inMouseCount / static_cast<float>(gridCols)));

		float cellWidth = (mouseMax.mX - mouseMin.mX) / gridCols;
		float cellHeight = (mouseMax.mY - mouseMin.mY) / gridRows;

		int spawned = 0;

		for (int row = 0; row < gridRows && spawned < inMouseCount; ++row)
		{
			for (int col = 0; col < gridCols && spawned < inMouseCount; ++col)
			{
				// Diagonal offset for every other row
				float offsetX = (row % 2 == 1) ? (cellWidth / 2.0f) : 0.0f;

				float x = mouseMin.mX + col * cellWidth + cellWidth / 2.0f + offsetX;
				float y = mouseMin.mY + row * cellHeight + cellHeight / 2.0f;

				// Skip if inside center exclusion zone
				if (x > centerMin.mX && x < centerMax.mX &&
					y > centerMin.mY && y < centerMax.mY)
				{
					continue;
				}

				go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
				go->SetLocation(Vector3(x, y, 0.f));
				++spawned;
			}
		}
	}


}


void Server::SetupWorld()
{
	//spawn some random mice
	CreateRandomMice(10);

	//spawn more random mice!
	//CreateRandomMice(10);
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{

	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer(playerId);
}

void Server::SpawnCatForPlayer(int inPlayerId)
{
	RoboCatPtr cat = std::static_pointer_cast<RoboCat>(GameObjectRegistry::sInstance->CreateGameObject('RCAT'));
	cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	cat->SetPlayerId(inPlayerId);
	//gotta pick a better spawn location than this...
	cat->SetLocation(Vector3(600.f - static_cast<float>(inPlayerId), 400.f, 0.f));
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer(playerId);
	if (cat)
	{
		cat->SetDoesWantToDie(true);
	}
}

RoboCatPtr Server::GetCatForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<RoboCat>(go);
		}
	}

	return nullptr;

}
