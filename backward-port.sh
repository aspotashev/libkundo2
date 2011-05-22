#!/bin/sh

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
	sed_i "s/KUndo2View/QUndoView/g" "$fn"
	sed_i "s/KUndo2Command/QUndoCommand/g" "$fn"
	sed_i "s/KUndo2Stack/KUndoStack/g" "$fn"
	sed_i "s/KUndo2QStack/QUndoStack/g" "$fn"
	sed_i "s/KUndo2Group/QUndoGroup/g" "$fn"

	sed_i "s/#include <kundo2view.h>/#include <QUndoView>/g" "$fn"
	sed_i "s/#include <kundo2command.h>/#include <QUndoCommand>/g" "$fn"
	sed_i "s/#include <kundo2stack.h>/#include <KUndoStack>/g" "$fn"
	sed_i "s/#include <kundo2qstack.h>/#include <QUndoStack>/g" "$fn"
	sed_i "s/#include <kundo2group.h>/#include <QUndoGroup>/g" "$fn"
done

rm -f "$TEMPFILE"

