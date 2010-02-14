#include "WulforSettings.h"

#include "dcpp/stdinc.h"
#include "dcpp/DCPlusPlus.h"
#include "dcpp/File.h"
#include "dcpp/Util.h"
#include "dcpp/SimpleXML.h"

#include <QApplication>
#include <QFile>
#include <QTextCodec>
#include <QLocale>
#include <QFile>
#include <QDir>
#include <QTranslator>
#include <QLibraryInfo>
#include <QPalette>
#include <QColor>
#include <QBrush>

#ifndef CLIENT_TRANSLATIONS_DIR
#define CLIENT_TRANSLATIONS_DIR ""
#endif

using namespace dcpp;

WulforSettings::WulforSettings():
        configFile(QString::fromStdString(Util::getPath(Util::PATH_USER_CONFIG)) + "EiskaltDC++.xml"),
        tor(0)
{
    //Load Qt tranlator
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    qApp->installTranslator(&qtTranslator);

    //Fill the maps with default values
    {
        strmap.insert(WS_CHAT_LOCAL_COLOR, "#078010");
        strmap.insert(WS_CHAT_OP_COLOR, "#000000");
        strmap.insert(WS_CHAT_BOT_COLOR, "#838383");
        strmap.insert(WS_CHAT_PRIV_LOCAL_COLOR, "#078010");
        strmap.insert(WS_CHAT_PRIV_USER_COLOR, "#ac0000");
        strmap.insert(WS_CHAT_SAY_NICK, "#2344e7");
        strmap.insert(WS_CHAT_CORE_COLOR, "#ff0000");
        strmap.insert(WS_CHAT_STAT_COLOR, "#ac0000");
        strmap.insert(WS_CHAT_USER_COLOR, "#ac0000");
        strmap.insert(WS_CHAT_TIME_COLOR, qApp->palette().text().color().name());
        strmap.insert(WS_CHAT_MSG_COLOR, qApp->palette().text().color().name());
        strmap.insert(WS_CHAT_USERLIST_COL_WIDTH, "");
        strmap.insert(WS_QCONNECT_HISTORY, "");
        strmap.insert(WS_DEFAULT_LOCALE, "UTF-8");
        strmap.insert(WS_DQUEUE_COLUMN_WIDTHS, "");
        strmap.insert(WS_SEARCH_COLUMN_WIDTHS, "");
        strmap.insert(WS_TRANSLATION_FILE, "");
        strmap.insert(WS_TRANSFERS_COLUMN_WIDTHS, "");
        strmap.insert(WS_SHARE_LPANE_COL_WIDTHS, "");
        strmap.insert(WS_SHARE_RPANE_COL_WIDTHS, "");

        intmap.insert(WB_CHAT_SHOW_TIMESTAMP, (int)true);
        intmap.insert(WB_CHAT_SHOW_JOINS, (int)true);
        intmap.insert(WB_MAINWINDOW_MAXIMIZED, (int)true);
        intmap.insert(WB_SEARCHFILTER_NOFREE, (int)false);
        intmap.insert(WB_ANTISPAM_ENABLED, (int)false);
        intmap.insert(WB_ANTISPAM_AS_FILTER, (int)false);
        intmap.insert(WB_IPFILTER_ENABLED, (int)false);
        intmap.insert(WB_TRAY_ENABLED, (int)true);
        intmap.insert(WB_SHOW_HIDDEN_USERS, (int)false);
        intmap.insert(WB_SHOW_JOINS, (int)false);
        intmap.insert(WI_CHAT_MAXPARAGRAPHS, 300);
        intmap.insert(WI_CHAT_USERLIST_COL_BITMAP, 0x7F);
        intmap.insert(WI_CHAT_WIDTH, -1);
        intmap.insert(WI_CHAT_USERLIST_WIDTH, -1);
        intmap.insert(WI_CHAT_SORT_COLUMN, 0);
        intmap.insert(WI_CHAT_SORT_ORDER, 0);
        intmap.insert(WI_MAINWINDOW_HEIGHT, -1);
        intmap.insert(WI_MAINWINDOW_WIDTH, -1);
        intmap.insert(WI_MAINWINDOW_X, -1);
        intmap.insert(WI_MAINWINDOW_Y, -1);
        intmap.insert(WI_MAINWINDOW_TBAR_X, -1);
        intmap.insert(WI_MAINWINDOW_TBAR_Y, -1);
        intmap.insert(WI_MAINWINDOW_FBAR_X, -1);
        intmap.insert(WI_MAINWINDOW_FBAR_Y, -1);
        intmap.insert(WI_DQUEUE_COL_BITMAP, 0x433);//0b10000110011
        intmap.insert(WI_SEARCH_COL_BITMAP, 0x1FFF);
        intmap.insert(WI_SEARCH_SORT_COLUMN, 1);
        intmap.insert(WI_SEARCH_SORT_ORDER, 0);
        intmap.insert(WI_SEARCH_SHARED_ACTION, 0);
        intmap.insert(WI_TRANSFER_COL_BITMAP, 0xFF);
        intmap.insert(WI_TRANSFER_HEIGHT, -1);
        intmap.insert(WI_SHARE_RPANE_COL_BITMAP, 0x0F);
        intmap.insert(WI_SHARE_RPANE_WIDTH, -1);
        intmap.insert(WI_SHARE_WIDTH, -1);
    }
}

WulforSettings::~WulforSettings(){
}

void WulforSettings::load(){
    if (!QFile::exists(configFile))
        return;

    try{
        SimpleXML xml;
        xml.fromXML(File(configFile.toStdString(), File::READ, File::OPEN).read());
        xml.resetCurrentChild();
        xml.stepIn();

        if (xml.findChild("Settings")){

            xml.stepIn();

            WStrMap::iterator it = strmap.begin();

            for (; it != strmap.end(); ++it){
                if (xml.findChild(it.key().toStdString()))
                    setStr(it.key(), QString::fromStdString(xml.getChildData()));

                xml.resetCurrentChild();
            }

            WIntMap::iterator iit = intmap.begin();

            for (; iit != intmap.end(); ++iit){
                if (xml.findChild(iit.key().toStdString()))
                    setInt(iit.key(), Util::toInt(xml.getChildData()));

                xml.resetCurrentChild();
            }
        }
    }
    catch (Exception ex){
    }
}

void WulforSettings::save(){
    SimpleXML xml;
    xml.addTag("EiskaltDC++");
    xml.stepIn();
    xml.addTag("Settings");
    xml.stepIn();

    WStrMap::iterator it = strmap.begin();

    for (; it != strmap.end(); ++it){
        xml.addTag(it.key().toStdString(), it.value().toStdString());
        xml.addChildAttrib(string("type"), string("string"));
    }

    WIntMap::iterator iit = intmap.begin();

    for (; iit != intmap.end(); ++iit){
        xml.addTag(iit.key().toStdString(), QString().setNum(iit.value()).toStdString());
        xml.addChildAttrib(string("type"), string("int"));
    }

    xml.stepOut();

    try{
        File out(QString(configFile + ".tmp").toStdString(), File::WRITE, File::CREATE | File::TRUNCATE);
        BufferedOutputStream<false> f(&out);

        f.write(SimpleXML::utf8Header);
        xml.toXML(&f);
        f.flush();

        out.close();

        File::deleteFile(configFile.toStdString());
        File::renameFile(configFile.toStdString() + ".tmp", configFile.toStdString());
    }
    catch (const FileException &){}
}

void WulforSettings::loadTranslation(){
    QString file = getStr(WS_TRANSLATION_FILE);

    if (file.isEmpty() || !QFile::exists(file)){
        QString lc_prefix = QLocale::system().name();

        file = QString(CLIENT_TRANSLATIONS_DIR) + QDir::separator();
        lc_prefix = lc_prefix.left(lc_prefix.indexOf("_"));
        file += "eiskaltdcpp." + lc_prefix + ".qm";

        if (!QFile::exists(file))
            return;
    }

    tor.load(file);

    qApp->installTranslator(&tor);
}

QString WulforSettings::getStr(QString key) throw (WulforSettings::BadKey){
    if (!strmap.contains(key))
        throw BadKey();

    return strmap.value(key);
}

int WulforSettings::getInt(QString key) throw (WulforSettings::BadKey){
    if (!intmap.contains(key))
       throw BadKey();

    return intmap.value(key);
}

bool WulforSettings::getBool(QString key) throw (WulforSettings::BadKey){
    return ((bool)getInt(key));
}

void WulforSettings::setStr(QString key, QString value) throw (WulforSettings::BadKey){
    if (!strmap.contains(key))
        throw BadKey();

    strmap[key] = value;
}

void WulforSettings::setInt(QString key, int value) throw (WulforSettings::BadKey){
    if (!intmap.contains(key))
       throw BadKey();

    intmap[key] = value;
}

void WulforSettings::setBool(QString key, bool value) throw (WulforSettings::BadKey){
    setInt(key, (int)value);
}
