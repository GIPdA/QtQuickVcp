/****************************************************************************
**
** This file was generated by a code generator based on imatix/gsl
** Any changes in this file will be lost.
**
****************************************************************************/
#ifndef COMMON_PUBLISH_H
#define COMMON_PUBLISH_H
#include <QObject>
#include <QSet>
#include <QDebug>
#include <QTimer>
#include <nzmqt/nzmqt.hpp>
#include <machinetalk/protobuf/message.pb.h>

namespace machinetalk { namespace common {

class Publish
    : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)
    Q_PROPERTY(QString socketUri READ socketUri WRITE setSocketUri NOTIFY socketUriChanged)
    Q_PROPERTY(QString debugName READ debugName WRITE setDebugName NOTIFY debugNameChanged)
    Q_PROPERTY(State connectionState READ state NOTIFY stateChanged)
    Q_PROPERTY(QString errorString READ errorString NOTIFY errorStringChanged)
    Q_PROPERTY(int heartbeatInterval READ heartbeatInterval WRITE setHeartbeatInterval NOTIFY heartbeatIntervalChanged)

public:
    explicit Publish(QObject *parent = nullptr);
    ~Publish();

    enum class State {
        Down = 0,
        Up = 1,
    };
    Q_ENUM(State)

    QString socketUri() const
    {
        return m_socketUri;
    }

    QString debugName() const
    {
        return m_debugName;
    }

    State state() const
    {
        return m_state;
    }

    QString errorString() const
    {
        return m_errorString;
    }

    int heartbeatInterval() const
    {
        return m_heartbeatInterval;
    }

    bool ready() const
    {
        return m_ready;
    }

public slots:

    void setSocketUri(const QString &uri)
    {
        if (m_socketUri == uri) {
            return;
        }

        m_socketUri = uri;
        emit socketUriChanged(uri);
    }

    void setDebugName(const QString &debugName)
    {
        if (m_debugName == debugName) {
            return;
        }

        m_debugName = debugName;
        emit debugNameChanged(debugName);
    }

    void setHeartbeatInterval(int interval)
    {
        if (m_heartbeatInterval == interval) {
            return;
        }

        m_heartbeatInterval = interval;
        emit heartbeatIntervalChanged(interval);
    }

    void setReady(bool ready)
    {
        if (m_ready == ready) {
            return;
        }

        m_ready = ready;
        emit readyChanged(ready);

        if (m_ready)
        {
            start();
        }
        else
        {
            stop();
        }
    }


    void sendSocketMessage(const QByteArray &topic, ContainerType type, Container &tx);
    void sendFullUpdate(const QByteArray &topic, Container &tx);
    void sendIncrementalUpdate(const QByteArray &topic, Container &tx);
    void addSocketTopic(const QByteArray &name);
    void removeSocketTopic(const QByteArray &name);
    void clearSocketTopics();

protected:
    void start(); // start trigger
    void stop(); // stop trigger

private:
    bool m_ready;
    QString m_debugName;

    QSet<QByteArray> m_socketTopics;   // the topics we are interested in
    QString m_socketUri;
    nzmqt::PollingZMQContext *m_context;
    nzmqt::ZMQSocket *m_socket;

    State         m_state;
    State         m_previousState;
    QString       m_errorString;

    QTimer      m_heartbeatTimer;
    int         m_heartbeatInterval;
    // more efficient to reuse a protobuf Messages
    Container m_socketRx;
    Container m_socketTx;

private slots:

    void heartbeatTimerTick();
    void resetHeartbeatTimer();
    void startHeartbeatTimer();
    void stopHeartbeatTimer();

    bool startSocket();
    void stopSocket();

    void processSocketMessage(const QList<QByteArray> &messageList);
    void socketError(int errorNum, const QString &errorMsg);

    void sendPing();

    void fsmDown();
    void fsmDownStartEvent();
    void fsmUp();
    void fsmUpStopEvent();
    void fsmUpHeartbeatTickEvent();


signals:
    void socketUriChanged(const QString &uri);
    void socketMessageReceived(const Container &rx);
    void debugNameChanged(const QString &debugName);
    void stateChanged(Publish::State state);
    void errorStringChanged(const QString &errorString);
    void heartbeatIntervalChanged(int interval);
    void readyChanged(bool ready);
    // fsm
    void fsmDownEntered(QPrivateSignal);
    void fsmDownExited(QPrivateSignal);
    void fsmDownStart(QPrivateSignal);
    void fsmUpEntered(QPrivateSignal);
    void fsmUpExited(QPrivateSignal);
    void fsmUpStop(QPrivateSignal);
    void fsmUpHeartbeatTick(QPrivateSignal);
};

} } // namespace machinetalk::common
#endif // COMMON_PUBLISH_H
