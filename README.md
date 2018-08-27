# Geekofall

~~Release the kraken!~~ Drop the Geeko!

![geekofall](https://user-images.githubusercontent.com/1793699/44660940-6a523480-aa09-11e8-89d3-cfc1a5349031.gif)

# Building from source

1. `zypper in sfml2-devel libBox2D-devel libX11-devel`
2. `DESTDIR=/usr/share/geekofall/ make`
3. There's no `make install`! So:
    * Copy `geekofall` to `/usr/lib64/xscreensaver/`
    * Copy `geeko.png` and `geeko.json` to `/usr/share/geekofall/`
4. Add `geekofall -root \n\` to `~/.xscreensaver`
5. Choose the screensaver in XScreensaver settings and you are ready to drop some Geekos!
