/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WINDOW_SINGLETHREADED_H
#define WINDOW_SINGLETHREADED_H

#include <QWindow>
#include <QMatrix4x4>
#include <QTimer>
#include <QMutex>
#include <QImage>
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLContext)
QT_FORWARD_DECLARE_CLASS(QOpenGLFramebufferObject)
QT_FORWARD_DECLARE_CLASS(QOffscreenSurface)
QT_FORWARD_DECLARE_CLASS(QQuickRenderControl)
QT_FORWARD_DECLARE_CLASS(QQuickWindow)
QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QQmlError)
QT_FORWARD_DECLARE_CLASS(QQmlComponent)
QT_FORWARD_DECLARE_CLASS(QQuickItem)

class WindowSingleThreaded : public QWindow
{
    Q_OBJECT

    QStringList m_loadMessages;

public:
    WindowSingleThreaded();
    ~WindowSingleThreaded();

    QQmlEngine *engine() { return m_qmlEngine; }

public slots:
    void startQuick(const QUrl &url);
    void resize(QSize newSize);
    void render();
    void unload();
    QImage getImage();
    bool initialised() { return m_quickInitialized; }
    quint32 width() { return QWindow::width(); }
    quint32 height() { return QWindow::height(); }

    QStringList loadMessages() { return m_loadMessages; }
    void addMessages(const QStringList &msgs) { m_loadMessages << msgs; }
    qreal calculateDelta(quint64 duration, qreal min, qreal max);

signals:
    void updated(GLuint texid);
    void resized();
    void messages(const QStringList &messages);

private slots:
    void run();
    void addQmlPath();

    void createFbo();
    void destroyFbo();
    void requestUpdate();
    void handleScreenChange();
    void handleWarnings(const QList<QQmlError> &warnings);

private:
    void updateSizes();
    void resizeFbo();

    QMutex m_mutex;
    QImage m_image;
    bool m_frameReady;
    QOpenGLContext *m_context;
    QOffscreenSurface *m_offscreenSurface;
    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlEngine *m_qmlEngine;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
    QOpenGLFramebufferObject *m_fbo;
    bool m_quickInitialized;
    bool m_quickReady;
    QTimer m_updateTimer;
    qreal m_dpr;
};

#endif
