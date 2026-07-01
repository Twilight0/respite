# Maintainer: Twilight0 <twilight0@vivaldi.net>
pkgname=respite
pkgver=1.0.0
pkgrel=1
pkgdesc="A GTK3 media player (fork of Parole, Xfce deps removed)"
arch=('x86_64' 'i686')
url="https://github.com/Twilight0/respite"
license=('GPL-2.0-or-later')
depends=(
  'dbus-glib'
  'gstreamer'
  'gst-plugins-base'
  'gst-plugins-good'
  'gtk3'
  'taglib'
  'xapp'
)
makedepends=(
  'meson'
  'gstreamer'
  'gst-plugins-base'
  'gst-plugins-good'
  'gtk3'
  'taglib'
  'libnotify'
  'xapp'
)
optdepends=(
  'libnotify: desktop notifications plugin'
  'gst-plugins-ugly: additional codec support'
  'gst-plugins-bad: additional codec support'
  'gst-libav: ffmpeg/libav decoder support'
)
conflicts=('parole')
source=("${pkgname}-${pkgver}.tar.gz")
sha256sums=('SKIP')

build() {
  cd "${pkgname}-${pkgver}"
  meson setup build --prefix=/usr
  meson compile -C build
}

package() {
  cd "${pkgname}-${pkgver}"
  DESTDIR="$pkgdir" meson install -C build
}
