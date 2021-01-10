# OSC Re-transmission Gateway

Receive OSC messages and re-transmit them to a list of predefined endpoints.

## Installation

### Binary

Debian/Ubuntu/etc:

```shell
$ ver=0.1
$ url=https://github.com/dimitry-ishenko-casparcg/retran/releases/download/v${ver}
$ wget ${url}/retran_${ver}_amd64.deb
$ sudo apt install ./retran_${ver}_amd64.deb
```

RaspberryPi:

```shell
$ ver=0.1
$ url=https://github.com/dimitry-ishenko-casparcg/retran/releases/download/v${ver}
$ wget ${url}/retran_${ver}_armhf.deb
$ sudo apt install ./retran_${ver}_armhf.deb
```

### From source

Stable version (requires [CMake](https://cmake.org/) >= 3.1):

```shell
$ ver=0.1
$ wget https://github.com/dimitry-ishenko-casparcg/retran/archive/v${ver}.tar.gz
$ tar xzf v${ver}.tar.gz
$ mkdir retran-${ver}/build
$ cd retran-${ver}/build
$ cmake ..
$ make
$ sudo make install
```

Latest master (requires[git](https://git-scm.com/) and
[CMake](https://cmake.org/) >= 3.1):

```shell
$ git https://github.com/dimitry-ishenko-casparcg/retran.git
$ mkdir retran/build
$ cd retran/build
$ cmake ..
$ make
$ sudo make install
```

## Authors

* **Dimitry Ishenko** - dimitry (dot) ishenko (at) (gee) mail (dot) com

## License

This project is distributed under the GNU GPL license. See the
[LICENSE.md](LICENSE.md) file for details.
