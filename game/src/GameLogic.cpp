#include "GameLogic.h"

#include "Random.h"

void GameLogic::Init() noexcept
{
	timer_.OnStart();
	world_.Init();
}
void GameLogic::Update() noexcept
{
	world_.Update(timer_.DeltaTime());
}
void GameLogic::DeInit() noexcept
{
	_bodyRefs.clear();
	_colRefs.clear();
	world_.Clear();
}





void GameLogic::CreateCircle() noexcept
{
	int currentID = 0;
	for (auto& circle : circles)
	{
		circle.bodyRef = world_.CreateBody();
		auto& circleBody = world_.GetBody(circle.bodyRef);
		circleBody.SetMass(1);
		const Math::Vec2F rndPos = Math::Vec2F(Math::Random::Range(100.f, Metrics::WIDTH - 100),
			Math::Random::Range(100.f, Metrics::HEIGHT - 100));
		circleBody.SetPosition(rndPos);
		circleBody.SetVelocity(Math::Vec2F(Math::Random::Range(-VelocityMaxOnStart, VelocityMaxOnStart),
			Math::Random::Range(-VelocityMaxOnStart, VelocityMaxOnStart)));
		circle.color = Color{ static_cast<unsigned char>(Math::Random::Range(0, 256)),
			static_cast<unsigned char>(Math::Random::Range(0, 256)),
			static_cast<unsigned char>(Math::Random::Range(0, 256)),
			static_cast <unsigned char>(255) };

		circle.colliderRef = world_.CreateCollider(circle.bodyRef);
		auto& circleCollider = world_.GetCollider(circle.colliderRef);
		circleCollider.ID = currentID++;;
		circleCollider._shape = Math::ShapeType::Circle;
		circleCollider.isTrigger = false;
		const auto bodyPosition = circleBody.Position();
		circleCollider.circleShape = Math::CircleF(bodyPosition, CircleRadius);
	}
}

void GameLogic::RenderCircle(SDL_Renderer* renderer) noexcept
{
	for (auto& circle : circles)
	{
		auto& circleBody = world_.GetBody(circle.bodyRef);
		Display::DrawCircle(renderer, circleBody.Position(), CircleRadius,
			circle.color, CircleSegements);
	}
}

void GameLogic::ReverseForceOnBorder() noexcept
{
	for (auto& circle : circles)
	{
		auto& circleBody = world_.GetBody(circle.bodyRef);
		const auto position = circleBody.Position();
		const auto velocity = circleBody.Velocity();

		if (position.X >= Metrics::WIDTH - BorderSizeForElements)
		{
			circleBody.SetPosition(Math::Vec2F(Metrics::WIDTH - BorderSizeForElements, position.Y));
			circleBody.SetVelocity(Math::Vec2F(-velocity.X, velocity.Y));
		}

		if (position.X <= 0 + BorderSizeForElements)
		{
			circleBody.SetPosition(Math::Vec2F(BorderSizeForElements, position.Y));
			circleBody.SetVelocity(Math::Vec2F(-velocity.X, velocity.Y));
		}

		if (position.Y >= Metrics::HEIGHT - BorderSizeForElements)
		{
			circleBody.SetPosition(Math::Vec2F(position.X, Metrics::HEIGHT - BorderSizeForElements));
			circleBody.SetVelocity(Math::Vec2F(velocity.X, -velocity.Y));
		}

		if (position.Y <= 0 + BorderSizeForElements)
		{
			circleBody.SetPosition(Math::Vec2F(position.X, 0 + BorderSizeForElements));
			circleBody.SetVelocity(Math::Vec2F(velocity.X, -velocity.Y));
		}
	}
}

void GameLogic::OnTriggerEnter(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{
	for (auto& circle : circles)
	{
		const auto circleCollider = world_.GetCollider(circle.colliderRef);
		if (circleCollider == colliderA || circleCollider == colliderB)
		{
			circle.CollisionNbr = true;
		}
	}
}

void GameLogic::OnTriggerExit(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{
	for (auto& circle : circles)
	{
		const auto circleCollider = world_.GetCollider(circle.colliderRef);
		if (circleCollider == colliderA || circleCollider == colliderB)
		{
			circle.CollisionNbr = false;
		}
	}
}

void GameLogic::OnCollisionEnter(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{

}

void GameLogic::OnCollisionExit(Engine::Collider colliderA, Engine::Collider colliderB) noexcept
{

}