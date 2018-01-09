const myAddon = require('./build/Release/addon');

const { DZConnect, DZPlayer, getEventTypes } = myAddon;

console.log(DZConnect.protoype);

const types = getEventTypes();

const setup = () => {
	let dzplayer;

	const dzconnect = new DZConnect({
		appId: '8',
		productId: 'NanoPlayer',
		productBuildId: '00001',
		userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE',
		connectEventCb: (event) => {
			switch (event) {
				case types.DZ_CONNECT_EVENT_USER_LOGIN_OK:
					console.log('Connexion réussie !');

					dzplayer.load('dzmedia:///track/10287076');
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
		'frmZGRBG5fNJQe2kCnMYN5AxLsciGKnhuFMbTZjwGU2DjIP4Ue',
		() => {
			console.log('Connexion réussie !');
		}
	);

	dzplayer = new DZPlayer(dzconnect);
	dzplayer.activate();
	dzplayer.setEventCb((event) => {});
	dzplayer.setOutputVolume(100);
	dzplayer.setCrossfadingDuration(3000);
};

setup();
setup();

/*const types = myAddon.getEventTypes();

const obj = new DZPlayer(
	{
		appId: '8',
		productId: 'NanoPlayer',
		productBuildId: '00001',
		userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE'
	},
	'frmZGRBG5fNJQe2kCnMYN5AxLsciGKnhuFMbTZjwGU2DjIP4Ue',
	function(event) {
		switch (event) {
			case types.DZ_CONNECT_EVENT_USER_LOGIN_OK:
				console.log('DZ_CONNECT_EVENT_USER_LOGIN_OK');
				break;
			case types.DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE:
				console.log('DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE');
				break;
		}
	},
	function() {
		console.log('done');
	}
);

obj.listen(
	function(event) {
		switch (event) {
			case types.DZ_CONNECT_EVENT_USER_LOGIN_OK:
				console.log('DZ_CONNECT_EVENT_USER_LOGIN_OK');
				break;
			case types.DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE:
				console.log('DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE');
				break;
		}
	},
	function() {
		console.log('done');
	}
);*/

// const types = myAddon.getEventTypes();

// console.log(types);

// myAddon.doProgress(
// 	{
// 		appId: '8',
// 		productId: 'NanoPlayer',
// 		productBuildId: '00001',
// 		userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE'
// 	},
// 	'frmZGRBG5fNJQe2kCnMYN5AxLsciGKnhuFMbTZjwGU2DjIP4Ue',
// 	function(event) {
// 		switch (event) {
// 			case types.DZ_CONNECT_EVENT_USER_LOGIN_OK:
// 				console.log('DZ_CONNECT_EVENT_USER_LOGIN_OK');
// 				break;
// 			case types.DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE:
// 				console.log('DZ_CONNECT_EVENT_USER_OFFLINE_AVAILABLE');
// 				break;
// 		}
// 	},
// 	function() {
// 		console.log('done');
// 	}
// );

// myAddon.registerEventCallback((type) => {
// 	console.log(type);
// });
// myAddon.dzConnect(
// 	{
// 		appId: '8',
// 		productId: 'NanoPlayer',
// 		productBuildId: '00001',
// 		userProfilePath: '/var/tmp/dzrcache_NDK_SAMPLE'
// 	},
// 	'frmZGRBG5fNJQe2kCnMYN5AxLsciGKnhuFMbTZjwGU2DjIP4Ue'
// );
