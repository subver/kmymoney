/***************************************************************************
 *   Copyright 2009  Cristian Onet onet.cristian@gmail.com                 *
 *   Copyright 2008  Thomas Baumgart ipwizard@users.sourceforge.net        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation; either version 2 of        *
 *   the License or (at your option) version 3 or any later version        *
 *   accepted by the membership of KDE e.V. (or its successor approved     *
 *   by the membership of KDE e.V.), which shall act as a proxy            *
 *   defined in Section 14 of version 3 of the license.                    *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>  *
 ***************************************************************************/

#ifndef KBANKING_KBACCOUNTSETTINGS_H
#define KBANKING_KBACCOUNTSETTINGS_H

#include <QWidget>

class MyMoneyAccount;
class MyMoneyKeyValueContainer;

class KBAccountSettings: public QWidget
{
public:
  KBAccountSettings(const MyMoneyAccount& acc, QWidget* parent);
  ~KBAccountSettings();

  void loadUi(const MyMoneyKeyValueContainer& kvp);
  void loadKvp(MyMoneyKeyValueContainer& kvp);

private:
  /// \internal d-pointer class.
  struct Private;
  /// \internal d-pointer instance.
  Private* const d;
};


#endif /* KBANKING_KBACCOUNTSETTINGS_H */
