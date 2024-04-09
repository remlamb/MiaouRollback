#pragma once
#include <raylib.h>

#include "ColliderObject.h"
#include "Timer.h"
#include "World.h"
#include "InputsManager.h"
#include "Player.h"



class GameLogic : public Engine::ContactListener {
 public:
  Engine::World world_;
  Engine::Timer timer_;

  Player player;
  InputsManager inputs;


  std::vector<Engine::BodyRef> _bodyRefs;
  std::vector<Engine::ColliderRef> _colRefs;

  int colliderCurrentID = 0;
  static constexpr std::size_t CirclesInTheWorld = 50;
  std::vector<ColliderObject> Colliders;

  static constexpr float VelocityMaxOnStart = 10.0f;
  static constexpr float BorderSizeForElements = 50.0f;

  static constexpr int screenWidth = 1480;
  //static constexpr int screenWidth = 1080;
  static constexpr int screenHeight = 720;

  const float borderSize = 20.0f;
  const Math::Vec2F platformSize{ 200.0f, 40.0f };

  void Init() noexcept;
  void Update() noexcept;
  void DeInit() noexcept;

  

  void OnTriggerEnter(Engine::Collider colliderA,
                      Engine::Collider colliderB) noexcept override;
  void OnTriggerExit(Engine::Collider colliderA,
                     Engine::Collider colliderB) noexcept override;
  void OnCollisionEnter(Engine::Collider colliderA,
                        Engine::Collider colliderB) noexcept override;
  void OnCollisionExit(Engine::Collider colliderA,
                       Engine::Collider colliderB) noexcept override;
};
