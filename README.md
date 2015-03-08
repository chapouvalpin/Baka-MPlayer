# [Kuro Player](http://kuro-player.u8sand.net)

## Overview

Kuro Player is a free and open source, cross-platform, **libmpv** based multimedia player.
Its simple design reflects the idea for an uncluttered, simple, and enjoyable environment for watching tv shows.

## Requirements

* gcc
* pkg-config
* libmpv-dev
* qtbase5-dev (>= 5.2.0)
  * qt5-qmake
  * qttools5-dev-tools
  * qtdeclarative5-dev
  * libqt5svg5-dev
  * libqt5x11extras5-dev
  * libqt5network5
* youtube-dl (optional, for streaming youtube videos)

Note: Packages may be named slightly different for each distro

## Compilation

### Windows

These instructions are for cross-compiling for Windows on a Linux system. (Note: the architecture can be either `x86_64` or `i686` depending on which platform you're compiling for)

	git clone -b release https://github.com/u8sand/Kuro-Player.git
	cd "Kuro-Player"
	mkdir build
	cp -r etc/sbin/windows/cross-compilation/{kuro-build.sh,src} build/
	cd build
	arch=x86_64
	./kuro-build.sh $arch

This is a very long process because you'll need to build the mingw32 toolchain `mxe` and all dependent libraries, `libmpv.a`, and finally `kuro-player.exe`. If everything succeeded without error, you'll get `Kuro-Player.$arch.zip` which should contain everything you need.

To rebuild simply delete the directory (in build) or the .zip file of what you need to rebuild and re-run `./kuro-build.sh $arch`

To add custom patches, put them in `src/patches/` prefixed with the name of what you're patching.

### Linux

If your distribution does not provide a package, you can compile it from source.

We've made scripts for some of the distributions... See `etc/sbin/linux/`

	git clone -b release https://github.com/u8sand/Kuro-Player.git
	cd "Kuro-Player"
	mkdir build
	cp -r etc/sbin/linux/* build/
	cd build
	distro=debian_based
	./$distro.sh

If this doesn't work or you do not have a distro listed here, you're on your own. You'll need to build mpv and then Kuro-Player, (the dependencies above are Kuro-Player). For help building mpv see `https://github.com/mpv-player/mpv-build`. Building and making Kuro-Player from source can be done like so:

	git clone -b release https://github.com/u8sand/Kuro-Player.git
	cd "Kuro-Player"
	./configure
	make -j `grep -c ^processor /proc/cpuinfo`
	sudo make install

The configuration file will be created on first run and will be written to `~/.config/kuro-player.ini`.

### Other Languages

By default, Kuro Player will compile in English if no language is specified during compilation. To compile a multi-lingual version of kuro-player, configure it like so:

	./configure CONFIG+=install_translations

For more configuration options see the `configure` source file or read the manual.

You can check out which languages we currently support by checking out `Kuro-Player/src/translations/`.

## Bug reports

Please use the [issues tracker](https://github.com/u8sand/Kuro-Player/issues) provided by GitHub to send us bug reports or feature requests.

## Contact

**IRC Channel**: `#kuro-player` on `irc.freenode.net`

You can ask us questions about using Kuro Player, give feedback, or discuss its development.
However, if possible, please avoid posting bugs there and use the [issue tracker](https://github.com/u8sand/Kuro-Player/issues) instead.
