#include "RoboCatClientPCH.hpp"
MouseClient::MouseClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
	mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("mouse"));
}
void MouseClient::Update()
{
	Mouse::Update();

}
/**/void MouseClient::Read(InputMemoryBitStream& inInputStream)
{
	bool stateBit;

	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 location;
		inInputStream.Read(location.mX);
		inInputStream.Read(location.mY);
		SetLocation(location);

		Vector3 velocity;
		inInputStream.Read(velocity.mX);
		inInputStream.Read(velocity.mY);
		sf::Vector2f velo(velocity.GetX(), velocity.GetY());
		SetVelocity(velo);


	}

	
	inInputStream.Read(stateBit);
	if (stateBit)
	{
		Vector3 color;
		inInputStream.Read(color);
		SetColor(color);
	}
	
	
}


