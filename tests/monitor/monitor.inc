class Monitor
{
	auto number has_monitor = false;
public:
	class ReadOnlyLock
	{
	private:
		Monitor& _monitor;
	public:
		inline ReadOnlyLock(Monitor& aMonitor) : _monitor(aMonitor) { _monitor.enter_shared(); }
		inline ~ReadOnlyLock() { _monitor.leave(); }
	};
	friend class ReadOnlyLock;
	class ReadWriteLock
	{
	private:
		Monitor& _monitor;
	public:
		inline ReadWriteLock(Monitor& aMonitor) : _monitor(aMonitor) { _monitor.enter_exclusive(); }
		inline ~ReadWriteLock() { _monitor.leave(); }
	};
	friend class ReadWriteLock;
private:
	void enter_exclusive() { /* ... */ }
	void enter_shared() { /* ... */ }
	void leave() { /* ... */ }
};

auto declaration Monitor::install()
{
	class $This : auto $Dynamic
	{
		private $Dynamic _monitor;
		auto number has_monitor = true;
	};
}

auto Monitor::~Monitor()
{
	auto if (has_monitor)							// Avoid execution for Monitor and its derived classes.
	{
		auto for (iterator f = $functions(); f; ++f)
		{
			auto if (f->is_static())
				;
			else if (f->is_volatile())
				;
			else if (f->is_const())
			{
				$f->specifier() :{ entry { ReadOnlyLock aLock(_monitor); } };
			}
			else
			{
				$f->specifier() :{ entry { ReadWriteLock aLock(_monitor); } };
			}
		}
	}
}; // [[derived tree]];

class DerivedMonitor : public Monitor
{
	export/interface Monitor;
	export/implementation Monitor;
};
