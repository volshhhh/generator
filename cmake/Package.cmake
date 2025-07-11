# Настройка установки и создания пакетов

# Установка исполняемого файла
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)

# Установка заголовочных файлов (если планируется создание библиотеки)
install(DIRECTORY entities/
    DESTINATION include/${PROJECT_NAME}/entities
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Установка документации
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    install(FILES README.md
        DESTINATION share/doc/${PROJECT_NAME}
    )
endif()

# Настройка CPack для создания пакетов
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})

# Генераторы пакетов
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP;DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ;DEB")
endif()

include(CPack)
