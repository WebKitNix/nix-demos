
var NIXOS = {
	_opener: undefined,

	registerUrlOpener: function(opener) {
		NIXOS._opener = opener;
	},

	openUrl: function(url) {
		NIXOS._opener(url);
	},

	_bookmarks: [],

	addBookmark: function(url, title, callback) {
		NIXOS._bookmarks.push({'url': url, 'title': title});

		if (callback !== undefined)
			callback(true);
	},

	getBookmarks: function(callback) {
		callback(NIXOS._bookmarks);
	}
};