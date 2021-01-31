/****************************************************************************
**
** Copyright (C) 2017 Alexander Rössler
** License: LGPL version 2.1
**
** This file is part of QtQuickVcp.
**
** All rights reserved. This program and the accompanying materials
** are made available under the terms of the GNU Lesser General Public License
** (LGPL) version 2.1 which accompanies this distribution, and is available at
** http://www.gnu.org/licenses/lgpl-2.1.html
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
** Lesser General Public License for more details.
**
** Contributors:
** Alexander Rössler <alexander AT roessler DOT systems>
**
****************************************************************************/

#include "filewatcher.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDirIterator>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(filewatcherCategory, "filewatcher");

namespace qtquickvcp {

FileWatcher::FileWatcher(QObject *parent)
    : QObject(parent)
    , m_fileUrl(QLatin1String(""))
    , m_enabled(true)
    , m_recursive(false)
{
    m_timer.setInterval(100);
    m_timer.setSingleShot(true);
    m_timer.callOnTimeout(this, &FileWatcher::fileChanged);

    connect(&m_fileSystemWatcher, &QFileSystemWatcher::fileChanged, this, &FileWatcher::onWatchedFileChanged);
    //connect(&m_fileSystemWatcher, &QFileSystemWatcher::directoryChanged, this, &FileWatcher::onWatchedDirectoryChanged);
}

QUrl FileWatcher::fileUrl() const
{
    return m_fileUrl;
}

bool FileWatcher::enabled() const
{
    return m_enabled;
}

bool FileWatcher::recursive() const
{
    return m_recursive;
}

QStringList FileWatcher::nameFilters() const
{
    return m_nameFilters;
}

void FileWatcher::setFileUrl(const QUrl &fileUrl)
{
    if (m_fileUrl == fileUrl)
        return;

    m_fileUrl = fileUrl;
    emit fileUrlChanged(m_fileUrl);
    updateWatchedFile();
}

void FileWatcher::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
    updateWatchedFile();
}

void FileWatcher::setRecursive(bool recursive)
{
    if (m_recursive == recursive)
        return;

    m_recursive = recursive;
    emit recursiveChanged(m_recursive);
    updateWatchedFile();
}

void FileWatcher::setNameFilters(const QStringList &nameFilters)
{
    if (m_nameFilters == nameFilters)
        return;

    m_nameFilters = nameFilters;
    emit nameFiltersChanged(m_nameFilters);

    updateRegExps();
    updateWatchedFile();
}

void FileWatcher::updateRegExps()
{
    m_regExps.clear();
    for (QString &filter: m_nameFilters) {
        m_regExps.append(QRegExp(filter, Qt::CaseInsensitive, QRegExp::WildcardUnix));
    }
}

bool FileWatcher::updateWatchedFile()
{
    const auto &files = m_fileSystemWatcher.files();
    if (files.length() > 0) {
        m_fileSystemWatcher.removePaths(files);
    }
    const auto &directories = m_fileSystemWatcher.directories();
    if (directories.length() > 0) {
        m_fileSystemWatcher.removePaths(directories);
    }

    if (!m_fileUrl.isValid() || !m_enabled)
        return false;

    if (!m_fileUrl.isLocalFile()) {
        qCWarning(filewatcherCategory) << "Can only watch local files";
        return false;
    }

    const auto &localFile = m_fileUrl.toLocalFile();
    if (localFile.isEmpty())
        return false;

    auto matchFileFilter = [&](QString const& name) {
        for (QRegExp const& regExp : qAsConst(m_regExps)) {
            if (regExp.exactMatch(name))
                return true;
        }
        return false;
    };

    if (QDir(localFile).exists()) {
        //QSet<QString> newPaths;

        if (matchFileFilter(localFile)) {
            m_fileSystemWatcher.addPath(localFile);
            //newPaths.insert(localFile);
            qDebug("Matched: %s", qPrintable(localFile));
        }

        if (m_recursive) {
            QDirIterator it(localFile, QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
            while (it.hasNext()) {
                const QString file = it.next();
                const QString extension = it.fileInfo().completeSuffix();

                if ((it.fileName() != QLatin1String(".."))
                       && (it.fileName() != QLatin1String("."))
                       && matchFileFilter(it.fileName()))
                {
                    m_fileSystemWatcher.addPath(file);
                    //newPaths.insert(it.filePath());
                    qDebug("Matched: %s", qPrintable(localFile));
                }
            }
        }

        return true;
        //return newPaths != QSet<QString>(files.begin(), files.end()).unite(QSet<QString>(directories.begin(), directories.end()));
    }
    else {
#ifdef QT_DEBUG
        qCWarning(filewatcherCategory) << "File to watch does not exist" << localFile;
#endif
    }
    return false;
}

void FileWatcher::onWatchedFileChanged(QString const& file)
{
    if (m_enabled) {
        qCDebug(filewatcherCategory) << "File changed:" << file;
        //emit fileChanged();
        if (!m_timer.isActive())
            m_timer.start();
    }
}

void FileWatcher::onWatchedDirectoryChanged(const QString &)
{
    /*if (updateWatchedFile()) {
        onWatchedFileChanged(); // propagate event
    }//*/
}

} // namespace qtquickvcp
