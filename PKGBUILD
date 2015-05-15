# Maintainer: Bernd Busse <bernd AT bussenet DOT de>

pkgname=arrowline-git
_pkgname=arrowline
pkgver=v0.4.7e24433
pkgrel=1
pkgdesc=""
arch=('i686' 'x86_64')
url="https://github.com/tryone144/arrowline"
license=('MIT')
depends=()
makedepends=('git')
source=('git://github.com/tryone144/arrowline.git')
md5sums=('SKIP')

pkgver() {
  cd "$_pkgname"
  git describe --always | sed 's|-|.|g'
}

build() {
  cd "$_pkgname"
  make
}

package() {
  cd "$_pkgname"
  make PREFIX="$pkgdir/usr" install
  install -Dm644 LICENSE "$pkgdir/usr/share/licenses/$_pkgname/LICENSE"
}

# vim:set ts=2 sw=2 et:
