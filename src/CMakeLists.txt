set(SRC
  "./main.cpp"
  "./Scenes/GameScene.cpp"
  "./Systems/RotationSystem/RotationSystem.cpp"
  "./Systems/BulletSystem/BulletSystem.cpp"
)

add_executable("${PROJECT_NAME}"
  "${SRC}"
)

target_include_directories("${PROJECT_NAME}"
  PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/vendor/Base/include"
)

target_link_libraries("${PROJECT_NAME}"
  PRIVATE
    base
)
