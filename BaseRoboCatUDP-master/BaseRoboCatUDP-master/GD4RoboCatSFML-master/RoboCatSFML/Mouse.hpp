class Mouse : public GameObject
{
public:
	CLASS_IDENTIFICATION('MOUS', GameObject)

	enum EMouseReplicationState
	{
		EMRS_Pose = 1 << 0,
		EMRS_Color = 1 << 1,

		EMRS_AllState = EMRS_Pose | EMRS_Color
	};
	sf::Vector2f GetVelocity();
	void SetVelocity(sf::Vector2f velocity);
	void Update();
	static	GameObject* StaticCreate() { return new Mouse(); }

	virtual uint32_t	GetAllStateMask()	const override { return EMRS_AllState; }

	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;
	virtual void		Read(InputMemoryBitStream& inInputStream) override;

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

	virtual bool HandleCollisionWithMouse(Mouse* inMouse) override;
	const Vector3& GetVelocity()						const { return mVelocity; }
private:
	Vector3				mVelocity;
protected:
	Mouse();
};

