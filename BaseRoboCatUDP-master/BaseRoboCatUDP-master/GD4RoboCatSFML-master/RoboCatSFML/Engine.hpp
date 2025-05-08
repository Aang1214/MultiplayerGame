class Engine
{
public:
	virtual ~Engine();
	//static std::unique_ptr<Engine> s_instance;
	static std::unique_ptr< Engine > sInstance;

	virtual int Run();
	void SetShouldKeepRunning(bool in_should_keep_running)
	{
		m_should_keep_running = in_should_keep_running;
	}
	virtual void HandleEvent(sf::Event& p_event);
	virtual bool PollEvent(sf::Event& p_event);

protected:
	Engine();
	virtual void DoFrame();

private:
	int DoRunLoop();
	bool m_should_keep_running;
};
