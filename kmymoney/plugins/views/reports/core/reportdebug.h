/*
 * Copyright 2005       Ace Jones <acejones@users.sourceforge.net>
 * Copyright 2007       Thomas Baumgart <tbaumgart@kde.org>
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

#ifndef REPORTDEBUG_H
#define REPORTDEBUG_H

// ----------------------------------------------------------------------------
// QT Includes

// ----------------------------------------------------------------------------
// KDE Includes

// ----------------------------------------------------------------------------
// Project Includes

namespace reports
{

// define to enable massive debug logging to stderr
#undef DEBUG_REPORTS
// #define DEBUG_REPORTS

#define DEBUG_ENABLED_BY_DEFAULT false

#ifdef DEBUG_REPORTS

// define to filter out account names & transaction amounts
// DO NOT check into CVS with this defined!! It breaks all
// unit tests.
#undef DEBUG_HIDE_SENSITIVE

#define DEBUG_ENTER(x) Debug ___DEBUG(x)
#define DEBUG_OUTPUT(x) ___DEBUG.output(x)
#define DEBUG_OUTPUT_IF(x,y) { if (x) ___DEBUG.output(y); }
#define DEBUG_ENABLE(x) Debug::enable(x)
#define DEBUG_ENABLE_KEY(x) Debug::setEnableKey(x)
#ifdef DEBUG_HIDE_SENSITIVE
#define DEBUG_SENSITIVE(x) QString("hidden")
#else
#define DEBUG_SENSITIVE(x) (x)
#endif

#else

#define DEBUG_ENTER(x)
#define DEBUG_OUTPUT(x)
#define DEBUG_OUTPUT_IF(x,y)
#define DEBUG_ENABLE(x)
#define DEBUG_SENSITIVE(x)
#endif

class Debug
{
  QString m_methodName;
  static QString m_sTabs;
  static bool m_sEnabled;
  bool m_enabled;
  static QString m_sEnableKey;
public:
  explicit Debug(const QString& _name);
  ~Debug();
  void output(const QString& _text);
  static void enable(bool _e) {
    m_sEnabled = _e;
  }
  static void setEnableKey(const QString& _s) {
    m_sEnableKey = _s;
  }
};

} // end namespace reports

#endif // REPORTDEBUG_H
