class MouseServer : public Mouse
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new MouseServer()); }
	void HandleDying() override;

	

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;
	//bool HandleCollisionWithYarn(Yarn* inYarn);
	void Update();
protected:
	MouseServer();
};

