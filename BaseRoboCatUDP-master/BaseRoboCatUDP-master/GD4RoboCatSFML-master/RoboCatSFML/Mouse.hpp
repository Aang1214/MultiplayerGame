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

	static	GameObject* StaticCreate() { return new Mouse(); }
	void SetVelocity(Vector3 velocity);
	void Update();
	virtual uint32_t	GetAllStateMask()	const override { return EMRS_AllState; }


	virtual uint32_t	Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const override;

	void		SetPlayerId(int inPlayerId) { mPlayerId = inPlayerId; }
	int			GetPlayerId() const { return mPlayerId; }

	virtual bool HandleCollisionWithCat(RoboCat* inCat) override;

	virtual bool HandleCollisionWithMouse(Mouse* inMouse) override;
	const Vector3& GetVelocity()						const { return mVelocity; }
	void ProcessCollisions();
	void ProcessCollisionsWithScreenWalls();
private:
	float				mWallRestitution;
	float				mMouseRestitution;
	Vector3				mVelocity;
	
	int			mPlayerId;
protected:
	Mouse();
};

typedef shared_ptr< Mouse >	MousePtr;

