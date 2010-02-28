/***************************************************************************
                          kmymoneycompletion.cpp  -  description
                             -------------------
    begin                : Mon Apr 26 2004
    copyright            : (C) 2000-2004 by Michael Edwardes
    email                : mte@users.sourceforge.net
                           Javier Campos Morales <javi_c@users.sourceforge.net>
                           Felix Rodriguez <frodriguez@users.sourceforge.net>
                           John C <thetacoturtle@users.sourceforge.net>
                           Thomas Baumgart <ipwizard@users.sourceforge.net>
                           Kevin Tambascio <ktambascio@users.sourceforge.net>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kmymoneycompletion.h"

// ----------------------------------------------------------------------------
// QT Includes

#include <QApplication>
#include <QFrame>
#include <QKeyEvent>
#include <QEvent>
#include <QDesktopWidget>
#include <QLineEdit>

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

#include <kmymoneyselector.h>
#include <kmymoneychecklistitem.h>
#include "kmymoneycombo.h"

const int kMyMoneyCompletion::MAX_ITEMS = 16;

kMyMoneyCompletion::kMyMoneyCompletion(QWidget *parent) :
    KVBox(parent)
{
  setWindowFlags(Qt::Popup);
  setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

  m_parent = parent;
  setFocusProxy(parent);

  m_selector = new KMyMoneySelector(this);

  connectSignals(m_selector, m_selector->listView());
}

void kMyMoneyCompletion::connectSignals(QWidget* widget, QTreeWidget* lv)
{
  m_widget = widget;
  m_lv = lv;
  connect(lv, SIGNAL(itemActivated(QTreeWidgetItem*,int)), this, SLOT(slotItemSelected(QTreeWidgetItem*, int)));
}

kMyMoneyCompletion::~kMyMoneyCompletion()
{
}

void kMyMoneyCompletion::adjustSize(void)
{
  QTreeWidgetItemIterator it(m_lv, QTreeWidgetItemIterator::NotHidden);
  int count = 0;
  while (*it) {
    ++count;
    ++it;
  }
  adjustSize(count);
}

void kMyMoneyCompletion::adjustSize(const int count)
{
  int w = m_widget->sizeHint().width();
  if (m_parent && w < m_parent->width())
    w = m_parent->width();

  QFontMetrics fm(font());
  if (w < fm.maxWidth()*15)
    w = fm.maxWidth() * 15;

  int h = 0;
  QTreeWidgetItemIterator it(m_lv, QTreeWidgetItemIterator::NotHidden);
  QTreeWidgetItem* item = *it;
  if (item)
    // the +1 in the next statement avoids the display of a scroll bar if count < MAX_ITEMS.
    h = item->treeWidget()->visualItemRect(item).height() * (count > MAX_ITEMS - 1 ? MAX_ITEMS : count + 1);

  resize(w, h);

  if (m_parent) {
    // the code of this basic block is taken from K3CompletionBox::show()
    // and modified to our local needs

    QRect screenSize = QApplication::desktop()->availableGeometry(parentWidget());

    QPoint orig = m_parent->mapToGlobal(QPoint(0, m_parent->height()));
    int x = orig.x();
    int y = orig.y();

    if (x + width() > screenSize.right())
      x = screenSize.right() - width();

    // check for the maximum height here to avoid flipping
    // of the completion box from top to bottom of the
    // edit widget. The offset (y) is certainly based
    // on the actual height.
    if (item) {
      if ((y + item->treeWidget()->visualItemRect(item).height() * MAX_ITEMS) > screenSize.bottom())
        y = y - height() - m_parent->height();
    }

    move(x, y);
  }
}

void kMyMoneyCompletion::showEvent(QShowEvent*)
{
  show(true);
}

void kMyMoneyCompletion::show(bool presetSelected)
{
  if (!m_id.isEmpty() && presetSelected)
    m_selector->setSelected(m_id);

  adjustSize();

  if (m_parent) {
    m_parent->installEventFilter(this);
    m_parent->grabKeyboard();
    // make sure to install the filter for the combobox lineedit as well
    // We have do this here because QObject::installEventFilter() is not
    // declared virtual and we have no chance to override it in KMyMoneyCombo
    KMyMoneyCombo* c = dynamic_cast<KMyMoneyCombo*>(m_parent);
    if (c && c->lineEdit()) {
      c->lineEdit()->installEventFilter(this);
    }
  }

  KVBox::show();
}

void kMyMoneyCompletion::hide(void)
{
  if (m_parent) {
    m_parent->removeEventFilter(this);
    m_parent->releaseKeyboard();
    // make sure to uninstall the filter for the combobox lineedit as well
    // We have do this here because QObject::installEventFilter() is not
    // declared virtual and we have no chance to override it in KMyMoneyCombo
    KMyMoneyCombo* c = dynamic_cast<KMyMoneyCombo*>(m_parent);
    if (c && c->lineEdit()) {
      c->lineEdit()->removeEventFilter(this);
    }
  }
  KVBox::hide();
}

bool kMyMoneyCompletion::eventFilter(QObject* o, QEvent* e)
{
  KMyMoneyCombo *c = dynamic_cast<KMyMoneyCombo*>(m_parent);
  QTreeWidgetItem* item;
  if (o == m_parent || (c && o == c->lineEdit())) {
    if (isVisible()) {
      if (e->type() == QEvent::KeyPress) {
        QKeyEvent* ev = static_cast<QKeyEvent*>(e);
        QKeyEvent evt(QEvent::KeyPress,
                      Qt::Key_Down, ev->modifiers(), QString(),
                      ev->isAutoRepeat(), ev->count());
        QKeyEvent evbt(QEvent::KeyPress,
                       Qt::Key_Up, ev->modifiers(), QString(),
                       ev->isAutoRepeat(), ev->count());

        switch (ev->key()) {
        case Qt::Key_Tab:
        case Qt::Key_Backtab:
          slotItemSelected(m_lv->currentItem(), 0);
          break;

        case Qt::Key_Down:
        case Qt::Key_PageDown:
          item = m_lv->currentItem();
          while (item) {
            item = m_lv->itemBelow(item);
            if (item && selector()->match(m_lastCompletion, item))
              break;
          }
          if (item) {
            m_lv->setCurrentItem(item);
            selector()->ensureItemVisible(item);
          }
          ev->accept();
          return true;

        case Qt::Key_Up:
        case Qt::Key_PageUp:
          item = m_lv->currentItem();
          while (item) {
            item = m_lv->itemAbove(item);
            if (item && selector()->match(m_lastCompletion, item))
              break;
          }
          if (item) {
            m_lv->setCurrentItem(item);
            // make sure, we always see a possible (non-selectable) group item
            if (m_lv->itemAbove(item))
              item = m_lv->itemAbove(item);
            selector()->ensureItemVisible(item);
          }
          ev->accept();
          return true;

        case Qt::Key_Escape:
          hide();
          ev->accept();
          return true;

        case Qt::Key_Enter:
        case Qt::Key_Return:
          slotItemSelected(m_lv->currentItem(), 0);
          ev->accept();
          return true;

        case Qt::Key_Home:
        case Qt::Key_End:
          if (ev->modifiers() & Qt::ControlModifier) {
            item = m_lv->currentItem();
            if (ev->key() == Qt::Key_Home) {
              while (item && m_lv->itemAbove(item)) {
                item = m_lv->itemAbove(item);
              }
              while (item && !selector()->match(m_lastCompletion, item)) {
                item = m_lv->itemBelow(item);
              }
            } else {
              while (item && m_lv->itemBelow(item)) {
                item = m_lv->itemBelow(item);
              }
              while (item && !selector()->match(m_lastCompletion, item)) {
                item = m_lv->itemAbove(item);
              }
            }
            if (item) {
              m_lv->setCurrentItem(item);
              // make sure, we always see a possible (non-selectable) group item
              if (m_lv->itemAbove(item))
                item = m_lv->itemAbove(item);
              selector()->ensureItemVisible(item);
            }
            ev->accept();
            return true;
          }
          break;

        default:
          break;

        }
      }
    }
  }
  return KVBox::eventFilter(o, e);
}

void kMyMoneyCompletion::slotMakeCompletion(const QString& txt)
{
  int cnt = selector()->slotMakeCompletion(txt.trimmed());

  if (m_parent && m_parent->isVisible() && !isVisible() && cnt)
    show(false);
  else {
    if (cnt != 0) {
      adjustSize();
    } else {
      hide();
    }
  }
}

void kMyMoneyCompletion::slotItemSelected(QTreeWidgetItem *item, int)
{
  KMyMoneyTreeWidgetItem* it_v = dynamic_cast<KMyMoneyTreeWidgetItem*>(item);
  if (it_v && it_v->isSelectable()) {
    QString id = it_v->id();
    // hide the widget, so we can debug the slots that are connect
    // to the signal we emit very soon
    hide();
    m_id = id;
    emit itemSelected(id);
  }
}

void kMyMoneyCompletion::setSelected(const QString& id)
{
  m_id = id;
  m_selector->setSelected(id, true);
}

#include "kmymoneycompletion.moc"
