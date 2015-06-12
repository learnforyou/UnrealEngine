// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OnlineBeaconClient.h"
#include "PartyBeaconState.h"

#include "PartyBeaconClient.generated.h"

struct FUniqueNetIdRepl;
struct FPartyReservation;
class FOnlineSessionSearchResult;

#define PARTY_BEACON_TYPE TEXT("PartyBeacon")

/**
 * Types of reservation requests that can be made by this beacon
 */
UENUM()
enum class EClientRequestType : uint8
{
	/** Make a reservation with an existing session */
	ExistingSessionReservation,
	/** Make an update to an existing reservation */
	ReservationUpdate,
	/** Reservation to configure an empty server  */
	EmptyServerReservation,
	/** Attempt to change an existing session to use a new world */
	ChangeWorldRequest,
	/** Simple reconnect (checks for existing reservation) */
	Reconnect
};

inline const TCHAR* ToString(EClientRequestType RequestType)
{
	switch (RequestType)
	{
	case EClientRequestType::ExistingSessionReservation:
	{
		return TEXT("Existing Session Reservation");
	}
	case EClientRequestType::ReservationUpdate:
	{
		return TEXT("Reservation Update");
	}
	case EClientRequestType::EmptyServerReservation:
	{
		return TEXT("Empty Server Reservation");
	}
	case EClientRequestType::ChangeWorldRequest:
	{
		return TEXT("Change World Request");
	}
	case EClientRequestType::Reconnect:
	{
		return TEXT("Reconnect Only");
	}
	}
	return TEXT("");
}

/**
 * Delegate triggered when a response from the party beacon host has been received
 *
 * @param ReservationResponse response from the server
 */
DECLARE_DELEGATE_OneParam(FOnReservationRequestComplete, EPartyReservationResult::Type /** ReservationResponse */);

/**
 * Delegate triggered when the host indicated a reservation count has changed
 *
 * @param NumRemaining number of slots remaining in the session
 */
DECLARE_DELEGATE_OneParam(FOnReservationCountUpdate, int32 /** NumRemaining */);

/**
 * A beacon client used for making reservations with an existing game session
 */
UCLASS(transient, notplaceable, config=Engine)
class ONLINESUBSYSTEMUTILS_API APartyBeaconClient : public AOnlineBeaconClient
{
	GENERATED_UCLASS_BODY()

	// Begin AOnlineBeacon Interface
	virtual FString GetBeaconType() override { return PARTY_BEACON_TYPE; }
	// End AOnlineBeacon Interface

	// Begin AOnlineBeaconClient Interface
	virtual void OnConnected() override;
	// End AOnlineBeaconClient Interface

	/**
	 * Sends a request to the remote host to allow the specified members to reserve space
	 * in the host's session. Note this request is async.
	 *
	 * @param ConnectInfoStr the URL of the server that the connection will be made to
	 * @param InSessionId Id of the session expected to be found at this destination
	 * @param RequestingPartyLeader the leader of this party that will be joining
	 * @param PartyMembers the list of players that want to reserve space
	 *
	 * @return true if the request able to be sent, false if it failed to send
	 */
	virtual bool RequestReservation(const FString& ConnectInfoStr, const FString& InSessionId, const FUniqueNetIdRepl& RequestingPartyLeader, const TArray<FPlayerReservation>& PartyMembers);

	/**
	 * Sends a request to the remote host to allow the specified members to reserve space
	 * in the host's session. Note this request is async.
	 *
	 * @param DesiredHost a search result describing the server that the connection will be made to
	 * @param RequestingPartyLeader the leader of this party that will be joining
	 * @param PartyMembers the list of players that want to reserve space
	 *
	 * @return true if the request able to be sent, false if it failed to send
	 */
	virtual bool RequestReservation(const FOnlineSessionSearchResult& DesiredHost, const FUniqueNetIdRepl& RequestingPartyLeader, const TArray<FPlayerReservation>& PartyMembers);

	/**
	 * Sends an update request to the remote host to append additional members to an existing party
	 * in the host's session. Note this request is async.
	 *
	 *  ** This version is for existing / established connections only, it will not actually attempt a connection **
	 *
	 * @param RequestingPartyLeader the leader of the party that will be updated
	 * @param PlayersToAdd the list of players that will be added to the party
	 *
	 * @return true if the request able to be sent, false if it failed to send
	 */
	virtual bool RequestReservationUpdate(const FUniqueNetIdRepl& RequestingPartyLeader, const TArray<FPlayerReservation>& PlayersToAdd);

	/**
	 * Sends an update request to the remote host to append additional members to an existing party
	 * in the host's session. Note this request is async.
	 *
	 * @param ConnectInfoStr the URL of the server that the connection will be made to
	 * @param InSessionId Id of the session expected to be found at this destination
	 * @param RequestingPartyLeader the leader of the party that will be updated
	 * @param PlayersToAdd the list of players that will be added to the party
	 *
	 * @return true if the request able to be sent, false if it failed to send
	 */
	virtual bool RequestReservationUpdate(const FString& ConnectInfoStr, const FString& InSessionId, const FUniqueNetIdRepl& RequestingPartyLeader, const TArray<FPlayerReservation>& PlayersToAdd);

	/**
	 * Sends an update request to the remote host to append additional members to an existing party
	 * in the host's session. Note this request is async.
	 *
	 * @param DesiredHost a search result describing the server that the connection will be made to
	 * @param RequestingPartyLeader the leader of the party that will be updated
	 * @param PlayersToAdd the list of players that will be added to the party
	 *
	 * @return true if the request able to be sent, false if it failed to send
	 */
	virtual bool RequestReservationUpdate(const FOnlineSessionSearchResult& DesiredHost, const FUniqueNetIdRepl& RequestingPartyLeader, const TArray<FPlayerReservation>& PlayersToAdd);

	/**
	 * Cancel an existing request to the remote host to revoke allocated space on the server.
	 * Note this request is async.
	 */
	virtual void CancelReservation();

	/**
	 * Response from the host session after making a reservation request
	 *
	 * @param ReservationResponse response from server
	 */
 	UFUNCTION(client, reliable)
	virtual void ClientReservationResponse(EPartyReservationResult::Type ReservationResponse);

	/**
	 * Response from the host session that the reservation count has changed
	 *
	 * @param NumRemainingReservations number of slots remaining until a full session
	 */
	UFUNCTION(client, reliable)
	virtual void ClientSendReservationUpdates(int32 NumRemainingReservations);

	/**
	 * Delegate triggered when a response from the party beacon host has been received
	 *
	 * @return delegate to handle response from the server
	 */
	FOnReservationRequestComplete& OnReservationRequestComplete() { return ReservationRequestComplete; }

	/**
	 * Delegate triggered when the host indicated a reservation count has changed
	 *
	 * @param NumRemaining number of slots remaining in the session
	 */
	FOnReservationCountUpdate& OnReservationCountUpdate() { return ReservationCountUpdate; }

	/**
	* @return the pending reservation associated with this beacon client
	*/
	const FPartyReservation& GetPendingReservation() const { return PendingReservation; }
	
protected:

	/** Delegate for reservation request responses */
	FOnReservationRequestComplete ReservationRequestComplete;
	/** Delegate for reservation count updates */
	FOnReservationCountUpdate ReservationCountUpdate;

	/** Session Id of the destination host */
	UPROPERTY()
	FString DestSessionId;
	/** Pending reservation that will be sent upon connection with the intended host */
	UPROPERTY()
	FPartyReservation PendingReservation;

	/** Type of request currently being handled by this client beacon */
	UPROPERTY()
	EClientRequestType RequestType;

	/** Has the reservation request been delivered */
	UPROPERTY()
	bool bPendingReservationSent;
	/** Has the reservation request been canceled */
	UPROPERTY()
	bool bCancelReservation;

	/**
	 * Tell the server about the reservation request being made
	 *
	 * @param SessionId expected session id on the other end (must match)
	 * @param Reservation pending reservation request to make with server
	 */
	UFUNCTION(server, reliable, WithValidation)
	virtual void ServerReservationRequest(const FString& SessionId, const FPartyReservation& Reservation);
	
	/**
	 * Tell the server about the reservation update request being made
	 *
	 * @param SessionId expected session id on the other end (must match)
	 * @param ReservationUpdate pending reservation request to make with server
	 */
	UFUNCTION(server, reliable, WithValidation)
	virtual void ServerUpdateReservationRequest(const FString& SessionId, const FPartyReservation& ReservationUpdate);

	/**
	 * Tell the server to cancel a pending or existing reservation
	 *
	 * @param PartyLeader id of the party leader for the reservation to cancel
	 */
	UFUNCTION(server, reliable, WithValidation)
	virtual void ServerCancelReservationRequest(const FUniqueNetIdRepl& PartyLeader);
};
