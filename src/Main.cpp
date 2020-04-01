#include <Hazel/Core/EntryPoint.h>
#include <Hazel.h>

#include <imgui.h>
#include <glm/glm.hpp>

#include <krpc.hpp>
#include <krpc/services/krpc.hpp>
#include <krpc/services/space_center.hpp>
#include <tuple>

#include <thread>
#include <chrono>

inline static glm::dvec3 c(const std::tuple<double, double, double>& vector)
{
	return { std::get<0>(vector), std::get<1>(vector), std::get<2>(vector) };
}

struct KSPData
{
	double Speed;
	glm::dvec3 Position;
};

KSPData s_Data;

#define DATA_GETTER(field) []() -> float { return s_Data.field; }

class MainLayer : public Hazel::Layer
{
public:

	MainLayer() : m_Airspeed("Airspeed", DATA_GETTER(Speed), 60, 5)
	{

	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Speed %.2f", s_Data.Speed);
		m_Airspeed.Render(400, 200);
		ImGui::End();
	}

	virtual void OnUpdate(Hazel::Timestep ts) override
	{
		m_Airspeed.Update();
	}

private:
	Hazel::Graph m_Airspeed;

};

using namespace krpc::services;

class KSP : public Hazel::Application
{
public:
	KSP() : s_krpcThread([&] { this->KRPCRun(); })
	{
		PushOverlay(new Hazel::DebugLayer());
		PushOverlay(new MainLayer());
	}


	void Update(Hazel::Timestep ts) override
	{
	}

	void Render() override
	{
		Hazel::RenderCommand::Clear();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
				try
				{
					SpaceCenter sc(&connection);
					SpaceCenter::Vessel vessel = sc.active_vessel();
					SpaceCenter::ReferenceFrame refFrame = vessel.orbit().body().reference_frame();
					auto posStream = vessel.position_stream(refFrame);
					auto flightStream = vessel.flight_stream(refFrame);
					while (IsRunning())
					{
						auto flight = flightStream();
						s_Data.Speed = flight.speed();
						s_Data.Position = c(posStream());
					}
				}
				catch (std::exception& error2)
				{

				}

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
