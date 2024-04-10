#pragma once
#include <raylib.h>

#include "ColliderObject.h"
#include "InputsManager.h"
#include "Player.h"
#include "Timer.h"
#include "World.h"

class GameLogic : public Engine::ContactListener {
 public:
  Engine::World world_;
  Engine::Timer timer_;

  Player player{world_};
  Player player2{world_};
  InputsManager inputs;
  std::vector<ColliderObject> Colliders;

  static constexpr int screenWidth = 1480;
  static constexpr int screenHeight = 720;

  const float borderSize = 20.0f;
  const Math::Vec2F platformSize{200.0f, 40.0f};

  void Init() noexcept;
  void Update() noexcept;
  void DeInit() noexcept;

  void ManageInput() noexcept;

  void OnTriggerEnter(Engine::Collider colliderA,
                      Engine::Collider colliderB) noexcept override;
  void OnTriggerExit(Engine::Collider colliderA,
                     Engine::Collider colliderB) noexcept override;
  void OnCollisionEnter(Engine::Collider colliderA,
                        Engine::Collider colliderB) noexcept override;
  void OnCollisionExit(Engine::Collider colliderA,
                       Engine::Collider colliderB) noexcept override;
};
