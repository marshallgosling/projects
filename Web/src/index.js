import './assets/styles.css'
import * as $ from 'jquery'
import * as M from 'materialize-css'

import RtmClient from './rtm-client'
import {
  Toast,
  validator,
  serializeFormData,
  imageToBlob,
  blobToImage
} from './common'

$(() => {
  M.AutoInit()

  const rtm = new RtmClient()

  fetch("/api/v1/channels", {
    method: 'get'
  }).then((response) => {
    return response.json();
  }).then((data) => {
    console.log(data)
    if(data.result && data.result.length > 0) {
      
      for(var i=0;i<data.result.length;i++) {
        let item = data.result[i];
        console.log("option "+item.channelid)
        var $newOpt = $("<option>").attr("value",item.channelid).text(item.channelid)
        $("#mySelect").append($newOpt);
      }

      var elems = document.querySelectorAll('select');
      M.FormSelect.init(elems);
    }
    
  })

  rtm.on('ConnectionStateChanged', (newState, reason) => {
    console.log('reason', reason)
    const view = $('<div/>', {
      text: ['newState: ' + newState, ', reason: ', reason].join('')
    })
    $('#log').append(view)
    if (newState === 'ABORTED') {
      if (reason === 'REMOTE_LOGIN') {
        Toast.error('You have already been kicked off!')
        $('#accountName').text('Agora Chatroom')

        $('#dialogue-list')[0].innerHTML = ''
        $('#chat-message')[0].innerHTML = ''
      }
    }
  })

  rtm.on('MessageFromPeer', async (message, peerId) => {
    if (message.messageType === 'IMAGE') {
      const blob = await rtm.client.downloadMedia(message.mediaId)
      blobToImage(blob, (image) => {
        const view = $('<div/>', {
          text: [' peer: ', peerId].join('')
        })
        $('#log').append(view)
        $('#log').append(`<img src= '${image.src}'/>`)
      })
    } else {
      console.log('message ' + message.text + ' peerId' + peerId)
      const view = $('<div/>', {
        text: ['message.text: ' + message.text, ', peer: ', peerId].join('')
      })
      $('#log').append(view)
    } 

  })

  rtm.on('MemberJoined', ({ channelName, args }) => {
    const memberId = args[0]
    console.log('channel ', channelName, ' member: ', memberId, ' joined')
    const view = $('<div/>', {
      text: ['event: MemberJoined ', ', channel: ', channelName, ', memberId: ', memberId].join('')
    })
    $('#log').append(view)
  })

  rtm.on('MemberLeft', ({ channelName, args }) => {
    const memberId = args[0]
    console.log('channel ', channelName, ' member: ', memberId, ' joined')
    const view = $('<div/>', {
      text: ['event: MemberLeft ', ', channel: ', channelName, ', memberId: ', memberId].join('')
    })
    $('#log').append(view)
  })

  rtm.on('ChannelMessage', async ({ channelName, args }) => {
    const [message, memberId] = args
    if (message.messageType === 'IMAGE') {
      const blob = await rtm.client.downloadMedia(message.mediaId)
      blobToImage(blob, (image) => {
        const view = $('<div/>', {
          text: ['event: ChannelMessage ', 'channel: ', channelName, ' memberId: ', memberId].join('')
        })
        $('#log').append(view)
        $('#log').append(`<img src= '${image.src}'/>`)
      })
    } else {
      console.log('channel ', channelName, ', messsage: ', message.text, ', memberId: ', memberId)
      const view = $('<div/>', {
        text: ['event: ChannelMessage ', 'channel: ', channelName, ', message: ', message.text, ', memberId: ', memberId].join('')
      })
      $('#log').append(view)
    }  
  })

  /**
     * Gao: add new feature onMetadataUpdate event
     */
   rtm.on('MetaDataUpdated', async ({ channelName }) => {
    //const memberId = args[0]

    console.log('MetaDataUpdated')
    const metadata = await rtm.channels[channelName].channel.getChannelMetadata();
    console.log(metadata)
    rtm.channels[channelName].metadata = metadata
    const view = $('<div/>', {
      text: ['event: MetaDataUpdated ', JSON.stringify(metadata.items)].join('')
    })
    $('#log').append(view)

    
    if(metadata.items.length == 0) {return}
    const auctions = JSON.parse(metadata.items[0].value);

    initView(auctions);
  })

  function initView(auctions) {
    if (auctions.length == 0) return;
    $('#cards').html('')
    for(var i=0;i<auctions.length;i++)
    {

      let auction = auctions[i]

      $('<div class="col s4 m5">'+
      '<div class="card">'+
      '  <div class="card-image">'+
      '    <img src="/storage/' + auction.cover + '">'+
      '  </div>'+
      '  <div class="card-content">'+
      '    <p>Product: '+auction.name + '</p>'+
      '     <p>Price: $' + auction.amount + '</p>'+
      '    <p>Owner: ' + auction.owner + '</p>'+
      '  </div>'+
      '  <div class="card-action">'+
      '  <label for="bidamount'+auction.id+'" class="active">Put your best bid</label>'+
      '  <input type="text" placeholder="$'+auction.amount+'" name="bidamount'+auction.id+'" id="bidamount'+auction.id+'">'+
      '    <button class="btn btn-raised btn-primary waves-effect waves-light custom-btn-pin bid-btn" data-id="'+ auction.id + '" data-amount="'+auction.amount+'">BID</button>'+
      '  </div>'+
      '</div>'+
      '</div>').appendTo('#cards');

    }
    $('.bid-btn').on('click', bidClick);

  }

  function bidClick(e) {
    e.preventDefault()
    let btn = $(this);
    let id = btn.data('id');
    
    let uid = rtm.accountName;
    let amount = $("#bidamount"+id).val();
   
    if(!amount) {
      Toast.error('Please put you bid amount!')
      return;
    }

    btn.attr('disabled','disabled');

    fetch("/api/v1/auction/bid", {
      method: 'post',
      body: JSON.stringify({ id, uid, amount }),
      headers: {
        'Content-Type': 'application/json'
      }
    }).then((response) => {
      btn.removeAttr('disabled');
      return response.json();
    }).then((data) => {
      data.result ? 
      Toast.notice(data.reason) :
      Toast.error(data.reason)    
    })
  }
  

  $('#login').on('click', function (e) {
    e.preventDefault()
    
    if (rtm._logined) {
      Toast.error('You already logined')
      return
    }

    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName'])) {
      return
    }

    $(this).attr('disabled', 'disabled');

    try {
      rtm.init(params.appId)
      window.rtm = rtm

      fetch("/api/v1/rtmtoken?uid="+params.accountName).then((response) => {
        return response.text();
      })
      .then((token) => {
        rtm.login(params.accountName, token).then(() => {
          console.log('login')
          rtm._logined = true
          Toast.notice('Login: ' + params.accountName)

          $("#logout").removeAttr('disabled');
          $("#join").removeAttr('disabled');
        }).catch((err) => {
          console.log(err)
        })
      })
      
    } catch (err) {
      Toast.error('Login failed, please open console see more details')
      console.error(err)
      $(this).removeAttr('disabled');
    }
  })

  $('#logout').on('click', function (e) {
    e.preventDefault()
    
    if (!rtm._logined) {
      Toast.error('You already logout')
      $(this).attr('disabled', 'disabled');
      $("#login").removeAttr('disabled');
      return
    }
    $(this).attr('disabled', 'disabled');
    rtm.logout().then(() => {
      console.log('logout')
      rtm._logined = false
      Toast.notice('Logout: ' + rtm.accountName)
      $("#login").removeAttr('disabled');
    }).catch((err) => {
      Toast.error('Logout failed, please open console see more details')
      console.log(err)
      $(this).removeAttr('disabled');
    })
  })

  $('#join').on('click', function (e) {
    e.preventDefault()
    
    if (!rtm._logined) {
      Toast.error('Please Login First')
      $(this).attr('disabled','disabled');
      return
    }
    
    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'channelName'])) {
      return
    }
    
    if (rtm.channels[params.channelName] ||
        (rtm.channels[params.channelName] && rtm.channels[params.channelName].joined)) {
      Toast.error('You already joined')
      $("#leave").removeAttr('disabled');
      return
    }
    $(this).attr('disabled', 'disabled');
    rtm.joinChannel(params.channelName).then(() => {
      const view = $('<div/>', {
        text: rtm.accountName + ' join channel success'
      })
      $('#log').append(view)
      $("#leave").removeAttr('disabled');
      $("#reload_action").removeAttr('disabled');
      $("#send_channel_message").removeAttr('disabled');
      rtm.channels[params.channelName].joined = true
    }).catch((err) => {
      $(this).removeAttr('disabled');
      Toast.error('Join channel failed, please open console see more details.')
      console.error(err)
    })
  })

  $('#leave').on('click', function (e) {
    e.preventDefault()
    if (!rtm._logined) {
      Toast.error('Please Login First')
      return
    }
    $(this).attr('disabled', 'disabled');

    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'channelName'])) {
      return
    }

    if (!rtm.channels[params.channelName] ||
      (rtm.channels[params.channelName] && !rtm.channels[params.channelName].joined)
    ) {
      Toast.error('You already leave')
      $("#join").removeAttr('disabled');
    }

    rtm.leaveChannel(params.channelName).then(() => {
      const view = $('<div/>', {
        text: rtm.accountName + ' leave channel success'
      })
      $('#log').append(view)
      if (rtm.channels[params.channelName]) {
        rtm.channels[params.channelName].joined = false
        rtm.channels[params.channelName] = null
      }
      $("#join").removeAttr('disabled');
      $("#reload_action").attr('disabled', 'disabled');
      $("#send_channel_message").attr('disabled', 'disabled');
      $("#cards").html('');
    }).catch((err) => {
      Toast.error('Leave channel failed, please open console see more details.')
      console.error(err)
      $("#leave").removeAttr('disabled');
    })
  })

  $('#send_channel_message').on('click', function (e) {
    e.preventDefault()
    if (!rtm._logined) {
      Toast.error('Please Login First')
      return
    }

    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'channelName', 'channelMessage'])) {
      return
    }

    if (!rtm.channels[params.channelName] ||
      (rtm.channels[params.channelName] && !rtm.channels[params.channelName].joined)
    ) {
      Toast.error('Please Join first')
    }

    rtm.sendChannelMessage(params.channelMessage, params.channelName).then(() => {
      const view = $('<div/>', {
        text: 'account: ' + rtm.accountName + ' send : ' + params.channelMessage + ' channel: ' + params.channelName
      })
      $('#log').append(view)
    }).catch((err) => {
      Toast.error('Send message to channel ' + params.channelName + ' failed, please open console see more details.')
      console.error(err)
    })
  })

  $('#send_peer_message').on('click', function (e) {
    e.preventDefault()
    if (!rtm._logined) {
      Toast.error('Please Login First')
      return
    }

    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'peerId', 'peerMessage'])) {
      return
    }

    rtm.sendPeerMessage(params.peerMessage, params.peerId).then(() => {
      const view = $('<div/>', {
        text: 'account: ' + rtm.accountName + ' send : ' + params.peerMessage + ' peerId: ' + params.peerId
      })
      $('#log').append(view)
    }).catch((err) => {
      Toast.error('Send message to peer ' + params.peerId + ' failed, please open console see more details.')
      console.error(err)
    })
  })

  $('#reload_action').on('click', function(e) {
    e.preventDefault()
    if (!rtm._logined) {
      Toast.error('Please Login First')
      return
    }

    const params = serializeFormData('loginForm')

    if (!validator(params, ['auction'])) {
      
      return
    }

    fetch("/api/v1/auction?channelid="+params.auction).then((response) => {
      return response.json();
    }).then((data) => {
      initView(data);
    })

  })

  $('#query_peer').on('click', function (e) {
    e.preventDefault()
    if (!rtm._logined) {
      Toast.error('Please Login First')
      return
    }

    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'memberId'])) {
      return
    }

    rtm.queryPeersOnlineStatus(params.memberId).then((res) => {
      const view = $('<div/>', {
        text: 'memberId: ' + params.memberId + ', online: ' + res[params.memberId]
      })
      $('#log').append(view)
    }).catch((err) => {
      Toast.error('query peer online status failed, please open console see more details.')
      console.error(err)
    })
  })

  $('#send-image').on('click', async function (e) {
    e.preventDefault()
    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'peerId'])) {
      return
    }
    const src = $('img').attr('src')
    imageToBlob(src, (blob) => {
      rtm.uploadImage(blob, params.peerId)
    })
    
  })

  $('#send-channel-image').on('click', async function (e) {
    e.preventDefault()
    const params = serializeFormData('loginForm')

    if (!validator(params, ['appId', 'accountName', 'channelName'])) {
      return
    }
    const src = $('img').attr('src')
    imageToBlob(src, (blob) => {
      rtm.sendChannelMediaMessage(blob, params.channelName).then(() => {
        const view = $('<div/>', {
          text: 'account: ' + rtm.accountName  + ' channel: ' + params.channelName
        })
        $('#log').append(view)
        $('#log').append(`<img src= '${src}'/>`)
      }).catch((err) => {
        Toast.error('Send message to channel ' + params.channelName + ' failed, please open console see more details.')
        console.error(err)
      })
    })  
  })
})
