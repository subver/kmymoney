/*
 * Copyright 2019       Thomas Baumgart <tbaumgart@kde.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "amountvalidator.h"
#include <cmath>

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

AmountValidator::AmountValidator(QObject * parent) :
  AmountValidator(-HUGE_VAL, HUGE_VAL, 1000, parent)
{
}

AmountValidator::AmountValidator(double bottom, double top, int decimals,
    QObject * parent) :
    QDoubleValidator(bottom, top, decimals, parent)
{
  setNotation(StandardNotation);
}
