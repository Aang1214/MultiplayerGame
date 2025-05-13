class MouseServer : public Mouse
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new MouseServer()); }
	void HandleDying() override;

	sf::Vector2f GetVelocity();
	void SetVelocity(sf::Vector2f velocity);

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;
	//virtual bool HandleCollisionWithMouse(Mouse* inMouse) override;

protected:
	MouseServer();
};

