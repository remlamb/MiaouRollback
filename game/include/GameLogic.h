#pragma once
#include <raylib.h>

#include "Collider.h"
#include "InputsManager.h"
#include "PlayerManager.h"
#include "Timer.h"
#include "World.h"

class GameLogic{
 public:
  Physics::World world_;
  Physics::Timer timer_;

  PlayerManager player{&world_};
  InputsManager inputs;
  std::vector<game::Collider> Colliders;

  static constexpr int screenWidth = 1480;
  static constexpr int screenHeight = 720;

  const float borderSize = 20.0f;
  const Math::Vec2F platformSize{200.0f, 40.0f};

  void Init() noexcept;
  void Update() noexcept;
  void DeInit() noexcept;

  void ManageInput() noexcept;
};
