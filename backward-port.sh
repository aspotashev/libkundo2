#!/bin/sh

for fn in `find -name '*.c' -o -name '*.cpp' -o -name '*.cc' -o -name '*.h' | grep -v '^\./b/'`
do
	sed -i "s/KUndo2View/QUndoView/g" "$fn"
	sed -i "s/KUndo2Command/QUndoCommand/g" "$fn"
	sed -i "s/KUndo2Stack/KUndoStack/g" "$fn"
	sed -i "s/KUndo2QStack/QUndoStack/g" "$fn"
	sed -i "s/KUndo2Group/QUndoGroup/g" "$fn"

	sed -i "s/#include <kundo2view.h>/#include <QUndoView>/g" "$fn"
	sed -i "s/#include <kundo2command.h>/#include <QUndoCommand>/g" "$fn"
	sed -i "s/#include <kundo2stack.h>/#include <KUndoStack>/g" "$fn"
	sed -i "s/#include <kundo2qstack.h>/#include <QUndoStack>/g" "$fn"
	sed -i "s/#include <kundo2group.h>/#include <QUndoGroup>/g" "$fn"
done

