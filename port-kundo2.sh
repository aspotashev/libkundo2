#!/bin/sh

COMMAND=$1
if test "$1" = "forward"
then
	BACKWARD=false
elif test "$1" = "backward"
then
	BACKWARD=true
else
	echo "Usage: ./port-kundo2.sh forward"
	echo "       ./port-kundo2.sh backward"
	exit
fi

TEMPFILE=$(tempfile)

# Unlike 'sed -i ...', this does not touch the file if there were no
# changes in it. Touching a file leads to its recompilation by CMake/Make.
#
# $1 -- sed command
# $2 -- file
function sed_i()
{
	sed "$1" "$2" > "$TEMPFILE"
	DIFF=$(diff "$2" "$TEMPFILE")
	if test -n "$DIFF"
	then
		mv "$TEMPFILE" "$2"
	fi
}

for fn in `find -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' | grep -v '^\./b/' | grep -v '^\./libs/kundo2/'`
do
	if ( $BACKWARD )
	then
		sed_i "s/KUndo2View/QUndoView/g;s/KUndo2Command/QUndoCommand/g;s/KUndo2Stack/KUndoStack/g;s/KUndo2QStack/QUndoStack/g;s/KUndo2Group/QUndoGroup/g;s/#include <kundo2view.h>/#include <QUndoView>/g;s/#include <kundo2command.h>/#include <QUndoCommand>/g;s/#include <kundo2stack.h>/#include <KUndoStack>/g;s/#include <kundo2qstack.h>/#include <QUndoStack>/g;s/#include <kundo2group.h>/#include <QUndoGroup>/g" "$fn"
	else
		sed_i "s/#include <QUndoView>/#include <kundo2view.h>/g;s/#include <QUndoCommand>/#include <kundo2command.h>/g;s/#include <QtGui\/QUndoCommand>/#include <kundo2command.h>/g;s/#include <KUndoStack>/#include <kundo2stack.h>/g;s/#include <kundostack.h>/#include <kundo2stack.h>/g;s/#include <QUndoStack>/#include <kundo2qstack.h>/g;s/#include \"qundostack\.h\"/#include <kundo2qstack.h>/g;s/#include <QUndoGroup>/#include <kundo2group.h>/g;s/#include \"qundogroup\.h\"/#include <kundo2group.h>/g;s/QUndoView\b/KUndo2View/g;s/QUndoCommand\b/KUndo2Command/g;s/KUndoStack\b/KUndo2Stack/g;s/QUndoStack\b/KUndo2QStack/g;s/QUndoGroup\b/KUndo2Group/g" "$fn"
	fi
done

rm -f "$TEMPFILE"

