const myAddon = require('./build/Release/addon');

const { DZConnect, DZPlayer } = myAddon;

const connectTypes = DZConnect.getEventTypes();
const playerTypes = DZPlayer.getEventTypes();

const setup = () => {
	let dzplayer;

	const dzconnect = new DZConnect({
		appId: '8',
		productId: 'NanoPlayer',
		productBuildId: '00001',
		userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE',
		connectEventCb: (event) => {
			switch (event) {
				case connectTypes.DZ_CONNECT_EVENT_USER_LOGIN_OK:
					console.log('Connexion réussie !');

					dzplayer.load('dzmedia:///track/4195713');
					dzplayer.play(1, 0);
					break;
			}
		}
	});

	dzconnect.debugLogDisable();
	dzconnect.activate();
	dzconnect.cachePathSet('/var/tmp/dzrcache_NDK_SAMPLE');
	dzconnect.offlineMode(false);
	dzconnect.setAccessToken(
		'fr49mph7tV4KY3ukISkFHQysRpdCEbzb958dB320pM15OpFsQs'
	);

	dzplayer = new DZPlayer(dzconnect);
	dzplayer.setEventCb((event) => {
		switch (event) {
			case playerTypes.DZ_PLAYER_EVENT_RENDER_TRACK_START:
				console.log('Playing...');
				break;

			case playerTypes.DZ_PLAYER_EVENT_RENDER_TRACK_END:
				console.log('Over...');
				process.exit();
				break;

			default:
				const eventName = Object.keys(playerTypes).filter(key => event === playerTypes[key])[0];
				console.log(eventName);
				break;
		}
	});
	dzplayer.activate();
	dzplayer.setOutputVolume(100);
	dzplayer.setCrossfadingDuration(3000);
};

setup();