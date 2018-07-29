/*
 * This file is part of KMyMoney, A Personal Finance Manager by KDE
 * Copyright (C) 2014 Christian Dávid <christian-david@web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef IBANBICSTORAGEPLUGIN_H
#define IBANBICSTORAGEPLUGIN_H

#include "sql/kmymoneystorageplugin.h"

class ibanBicStoragePlugin : public KMyMoneyPlugin::storagePlugin
{
  Q_OBJECT
  Q_INTERFACES(KMyMoneyPlugin::storagePlugin)
//  Q_PLUGIN_METADATA(IID "org.kmymoney.payeeIdentifier.ibanbic.sqlStoragePlugin")

public:
  explicit ibanBicStoragePlugin(QObject* parent = 0, const QVariantList& options = QVariantList());
  bool removePluginData(QSqlDatabase connection) final override;
  bool setupDatabase(QSqlDatabase connection) final override;
  static QString iid();
};

#endif // IBANBICSTORAGEPLUGIN_H