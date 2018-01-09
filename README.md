An **unofficial** Node.js wrapper for Deezer Native C SDK.

This module aims to provide a straightforward binding to Deezer Native SDK, with a very similar API (method names and arguments should be quite the same as the original C SDK.

## Usage

```bash
> npm i native-deezer
```

```js
const { DZConnect, DZPlayer, getEventTypes } = require('native-deezer');

const Types = getEventTypes();

let dzplayer;

// Instanciate a new Deezer connection
const dzconnect = new DZConnect({
  appId: '8', // Your application ID, retrieved when registering a new application on Deezer Developer site (see below)
  productId: 'NativeDeezerDemo',
  productBuildId: '00001',
  userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE', // Where to store user profile data
  connectEventCb: event => {
    // This will be called on each connection event
    switch (event) {
      case types.DZ_CONNECT_EVENT_USER_LOGIN_OK:
        console.log('Login success !');
        
        // Load a track into the playlist
        dzplayer.load('dzmedia:///track/10287076');
        // Start playing
        dzplayer.play(1, 0);
        break;
    }
  }
});

dzconnect.debugLogDisable();      // Disable logs
dzconnect.activate();             // Activate connection
dzconnect.cachePathSet(           // Set cache path
  '/var/tmp/dzrcache_NDK_SAMPLE'
);
dzconnect.offlineMode(false);     // Disable offline mode
dzconnect.setAccessToken('fr49mph7tV4KY3ukISkFHQysRpdCEbzb958dB320pM15OpFsQs'); // Set user access token

// Instanciate a new DZPlayer connection
dzplayer = new DZPlayer(dzconnect);
dzplayer.activate();                    // Activate player
dzplayer.setEventCb(                    // Set cllback for player events
  event => console.log(event)
);
dzplayer.setOutputVolume(100);          // Set player output volume
dzplayer.setCrossfadingDuration(3000);  // Set crossfading duration
```

For further detailed information, refer to the Deezer Native SDK documentation : https://developers.deezer.com/sdk/native

## API

To-do

## Requirements

See https://developers.deezer.com/sdk/native#_external_dependencies


## See Also

- [`Deezer Native C SDK`](https://developers.deezer.com/sdk/native)

## License

MIT

