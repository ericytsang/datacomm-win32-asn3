#ifndef SERVERCONTROLTHREAD_H_
#define SERVERCONTROLTHREAD_H_

#include "../Buffer/MessageQueue.h"
#include "Playlist.h"
#include "../protocol.h"

class UDPSocket;
class FileTransferer;

#define IP_ADDR_LEN 16

class ServerControlThread
{
public:
    static ServerControlThread * getInstance();
    void addConnection(TCPSocket* connection);
    void start();
    void stop();
    void setPlaylist( Playlist * );
    Playlist * getPlaylist();
    void sendPlaylistToAll( void );
    void setUDPSocket( UDPSocket * );
protected:
    ServerControlThread();
    ~ServerControlThread();
private:
    static DWORD WINAPI _threadRoutine( void * params );
    static DWORD WINAPI _multicastRoutine( void * params );
	static DWORD WINAPI _sendFileToOne( void * params );
    void _handleMsgChangeStream( RequestPacket * );
    void _handleMsgRequestDownload( RequestPacket *, TCPSocket* socket);
    void _handleMsgCancelDownload( RequestPacket *, TCPSocket* socket );
    void _handleMsgDisconnect( int clientIndex );
    static VOID CALLBACK _sendPlaylistToAllRoutine( ULONG_PTR );
    static VOID CALLBACK _sendPlaylistToOne( ULONG_PTR tcpSock );
    //static void _handleSockMsgqMsg(ServerControlThread* dis);
    /**
     * pointer to the UDPSocket owned by the control thread.
     */
    UDPSocket * udpSocket;
    /**
     * Message queue used to communicate to {_thread}, and get it to do tasks.
     */
    MessageQueue _msgq;
    /**
     * Message queue used to receive data from {_TCPSocket}.
     */
    std::vector< TCPSocket * > _socks;
    std::vector< HANDLE > _sockHandles;
	FileTransferer* fileTransferer;
    /**
     * handle to the thread used to run {ServerControlThread::_threadRoutine}.
     */
    HANDLE _thread;
    /**
     * handle to the thread used to run {ServerControlThread::_multicastRoutine}.
     */
    HANDLE _multicastThread;
    /**
     * handle to an event object, used to stop the execution of thread.
     */
    HANDLE _threadStopEv;

    HANDLE access;

    Playlist * playlist;
	SongName* currentsong;
};

#endif
