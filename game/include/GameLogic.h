#pragma once
#include <raylib.h>
#include "World.h"
#include "Timer.h"

struct Circle
{
	Engine::BodyRef bodyRef;
	Engine::ColliderRef colliderRef;
	Color color;
	int CollisionNbr = 0;
};


class GameLogic : public Engine::ContactListener
{
public:
	Engine::World world_;
	Engine::Timer timer_;

	std::vector<Engine::BodyRef> _bodyRefs;
	std::vector<Engine::ColliderRef> _colRefs;

	static constexpr std::size_t CirclesInTheWorld = 5;
	std::array<Circle, CirclesInTheWorld> circles;

	static constexpr float VelocityMaxOnStart = 80.0f;
	static constexpr float BorderSizeForElements = 50.0f;
	static constexpr int CircleSegements = 20;
	static constexpr int CircleRadius = 12;

	void Init() noexcept;
	void Update() noexcept;
	void DeInit() noexcept;



	void CreateCircle() noexcept;
	void RenderCircle(SDL_Renderer* renderer) noexcept;
	void ReverseForceOnBorder() noexcept;
	void OnTriggerEnter(Engine::Collider colliderA,
		Engine::Collider colliderB) noexcept override;
	void OnTriggerExit(Engine::Collider colliderA,
		Engine::Collider colliderB) noexcept override;
	void OnCollisionEnter(Engine::Collider colliderA,
		Engine::Collider colliderB) noexcept override;
	void OnCollisionExit(Engine::Collider colliderA,
		Engine::Collider colliderB) noexcept override;
};

