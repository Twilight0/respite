# Maintainer: Twilight0 <twilight0@vivaldi.net>
pkgname=respite
pkgver=1.0.0
pkgrel=7
pkgdesc="A GTK3 media player (fork of Parole, Xfce deps removed)"
arch=('x86_64' 'i686')
url="https://github.com/Twilight0/respite"
license=('GPL-2.0-or-later')
depends=(
  'dbus-glib'
  'gstreamer'
  'gst-plugins-base'
  'gst-plugins-good'
  'gst-plugins-ugly'
  'gtk3'
  'taglib'
  'xapp'
)
makedepends=(
  'glib2'
  'meson'
  'gstreamer'
  'gst-plugins-base'
  'gst-plugins-good'
  'gtk3'
  'taglib'
  'libnotify'
  'python'
  'xapp'
)
optdepends=(
  'libnotify: desktop notifications plugin'
  'gst-plugins-ugly: additional codec support'
  'gst-plugins-bad: additional codec support'
  'gst-libav: ffmpeg/libav decoder support'
  'xapp: tray icon support (Cinnamon/MATE/Xfce)'
)
source=("${pkgname}-${pkgver}.tar.gz")
sha256sums=('SKIP')

prepare() {
  cd "${pkgname}-${pkgver}"
}

build() {
  cd "${pkgname}-${pkgver}"
  meson setup build --prefix=/usr
  meson compile -C build
}

package() {
  cd "${pkgname}-${pkgver}"
  DESTDIR="$pkgdir" meson install -C build
}
