# weather-bot

`weather-bot` is a Telegram bot written in C, that uses the [Telebot API](https://github.com/smartnode/telebot).
`weather-bot` uses the [Open Meteo](https://open-meteo.com/) service to request weather temperature information, through the [Rust library](https://github.com/angelodlfrtr/open-meteo-rs).

Follow instructions on [Telegram Bots](https://core.telegram.org/bots) to register your API Token to use on `weather-bot`.
For using the Open Meteo service, sign up to get a free api key [here](https://geocode.maps.co/).


## Dependencies

* [Requirements](#requirements-memo)
    - [Clang compiler](#clang-compiler)
    - [CMake 3.24](#cmake-324)
    - [json-c](#json-c)
    - [Rust](#rust)
    - [cbindgen](#cbindgen)
* [HowTo](#howto)
    - [VSCode](#vscode)
    - [Building](#building)
    - [Unit tests](#unit-tests)
    - [Running](#running)

## Requirements :memo:

In order to make `weather-bot` run as needed, it is necessary to install the dependencies listed below beforehand:

### CLang compiler

Is possible to compile `weather-bot` using any C and C++ compatible compiler but this ecosystem (building and executing) were developed and tested using CLang/CLang++ compilers, so to avoid any incompatibility on trying to use this project, install any version of CLang/Clang++ compiler.

### CMake 3.24 

You may install the package on its official [page](https://cmake.org/). Higher versions are also supported.

- On **Linux**

    Ubuntu/Debian users can install the package by using the command `apt-get` on their Linux shell.

    It is also recommended to install by using the standard package manager offered by your prefered distro.

- On **MacOS**

    Mac users may obtain the package through [MacPorts](https://www.macports.org/) or [Homebrew](https://brew.sh/).

### json-c

- On **Linux**

```sh
sudo apt-get install libcurl4-openssl-dev libjson-c-dev libssl-dev cmake binutils make
```

- On **MacOS**

On Mac OSX, first install macports from [MacPorts](https://www.macports.org/install.php) and in Terminal

```sh
sudo port install cmake json-c curl
```

### Rust

There's several ways to install all Rust ecosystem stuff (compiler, cargo, cbindgen), but prefer the official way following instructions on this link of [Official Rust website](https://www.rust-lang.org/tools/install).
Using apt-get (or your distro package manager) or [MacPorts](https://www.macports.org/) and [Brew](https://brew.sh/), on MacOSX should work, but sometimes Rust version available on these package managers are very outdated, so consider installing Rust through official website.

### cbindgen

After installing Rust packages, install cbindgen generator.

```sh
cargo install --force cbindgen
```

## HowTo

Follow instructions bellow to build and execute `weather-bot`.

### VSCode

If you're using [VSCode](https://code.visualstudio.com/) as your preferred IDE, consider using the [VSCode json files](/docs/.vscodes.samples) ready made and available to launch `weather-bot` using VSCode IDE.

### Building

Building `weather-bot` dependends fully on CMake be installed on your computer.

On `weather-bot` project root path, just run the command below:

```sh
cmake -DCMAKE_BUILD_TYPE:STRING=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -DCMAKE_C_COMPILER:FILEPATH=/usr/bin/clang -DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++ -B./ -G "Unix Makefiles"
```

Followed by make:

```sh
make
```

If you prefer, there's a **cmake.sh** prepared script that provide a cmake call with options that keep compilation clean, all generated stuff will be on **weather-bot/build** directory instead sparsed on **src** and **test** directories.

```sh
./cmake.sh
cd build
make
```

If all previous dependencies are correctly installed, the compilation will be successful.

### Unit tests

```sh
make test
```

Or if you prefer, you can run the **weather_bot_tests** generated executable:

```sh
cd build/test
./weather_bot_tests
make test
```

Or if you still prefer, you could execute it directly from VSCode IDE by running or debugging using the ready made targets available from our [VSCode samples](/docs/) directory.

### Running

After building `weather-bot` project, you can run the generated **weather_bot** executable directly from **build** directory:

```sh
cd build
./weather_bot <your_open_meteo_api_key> <your_telebot_key>
make test
```

Or if you prefer, you could execute it directly from VSCode IDE by running or debugging using the ready made targets available from our [VSCode samples](/docs/.vscode.samples) directory.
