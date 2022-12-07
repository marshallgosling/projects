# Agora Auction & Bidding Demo Tutorial

With this demo app, you can:

Client side:
- Login RTM server
- Join channel
- Receive channel metadata
- Generate Auction data list
- Bid action
- Leave channel
- Logout RTM server

Server side:
- Dashboard & Management system (Manage auction & bidding)
- Log viewer
- Redis viewer
- Linux Java demo app (server side metadata management app)

## Running the App
Create a developer account at [Agora.io](https://dashboard.agora.io/signin/), and obtain an App ID.

- LNMP systems
- Server deploy
- Website demo
- Linux bin file

## Server Management & Dashboard demo
[Auction Server](https://github.com/marshallgosling/auction_server), this is a backup application using PHP Laravel.

  
## Integration

### API Documents

- **/api/v1/rtmtoken** Get RTM token
- **/api/v1/auction/bid** Post bid action
- **/api/v1/auction** Get auction data list

#### /api/v1/rtmtoken
- **Method** GET
- **Params** uid
- **String Response** token string
  
| Param | Type | Definitioin |
| ----- | ----- | ----- |
| uid | string | Username |

#### /api/v1/auction/bid

- **Method** POST
- **ContentType** application/json
- **JSON Payload** { id, uid, amount } 
- **JSON Response** { result, reason }

| Param | Type | Definitioin |
| ----- | ----- | ----- |
| id |  int | The primary key of auction table |
| uid | string | Username |
| amount | int | The bid amount |
| result | bool | The result of post action |
| reason | string | Help infomation |

#### /api/v1/auction

- **Method** GET
- **Params** channelid
- **JSON Response** [{ id, name, cover, channelid, status, ... }...]

| Param | Type | Definitioin |
| ----- | ----- | ----- |
| id |  int | The primary key of auction table |
| name | string | Auction name |
| cover | string | Image url, along with baseUrl  |
| channelid | string | channel name(id) in RTM |
| status | int | Auction status, 1: Online, 2: Offline, 0:Pending |
| owner | string | The current owner of the Auction |
| last_bid_at | date | Last valid bid datetime |
| last_bid | int | Last valid bid primry key |
| start_at | date |  Auction start time |
| end_at | date | Auction end time |

### onMetadataUpdated callback

```
{
    "_rev": 6875569,
    "_rev_auction": 6868604,
    "auction": "[{\"id\":1,\"name\":\"Super Car\",\"code\":\"supercar\",\"cover\":\"images\\/120419778_mercedes.jpeg\",\"owner\":\"migo\",\"status\":1,\"duration\":0,\"channelid\":\"auction\",\"base_amount\":10000,\"amount\":10120,\"last_bid\":12,\"start_at\":\"2022-12-04 00:00:00\",\"end_at\":\"2022-12-28 00:00:00\",\"last_bid_at\":\"2022-12-05 02:04:26\",\"created_at\":\"2022-12-05 06:06:16\",\"updated_at\":\"2022-12-05 02:04:26\"}]"
}
```

| Param | Type | Definitioin |
| ----- | ----- | ----- |
| id |  int | The primary key of auction table |
| name | string | Auction name |
| cover | string | Image url, along with baseUrl  |
| channelid | string | channel name(id) in RTM |
| status | int | Auction status, 1: Online, 2: Offline, 0:Pending |
| owner | string | The current owner of the Auction |
| last_bid_at | date | Last valid bid datetime |
| last_bid | int | Last valid bid primry key |
| start_at | date |  Auction start time |
| end_at | date | Auction end time |
