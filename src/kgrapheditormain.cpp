/* This file is part of KGraphViewer.
   Copyright (C) 2005-2010 Gael de Chalendar <kleag@free.fr>

   KGraphViewer is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation, version 2.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA
*/


#include "kgrapheditor.h"
#include <QApplication>
#include <kaboutdata.h>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <QDir>
#include <QByteArray>
#include <QDBusInterface>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>

#include <klocalizedstring.h>
#include <KAboutData>
#include "kgrapheditoradaptor.h"

static const char description[] =
I18N_NOOP("A Graphviz dot graph viewer for KDE");

static const char version[] = "0.1";

int main(int argc, char **argv)
{
  KAboutData about(QStringLiteral("kgrapheditor"),
                   i18n("KGraphEditor"),
                   version,
                   i18n(description),
                   KAboutLicense::GPL,
                   i18n("(C) 2005-2010 Gaël de Chalendar"),
                   QString(),
                   0,
                   "kleag@free.fr");
 
  QApplication app(argc, argv);
  app.setApplicationName(QStringLiteral("kgrapheditor"));
  app.setApplicationDisplayName(QStringLiteral("KGraphEditor"));
  app.setApplicationVersion(version);
  app.setOrganizationDomain(QStringLiteral("kde.org"));
  app.setOrganizationName(QStringLiteral("KDE"));
  
  QCommandLineParser options;
  options.setApplicationDescription(i18n(description));
  options.addHelpOption();
  options.addVersionOption();
  options.addPositionalArgument(QStringLiteral("url"), i18n("Path or URL to scan"), i18n("[url]"));
  about.setupCommandLine(&options);
  options.process(app);
  about.processCommandLine(&options);
  
// see if we are starting with session management
  if (app.isSessionRestored())
  {
      RESTORE(KGraphEditor);
  }
  else
  {
      // no session.. just start up normally
      QStringList args = options.positionalArguments();

      KGraphEditor *widget = 0;
      if ( args.count() == 0 )
      {
        widget = new KGraphEditor;
        new KgrapheditorAdaptor(widget);
        QDBusConnection::sessionBus().registerObject("/KGraphEditor", widget);
        widget->show();
      }
      else
      {
        QDBusReply<bool> reply = QDBusConnection::sessionBus().interface()->isServiceRegistered( "org.kde.kgrapheditor" );
      
        bool instanceExists = reply.value();
  
        for (int i = 0; i < args.count(); i++ )
        {
          if (instanceExists 
              && (QMessageBox::question(0, 
                                         i18n("A KGraphEditor window is already open, do you want to open the file in it?"),
                                         i18n("Opening in new window confirmation"),
                                             "openInNewWindowMode"   ) == QMessageBox::Yes) )
          {
            QByteArray tosenddata;
            QDataStream arg(&tosenddata, QIODevice::WriteOnly);
            QString strarg = args[i];
            QUrl url;
            if (strarg.left(1) == "/")
              url = QUrl::fromUserInput(strarg);
            else url = QUrl::fromLocalFile(QDir::currentPath() + '/' + strarg);
            arg << url;
            QDBusInterface iface("org.kde.kgrapheditor", "/KGraphEditor", "", QDBusConnection::sessionBus());
            if (iface.isValid()) 
            {
              QDBusReply<void> reply = iface.call("openUrl", url.url(QUrl::PreferLocalFile));
              if (reply.isValid()) 
              {
                qDebug() << "Reply was valid" << endl;
                return 0;
              }

              qWarning() << "Call failed: " << reply.error().message() << endl;
              return 1;
            }
            qWarning() << "Invalid interface" << endl;
            exit(0);
          }
          else
          {
            widget = new KGraphEditor;
            new KgrapheditorAdaptor(widget);
            QDBusConnection::sessionBus().registerObject("/KGraphEditor", widget);
            widget->show();
            widget->openUrl( QUrl::fromUserInput(args[0]) );
          }
        }
      }
    if (widget != 0)
    {
      widget->  reloadPreviousFiles();
    }
    
  }
  return app.exec();
}
