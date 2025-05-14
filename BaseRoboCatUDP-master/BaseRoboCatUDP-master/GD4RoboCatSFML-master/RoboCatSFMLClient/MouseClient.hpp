class MouseClient : public Mouse
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new MouseClient()); }
	virtual void Update();
	virtual void		Read(InputMemoryBitStream& inInputStream) override;
protected:
	MouseClient();
	SpriteComponentPtr	mSpriteComponent;
private:

	//SpriteComponentPtr	mSpriteComponent;
};
