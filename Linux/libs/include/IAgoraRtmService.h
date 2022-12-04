//
//  The Agora RTM SDK
//
//
//  Copyright (c) 2020 Agora.io. All rights reserved.
//
#pragma once

#include <cstdint>
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define AGORA_CALL __cdecl
#if defined(AGORARTC_EXPORT)
#define AGORA_API extern "C" __declspec(dllexport)
#else
#define AGORA_API extern "C" __declspec(dllimport)
#endif
#define _AGORA_CPP_API

#elif defined(__APPLE__)
#define AGORA_API __attribute__((visibility("default"))) extern "C"
#define AGORA_CALL
#define _AGORA_CPP_API

#elif defined(__ANDROID__) || defined(__linux__)
#if defined(__ANDROID__) && defined(FEATURE_RTM_STANDALONE_SDK)
#define AGORA_API extern "C" __attribute__((visibility("default")))
#define _AGORA_CPP_API
#else
#define AGORA_API extern "C" __attribute__((visibility("default")))
#define _AGORA_CPP_API __attribute__((visibility("default")))
#endif
#define AGORA_CALL

#else
#define AGORA_API extern "C"
#define AGORA_CALL
#define _AGORA_CPP_API
#endif


/**
 The prefix for ending a call. You can use it with the \ref agora::rtm::IRtmService::sendMessageToPeer(const char *peerId, const IMessage *message, const SendMessageOptions &options) "sendMessageToPeer" method to be compatible with the endCall method of the legacy Agora Signaling SDK.
 */
#define AGORA_RTM_ENDCALL_PREFIX "AgoraRTMLegacyEndcallCompatibleMessagePrefix"

namespace agora {
  namespace rtm {

      /**
       @brief Error codes related to Initialization.
       */
      enum INIT_ERR_CODE {

          /**
           0: Initialization succeeds.
           */
          INIT_ERR_OK = 0,

          /**
           1: A common failure occurs during initialization.
           */
          INIT_ERR_FAILURE = 1,

          /**
           2: The SDK is already initialized.
           */
          INIT_ERR_ALREADY_INITIALIZED = 2,

          /**
           3: The App ID is invalid.
           */
          INIT_ERR_INVALID_APP_ID = 3,

          /**
           4: The event handler is empty.
           */
          INIT_ERR_INVALID_ARGUMENT = 4,
      };


    /**
     @brief Error codes related to login.
     */
    enum LOGIN_ERR_CODE {

      /**
       0: The method call succeeds, or login succeeds.
       */
      LOGIN_ERR_OK = 0,

      /**
       1: Login fails. The reason is unknown.
       */
      LOGIN_ERR_UNKNOWN = 1,

      /**
       2: Login is rejected by the server.
       */
      LOGIN_ERR_REJECTED = 2, // Occurs when not initialized or server reject

      /**
       3: Invalid login argument.
       */
      LOGIN_ERR_INVALID_ARGUMENT = 3,

      /**
       4: The App ID is invalid.
       */
      LOGIN_ERR_INVALID_APP_ID = 4,

      /**
       5: The token is invalid.
       */
      LOGIN_ERR_INVALID_TOKEN = 5,

      /**
       6: The token has expired, and hence login is rejected.
       */
      LOGIN_ERR_TOKEN_EXPIRED = 6,

      /**
       7: Unauthorized login.
       */
      LOGIN_ERR_NOT_AUTHORIZED = 7,

      /**
       8: The user has already logged in or is logging in the Agora RTM system, or the user has not called the \ref agora::rtm::IRtmService::logout "logout" method to leave the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" state.
       */
      LOGIN_ERR_ALREADY_LOGGED_IN = 8,

      /**
       9: The login times out. The current timeout is set as 12 seconds. You need to log in again.
       */
      LOGIN_ERR_TIMEOUT = 9,

      /**
       10: The call frequency of the \ref agora::rtm::IRtmService::login "login" method exceeds the limit of two queries per second.
       */
      LOGIN_ERR_TOO_OFTEN = 10,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
      */
      LOGIN_ERR_NOT_INITIALIZED = 101,
    };

    /**
     @brief Error codes related to logout.
     */
    enum LOGOUT_ERR_CODE {

      /**
       0: The method call succeeds, or logout succeeds.
       */
      LOGOUT_ERR_OK = 0,

      /**
       1: **RESERVED FOR FUTURE USE**
       */
      LOGOUT_ERR_REJECTED = 1,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      LOGOUT_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the user logs out of the Agora RTM system.
       */
      LOGOUT_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to renewing the RTM Token.
     */
    enum RENEW_TOKEN_ERR_CODE {

      /**
       0: The method call succeeds, or the renewing operation succeeds.
       */
      RENEW_TOKEN_ERR_OK = 0,

      /**
       1: Common failure. The user fails to renew the token.
       */
      RENEW_TOKEN_ERR_FAILURE = 1,

      /**
       2: The method call fails. The argument is invalid.
       */
      RENEW_TOKEN_ERR_INVALID_ARGUMENT = 2,

      /**
       3: **RESERVED FOR FUTURE USE**
       */
      RENEW_TOKEN_ERR_REJECTED = 3,

      /**
       4: The method call frequency exceeds the limit of two queries per second.
       */
      RENEW_TOKEN_ERR_TOO_OFTEN = 4,

      /**
       5: The token has expired.
       */
      RENEW_TOKEN_ERR_TOKEN_EXPIRED = 5,

      /**
       6: The token is invalid.
       */
      RENEW_TOKEN_ERR_INVALID_TOKEN = 6,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      RENEW_TOKEN_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before renewing the token.
       */
      RENEW_TOKEN_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Connection states between the SDK and the Agora RTM system.
     */
    enum CONNECTION_STATE {

      /**
       1: The initial state. The SDK is disconnected from the Agora RTM system.

       When the user calls the \ref agora::rtm::IRtmService::login "login" method, the SDK starts to log in the Agora RTM system, triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback, and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTING "CONNECTION_STATE_CONNECTING" state.
       */
      CONNECTION_STATE_DISCONNECTED = 1,

      /**
       2: The SDK is logging in the Agora RTM system.

       - If the user successfully logs in the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTED "CONNECTION_STATE_CONNECTED" state.
       - If the user fails to login the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginFailure "onLoginFailure" callback, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_DISCONNECTED "CONNECTION_STATE_DISCONNECTED" state.
       */
      CONNECTION_STATE_CONNECTING = 2,

      /**
       3: The SDK has logged in the Agora RTM system.

       - If the connection state between the SDK and the Agora RTM system is interrupted because of network issues, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       - If the login is banned by the server because, for example, another instance logs in the Agora RTM system with the same user ID, the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback is triggered, and the SDK is switched to the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" state.
       - If the user calls the \ref agora::rtm::IRtmService::logout "logout" method to log out of the Agora RTM system and receives the \ref agora::rtm::IRtmServiceEventHandler::onLogout "onLogout" callback (error code = `LOGOUT_ERR_OK`), the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_DISCONNECTED "CONNECTION_STATE_DISCONNECTED" state.
       */
      CONNECTION_STATE_CONNECTED = 3,

      /**
       4: The connection state between the SDK and the Agora RTM system is interrupted due to network issues, and the SDK keeps re-logging in the Agora RTM system.

       - If the SDK successfully re-logs in the Agora RTM system, the SDK triggers the \ref agora::rtm::IRtmServiceEventHandler::onConnectionStateChanged "onConnectionStateChanged" callback and switches to the \ref agora::rtm::CONNECTION_STATE_CONNECTED "CONNECTION_STATE_CONNECTED" state. The SDK automatically adds the user back to the channels he or she was in when the connection was interrupted, and synchronizes the local user's attributes with the server.
       - If the SDK cannot re-log in the Agora RTM system, it stays in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state and keeps re-logging in the system.
       */
      CONNECTION_STATE_RECONNECTING = 4,

      /**
       5: The SDK gives up logging in the Agora RTM system, mainly because another instance has logged in the Agora RTM system with the same user ID.

       You must call the \ref agora::rtm::IRtmService::logout "logout" method to leave this state before calling the \ref agora::rtm::IRtmService::login "login" method again.
       */
      CONNECTION_STATE_ABORTED = 5,
    };

    /**
     @brief Reasons for a connection state change.
     */
    enum CONNECTION_CHANGE_REASON {

      /**
       1: The SDK is logging in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN = 1,

      /**
       2: The SDK has logged in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN_SUCCESS = 2,

      /**
       3: The SDK fails to log in the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGIN_FAILURE = 3,

      /**
       4: The SDK fails to log in the Agora RTM system within 12 seconds and gives up.
       */
      CONNECTION_CHANGE_REASON_LOGIN_TIMEOUT = 4,

      /**
       5: The connection between the SDK and the Agora RTM system is interrupted. The system defines an interruption when the SDK loses connection with the Agora RTM system for network reasons and cannot recover in four seconds.
       */
      CONNECTION_CHANGE_REASON_INTERRUPTED = 5,

      /**
       6: The user has called the \ref agora::rtm::IRtmService::logout "logout" method to log out of the Agora RTM system.
       */
      CONNECTION_CHANGE_REASON_LOGOUT = 6,

      /**
       7: The SDK login to the Agora RTM system is banned by Agora.
       */
      CONNECTION_CHANGE_REASON_BANNED_BY_SERVER = 7,

      /**
       8: Another user is logging in the Agora RTM system with the same User ID.
       */
      CONNECTION_CHANGE_REASON_REMOTE_LOGIN = 8,
      
      /**
      9: The token has expired.
      */
      CONNECTION_CHANGE_REASON_TOKEN_EXPIRED = 9,
    };

    /**
     @brief Error codes related to sending a peer-to-peer message.
     */
    enum PEER_MESSAGE_ERR_CODE {

      /**
       0: The method call succeeds, or the specified user receives the peer-to-peer message.
       */
      PEER_MESSAGE_ERR_OK = 0,

      /**
       1: The sender fails to send the peer-to-peer message.
       */
      PEER_MESSAGE_ERR_FAILURE = 1,

      /**
       2: A timeout occurs when sending the peer-to-peer message. The current timeout is set as 10 seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      PEER_MESSAGE_ERR_SENT_TIMEOUT = 2,

      /**
       3: The specified user is offline and does not receive this peer-to-peer message.
       */
      PEER_MESSAGE_ERR_PEER_UNREACHABLE = 3,

     /**
      4: The receiver is offline and does not receive this offline peer-to-peer message, but the server has cached it and will re-send it once he/she is back online.
      */
      PEER_MESSAGE_ERR_CACHED_BY_SERVER = 4,

     /**
      5: The method call frequency exceeds the limit of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds, with channel and peer messages taken together..
      */
      PEER_MESSAGE_ERR_TOO_OFTEN = 5,

     /**
      6: The user ID is invalid.
      */
      PEER_MESSAGE_ERR_INVALID_USERID = 6,

     /**
      7: The message is null or exceeds 32 KB in length.
	  */
      PEER_MESSAGE_ERR_INVALID_MESSAGE = 7,

      /**
       8: The message receiver‘s SDK is of an earlier version and hence cannot recognize this message.
       */
      PEER_MESSAGE_ERR_IMCOMPATIBLE_MESSAGE = 8,

     /**
      101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
      */
      PEER_MESSAGE_ERR_NOT_INITIALIZED = 101,

     /**
      102: The sender does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before sending the peer-to-peer message.
      */
      PEER_MESSAGE_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to joining a channel.
     */
    enum JOIN_CHANNEL_ERR {

      /**
       0: The method call succeeds, or the user joins the channel successfully.
       */
      JOIN_CHANNEL_ERR_OK = 0,

      /**
       1: Common failure. The user fails to join the channel.
       */
      JOIN_CHANNEL_ERR_FAILURE = 1,

      /**
       2: For v1.0.0 and earlier, the SDK returns this error when you try to join a channel that you have already joined. The SDK does not return this error code after v1.0.0.
       */
      JOIN_CHANNEL_ERR_REJECTED = 2, // Usually occurs when the user is already in the channel

      /**
       3: The user fails to join the channel because the argument is invalid.
       */
      JOIN_CHANNEL_ERR_INVALID_ARGUMENT = 3,

      /**
       4: A timeout occurs when joining the channel. The current timeout is set as five seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      JOIN_CHANNEL_TIMEOUT = 4,

     /**
      5: The number of the RTM channels you are in exceeds the limit of 20.
      */
      JOIN_CHANNEL_ERR_EXCEED_LIMIT = 5,

     /**
      6: The user is joining or has joined the channel.
      */
      JOIN_CHANNEL_ERR_ALREADY_JOINED = 6,

      /**
      7: The method call frequency exceeds 50 queries every three seconds.
      */
      JOIN_CHANNEL_ERR_TOO_OFTEN = 7,

      /**
       8: The frequency of joining the same channel exceeds two times every five seconds.
       */
      JOIN_CHANNEL_ERR_JOIN_SAME_CHANNEL_TOO_OFTEN = 8,

     /**
      101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
      */
      JOIN_CHANNEL_ERR_NOT_INITIALIZED = 101,

     /**
      102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before joining the channel.
      */
      JOIN_CHANNEL_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to leaving a channel.
     */
    enum LEAVE_CHANNEL_ERR {

      /**
       0: The method call succeeds, or the user leaves the channel successfully.
       */
      LEAVE_CHANNEL_ERR_OK = 0,

      /**
       1: Common failure. The user fails to leave the channel.
       */
      LEAVE_CHANNEL_ERR_FAILURE = 1,

      /**
       2: **RESERVED FOR FUTURE USE**
       */
      LEAVE_CHANNEL_ERR_REJECTED = 2,

      /**
       3: The user is not in the channel.
       */
      LEAVE_CHANNEL_ERR_NOT_IN_CHANNEL = 3,

      /**
       4: The user is banned to join the channel.
       */
      LEAVE_CHANNEL_ERR_KICKED = 4,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      LEAVE_CHANNEL_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before calling the \ref agora::rtm::IChannel::leave "leave" method.
       */
      LEAVE_CHANNEL_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Reasons why a user leaves the channel.
     */
    enum LEAVE_CHANNEL_REASON {

      /**
       1: The user has quit the call.
       */
      LEAVE_CHANNEL_REASON_QUIT = 1,

      /**
       2: The user is banned by the server.
       */
      LEAVE_CHANNEL_REASON_KICKED = 2,
    };

    /**
     @brief Error codes related to sending a channel message.
     */
    enum CHANNEL_MESSAGE_ERR_CODE {

      /**
       0: The method call succeeds, or the server receives the channel message.
       */
      CHANNEL_MESSAGE_ERR_OK = 0,

      /**
       1: Common failure. The user fails to send the channel message.
       */
      CHANNEL_MESSAGE_ERR_FAILURE = 1,

      /**
       2: The SDK does not receive a response from the server in 10 seconds. The current timeout is set as 10 seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      CHANNEL_MESSAGE_ERR_SENT_TIMEOUT = 2,

      /**
       3: The method call frequency exceeds the limit of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds, with channel and peer messages taken together..
       */
      CHANNEL_MESSAGE_ERR_TOO_OFTEN = 3,

      /**
       4: The message is null or exceeds 32 KB in length.
       */
      CHANNEL_MESSAGE_ERR_INVALID_MESSAGE = 4,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      CHANNEL_MESSAGE_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before sending out a channel message.
       */
      CHANNEL_MESSAGE_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to retrieving a channel member list.
     */
    enum GET_MEMBERS_ERR {

      /**
       0: The method call succeeds, or the operation succeeds.
       */
      GET_MEMBERS_ERR_OK = 0,

      /**
       1: Common failure. The user fails to retrieve a member list of the channel.
       */
      GET_MEMBERS_ERR_FAILURE = 1,

      /**
       2: **RESERVED FOR FUTURE USE**
       */
      GET_MEMBERS_ERR_REJECTED = 2,

      /**
       3: A timeout occurs when retrieving a member list of the channel. The current timeout is set as five seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      GET_MEMBERS_ERR_TIMEOUT = 3,

      /**
       4: The method call frequency exceeds the limit of five queries every two seconds.
       */
      GET_MEMBERS_ERR_TOO_OFTEN = 4,

      /**
       5: The user is not in channel.
       */
      GET_MEMBERS_ERR_NOT_IN_CHANNEL = 5,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      GET_MEMBERS_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before retrieving a member list.
       */
      GET_MEMBERS_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to querying the online status of the specified peers.
     */
    enum QUERY_PEERS_ONLINE_STATUS_ERR {

      /**
       0: The method call succeeds, or the operation succeeds.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_OK = 0,

      /**
       1: Common failure. The user fails to query the online status of the specified peers.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_FAILURE = 1,

      /**
       2: The method call fails. The argument is invalid.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_INVALID_ARGUMENT = 2,

      /**
       3: **RESERVED FOR FUTURE USE**
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_REJECTED = 3,

      /**
       4: The SDK fails to receive a response from the server in 10 seconds. The current timeout is set as 10 seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_TIMEOUT = 4,

      /**
       5: The method call frequency exceeds the limit of (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_TOO_OFTEN = 5,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before querying the online status.
       */
      QUERY_PEERS_ONLINE_STATUS_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief The online states of a peer.
     */
    enum PEER_ONLINE_STATE {

      /**
       0: The peer is online (the SDK has logged in the Agora RTM system).
       */
      PEER_ONLINE_STATE_ONLINE = 0,

      /**
       1: The peer is temporarily unreachable (the server has not received a packet from the SDK for more than six seconds).
       */
      PEER_ONLINE_STATE_UNREACHABLE = 1,

      /**
       2: The peer is offline (the SDK has not logged in the Agora RTM system, or it has logged out of the system, or the server has not received a packet from the SDK for more than 30 seconds).
       */
      PEER_ONLINE_STATE_OFFLINE = 2,
    };

    /**
     @brief Subscription types.
     */
    enum PEER_SUBSCRIPTION_OPTION {
      /**
       0: Takes out a subscription to the online status of specified users.
       */
      PEER_SUBSCRIPTION_OPTION_ONLINE_STATUS = 0,
    };

    /**
     @brief Error codes related to subscribing to or unsubscribing from the status of specified peers.
     */
    enum PEER_SUBSCRIPTION_STATUS_ERR {

      /**
       0: The method call succeeds, or the operation succeeds.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_OK = 0,

      /**
       1: Common failure. The user fails to subscribe to or unsubscribe from the status of the specified peers.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_FAILURE = 1,

      /**
       2: The method call fails. The argument is invalid.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_INVALID_ARGUMENT = 2,

      /**
       3: **RESERVED FOR FUTURE USE**
       */
      PEER_SUBSCRIPTION_STATUS_ERR_REJECTED = 3,

      /**
       4: The SDK fails to receive a response from the server within 10 seconds. The current timeout is set as 10 seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_TIMEOUT = 4,

      /**
       5: The method call frequency exceeds the limit of 10 queries every five seconds.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_TOO_OFTEN = 5,

      /**
       6: The number of peers, to whom you subscribe, exceeds the limit of 512.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_OVERFLOW = 6,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before this operation.
       */
      PEER_SUBSCRIPTION_STATUS_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to getting a list of the peers by subscription option type.
     */
    enum QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR {

      /**
       0: The method call succeeds, or the operation succeeds.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_OK = 0,

      /**
       1: Common failure. The user fails to query peers by subscription option type.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_FAILURE = 1,

      /**
       2: The SDK fails to receive a response from the server within 5 seconds. The current timeout is set as 5 seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_TIMEOUT = 2,

      /**
       3: The method call frequency exceeds the limit of 10 queries every five seconds.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_TOO_OFTEN = 3,

      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the query.
       */
      QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to attrubute operations.
     */
    enum ATTRIBUTE_OPERATION_ERR {

        /**
         0: The method call succeeds, or the attribute operation succeeds.
         */
        ATTRIBUTE_OPERATION_ERR_OK = 0,

        /**
         1: @deprecated
         */
        ATTRIBUTE_OPERATION_ERR_NOT_READY = 1,

        /**
         2: Common failure. The attribute operation fails.
         */
        ATTRIBUTE_OPERATION_ERR_FAILURE = 2,

        /**
         3: The argument you put for this attribute operation is invalid. For example, you cannot set a user or channel attribute as "".
         */
        ATTRIBUTE_OPERATION_ERR_INVALID_ARGUMENT = 3,

        /**
         4: The attribute size exceeds the limit.

         - For user attribute operations: The user's overall attribute size would exceed the limit of 16 KB, one of the user's attributes would exceeds 8 KB in size, or the number of this user's attributes would exceed 32 after this attribute operation.
         - For channel attribute operations: The channel's overall attribute size would exceed the limit of 32 KB, one of the channel attributes would exceed 8 KB in size, or the number of this channel's attributes would exceed 32 after this attribute operation.
         */
        ATTRIBUTE_OPERATION_ERR_SIZE_OVERFLOW = 4,

        /**
         5: The method call frequency exceeds the limit.

         - For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.
         - For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 40 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.
         - For \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes", \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes", \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" and \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.
         - For \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" and \ref agora::rtm::IRtmService::getChannelAttributesByKeys "getChannelAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.
         */
        ATTRIBUTE_OPERATION_ERR_TOO_OFTEN = 5,

        /**
         6: The specified user is not found, either because the user is offline or because the user does not exist.
         */
        ATTRIBUTE_OPERATION_ERR_USER_NOT_FOUND = 6,

        /**
         7: A timeout occurs during the attribute operation. The current timeout is set as five seconds. Possible reasons: The user is in the \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" or \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state.
         */
        ATTRIBUTE_OPERATION_ERR_TIMEOUT = 7,

        /**
         101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
         */
        ATTRIBUTE_OPERATION_ERR_NOT_INITIALIZED = 101,

        /**
         102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the attribute operation.
         */
        ATTRIBUTE_OPERATION_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to metadata operation.
    */
    enum METADATA_OPERATION_ERR {
        /**
         0: The method call succeeds, or the metadata operation succeeds.
         */
        METADATA_OPERATION_ERR_OK = 0,

        /**
         1: Common failure. The metadata operation fails.
         */
        METADATA_OPERATION_ERR_FAILURE = 1,

        /**
         2: The user ID is invalid.
        */
        METADATA_OPERATION_ERR_INVALID_USER_ID = 2,

        /**
         3: The argument you put for this metadata operation is invalid.
         */
        METADATA_OPERATION_ERR_INVALID_ARGUMENT = 3,

        /**
         4: The size of metadata item's key or value exceeds the limit.
         */
        METADATA_OPERATION_ERR_SIZE_OVERFLOW = 4,

        /**
         5: The key of metadata item is duplicate.
         */
        METADATA_OPERATION_ERR_DUPLICATE_KEY = 5,

        /**
         6: The method call frequency exceeds the limit.
         */
        METADATA_OPERATION_ERR_TOO_OFTEN = 6,

        /**
         7: A timeout occurs during the metadata operation.
         */
        METADATA_OPERATION_ERR_TIMEOUT = 7,

        /**
         8: metadata revision is outdate, or update items that were not created。
         */
        METADATA_OPERATION_ERR_OUTDATE_REVISION = 8,

        /**
         9: operate channel metadata failed due to not in channel.
         */
        METADATA_OPERATION_ERR_NOT_IN_CHANNEL = 9,

        /**
         101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
         */
        METADATA_OPERATION_ERR_NOT_INITIALIZED = 101,

        /**
         102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the metadata operation.
         */
        METADATA_OPERATION_ERR_USER_NOT_LOGGED_IN = 102,
    };

    /**
     @brief Error codes related to subscribe/unsubscribe metadata.
    */
    enum METADATA_SUBSCRIPTION_ERR {
      /**
       0: The method call succeeds, or the metadata subscription succeeds.
       */
      METADATA_SUBSCRIPTION_ERR_OK = 0,

      /**
       1: Common failure. The metadata subscription fails.
       */
      METADATA_SUBSCRIPTION_ERR_FAILURE = 1,

      /**
       2: A timeout occurs during the metadata subscription.
       */
      METADATA_SUBSCRIPTION_ERR_TIMEOUT = 2,

      /**
       3: The argument you put for this metadata subscription is invalid.
       */
      METADATA_SUBSCRIPTION_ERR_INVALID_ARGUMENT = 3,

      /**
       4: The method call frequency exceeds the limit.
       */
      METADATA_SUBSCRIPTION_ERR_TOO_OFTEN = 4,

      /**
       5: The number of users, to whom you subscribe, exceeds the limit.
       */
      METADATA_SUBSCRIPTION_ERR_OVERFLOW = 5,

      /**
       5: Unsubscribe user that was not be subscribed.
       */
      METADATA_SUBSCRIPTION_USER_NOT_SUBSCRIBED = 6,
      /**
       101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
       */
      METADATA_SUBSCRIPTION_ERR_NOT_INITIALIZED = 101,

      /**
       102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before the metadata subscription.
       */
      METADATA_SUBSCRIPTION_ERR_NOT_LOGGED_IN = 102,
    };

     /**
      @brief Error codes related to retrieving the channel member count of specified channels.
      */
     enum GET_CHANNEL_MEMBER_COUNT_ERR_CODE {

        /**
         0: The method call succeeds, or the operation succeeds.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_OK = 0,

        /**
         1: Unknown common failure.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_FAILURE = 1,

        /**
         2: One or several of your channel IDs is invalid, or @p channelCount &lt; 0.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_INVALID_ARGUMENT = 2,

        /**
         3: The method call frequency exceeds the limit of one query per second.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_TOO_OFTEN = 3,

        /**
         4: A timeout occurs during this operation. The current timeout is set as five seconds.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_TIMEOUT = 4,

        /**
         5:@p channelCount is greater than 32.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_EXCEED_LIMIT = 5,

        /**
         101: \ref agora::rtm::IRtmService "IRtmService" is not initialized.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_NOT_INITIALIZED = 101,

        /**
         102: The user does not call the \ref agora::rtm::IRtmService::login "login" method, or the method call of \ref agora::rtm::IRtmService::login "login" does not succeed before this operation.
         */
        GET_CHANNEL_MEMBER_COUNT_ERR_USER_NOT_LOGGED_IN = 102,
     };

    /**
     @brief Message types.
     */
    enum MESSAGE_TYPE {

        /**
        0: The message type is undefined.
        */
        MESSAGE_TYPE_UNDEFINED = 0,

        /**
        1: A text message.
        */
        MESSAGE_TYPE_TEXT = 1,

        /**
        2: A raw message. A raw message is a binary message whose size does not exceed 32 KB.
        */
        MESSAGE_TYPE_RAW = 2,

        /**
         3: A file message. The size of a file message must be less than 32 KB.
         */
        MESSAGE_TYPE_FILE = 3,

        /**
         4: An image message. The size of an image message must be less than 32 KB.
         */
        MESSAGE_TYPE_IMAGE = 4,
    };

     /**
      @brief Message sending options.
      */
    struct SendMessageOptions{

    };


    /**
     @brief A data structure holding a user attribute key and its value.
     */
    struct RtmAttribute
    {

        /**
         User attribute name. Must be visible characters and not exceed 32 Bytes.
         */
        const char* key;

        /**
         Value of the user attribute. Must not exceed 8 KB.
         */
        const char* value;
    };

    class IMetadataItem {
    protected:
        virtual ~IMetadataItem() {}
    public:  
        /**
         Sets the key of the metadata item.
         */
        virtual void setKey(const char* key) = 0;
        
        /**
         Gets the key of the metadata item.
         */
        virtual const char* getKey() const = 0;

        /**
         Sets the value of the metadata item.
         */
        virtual void setValue(const char* value) = 0;

        /**
         Gets the value of the metadata item.
         */
        virtual const char* getValue() const = 0;

        /**
         Sets the revision of the metadata item.
         */
        virtual void setRevision(long long revision) = 0;

        /**
         Gets the revision of the metadata item.
         */
        virtual long long getRevision() const = 0;

        /**
         Gets the timestamp of when the metadata item was last updated.
         */
        virtual long long getUpdateTs() const = 0;

        /**
          Gets the User ID of the user who makes the latest update to the metadata item.
         */
        virtual const char* getAuthorUserId() const = 0;

        /**
         Release all resources used by the \ref agora::rtm::IRtmService "IMetadataItem" instance.
         */
        virtual void release() = 0;
    };

    struct RtmMetadataOptions {
        /**
         the revision of whole metadata.
         */
        long long majorRevision;

        /**
         Indicates whether or not to notify server update the modify timestamp of metadata
         */
        bool enableRecordTs;

        /**
         Indicates whether or not to notify server update the modify user id of metadata
         */
        bool enableRecordUserId;

        RtmMetadataOptions()
        : majorRevision(-1), 
          enableRecordTs(false),
          enableRecordUserId(false) {}
    };

    struct Metadata {
        /**
         An array of the metadata item.
         */
        const IMetadataItem** items;

        /**
         The total number of metadata items.
         */
        int numberOfItems;

        /**
         the revision of whole metadata.
         */
        long long majorRevision;

        Metadata() 
        : items(nullptr), 
          numberOfItems(0), 
          majorRevision(-1){}
    };

     /**
      @brief The class for setting or getting attributes of a channel.
       */
     class IRtmChannelAttribute
     {
     protected:
        virtual ~IRtmChannelAttribute() {}
     public:

        /**
         Sets the key of the channel attribute.

         @param key Key of channel attribute. Must be visible characters and not exceed 32 Bytes.
         */
        virtual void setKey(const char *key) = 0;

        /**
         Gets the key of the channel attribute.

         @return Key of the channel attribute.
         */
        virtual const char *getKey() const = 0;

        /**
         Sets the value of the channel attribute.

         @param value Value of the channel attribute. Must not exceed 8 KB in length.
         */
        virtual void setValue(const char *value) = 0;

        /**
         Gets the value of the channel attribute.

         @return Value of the channel attribute.
         */
        virtual const char *getValue() const = 0;

        /**
         Gets the User ID of the user who makes the latest update to the channel attribute.

         @return User ID of the user who makes the latest update to the channel attribute.
         */
        virtual const char *getLastUpdateUserId() const = 0;

        /**
         Gets the timestamp of when the channel attribute was last updated.

         @return Timestamp of when the channel attribute was last updated in milliseconds.
         */
        virtual long long getLastUpdateTs() const = 0;

        /**
         Release all resources used by the \ref agora::rtm::IRtmChannelAttribute "IRtmChannelAttribute" instance.
         */
        virtual void release() = 0;
     };

     /**
      @brief Data structure holding channel attribute-specific options.
      */
     struct ChannelAttributeOptions{

        /**
         Indicates whether or not to notify all channel members of a channel attribute change.

         @note This flag is valid only within the current method call.

         - true: Notify all channel members of a channel attribute change.
         - false: (Default) Do not notify all channel members of a channel attribute change.
         */
        bool enableNotificationToChannelMembers;
        ChannelAttributeOptions():enableNotificationToChannelMembers(false){}
     };

    /**
     @brief The class for setting and retrieving attributes of a channel or peer-to-peer message.
     */
    class IMessage
    {
    protected:
      virtual ~IMessage() {}
    public:

      /**
       Retrieves the unique ID of the message.

       @return The message ID.
       */
      virtual long long getMessageId() const = 0;

      /**
       Retrieves the message type.

       @return The message type. See #MESSAGE_TYPE.
       */
      virtual MESSAGE_TYPE getMessageType() const = 0;

      /**
       Sets the content of a text message, or the text description of a raw message.

       @param str The text message to be set. Must not exceed 32 KB in length. If the message is a raw message, ensure that the overall size of the text description and the raw message data does not exceed 32 KB.
       */
      virtual void setText(const char *str) = 0;

      /**
       Retrieves the content of a text message, or the text description of a raw message.

       @return The content of the received text message, or the text description of the received raw message.
       */
      virtual const char *getText() const = 0;

      /**
       Retrieves the starting address of the raw message in the memory.

       @return The starting address of the raw message in the memory.
       */
      virtual const char *getRawMessageData() const = 0;

      /**
       Retrieves the length of the raw message.

       @return The length of the raw message in Bytes.
       */
      virtual int getRawMessageLength() const = 0;
      /**
       Allows the receiver to retrieve the timestamp of when the messaging server receives this message.

       @note
       - You can infer from the returned timestamp the *approximate* time as to when this message was sent.
       - The returned timestamp is on a millisecond time-scale. It is for demonstration purposes only, not for strict ordering of messages.


       @return The timestamp (ms) of when the messaging server receives this message.
       */
      virtual long long getServerReceivedTs() const = 0;

      /**
       Allows the receiver to check whether this message has been cached on the server (Applies to peer-to-peer message only).

       @note
       - This method returns false if a message is not cached by the server. Only if the sender sends the message as an offline message (sets \ref agora::rtm::SendMessageOptions::enableOfflineMessaging "enableOfflineMessaging" as true) when the specified user is offline, does the method return true when the user is back online.
       - For now we only cache 200 offline messages for up to seven days for each message receiver. When the number of the cached messages reaches this limit, the newest message overrides the oldest one.

       @return
       - true: This message has been cached on the server (the server caches this message and re-sends it to the receiver when he/she is back online).
       - false: This message has not been cached on the server.
       */
      virtual bool isOfflineMessage() const = 0;

      /**
       Releases all resources used by the \ref agora::rtm::IMessage "IMessage" instance.

       @note For the message receiver: please access and save the content of the IMessage instance when receiving the \ref agora::rtm::IChannelEventHandler::onMessageReceived "onMessageReceived" or the \ref agora::rtm::IRtmServiceEventHandler::onMessageReceivedFromPeer "onMessageReceivedFromPeer" callback. The SDK will release the IMessage instance when the callback ends.
       */
      virtual void release() = 0;
    };

    /**
     @brief The class for retrieving the attributes of a channel member.
     */
    class IChannelMember
    {
    protected:
      virtual ~IChannelMember() {}
    public:

      /**
       Retrieves the user ID of a user in the channel.

       @return User ID of a user in the channel.
       */
      virtual const char * getUserId() const = 0;

      /**
       Retrieves the channel ID of the user.

       @return Channel ID of the user.
       */
      virtual const char * getChannelId() const = 0;

      /**
       Releases all resources used by the \ref agora::rtm::IChannelMember "IChannelMember" instance.
       */
      virtual void release() = 0;
    };

    /**
     @brief The data structure holding an RTM user and the user's online status.
     */
    struct PeerOnlineStatus
    {

      /**
       The user ID of the peer.
       */
      const char* peerId;

      /**
       @deprecated Deprecated as of v1.2.0. Use \ref agora::rtm::PeerOnlineStatus::onlineState "onlineState" instead.

       The online status of the peer.

       - true: The user is online (the user has logged in the Agora RTM system).
       - false: The user is offline (the user has logged out of the Agora RTM system, has not logged in, or has failed to logged in).
       */
      bool isOnline;

      /**
       The online state of the peer. See #PEER_ONLINE_STATE.

       @note
       - The server will never return the `unreachable` state, if you <i>query</i> the online status of specified peers (\ref agora::rtm::IRtmService::queryPeersOnlineStatus "queryPeersOnlineStatus"). See also: \ref agora::rtm::IRtmServiceEventHandler::onQueryPeersOnlineStatusResult "onQueryPeersOnlineStatusResult".
       - The server may return the `unreachable` state, if you <i>subscribe to</i> the online status of specified peers (\ref agora::rtm::IRtmService::subscribePeersOnlineStatus "subscribePeersOnlineStatus"). See also: \ref agora::rtm::IRtmServiceEventHandler::onPeersOnlineStatusChanged "onPeersOnlineStatusChanged".
       */
      PEER_ONLINE_STATE onlineState;
    };

    /**
     @brief The data structure holding a channel ID and the current channel member count.
     */
    struct ChannelMemberCount
    {
      /**
       The ID of the channel.
       */
      const char* channelId;
       // count of channel, 0 if channel not found.
      /**
       The current member count of the channel.

       @note @p count is 0, if a channel with the specified @p channelId is not found.
       */
      int count;
    };

    /**
     @brief Callbacks for the Agora RTM channel methods.
     */
    class IChannelEventHandler
    {
    public:

      virtual ~IChannelEventHandler()
      {
      }

      /**
       Occurs when successfully joining a channel.

       When the local user calls the \ref agora::rtm::IChannel::join "join" method and successfully joins the channel:
       - The SDK triggers this callback;
       - All remote users in the channel receive the \ref agora::rtm::IChannelEventHandler::onMemberJoined "onMemberJoined" callback.
       */
      virtual void onJoinSuccess()
      {
      }

      /**
       Occurs when failing to join a channel.

       The local user receives this callback when the \ref agora::rtm::IChannel::join "join" method call fails.

       @param errorCode The error code. See #JOIN_CHANNEL_ERR.
       */
      virtual void onJoinFailure(JOIN_CHANNEL_ERR errorCode)
      {
          (JOIN_CHANNEL_ERR) errorCode;
      }
      
      /**
       Returns the result of the \ref agora::rtm::IChannel::leave "leave" method call.

       @param errorCode The error code. See #LEAVE_CHANNEL_ERR.
       */
      virtual void onLeave(LEAVE_CHANNEL_ERR errorCode)
      {
          (LEAVE_CHANNEL_ERR) errorCode;
      }

      /**
       Occurs when receiving a channel message.

       @param userId The message sender.
       @param message The received channel message. See \ref agora::rtm::IMessage "IMessage".
       */
      virtual void onMessageReceived(const char *userId, const IMessage *message)
      {
          (const char *) userId;
          (IMessage *) message;
      }

      /**
       Returns the result of the \ref agora::rtm::IChannel::sendMessage "sendMessage" method call.

       @param messageId The ID of the sent channel message.
       @param state The error codes. See #CHANNEL_MESSAGE_ERR_CODE.
       */
      virtual void onSendMessageResult(long long messageId, CHANNEL_MESSAGE_ERR_CODE state)
      {
          (long long) messageId;
          (CHANNEL_MESSAGE_ERR_CODE) state;
      }

      /**
       Occurs when a remote user joins the channel.

       When a remote user calls the \ref agora::rtm::IChannel::join "join" method and receives the \ref agora::rtm::IChannelEventHandler::onJoinSuccess "onJoinSuccess" callback (successfully joins the channel), the local user receives this callback.

       @note This callback is disabled when the number of the channel members exceeds 512.

       @param member The user joining the channel. See IChannelMember.
       */
      virtual void onMemberJoined(IChannelMember *member)
      {
          (IChannelMember *) member;
      }

      /**
       Occurs when a remote member leaves the channel.

       When a remote member in the channel calls the \ref agora::rtm::IChannel::leave "leave" method and receives the the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave (LEAVE_CHANNEL_ERR_OK)" callback, the local user receives this callback.

       @note This callback is disabled when the number of the channel members exceeds 512.

       @param member The channel member that leaves the channel. See IChannelMember.
       */
      virtual void onMemberLeft(IChannelMember *member)
      {
          (IChannelMember *) member;
      }

      /**
       Returns the result of the \ref agora::rtm::IChannel::getMembers "getMembers" method call.

       When the method call succeeds, the SDK returns the member list of the channel.

       @param members The member list. See IChannelMember.
       @param userCount The number of members.
       @param errorCode Error code. See #GET_MEMBERS_ERR.
       */
      virtual void onGetMembers(IChannelMember **members, int userCount, GET_MEMBERS_ERR errorCode)
      {
          (IChannelMember **) members;
          (int) userCount;
      }

      /**
       Returns the result of the agora::rtm::IChannel::addChannelMetadata method call.
       */
      virtual void onAddChannelMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IChannel::updateChannelMetadata method call.
       */
      virtual void onUpdateChannelMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IChannel::deleteChannelMetadata method call.
       */
      virtual void onDeleteChannelMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }
  
      /**
       Returns the result of the agora::rtm::IChannel::setChannelMetadata method call.
       */
      virtual void onSetChannelMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IChannel::clearChannelMetadata method call.
       */
      virtual void onClearChannelMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }
  
      /**
       Returns the result of the agora::rtm::IChannel::getChannelMetadata method call.
       */
      virtual void onGetChannelMetadataResult(long long requestId, Metadata* data, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Occurs when channel's metadata are updated
       */
      virtual void onMetadataUpdated(Metadata* data)
      {
        (Metadata*) data;
      }
      /**
       Occurs when channel attributes are updated, and returns all attributes of the channel.

       @note This callback is enabled only when the user, who updates the attributes of the channel, sets \ref agora::rtm::ChannelAttributeOptions::enableNotificationToChannelMembers "enableNotificationToChannelMembers" as true. Also note that this flag is valid only within the current channel attribute method call.

       @param attributes All attribute of this channel.
       @param numberOfAttributes The total number of the channel attributes.
       */
      virtual void onAttributesUpdated(const IRtmChannelAttribute* attributes[], int numberOfAttributes)
      {
          (const IRtmChannelAttribute**) attributes;
          (int) numberOfAttributes;
      }

      /**
       Occurs when the number of the channel members changes, and returns the new number.

       @note
       - When the number of channel members &le; 512, the SDK returns this callback when the number changes at the frequency of once per second.
       - When the number of channel members exceeds 512, the SDK returns this callback when the number changes at the frequency of once every three seconds.
       - You will receive this callback when successfully joining an RTM channel, so Agora recommends implementing this callback to receive timely updates on the number of the channel members.

       @param memberCount Member count of this channel.
       */
      virtual void onMemberCountUpdated(int memberCount)
      {
         (int) memberCount;
      }
    };

    /**
     @brief Agora RTM channel methods.
     */
    class IChannel
    {
    protected:
      virtual ~IChannel() {}
    public:
//            virtual void setEventHandler(IChannelEventHandler *eventHandler) = 0;

      /**
       Joins a channel.

       @note You can join a maximum of 20 RTM channels at the same time. When the number of the channels you join exceeds the limit, you receive the \ref agora::rtm::JOIN_CHANNEL_ERR "JOIN_CHANNEL_ERR_FAILURE" error code.

       - If this method call succeeds:
          - The local user receives the \ref agora::rtm::IChannelEventHandler::onJoinSuccess "onJoinSuccess" callback.
          - All remote users receive the \ref agora::rtm::IChannelEventHandler::onMemberJoined "onMemberJoined" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IChannelEventHandler::onJoinFailure "onJoinFailure" callback. See #JOIN_CHANNEL_ERR for the error codes.
       @return
       - 0: Success.
       - &ne;0: Failure. See #JOIN_CHANNEL_ERR for the error codes.
      */
      virtual int join() = 0;

      /**
       Leaves a channel.

       - If this method call succeeds:
          - The local user receives the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave" callback with the LEAVE_CHANNEL_ERR_OK state.
          - All remote users receive the \ref agora::rtm::IChannelEventHandler::onMemberLeft "onMemberLeft" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IChannelEventHandler::onLeave "onLeave" callback with an error code. See \ref agora::rtm::LEAVE_CHANNEL_ERR "LEAVE_CHANNEL_ERR" for the error codes.
       @return
       - 0: Success.
       - &ne;0: Failure. See #LEAVE_CHANNEL_ERR for the error codes.
       */
      virtual int leave() = 0;

      /**
       @deprecated Allows a channel member to send a message to all members in the channel.

       Agora does not recommend using this method to send a channel message. Use \ref agora::rtm::IChannel::sendMessage(const IMessage *message, const SendMessageOptions &options) "sendMessage" instead.

       If this method call succeeds:
       - The \ref agora::rtm::IChannelEventHandler::onSendMessageResult "onSendMessageResult" callback returns the result.
       - All remote users in the channel receive the \ref agora::rtm::IChannelEventHandler::onMessageReceived "onMessageReceived" callback.

       @note You can send messages, including peer-to-peer and channel messages, at a maximum frequency of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds.

       @param message The message to be sent. See \ref agora::rtm::IMessage "IMessage".
       @return
       - 0: Success.
       - &ne;0: Failure. See #CHANNEL_MESSAGE_ERR_CODE for the error codes.
       */
      virtual int sendMessage(const IMessage *message) = 0;

      /**
       Allows a channel member to send a message to all members in the channel.

       If this method call succeeds:
       - The \ref agora::rtm::IChannelEventHandler::onSendMessageResult "onSendMessageResult" callback returns the result.
       - All remote users in the channel receive the \ref agora::rtm::IChannelEventHandler::onMessageReceived "onMessageReceived" callback.

       @note You can send messages, including peer-to-peer and channel messages, at a maximum frequency of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds.

       @param message The message to be sent. See \ref agora::rtm::IMessage "IMessage".
       @param options Options when sending the channel message. See See \ref agora::rtm::SendMessageOptions "SendMessageOptions".
       @return
       - 0: Success.
       - &ne;0: Failure. See #CHANNEL_MESSAGE_ERR_CODE for the error codes.
       */
      virtual int sendMessage(const IMessage *message, const SendMessageOptions &options) = 0;

      /**
       Retrieves the channel ID.

       @return The channel ID of the channel.
       */
      virtual const char *getId() const = 0;

      /**
       Retrieves a member list of the channel.

       The \ref agora::rtm::IChannelEventHandler::onGetMembers "onGetMembers" callback returns the result of this method call.

       @note You can call this method at a maximum frequency of five calls every two seconds. This method returns a maximum of 512 members. If the number of channel members exceed 512, the method returns 512 members randomly.

       @return
       - 0: Success.
       - &ne;0: Failure. See #GET_MEMBERS_ERR for the error codes.
       */
      virtual int getMembers() = 0;

      /**
       Add new metadata items to the channel.
       */
      virtual int addChannelMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       update metadata items of the channel.
       */
      virtual int updateChannelMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       delete metadata items of the channel.
       */
      virtual int deleteChannelMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;

      /**
       set the metadata of the channel.
       */
      virtual int setChannelMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       delete all metadata items of the channel.
       */
      virtual int clearChannelMetadata(const RtmMetadataOptions &options, long long &requestId) = 0;

      /**
       get all metadata items of the channel.
       */
      virtual int getChannelMetadata(long long &requestId) = 0;
      // sync_call

      /**
       Releases all resources used by the \ref agora::rtm::IChannel "IChannel" instance.
       */
      virtual void release() = 0;
    };

    class IRtmServiceEventHandler
    {
    public:
      virtual ~IRtmServiceEventHandler()
      {
      }

      /**
       Occurs when a user logs in the Agora RTM system.

       The local user receives this callback when the \ref agora::rtm::IRtmService::login "login" method call succeeds.
       */
      virtual void onLoginSuccess() {}

      /**
       Occurs when a user fails to log in the Agora RTM system.

       The local user receives this callback when the \ref agora::rtm::IRtmService::login "login" method call fails. See \ref agora::rtm::LOGIN_ERR_CODE "LOGIN_ERR_CODE" for the error codes.
       */
      virtual void onLoginFailure(LOGIN_ERR_CODE errorCode)
      {
          (LOGIN_ERR_CODE) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::renewToken "renewToken" method call.

       @param token Your new token.
       @param errorCode The error code. See #RENEW_TOKEN_ERR_CODE.
       */
      virtual void onRenewTokenResult(const char* token, RENEW_TOKEN_ERR_CODE errorCode)
      {
          (const char*) token;
          (RENEW_TOKEN_ERR_CODE) errorCode;
      }

      /**
       Occurs when the RTM server detects that the RTM token has exceeded the specified validity period.

       - If the SDK is in the \ref agora::rtm::CONNECTION_STATE_CONNECTED "CONNECTION_STATE_CONNECTED" state, the connection state switches to \ref agora::rtm::CONNECTION_STATE_ABORTED "CONNECTION_STATE_ABORTED" and you need to call the \ref agora::rtm::IRtmService::login "login" method to log in again.
       - If the SDK is in the \ref agora::rtm::CONNECTION_STATE_RECONNECTING "CONNECTION_STATE_RECONNECTING" state, you will receive this callback when the network reconnects and you need to call \ref agora::rtm::IRtmService::renewToken "renewToken" to pass the new token on to the server.
       */
      virtual void onTokenExpired()
      {
      }

      /**
       Occurs when the token expires in 30 seconds.
       
       - Upon receiving this callback, generate a new token on the server and call the \ref agora::rtm::IRtmService::renewToken "renewToken" method to pass the new token to the SDK.
       - If the token used in the \ref agora::rtm::IRtmService::login "login" method expires, the user becomes offline and the SDK attempts to reconnect.
       */
      virtual void onTokenPrivilegeWillExpire()
      {
      }

      /**
       Occurs when a user logs out of the Agora RTM system.

       The local user receives this callback when the SDK calls the \ref agora::rtm::IRtmService::logout "logout" method. See \ref agora::rtm::LOGOUT_ERR_CODE "LOGOUT_ERR_CODE" for the error codes.
       */
      virtual void onLogout(LOGOUT_ERR_CODE errorCode)
      {
          (LOGOUT_ERR_CODE) errorCode;
      }

      /**
       Occurs when the connection state changes between the SDK and the Agora RTM system.

       @param state The new connection state. See #CONNECTION_STATE.
       @param reason The reason for the connection state change. See #CONNECTION_CHANGE_REASON.
       */
      virtual void onConnectionStateChanged(CONNECTION_STATE state, CONNECTION_CHANGE_REASON reason)
      {
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::sendMessageToPeer "sendMessageToPeer" method call.

       @param messageId The ID of the sent message.
       @param errorCode The peer-to-peer message state. See #PEER_MESSAGE_ERR_CODE.

       */
      virtual void onSendMessageResult(long long messageId, PEER_MESSAGE_ERR_CODE errorCode)
      {
          (long long) messageId;
          (PEER_MESSAGE_ERR_CODE) errorCode;
      }

      /**
       Occurs when receiving a peer-to-peer message.

       @param peerId The ID of the message sender.
       @param message The received peer-to-peer message. See \ref agora::rtm::IMessage "IMessage".
       */
      virtual void onMessageReceivedFromPeer(const char *peerId, const IMessage *message)
      {
          (char *) peerId;
          (IMessage *) message;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::queryPeersOnlineStatus "queryPeersOnlineStatus" method call.

       @param requestId The unique ID of this request.
       @param peersStatus The online status of the peer. See PeerOnlineStatus.
       @param peerCount The number of the queried peers.
       @param errorCode Error Codes. See #QUERY_PEERS_ONLINE_STATUS_ERR.
       */
      virtual void onQueryPeersOnlineStatusResult(long long requestId, const PeerOnlineStatus* peersStatus, int peerCount, QUERY_PEERS_ONLINE_STATUS_ERR errorCode)
      {
          (long long) requestId;
          (const PeerOnlineStatus*) peersStatus;
          (int) peerCount;
          (QUERY_PEERS_ONLINE_STATUS_ERR) errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::subscribePeersOnlineStatus "subscribePeersOnlineStatus" or \ref agora::rtm::IRtmService::unsubscribePeersOnlineStatus "unsubscribePeersOnlineStatus" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #PEER_SUBSCRIPTION_STATUS_ERR.
       */
      virtual void onSubscriptionRequestResult(long long requestId, PEER_SUBSCRIPTION_STATUS_ERR errorCode)
      {
          (long long)requestId;
          (PEER_SUBSCRIPTION_STATUS_ERR)errorCode;
      }

      /**
       Returns the result of the \ref agora::rtm::IRtmService::queryPeersBySubscriptionOption "queryPeersBySubscriptionOption" method call.

       @param requestId The unique ID of this request.
       @param peerIds A user ID array of the specified users, to whom you subscribe.
       @param peerCount Count of the peers.
       @param errorCode Error Codes. See #QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR.
       */
      virtual void onQueryPeersBySubscriptionOptionResult(long long requestId, const char* peerIds[], int peerCount, QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR errorCode)
      {
          (const char**)peerIds;
          (int)peerCount;
          (QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR)errorCode;
      }

      /**
       Occurs when the online status of the peers, to whom you subscribe, changes.

       - When the subscription to the online status of specified peers succeeds, the SDK returns this callback to report the online status of peers, to whom you subscribe.
       - When the online status of the peers, to whom you subscribe, changes, the SDK returns this callback to report whose online status has changed.
       - If the online status of the peers, to whom you subscribe, changes when the SDK is reconnecting to the server, the SDK returns this callback to report whose online status has changed when successfully reconnecting to the server.

       @param peersStatus An array of peers' online states. See PeerOnlineStatus.
       @param peerCount Count of the peers, whose online status changes.
       */
      virtual void onPeersOnlineStatusChanged(const PeerOnlineStatus peersStatus[], int peerCount)
      {
          (const PeerOnlineStatus*)peersStatus;
          (int)peerCount;
      }

      /**
       Returns the result of the agora::rtm::IRtmService::addLocalUserMetadata method call.
       */
      virtual void onAddLocalUserMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IRtmService::updateLocalUserMetadata method call.
       */
      virtual void onUpdateLocalUserMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IRtmService::deleteLocalUserMetadata method call.
       */
      virtual void onDeleteLocalUserMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }
  
      /**
       Returns the result of the agora::rtm::IRtmService::setLocalUserMetadata method call.
       */
      virtual void onSetLocalUserMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IRtmService::clearLocalUserMetadata method call.
       */
      virtual void onClearLocalUserMetadataResult(long long requestId, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (METADATA_OPERATION_ERR) errorCode;
      }
      
      /**
       Returns the result of the agora::rtm::IRtmService::getUserMetadata method call.
       */
      virtual void onGetUserMetadataResult(long long requestId, const char* userId, Metadata* data, METADATA_OPERATION_ERR errorCode)
      {
        (long long) requestId;
        (const char*) userId;
        (Metadata*) data;
        (METADATA_OPERATION_ERR) errorCode;
      }
  
      /**
       Returns the result of the agora::rtm::IRtmService::subscribeUserMetadata method call.
       */
      virtual void onSubscribeUserMetadataResult(long long requestId, const char* userId, METADATA_SUBSCRIPTION_ERR errorCode)
      {
        (long long) requestId;
        (const char*) userId;
        (METADATA_SUBSCRIPTION_ERR) errorCode;
      }

      /**
       Returns the result of the agora::rtm::IRtmService::unsubscribeUserMetadata method call.
       */
      virtual void onUnsubscribeUserMetadataResult(long long requestId, const char* userId, METADATA_SUBSCRIPTION_ERR errorCode)
      {
        (long long) requestId;
        (const char*) userId;
        (METADATA_SUBSCRIPTION_ERR) errorCode;
      }

      /**
       Occurs when user's metadata are updated
       */
      virtual void onUserMetadataUpdated(const char* userId, Metadata* data)
      {
        (const char*) userId;
        (Metadata*) data;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onSetLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onAddOrUpdateLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onDeleteLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onClearLocalUserAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" or \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param userId The user ID of the specified user.
       @param attributes An array of the returned attributes. See RtmAttribute.
       @param numberOfAttributes The total number of the user's attributes
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onGetUserAttributesResult(long long requestId, const char* userId, const RtmAttribute* attributes, int numberOfAttributes, ATTRIBUTE_OPERATION_ERR errorCode)
      {
          (long long) requestId;
          (const RtmAttribute*) attributes;
          (int) numberOfAttributes;
          (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onSetChannelAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onAddOrUpdateChannelAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onDeleteChannelAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" method call.

       @param requestId The unique ID of this request.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onClearChannelAttributesResult(long long requestId, ATTRIBUTE_OPERATION_ERR errorCode)
      {
         (long long) requestId;
         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" or \ref agora::rtm::IRtmService::getChannelAttributesByKeys "getChannelAttributesByKeys" method call.

       @param requestId The unique ID of this request.
       @param attributes An array of the returned channel attributes.
       @param numberOfAttributes The total number of the attributes.
       @param errorCode Error Codes. See #ATTRIBUTE_OPERATION_ERR.
       */
      virtual void onGetChannelAttributesResult(long long requestId, const IRtmChannelAttribute* attributes[], int numberOfAttributes, ATTRIBUTE_OPERATION_ERR errorCode)
      {
//         (long long) requestId;
//         (const IRtmChannelAttribute**) attributes;
//         (int) numberOfAttributes;
//         (ATTRIBUTE_OPERATION_ERR) errorCode;
      }

      /**
       Reports the result of the \ref agora::rtm::IRtmService::getChannelMemberCount "getChannelMemberCount" method call.

       @param requestId The unique ID of this request.
       @param channelMemberCounts An array of the channel member counts.
       @param channelCount The total number of the channels.
       @param errorCode Error Codes. See #GET_CHANNEL_MEMBER_COUNT_ERR_CODE.
       */
      virtual void onGetChannelMemberCountResult(long long requestId, const ChannelMemberCount* channelMemberCounts , int channelCount, GET_CHANNEL_MEMBER_COUNT_ERR_CODE errorCode)
       {
          (long long) requestId;
          (const ChannelMemberCount*) channelMemberCounts;
          (int) channelCount;
          (GET_CHANNEL_MEMBER_COUNT_ERR_CODE)errorCode;
       }
    };

    class IRtmCallManager;
    class IRtmCallEventHandler;

    /**
     @brief Output log filter level.
     */
    enum LOG_FILTER_TYPE
    {

      /**
       0: Do not output any log information.
	     */
      LOG_FILTER_OFF = 0,

      /**
       0x000f: Output CRITICAL, ERROR, WARNING, and INFO level log information.
	     */
      LOG_FILTER_INFO = 0x000f,

      /**
       0x000e: Output CRITICAL, ERROR, and WARNING level log information.
	     */
      LOG_FILTER_WARN = 0x000e,

      /**
       0x000c: Output CRITICAL and ERROR level log information.
	     */
      LOG_FILTER_ERROR = 0x000c,

      /**
       0x0008: Output CRITICAL level log information.
	     */
      LOG_FILTER_CRITICAL = 0x0008,
      /**
       Reserved for future use.
	     */
      LOG_FILTER_MASK = 0x80f,
    };

    class IRtmService
    {
    protected:
      virtual ~IRtmService() {}
    public:

      /**
       Initializes an IRtmService instance.

       All methods in the IRtmService class are executed asynchronously.

       @param appId The App ID issued by Agora to you. Apply for a new App ID from Agora if it is missing from your kit.
       @param eventHandler An IRtmServiceEventHandler object that invokes callbacks to be passed to the application on Agora RTM SDK runtime events.
       @return
       - 0: Success.
       - &ne;0: Failure. See #INIT_ERR_CODE.
       */
      virtual int initialize(const char *appId, IRtmServiceEventHandler *eventHandler) = 0;

      /**
       Adds an IRtmServiceEventHandler object.

       @param eventHandler An IRtmServiceEventHandler object.
       */
      virtual void addEventHandler(IRtmServiceEventHandler *eventHandler) = 0;

      /**
       Removes an IRtmServiceEventHandler object.

       @param eventHandler An IRtmServiceEventHandler object.
       */
      virtual void removeEventHandler(IRtmServiceEventHandler *eventHandler) = 0;

      /**
       Releases all resources used by the \ref agora::rtm::IRtmService "IRtmService" instance.
       */
      virtual void release(bool sync = false) = 0;

      /**
       Logs in the Agora RTM system.

       - If this method call succeeds, the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback.
       - If this method call fails, the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginFailure "onLoginFailure" callback. See \ref agora::rtm::LOGIN_ERR_CODE "LOGIN_ERR_CODE" for the error codes.

       @note
       - If you log in with the same user ID from a different instance, you will be kicked out of your previous login and removed from previously joined channels.
       - The call frequency limit for this method is 2 queries per second.
       - Only after you successfully call this method (when the local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLoginSuccess "onLoginSuccess" callback), can you call the key RTM methods except:
            - \ref agora::rtm::IRtmService::createChannel "createChannel"
            - \ref agora::rtm::IRtmService::createMessage "createMessage"
            - \ref agora::rtm::IMessage::setText "setText"
            - \ref agora::rtm::IRtmService::getRtmCallManager "getRtmCallManager"
            - \ref agora::rtm::IRtmCallManager::createLocalCallInvitation "createLocalCallInvitation"
            - \ref agora::rtm::IRtmService::createChannelAttribute "createChannelAttribute"

       @param token Token used to log in the Agora RTM system and used when dynamic authentication is enabled. Set @p token as @p nil in the integration and test stages.
       @param userId The user ID of the user logging in Agora's RTM system. The string must not exceed 64 bytes in length. It cannot be empty, null or "null".
       Supported characters:
       - All lowercase English letters: a to z
       - All uppercase English letters: A to Z
       - All numeric characters: 0 to 9
       - The space character.
       - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       @return
       - 0: Success.
       - &ne;0: Failure. See #LOGIN_ERR_CODE for the error codes.
       */
      virtual int login(const char *token, const char *userId) = 0;

      /**
       Logs out of the Agora RTM system.

       The local user receives the \ref agora::rtm::IRtmServiceEventHandler::onLogout "onLogout" callback. See \ref agora::rtm::LOGOUT_ERR_CODE "LOGIN_ERR_CODE" for the error codes.

       @return
       - 0: Success.
       - &ne;0: Failure. See #LOGOUT_ERR_CODE for the error codes.
       */
      virtual int logout() = 0;

      /**
       Renews the RTM Token of the SDK.

       You are required to renew your RTM Token when receiving the \ref agora::rtm::IRtmServiceEventHandler::onTokenExpired "onTokenExpired" callback, and the \ref agora::rtm::IRtmServiceEventHandler::onRenewTokenResult "onRenewTokenResult" callback returns the result of this method call. The call frequency limit for this method is 2 calls per second.

       @param token Your new RTM Token. You need to generate the RTM Token yourself. See *Generate an RTM Token*.
       @return
       - 0: Success.
       - &ne;0: Failure. See #RENEW_TOKEN_ERR_CODE for the error codes.
       */
      virtual int renewToken(const char* token) = 0;

      /**
       @deprecated Sends a peer-to-peer message to a specified user (receiver).

       @note
       - We do not recommend using this method to send a peer-to-peer message. Use \ref agora::rtm::IRtmService::sendMessageToPeer(const char *peerId, const IMessage *message, const SendMessageOptions &options) "sendMessageToPeer" instead.
       - You can send messages, including peer-to-peer and channel messages, at a maximum frequency of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds.

       @param peerId User ID of the receiver.
       @param message The message to be sent. For information about creating a message, see \ref agora::rtm::IMessage "IMessage".
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_MESSAGE_ERR_CODE for the error codes.
       */
      virtual int sendMessageToPeer(const char *peerId, const IMessage *message) = 0;

      /**
       Sends an (offline) peer-to-peer message to a specified user (receiver).

       This method allows you to send a message to a specified user when he/she is offline. If you set a message as an offline message and the specified user is offline when you send it, the RTM server caches it. Please note that for now we only cache 200 offline messages for up to seven days for each receiver. When the number of the cached messages reaches this limit, the newest message overrides the oldest one.

       If you use this method to send off a <i>text</i> message that starts off with AGORA_RTM_ENDCALL_PREFIX_\<channelId\>_\<your additional information\>, then this method is compatible with the endCall method of the legacy Agora Signaling SDK. Replace \<channelId\> with the channel ID from which you want to leave (end call), and replace \<your additional information\> with any additional information. Note that you must not put any "_" (underscore" in your additional information but you can set \<your additional information\> as empty "".

       - The \ref agora::rtm::IRtmServiceEventHandler::onSendMessageResult "onSendMessageResult" callback returns the result of this method call.
       - When the message arrives at the receiver, the receiver receives the \ref agora::rtm::IRtmServiceEventHandler::onMessageReceivedFromPeer "onMessageReceivedFromPeer" callback.

       If you use this method to send off a <i>text</i> message that starts off with AGORA_RTM_ENDCALL_PREFIX_\<channelId\>_\<your additional information\>, then this method is compatible with the endCall method of the legacy Agora Signaling SDK. Replace \<channelId\> with the channel ID from which you want to leave (end call), and replace \<your additional information\> with any additional information. Note that you must not put any "_" (underscore" in your additional information but you can set \<your additional information\> as empty "".

       @note You can send messages, including peer-to-peer and channel messages, at a maximum frequency of (RTM SDK for Windows C++) 180 calls every three seconds or (RTM SDK for Linux C++) 1500 calls every three seconds.

       @param peerId User ID of the receiver. The string must not exceed 64 bytes in length. It cannot be empty, null, or "null". Supported characters:
       - The 26 lowercase English letters: a to z
       - The 26 uppercase English letters: A to Z
       - The 10 numbers: 0 to 9
       - Space
       - "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       @param message The message to be sent. For information about creating a message, See \ref agora::rtm::IMessage "IMessage".
       @param options Options when sending the message to a peer. See \ref agora::rtm::SendMessageOptions "SendMessageOptions".
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_MESSAGE_ERR_CODE for the error codes.
       */
      virtual int sendMessageToPeer(const char *peerId, const IMessage *message, const SendMessageOptions &options) = 0;

      /**
       Creates an Agora RTM channel.

       - If the method call succeeds, the SDK returns an IChannel instance.
       - If this method call fails, the SDK returns @p null.

       @note You can create multiple channels in an IRtmService instance. But you can only join a maximum of 20 channels at the same time. As a good practice, Agora recommends calling the \ref agora::rtm::IChannel::release "release" method to release all resources of an RTM channel that you no longer use.

       @param channelId The unique channel name. A @p channelId cannot be empty, null, or "null". Must be less than 64 bytes in length.
       Supported characters:
       - All lowercase English letters: a to z
       - All uppercase English letters: A to Z
       - All numeric characters: 0 to 9
       - The space character.
       - Punctuation characters and other symbols, including: "!", "#", "$", "%", "&", "(", ")", "+", "-", ":", ";", "<", "=", ".", ">", "?", "@", "[", "]", "^", "_", " {", "}", "|", "~", ","
       @param eventHandler See IChannelEventHandler.
       @return
       - An \ref agora::rtm::IChannel "IChannel" object: Success. If a channel with the same @p channelId does not exist, the method returns the created channel instance. If a channel with the same @p channelId already exists, the method returns the existing channel instance.
       - null: Failure. Possible reasons: The @p channelId is invalid or you have joined the maximum number of channels.
       */
      virtual IChannel * createChannel(const char *channelId, IChannelEventHandler *eventHandler) = 0;

      /**
       Gets an \ref agora::rtm::IRtmCallManager "IRtmCallManager" object.

       Each \ref agora::rtm::IRtmService "IRtmService" object corresponds to a unique \ref agora::rtm::IRtmCallManager "IRtmCallManager" object. That said, the \ref agora::rtm::IRtmCallManager "IRtmCallManager" object is \ref agora::rtm::IRtmService "IRtmService"-specific.

       @note When you do no use \ref agora::rtm::IRtmCallManager "IRtmCallManager", call the \ref agora::rtm::IRtmCallManager::release "release" method to release all of its resources.

       @param eventHandler An \ref agora::rtm::IRtmCallEventHandler "IRtmCallEventHandler" object.
       @return An \ref agora::rtm::IRtmCallManager "IRtmCallManager" object.
       */
      virtual IRtmCallManager* getRtmCallManager(IRtmCallEventHandler*eventHandler) = 0;

      /**
       Creates an empty text \ref agora::rtm::IMessage "IMessage" instance.

       @note
       - An \ref agora::rtm::IMessage "IMessage" instance can be used either for a channel message or for a peer-to-peer message.
       - When you no longer need an \ref agora::rtm::IMessage "IMessage" instance, please call the \ref agora::rtm::IMessage::release "release" method to release all resources that it is using.
       - You can set the content of the text message at a later point using the \ref agora::rtm::IMessage::setText "setText" method, but please ensure that the size of the text message does not exceed 32 KB.

       @return An empty text \ref agora::rtm::IMessage "IMessage" instance.
       */
      virtual IMessage *createMessage() = 0;

      /**
       Creates a text \ref agora::rtm::IMessage "IMessage" instance.

       @note
       - An \ref agora::rtm::IMessage "IMessage" instance can be used either for a channel message or for a peer-to-peer message.
       - When you no longer need an \ref agora::rtm::IMessage "IMessage" instance, please call the \ref agora::rtm::IMessage::release "release" method to release all resources that it is using.

       @param message The text content of the message. Must not exceed 32 KB in length.

       @return A text \ref agora::rtm::IMessage "IMessage" instance.
       */
      virtual IMessage *createMessage(const char* message) = 0;

      /**
       Creates a raw \ref agora::rtm::IMessage "IMessage" instance.

       @note
       - An \ref agora::rtm::IMessage "IMessage" instance can be used either for a channel message or for a peer-to-peer message.
       - When you no longer need an \ref agora::rtm::IMessage "IMessage" instance, please call the \ref agora::rtm::IMessage::release "release" method to release all resources that it is using.
       - You can set a brief text description of the raw message at a later point using the \ref agora::rtm::IMessage::setText "setText" method, but please ensure that the size of the raw message and the description combined does not exceed 32 KB.

       @param rawData The starting address of the raw message.
       @param length Length of the raw message. Must not exceed 32 KB in length.

       @return A raw \ref agora::rtm::IMessage "IMessage" instance.
       */
      virtual IMessage *createMessage(const uint8_t* rawData, int length) = 0;

      /**
       Creates a raw \ref agora::rtm::IMessage "IMessage" instance and sets its description.

       @note
       - An \ref agora::rtm::IMessage "IMessage" instance can be used either for a channel message or for a peer-to-peer message.
       - When you no longer need an \ref agora::rtm::IMessage "IMessage" instance, please call the \ref agora::rtm::IMessage::release "release" method to release all resources that it is using.
       - You can set @p description as "" and add a brief text description of the raw message at a later point using the \ref agora::rtm::IMessage::setText "setText" method, but please ensure that the size of the raw message and the description combined does not exceed 32 KB.

       @param rawData The starting address of the raw message.
       @param length The length of the raw message. Must not exceed 32 KB in length.
       @param description A brief text description of the raw message. If you set a text description, ensure that the size of the raw message and the description combined does not exceed 32 KB.

       @return A raw \ref agora::rtm::IMessage "IMessage" instance with a brief text description.
       */
      virtual IMessage *createMessage(const uint8_t* rawData, int length, const char* description) = 0;

      /**
       Creates an \ref agora::rtm::IRtmChannelAttribute "IRtmChannelAttribute" instance.

       @return An \ref agora::rtm::IRtmChannelAttribute "IRtmChannelAttribute" instance.
       */
      virtual IRtmChannelAttribute *createChannelAttribute() = 0;

      virtual IMetadataItem *createMetadataItem() = 0;

      /**
       Provides the technical preview functionalities or special customizations by configuring the SDK with JSON options.

       @note The JSON options are not public by default. Agora is working on making commonly used JSON options public in a standard way. Contact [support@agora.io](mailto:support@agora.io) for more information.

       @param parameters SDK options in the JSON format.
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setParameters(const char* parameters) = 0;
//            virtual int getParameters(const char* key, any_document_t& result) = 0;

      /**
       Queries the online status of the specified users.

       - Online: The user has logged in the Agora RTM system.
       - Offline: The user has logged out of the Agora RTM system.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onQueryPeersOnlineStatusResult "onQueryPeersOnlineStatusResult" callback.

       @param peerIds[] An array of the specified user IDs.
       @param peerCount Length of the list.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #QUERY_PEERS_ONLINE_STATUS_ERR for the error codes.
       */
      virtual int queryPeersOnlineStatus(const char* peerIds[], int peerCount, long long &requestId) = 0;

      /**
       Subscribes to the online status of the specified users.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onSubscriptionRequestResult "onSubscriptionRequestResult" callback.

       - When the method call succeeds, the SDK returns the \ref agora::rtm::IRtmServiceEventHandler::onPeersOnlineStatusChanged "onPeersOnlineStatusChanged" callback to report the online status of peers, to whom you subscribe.
       - When the online status of the peers, to whom you subscribe, changes, the SDK returns the \ref agora::rtm::IRtmServiceEventHandler::onPeersOnlineStatusChanged "onPeersOnlineStatusChanged" callback to report whose online status has changed.
       - If the online status of the peers, to whom you subscribe, changes when the SDK is reconnecting to the server, the SDK returns the \ref agora::rtm::IRtmServiceEventHandler::onPeersOnlineStatusChanged "onPeersOnlineStatusChanged" callback to report whose online status has changed when successfully reconnecting to the server.

       @note
       - When you log out of the Agora RTM system, all the status that you subscribe to will be cleared. To keep the original subscription after you re-log in the system, you need to redo the whole subscription process.
       - When the SDK reconnects to the server from the state of being interupted, the SDK automatically subscribes to the peers and states before the interruption without human intervention.

       @param peerIds An array of the specified user IDs.
       @param peerCount Length of the peer list.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_SUBSCRIPTION_STATUS_ERR for the error codes.
       */
      virtual int subscribePeersOnlineStatus(const char* peerIds[], int peerCount, long long &requestId) = 0;

      /**
       Unsubscribes from the online status of the specified users.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onSubscriptionRequestResult "onSubscriptionRequestResult" callback.

       @param peerIds An array of the specified user IDs.
       @param peerCount Length of the peer list.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #PEER_SUBSCRIPTION_STATUS_ERR for the error codes.
       */
      virtual int unsubscribePeersOnlineStatus(const char* peerIds[], int peerCount, long long &requestId) = 0;

      /**
       Gets a list of the peers, to whose specific status you have subscribed.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onQueryPeersBySubscriptionOptionResult "onQueryPeersBySubscriptionOptionResult" callback.

       @param option The status type, to which you have subscribed. See #PEER_SUBSCRIPTION_OPTION.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #QUERY_PEERS_BY_SUBSCRIPTION_OPTION_ERR for the error codes.
       */
      virtual int queryPeersBySubscriptionOption(PEER_SUBSCRIPTION_OPTION option, long long &requestId) = 0;

      /**
       Adds metadata items to local user's metadata.
       */
      virtual int addLocalUserMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       Update the local user's metadata items.
       */
      virtual int updateLocalUserMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       Delete the local user's metadata items.
       */
      virtual int deleteLocalUserMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       Set the local user's metadata.
       */
      virtual int setLocalUserMetadata(const IMetadataItem* items[], int numberOfItems, const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       Delete the local user's metadata.
       */
      virtual int clearLocalUserMetadata(const RtmMetadataOptions &options, long long &requestId) = 0;
      
      /**
       Get the user's metadata of a specified user.
       */
      virtual int getUserMetadata(const char* userId, long long &requestId) = 0;

      /**
       Subscribe to user metadata update events for a specific users.
       */
      virtual int subscribeUserMetadata(const char* userId, long long &requestId) = 0;

      /**
       Unsubscribe to user metadata update events for a specific users.
       */
      virtual int unsubscribeUserMetadata(const char* userId, long long &requestId) = 0;

      /**
       Substitutes the local user's attributes with new ones.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onSetLocalUserAttributesResult "onSetLocalUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param attributes The new attributes. See RtmAttribute.
       @param numberOfAttributes The number of the attributes.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int setLocalUserAttributes(const RtmAttribute* attributes, int numberOfAttributes, long long &requestId) = 0;

      /**
       Adds or updates the local user's attribute(s).

       This method updates the local user's attribute(s) if it finds that the attribute(s) has/have the same key(s), or adds attribute(s) to the local user if it does not.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onAddOrUpdateLocalUserAttributesResult "onAddOrUpdateLocalUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param attributes The attrubutes to be added or updated. See RtmAttribute.
       @param numberOfAttributes The number of the attributes.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int addOrUpdateLocalUserAttributes(const RtmAttribute* attributes, int numberOfAttributes, long long &requestId) = 0;

      /**
       Deletes the local user's attributes by attribute keys.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onDeleteLocalUserAttributesResult "onDeleteLocalUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param attributeKeys An array of the attribute keys to be deleted.
       @param numberOfKeys Number of the attribute keys.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int deleteLocalUserAttributesByKeys(const char* attributeKeys[], int numberOfKeys, long long &requestId) = 0;

      /**
       Clears all attributes of the local user.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onClearLocalUserAttributesResult "onClearLocalUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::setLocalUserAttributes "setLocalUserAttributes", \ref agora::rtm::IRtmService::addOrUpdateLocalUserAttributes "addOrUpdateLocalUserAttributes", \ref agora::rtm::IRtmService::deleteLocalUserAttributesByKeys "deleteLocalUserAttributesByKeys" and \ref agora::rtm::IRtmService::clearLocalUserAttributes "clearLocalUserAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int clearLocalUserAttributes(long long &requestId) = 0;

      /**
       Gets all attributes of a specified user.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetUserAttributesResult "onGetUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 40 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.

       @param userId The user ID of the specified user.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int getUserAttributes(const char* userId, long long &requestId) = 0;

      /**
       Gets the attributes of a specified user by attribute keys.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetUserAttributesResult "onGetUserAttributesResult" callback.

       @note For \ref agora::rtm::IRtmService::getUserAttributes "getUserAttributes" and \ref agora::rtm::IRtmService::getUserAttributesByKeys "getUserAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 40 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.

       @param userId The user ID of the specified user.
       @param attributeKeys An array of the attribute keys.
       @param numberOfKeys The number of the attribute keys.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int getUserAttributesByKeys(const char* userId, const char* attributeKeys[], int numberOfKeys, long long &requestId) = 0;

      /**
       Resets the attributes of a specified channel.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onSetChannelAttributesResult "onSetChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to update its attributes.
       - The attributes of a channel will be cleared if the channel remains empty (has no members) for a couple of minutes.
       - If more than one user can update the channel attributes, then Agora recommends calling \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" to update the cache before calling this method.
       - For \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes", \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes", \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" and \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param channelId The channel ID of the specified channel.
       @param attributes An array of channel attributes. See \ref agora::rtm::IRtmChannelAttribute "IRtmChannelAttribute".
       @param numberOfAttributes The total number of the channel attributes.
       @param options Options for this attribute operation. See ChannelAttributeOptions.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int setChannelAttributes(const char* channelId, const IRtmChannelAttribute* attributes[], int numberOfAttributes, const ChannelAttributeOptions &options, long long &requestId) = 0;

      /**
       Adds or updates the attribute(s) of a specified channel.

       This method updates the specified channel's attribute(s) if it finds that the attribute(s) has/have the same key(s), or adds attribute(s) to the channel if it does not.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onAddOrUpdateChannelAttributesResult "onAddOrUpdateChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to update its attributes.
       - The attributes of a channel will be cleared if the channel remains empty (has no members) for a couple of minutes.
       - If more than one user can update the channel attributes, then Agora recommends calling \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" to update the cache before calling this method.
       - For \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes", \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes", \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" and \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param channelId The channel ID of the specified channel.
       @param attributes An array of channel attributes. See \ref agora::rtm::IRtmChannelAttribute "IRtmChannelAttribute".
       @param numberOfAttributes The total number of the channel attributes.
       @param options Options for this attribute operation. See ChannelAttributeOptions.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int addOrUpdateChannelAttributes(const char* channelId, const IRtmChannelAttribute* attributes[], int numberOfAttributes, const ChannelAttributeOptions &options, long long &requestId) = 0;

      /**
       Deletes the attributes of a specified channel by attribute keys.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onDeleteChannelAttributesResult "onDeleteChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to update its attributes.
       - The attributes of a channel will be cleared if the channel remains empty (has no members) for a couple of minutes.
       - If more than one user can update the channel attributes, then Agora recommends calling \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" to update the cache before calling this method.
       - For \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes", \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes", \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" and \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param channelId The channel ID of the specified channel.
       @param attributeKeys An array of channel attribute keys.
       @param numberOfKeys The total number of the channel attributes.
       @param options Options for this attribute operation. See ChannelAttributeOptions.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int deleteChannelAttributesByKeys(const char* channelId, const char* attributeKeys[], int numberOfKeys, const ChannelAttributeOptions &options, long long &requestId) = 0;

      /**
       Clears all attributes of a specified channel.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onClearChannelAttributesResult "onClearChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to clear its attributes.
       - The attributes of a channel will be cleared if the channel remains empty (has no members) for a couple of minutes.
       - For \ref agora::rtm::IRtmService::setChannelAttributes "setChannelAttributes", \ref agora::rtm::IRtmService::addOrUpdateChannelAttributes "addOrUpdateChannelAttributes", \ref agora::rtm::IRtmService::deleteChannelAttributesByKeys "deleteChannelAttributesByKeys" and \ref agora::rtm::IRtmService::clearChannelAttributes "clearChannelAttributes" taken together: the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 100 calls every five seconds.

       @param channelId The channel ID of the specified channel.
       @param options Options for this attribute operation. See ChannelAttributeOptions.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int clearChannelAttributes(const char* channelId, const ChannelAttributeOptions &options, long long &requestId) = 0;

      /**
       Gets all attributes of a specified channel.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetChannelAttributesResult "onGetChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to get its attributes.
       - For \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" and \ref agora::rtm::IRtmService::getChannelAttributesByKeys "getChannelAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.

       @param channelId The channel ID of the specified channel.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int getChannelAttributes(const char* channelId, long long &requestId) = 0;

      /**
       Gets the attributes of a specified channel by attribute keys.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetChannelAttributesResult "onGetChannelAttributesResult" callback.

       @note
       - You do not have to join the specified channel to get its attributes.
       - For \ref agora::rtm::IRtmService::getChannelAttributes "getChannelAttributes" and \ref agora::rtm::IRtmService::getChannelAttributesByKeys "getChannelAttributesByKeys" taken together, the maximum call frequency is (RTM SDK for Windows C++) 10 calls every five seconds or (RTM SDK for Linux C++) 400 calls every five seconds.
       @param channelId The channel ID of the specified channel.
       @param attributeKeys An array of attribute keys.
       @param numberOfKeys The total number of the attribute keys.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #ATTRIBUTE_OPERATION_ERR for the error codes.
       */
      virtual int getChannelAttributesByKeys(const char* channelId, const char* attributeKeys[], int numberOfKeys, long long &requestId) = 0;

      /**
       Gets the member count of specified channels.

       The SDK returns the result by the \ref agora::rtm::IRtmServiceEventHandler::onGetChannelMemberCountResult "onGetChannelMemberCountResult" callback.

       @note
       - The call frequency limit for this method is one query per second.
       - We do not support getting the member counts of more than 32 channels in one method call.
       - You do not have to join the specified channels to call this method.

       @param channelIds An array of the specified channel IDs.
       @param channelCount The total number of the channels.
       @param requestId The unique ID of this request.
       @return
       - 0: Success.
       - &ne;0: Failure. See #GET_CHANNEL_MEMBER_COUNT_ERR_CODE for the error codes.
       */
      virtual int getChannelMemberCount(const char* channelIds[], int channelCount, long long &requestId) = 0;

      /**
       Specifies the default path to the SDK log file.

       @note
       - Ensure that the directory holding the log file exists and is writable.
       - Ensure that you call this method immediately after calling the \ref agora::rtm::IRtmService::initialize "initialize" method to initialize an \ref agora::rtm::IRtmService "IRtmService" instance, otherwise the output log may be incomplete.
       - For the Linux C++ platform, <i>agorartm.log</i> is in the current working directory.

       @param logfile The absolute file path to the log file. The string of @p logfile is in UTF-8.
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFile(const char* logfile) = 0;

      /**
       Sets the output log level of the SDK.

       You can use one or a combination of the filters. The log level follows the sequence of OFF, CRITICAL, ERROR, WARNING, and INFO. Choose a level to see the logs preceding that level. If, for example, you set the log level to WARNING, you see the logs within levels CRITICAL, ERROR, and WARNING.

       @param filter The log filter level. See #LOG_FILTER_TYPE.
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFilter(LOG_FILTER_TYPE filter) = 0;

      /**
       Sets the size of a single log file. The SDK has two log files with the same size.

       @param fileSizeInKBytes The size of a single log file (KB). For RTM C++ SDK for Windows, the default is `10240` (KB). For RTM C++ SDK for Linux,  the default is `102400` (KB). The value range is [512 KB, 1 GB].
       @return
       - 0: Success.
       - &ne;0: Failure.
       */
      virtual int setLogFileSize(int fileSizeInKBytes) = 0;
    };
////////////////////////////////////////////////////////
/** \addtogroup createRtmService
@{
*/
////////////////////////////////////////////////////////

/**
Creates an \ref agora::rtm::IRtmService "IRtmService" instance.

@note When you no longer need an \ref agora::rtm::IRtmService "IRtmService" instance, ensure that you call the \ref agora::rtm::IRtmService::release "release" method to release all resources that it uses.

@return An \ref agora::rtm::IRtmService "IRtmService" instance.
*/
AGORA_API IRtmService* AGORA_CALL createRtmService();

////////////////////////////////////////////////////////
/** @} */
////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
/** \addtogroup getRtmSdkVersion
@{
*/
////////////////////////////////////////////////////////

/**
Gets the version of the Agora RTM SDK.

@return The version of the Agora RTM SDK.
*/
AGORA_API const char* AGORA_CALL getRtmSdkVersion();

#ifndef AGORA_SDK_BOTH_RTM_AND_RTC
/**
 Region for the Agora RTM service. After setting a region, the RTM SDK can only connect to the Agora RTM service in the specified region.
 */
enum RTM_AREA_CODE {
  /**
   * Mainland China.
   */
  AREA_CODE_CN = (1 << 0),
  /**
   * North America.
   */
  AREA_CODE_NA = (1 << 1),
  /**
   * Europe.
   */
  AREA_CODE_EU = (1 << 2),
  /**
   * Asia excluding Mainland China.
   */
  AREA_CODE_AS = (1 << 3),
  /**
   * Japan.
   */
  AREA_CODE_JP = (1 << 4),
  /**
   * India.
   */
  AREA_CODE_IN = (1 << 5),
  /**
   * (Default) Global.
   */
  AREA_CODE_GLOB = (0xFFFFFFFF)
};
#endif  //AGORA_SDK_BOTH_RTM_AND_RTC

/* Type of the cloud proxy. */
enum RTM_CLOUD_PROXY_TYPE {
  /**
   * No cloud proxy.
   */
  RTM_NONE_PROXY = 0,
  /**
   * TLS cloud proxy.
   */
  RTM_TCP_PROXY = 1
};

/**
 Context of the `%IRtmService` instance.
 */
struct RtmServiceContext
{
  /**
   * Region for the Agora RTM service. The default is #AREA_CODE_GLOB.
   */
  int areaCode = AREA_CODE_GLOB;

  /**
   * Type of the cloud proxy. The default is `RTM_NONE_PROXY`. See #RTM_CLOUD_PROXY_TYPE.
   */
  RTM_CLOUD_PROXY_TYPE proxyType = RTM_NONE_PROXY;
};

/**
 Error codes returned by \ref agora::rtm::setRtmServiceContext "setRtmServiceContext". See #RTM_AREA_CODE.
 */
enum SET_RTM_SERVICE_CONTEXT_ERR_CODE
{
  /**
   0: Successfully configured the context for the Agora RTM service.
   */
  SET_RTM_SERVICE_CONTEXT_ERR_OK  = 0,
  /**
   1: Failed to configure the context for the Agora RTM service because at least one \ref agora::rtm::IRtmService "IRtmService" instance already exists. Call \ref agora::rtm::IRtmService::release "release" to destroy any existing `%IRtmService` instance before configuring the context for the Agora RTM service.
   */
  SET_RTM_SERVICE_CONTEXT_ERR_FAILURE = 1,
};

/**
 Sets the context of all `IRtmService` instances. All `%IRtmservice` instances you create after calling this method match the context.

 @note You must call this method before calling #createRtmService to create an `%IRtmservice` instance or after destroying any existing `%IRtmservice` instance. Otherwise, this method returns the #SET_RTM_SERVICE_CONTEXT_ERR_FAILURE error code.

 @param context Context of the `%IRtmservice` instance, including geofencing and cloud proxy. See \ref agora::rtm::RtmServiceContext "RtmServiceContext".

 @return #SET_RTM_SERVICE_CONTEXT_ERR_CODE.
 */
AGORA_API SET_RTM_SERVICE_CONTEXT_ERR_CODE AGORA_CALL setRtmServiceContext(const RtmServiceContext& context);

////////////////////////////////////////////////////////
/** @} */
////////////////////////////////////////////////////////
  }
}