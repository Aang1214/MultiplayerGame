class MouseServer : public Mouse
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new MouseServer()); }
	void HandleDying() override;

	

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;
	//bool HandleCollisionWithYarn(Yarn* inYarn);
	virtual bool HandleCollisionWithMouse(Mouse* inMouse) override;
	void Update();
protected:
	MouseServer();
};

