<h1 align="center">scfetch</h1>
<h3 align="center">
system information tool written in C++ for GNU/Linux.
</h3>
![screen](https://github.com/farnsworth3010/scfetch/blob/main/screen.png)
# 💥 Features

* Nice ASCII art
* Get main info about your system
# 💾 Building and installing

```
$ git clone https://github.com/farnsworth3010/scfetch.git
$ cd scfetch
$ mkdir -p build
$ cd build
$ cmake ..
$ sudo make install
```

# 🏃 Using

```
$ scfetch
```

## Choosing distro ASCII
![screenlist](https://github.com/farnsworth3010/scfetch/blob/main/screenlist.png)

List of available distros 
```
$ scfetch list
```

Select distro
```
$ scfetch [distro]

ex:
$ scfetch "Manjaro Linux"
$ scfetch "Ubuntu"
```

# Getting help

```
$ scfetch help
```