#include <Hazel/Core/EntryPoint.h>

#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <thread>

class KSP : public Hazel::Application
{
public:
	KSP() : s_krpcThread([&] { this->KRPCRun(); })
	{
		
	}


	void Update(Hazel::Timestep ts)
	{

	}

	void Render()
	{

	}

	~KSP()
	{
		s_krpcThread.join();
	}

private:
	void KRPCRun()
	{
		HZ_INFO("Started KRPC connection thread!");
		while (IsRunning())
		{
			try
			{
				auto connection = krpc::connect();
				krpc::services::KRPC krpc(&connection);
				HZ_INFO("Connected to kRPC server version {}", krpc.get_status().version());
			}
			catch (std::exception& error)
			{
				HZ_ERROR("Unable to connect to KSP! {}", error.what());
			}
		}
		HZ_INFO("Exiting KRPC thread");
	}

private:
	std::thread s_krpcThread;
};

Hazel::Application* HZCreateApplication(int argc, char** argv)
{
	return new KSP();
}

void HZGetApplicationInfo(Hazel::ApplicationInfo* info)
{
	info->Name = "Mission Control";
	info->Publisher = "Troy Inc.";
	info->Version = "v0.0.1";
	info->VersionInt = 1;
}
