pkgname=dwm-flexipatch
pkgver=6.4
pkgrel=1
pkgdesc="A dynamic window manager for X"
url="http://dwm.suckless.org"
arch=('i686' 'x86_64')
license=('MIT')
options=(zipman)
depends=('libx11' 'libxinerama' 'imlib2')
conflicts=('dwm')
provides=('dwm')
makedepends=('git' 'libxft')

prepare() {
  cd ..
  make clean
  rm -f config.h
}

build() {
  cd ..
  make X11INC=/usr/include/X11 X11LIB=/usr/lib/X11
}

package() {
  cd ..
  make PREFIX=/usr DESTDIR="$pkgdir" install
  install -m644 -D LICENSE "$pkgdir/usr/share/licenses/$pkgname"/LICENSE
  install -m644 -D README "$pkgdir/usr/share/doc/$pkgname"/README
}
