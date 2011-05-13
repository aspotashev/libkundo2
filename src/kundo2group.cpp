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

#include "kundogroup2.h"
#include "kundostack2.h"
#include "kundostack2_p.h"
#include <KDE/KLocale>

#ifndef QT_NO_UNDOGROUP

/*!
    \class KUndoGroup2
    \brief The KUndoGroup2 class is a group of KUndoStack2 objects.
    \since 4.2

    For an overview of the Qt's undo framework, see the
    \link qundo.html overview\endlink.

    An application often has multiple undo stacks, one for each opened document. At the
    same time, an application usually has one undo action and one redo action, which
    triggers undo or redo in the active document.

    KUndoGroup2 is a group of KUndoStack2 objects, one of which may be active. It has
    an undo() and redo() slot, which calls KUndoStack2::undo() and KUndoStack2::redo()
    for the active stack. It also has the functions createUndoAction() and createRedoAction().
    The actions returned by these functions behave in the same way as those returned by
    KUndoStack2::createUndoAction() and KUndoStack2::createRedoAction() of the active
    stack.

    Stacks are added to a group with addStack() and removed with removeStack(). A stack
    is implicitly added to a group when it is created with the group as its parent
    QObject.

    It is the programmer's responsibility to specify which stack is active by
    calling KUndoStack2::setActive(), usually when the associated document window receives focus.
    The active stack may also be set with setActiveStack(), and is returned by activeStack().

    When a stack is added to a group using addStack(), the group does not take ownership
    of the stack. This means the stack has to be deleted separately from the group. When
    a stack is deleted, it is automatically removed from a group. A stack may belong to
    only one group. Adding it to another group will cause it to be removed from the previous
    group.

    A KUndoGroup2 is also useful in conjunction with KUndoView2. If a KUndoView2 is
    set to watch a group using KUndoView2::setGroup(), it will update itself to display
    the active stack.
*/

/*!
    Creates an empty KUndoGroup2 object with parent \a parent.

    \sa addStack()
*/

KUndoGroup2::KUndoGroup2(QObject *parent)
    : QObject(parent), m_active(0)
{
}

/*!
    Destroys the KUndoGroup2.
*/
KUndoGroup2::~KUndoGroup2()
{
    // Ensure all KUndoStack2s no longer refer to this group.
    QList<KUndoStack2 *>::iterator it = m_stack_list.begin();
    QList<KUndoStack2 *>::iterator end = m_stack_list.end();
    while (it != end) {
        (*it)->m_group = 0;
        ++it;
    }
}

/*!
    Adds \a stack to this group. The group does not take ownership of the stack. Another
    way of adding a stack to a group is by specifying the group as the stack's parent
    QObject in KUndoStack2::KUndoStack2(). In this case, the stack is deleted when the
    group is deleted, in the usual manner of QObjects.

    \sa removeStack() stacks() KUndoStack2::KUndoStack2()
*/

void KUndoGroup2::addStack(KUndoStack2 *stack)
{
    if (m_stack_list.contains(stack))
        return;
    m_stack_list.append(stack);

    if (KUndoGroup2 *other = stack->m_group)
        other->removeStack(stack);
    stack->m_group = this;
}

/*!
    Removes \a stack from this group. If the stack was the active stack in the group,
    the active stack becomes 0.

    \sa addStack() stacks() KUndoStack2::~KUndoStack2()
*/

void KUndoGroup2::removeStack(KUndoStack2 *stack)
{
    if (m_stack_list.removeAll(stack) == 0)
        return;
    if (stack == m_active)
        setActiveStack(0);
    stack->m_group = 0;
}

/*!
    Returns a list of stacks in this group.

    \sa addStack() removeStack()
*/

QList<KUndoStack2*> KUndoGroup2::stacks() const
{
    return m_stack_list;
}

/*!
    Sets the active stack of this group to \a stack.

    If the stack is not a member of this group, this function does nothing.

    Synonymous with calling KUndoStack2::setActive() on \a stack.

    The actions returned by createUndoAction() and createRedoAction() will now behave
    in the same way as those returned by \a stack's KUndoStack2::createUndoAction()
    and KUndoStack2::createRedoAction().

    \sa KUndoStack2::setActive() activeStack()
*/

void KUndoGroup2::setActiveStack(KUndoStack2 *stack)
{
    if (m_active == stack)
        return;

    if (m_active != 0) {
        disconnect(m_active, SIGNAL(canUndoChanged(bool)),
                    this, SIGNAL(canUndoChanged(bool)));
        disconnect(m_active, SIGNAL(undoTextChanged(QString)),
                    this, SIGNAL(undoTextChanged(QString)));
        disconnect(m_active, SIGNAL(canRedoChanged(bool)),
                    this, SIGNAL(canRedoChanged(bool)));
        disconnect(m_active, SIGNAL(redoTextChanged(QString)),
                    this, SIGNAL(redoTextChanged(QString)));
        disconnect(m_active, SIGNAL(indexChanged(int)),
                    this, SIGNAL(indexChanged(int)));
        disconnect(m_active, SIGNAL(cleanChanged(bool)),
                    this, SIGNAL(cleanChanged(bool)));
    }

    m_active = stack;

    if (m_active == 0) {
        emit canUndoChanged(false);
        emit undoTextChanged(QString());
        emit canRedoChanged(false);
        emit redoTextChanged(QString());
        emit cleanChanged(true);
        emit indexChanged(0);
    } else {
        connect(m_active, SIGNAL(canUndoChanged(bool)),
                this, SIGNAL(canUndoChanged(bool)));
        connect(m_active, SIGNAL(undoTextChanged(QString)),
                this, SIGNAL(undoTextChanged(QString)));
        connect(m_active, SIGNAL(canRedoChanged(bool)),
                this, SIGNAL(canRedoChanged(bool)));
        connect(m_active, SIGNAL(redoTextChanged(QString)),
                this, SIGNAL(redoTextChanged(QString)));
        connect(m_active, SIGNAL(indexChanged(int)),
                this, SIGNAL(indexChanged(int)));
        connect(m_active, SIGNAL(cleanChanged(bool)),
                this, SIGNAL(cleanChanged(bool)));
        emit canUndoChanged(m_active->canUndo());
        emit undoTextChanged(m_active->undoText());
        emit canRedoChanged(m_active->canRedo());
        emit redoTextChanged(m_active->redoText());
        emit cleanChanged(m_active->isClean());
        emit indexChanged(m_active->index());
    }

    emit activeStackChanged(m_active);
}

/*!
    Returns the active stack of this group.

    If none of the stacks are active, or if the group is empty, this function
    returns 0.

    \sa setActiveStack() KUndoStack2::setActive()
*/

KUndoStack2 *KUndoGroup2::activeStack() const
{
    return m_active;
}

/*!
    Calls KUndoStack2::undo() on the active stack.

    If none of the stacks are active, or if the group is empty, this function
    does nothing.

    \sa redo() canUndo() setActiveStack()
*/

void KUndoGroup2::undo()
{
    if (m_active != 0)
        m_active->undo();
}

/*!
    Calls KUndoStack2::redo() on the active stack.

    If none of the stacks are active, or if the group is empty, this function
    does nothing.

    \sa undo() canRedo() setActiveStack()
*/


void KUndoGroup2::redo()
{
    if (m_active != 0)
        m_active->redo();
}

/*!
    Returns the value of the active stack's KUndoStack2::canUndo().

    If none of the stacks are active, or if the group is empty, this function
    returns false.

    \sa canRedo() setActiveStack()
*/

bool KUndoGroup2::canUndo() const
{
    return m_active != 0 && m_active->canUndo();
}

/*!
    Returns the value of the active stack's KUndoStack2::canRedo().

    If none of the stacks are active, or if the group is empty, this function
    returns false.

    \sa canUndo() setActiveStack()
*/

bool KUndoGroup2::canRedo() const
{
    return m_active != 0 && m_active->canRedo();
}

/*!
    Returns the value of the active stack's KUndoStack2::undoActionText().

    If none of the stacks are active, or if the group is empty, this function
    returns an empty string.

    \sa undoItemText() redoActionText() setActiveStack()
*/

QString KUndoGroup2::undoText() const
{
    return m_active == 0 ? QString() : m_active->undoText();
}

/*!
    Returns the value of the active stack's KUndoStack2::redoActionText().

    If none of the stacks are active, or if the group is empty, this function
    returns an empty string.

    \sa redoItemText() undoActionText() setActiveStack()
*/

QString KUndoGroup2::redoText() const
{
    return m_active == 0 ? QString() : m_active->redoText();
}

/*!
    Returns the value of the active stack's KUndoStack2::isClean().

    If none of the stacks are active, or if the group is empty, this function
    returns true.

    \sa setActiveStack()
*/

bool KUndoGroup2::isClean() const
{
    return m_active == 0 || m_active->isClean();
}

#ifndef QT_NO_ACTION

/*!
    Creates an undo QAction object with parent \a parent.

    Triggering this action will cause a call to KUndoStack2::undo() on the active stack.
    The text of this action will always be the text of the command which will be undone
    in the next call to undo(), prefixed by \a prefix. If there is no command available
    for undo, if the group is empty or if none of the stacks are active, this action will
    be disabled.

    If \a prefix is empty, the default prefix "Undo" is used.

    \sa createRedoAction() canUndo() KUndoCommand2::text()
*/

QAction *KUndoGroup2::createUndoAction(QObject *parent) const
{
    KUndoAction2 *result = new KUndoAction2(i18n("Undo %1"), i18nc("Default text for undo action", "Undo"), parent);
    result->setEnabled(canUndo());
    result->setPrefixedText(undoText());
    connect(this, SIGNAL(canUndoChanged(bool)),
            result, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(undoTextChanged(QString)),
            result, SLOT(setPrefixedText(QString)));
    connect(result, SIGNAL(triggered()), this, SLOT(undo()));
    return result;
}

/*!
    Creates an redo QAction object with parent \a parent.

    Triggering this action will cause a call to KUndoStack2::redo() on the active stack.
    The text of this action will always be the text of the command which will be redone
    in the next call to redo(), prefixed by \a prefix. If there is no command available
    for redo, if the group is empty or if none of the stacks are active, this action will
    be disabled.

    If \a prefix is empty, the default prefix "Undo" is used.

    \sa createUndoAction() canRedo() KUndoCommand2::text()
*/

QAction *KUndoGroup2::createRedoAction(QObject *parent) const
{
    KUndoAction2 *result = new KUndoAction2(i18n("Redo %1"), i18nc("Default text for redo action", "Redo"), parent);
    result->setEnabled(canRedo());
    result->setPrefixedText(redoText());
    connect(this, SIGNAL(canRedoChanged(bool)),
            result, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(redoTextChanged(QString)),
            result, SLOT(setPrefixedText(QString)));
    connect(result, SIGNAL(triggered()), this, SLOT(redo()));
    return result;
}

#endif // QT_NO_ACTION

/*! \fn void KUndoGroup2::activeStackChanged(KUndoStack2 *stack)

    This signal is emitted whenever the active stack of the group changes. This can happen
    when setActiveStack() or KUndoStack2::setActive() is called, or when the active stack
    is removed form the group. \a stack is the new active stack. If no stack is active,
    \a stack is 0.

    \sa setActiveStack() KUndoStack2::setActive()
*/

/*! \fn void KUndoGroup2::indexChanged(int idx)

    This signal is emitted whenever the active stack emits KUndoStack2::indexChanged()
    or the active stack changes.

    \a idx is the new current index, or 0 if the active stack is 0.

    \sa KUndoStack2::indexChanged() setActiveStack()
*/

/*! \fn void KUndoGroup2::cleanChanged(bool clean)

    This signal is emitted whenever the active stack emits KUndoStack2::cleanChanged()
    or the active stack changes.

    \a clean is the new state, or true if the active stack is 0.

    \sa KUndoStack2::cleanChanged() setActiveStack()
*/

/*! \fn void KUndoGroup2::canUndoChanged(bool canUndo)

    This signal is emitted whenever the active stack emits KUndoStack2::canUndoChanged()
    or the active stack changes.

    \a canUndo is the new state, or false if the active stack is 0.

    \sa KUndoStack2::canUndoChanged() setActiveStack()
*/

/*! \fn void KUndoGroup2::canRedoChanged(bool canRedo)

    This signal is emitted whenever the active stack emits KUndoStack2::canRedoChanged()
    or the active stack changes.

    \a canRedo is the new state, or false if the active stack is 0.

    \sa KUndoStack2::canRedoChanged() setActiveStack()
*/

/*! \fn void KUndoGroup2::undoTextChanged(const QString &undoText)

    This signal is emitted whenever the active stack emits KUndoStack2::undoTextChanged()
    or the active stack changes.

    \a undoText is the new state, or an empty string if the active stack is 0.

    \sa KUndoStack2::undoTextChanged() setActiveStack()
*/

/*! \fn void KUndoGroup2::redoTextChanged(const QString &redoText)

    This signal is emitted whenever the active stack emits KUndoStack2::redoTextChanged()
    or the active stack changes.

    \a redoText is the new state, or an empty string if the active stack is 0.

    \sa KUndoStack2::redoTextChanged() setActiveStack()
*/

#endif // QT_NO_UNDOGROUP
