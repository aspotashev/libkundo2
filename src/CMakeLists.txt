project(kundo2)

include_directories(
	${QT_INCLUDES}
)

set(kundo2_LIB_SRCS
	kundo2stack.cpp
	kundo2group.cpp
	kundo2view.cpp
	kundo2model.cpp
)

kde4_add_library(kundo2 SHARED ${kundo2_LIB_SRCS})
target_link_libraries(kundo2
	${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS})

install( FILES
	kundo2stack.h
	kundo2group.h
	kundo2view.h
	kundo2model.h
	kundo2command.h
	kundo2_export.h
	DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )

install(TARGETS kundo2 ${INSTALL_TARGETS_DEFAULT_ARGS})

