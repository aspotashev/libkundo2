#!/bin/sh

# Unlike 'sed -i ...', this does not touch the file if there were no
# changes in it. Touching a file leads to its recompilation by CMake/Make.
#
# $1 -- sed command
# $2 -- file
function sed_i()
{
	TEMPFILE=$(tempfile)
	sed "$1" "$2" > "$TEMPFILE"
	DIFF=$(diff "$2" "$TEMPFILE")
	if test -n "$DIFF"
	then
		mv "$TEMPFILE" "$2"
	else
		rm "$TEMPFILE"
	fi
}

for fn in `find -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h'`
do
#	sed -i "s/#include <QUndoView>/#include <kundoview2.h>/g" "$fn"
#	sed -i "s/#include <QUndoCommand>/#include <kundocommand2.h>/g" "$fn"
#	sed -i "s/#include <QtGui\/QUndoCommand>/#include <kundocommand2.h>/g" "$fn"
#	sed -i "s/#include <kundostack.h>/#include <kundostack2.h>/g" "$fn"
#	sed -i "s/#include <KUndoStack>/#include <kundostack2.h>/g" "$fn"
#
#	sed -i "s/QUndoView\b/KUndoView2/g" "$fn"
#	sed -i "s/QUndoCommand\b/KUndoCommand2/g" "$fn"
#	sed -i "s/KUndoStack\b/KUndoStack2/g" "$fn"

#-----------------
	sed_i "s/#include <QUndoView>/#include <kundo2view.h>/g" "$fn"

	sed_i "s/#include <QUndoCommand>/#include <kundo2command.h>/g" "$fn"
	sed_i "s/#include <QtGui\/QUndoCommand>/#include <kundo2command.h>/g" "$fn"

	sed_i "s/#include <KUndoStack>/#include <kundo2stack.h>/g" "$fn"
	sed_i "s/#include <kundostack.h>/#include <kundo2stack.h>/g" "$fn"

	sed_i "s/#include <QUndoStack>/#include <kundo2qstack.h>/g" "$fn"
	sed_i "s/#include \"qundostack\.h\"/#include <kundo2qstack.h>/g" "$fn"

	sed_i "s/#include <QUndoGroup>/#include <kundo2group.h>/g" "$fn"
	sed_i "s/#include \"qundogroup\.h\"/#include <kundo2group.h>/g" "$fn"
#-----------------

	sed_i "s/QUndoView\b/KUndo2View/g" "$fn"
	sed_i "s/QUndoCommand\b/KUndo2Command/g" "$fn"
	sed_i "s/KUndoStack\b/KUndo2Stack/g" "$fn"
	sed_i "s/QUndoStack\b/KUndo2QStack/g" "$fn"
	sed_i "s/QUndoGroup\b/KUndo2Group/g" "$fn"
done

