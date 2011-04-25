project(kundo2)

include_directories(
	${QT_INCLUDES}
)

set(kundo2_LIB_SRCS
	kundostack2.cpp
	kundogroup2.cpp
	kundoview2.cpp
	kundomodel2.cpp
)

kde4_add_library(kundo2 SHARED ${kundo2_LIB_SRCS})
target_link_libraries(kundo2 ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${KDE4_KDECORE_LIBS})


install( FILES
	kundostack2.h
	kundogroup2.h
	kundoview2.h
	kundomodel2.h
	kundo2_export.h
	DESTINATION ${INCLUDE_INSTALL_DIR} COMPONENT Devel )

install(TARGETS kundo2 ${INSTALL_TARGETS_DEFAULT_ARGS})

