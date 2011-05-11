/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef KUNDOSTACK2_H
#define KUNDOSTACK2_H

#include <QtCore/qobject.h>
#include <QtCore/qstring.h>
#include <QtCore/qlist.h>
#include <QtGui/qaction.h>

#include "kundo2_export.h"

class QAction;
class KUndoCommand2Private;
class KUndoGroup2;
class KActionCollection;

#ifndef QT_NO_UNDOCOMMAND

class KUNDO2_EXPORT KUndoCommand2
{
    KUndoCommand2Private *d;

public:
    explicit KUndoCommand2(KUndoCommand2 *parent = 0);
    explicit KUndoCommand2(const QString &text, KUndoCommand2 *parent = 0);
    virtual ~KUndoCommand2();

    virtual void undo();
    virtual void redo();

    QString actionText() const;
    QString text() const;
    void setText(const QString &text);

    virtual int id() const;
    virtual bool mergeWith(const KUndoCommand2 *other);

    int childCount() const;
    const KUndoCommand2 *child(int index) const;

private:
    Q_DISABLE_COPY(KUndoCommand2)
    friend class KUndoStack2;
};

#endif // QT_NO_UNDOCOMMAND

#ifndef QT_NO_UNDOSTACK

class KUNDO2_EXPORT KUndoStack2 : public QObject
{
    Q_OBJECT
//    Q_DECLARE_PRIVATE(KUndoStack2)
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    Q_PROPERTY(int undoLimit READ undoLimit WRITE setUndoLimit)

public:
    explicit KUndoStack2(QObject *parent = 0);
    ~KUndoStack2();
    void clear();

    void push(KUndoCommand2 *cmd);

    bool canUndo() const;
    bool canRedo() const;
    QString undoText() const;
    QString redoText() const;

    int count() const;
    int index() const;
    QString actionText(int idx) const;
    QString text(int idx) const;

#ifndef QT_NO_ACTION
    QAction *createUndoAction(QObject *parent) const;
    QAction *createRedoAction(QObject *parent) const;
#endif // QT_NO_ACTION

    bool isActive() const;
    bool isClean() const;
    int cleanIndex() const;

    void beginMacro(const QString &text);
    void endMacro();

    void setUndoLimit(int limit);
    int undoLimit() const;

    const KUndoCommand2 *command(int index) const;

    // functions from KUndoStack
    QAction* createRedoAction(KActionCollection* actionCollection, const QString& actionName = QString());
    QAction* createUndoAction(KActionCollection* actionCollection, const QString& actionName = QString());

public Q_SLOTS:
    void setClean();
    void setIndex(int idx);
    void undo();
    void redo();
    void setActive(bool active = true);

Q_SIGNALS:
    void indexChanged(int idx);
    void cleanChanged(bool clean);
    void canUndoChanged(bool canUndo);
    void canRedoChanged(bool canRedo);
    void undoTextChanged(const QString &undoActionText);
    void redoTextChanged(const QString &redoActionText);

private:
    // from QUndoStackPrivate
    QList<KUndoCommand2*> m_command_list;
    QList<KUndoCommand2*> m_macro_stack;
    int m_index;
    int m_clean_index;
    KUndoGroup2 *m_group;
    int m_undo_limit;

    // also from QUndoStackPrivate
    void setIndex(int idx, bool clean);
    bool checkUndoLimit();

    Q_DISABLE_COPY(KUndoStack2)
    friend class KUndoGroup2;
};

#endif // QT_NO_UNDOSTACK

#endif // KUNDOSTACK2_H
