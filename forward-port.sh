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
	fi

	rm "$TEMPFILE"
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

	sed_i "s/#include <QUndoView>/#include <kundoview2.h>/g" "$fn"
	sed_i "s/#include <QUndoCommand>/#include <kundocommand2.h>/g" "$fn"
	sed_i "s/#include <QtGui\/QUndoCommand>/#include <kundocommand2.h>/g" "$fn"
	sed_i "s/#include <kundostack.h>/#include <kundostack2.h>/g" "$fn"
	sed_i "s/#include <KUndoStack>/#include <kundostack2.h>/g" "$fn"
	sed_i "s/#include <QUndoStack>/#include <kundostack2.h>/g" "$fn"
	sed_i "s/#include <QUndoGroup>/#include <kundogroup2.h>/g" "$fn"
	sed_i "s/#include \"qundostack\.h\"/#include <kundostack2.h>/g" "$fn"
	sed_i "s/#include \"qundogroup\.h\"/#include <kundogroup2.h>/g" "$fn"

	# temporary
	sed_i "s/#include <QtGui\/KUndoCommand2>/#include <kundocommand2.h>/g" "$fn"

	sed_i "s/QUndoView\b/KUndoView2/g" "$fn"
	sed_i "s/QUndoCommand\b/KUndoCommand2/g" "$fn"
	sed_i "s/KUndoStack\b/KUndoStack2/g" "$fn"
	sed_i "s/QUndoStack\b/KUndoStack2/g" "$fn"
	sed_i "s/QUndoGroup\b/KUndoGroup2/g" "$fn"
done

