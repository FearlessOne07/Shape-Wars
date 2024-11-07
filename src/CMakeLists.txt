add_executable("${PROJECT_NAME}")  # No quotes around "main"
target_sources("${PROJECT_NAME}"    # No quotes around "main"
  PRIVATE
  "./main.cpp"
  "./Core/Game/Game.cpp"
  "./Core/SceneManager/SceneManager.cpp"
  "./Scenes/GameScene/GameScene.cpp"
  "./Player/Player.cpp"
  "./BulletManager/BulletManager.cpp"
  "./EntityManager/EntityManager.cpp"
  "./BulletManager/PlayerBullet/PlayerBullet.cpp"
  "./Enemies/Chaser/Chaser.cpp"
  "./WaveSpawner/WaveSpawner.cpp"
)

target_include_directories("${PROJECT_NAME}"
  PUBLIC
  "${CMAKE_SOURCE_DIR}/vendor/raylib/src/"
  "${CMAKE_SOURCE_DIR}/vendor/jsoncpp/include/"
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_directories("${PROJECT_NAME}"
  PRIVATE
  "${CMAKE_SOURCE_DIR}/lib"
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  raylib
  jsoncpp
)
