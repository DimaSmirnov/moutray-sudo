# Author: Dmitriy Smirnov <other@igus.lv>
pkgname=mountray-sudo
pkgver=0.1
pkgrel=1
pkgdesc="Mountray-sudo - tray mounter for hot-pluggable devices. Uses native udev. SUDO version."
arch=('any')
depends=('sudo' 'udev' 'libnotify')
license=("GPL")
url="https://github.com/DimaSmirnov/mountray-sudo"
source=("http://www.igus.lv/mountray/mountray-sudo.tar.xz")

build() {
  cd "$srcdir"
 make
}
package() {
  cd "$srcdir"
  install -D mountray-sudo "$pkgdir/usr/bin/mountray-sudo"
  install -d "$pkgdir/usr/share/pixmaps/mountray/"
  cd "$srcdir/icons"
  install -D * "$pkgdir/usr/share/pixmaps/mountray/"
}
