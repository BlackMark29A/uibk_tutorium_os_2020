# Tutorium 2020-06-10 (draft)

In light of the upcoming exam, I thought I'd give you some tooling to help streamline your development and especially debug cycle on the zid-gpl server.

I assume that a large number of you uses Visual Studio Code as your text editor, so I'll only cover that. If you're using different tools you're gonna have to figure out what works best for you and your tools.

The amount of time you have available for the exam is somewhat limited and you **will** run out of time if you need to set up your build environment first.
So make sure you have your tooling in-order before the exam starts.

## Visual Studio Code Remote - SSH

If you don't have this extension already [you can get it here](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-ssh). This allows you to use your local Visual Studio Code on whatever OS you're using, to develop/deploy/debug on a remote machine, like zid-gpl.

### Remote Extensions

If you connect to some remote system using this extension you will not have any extensions installed in your remote session. So you'll have to reinstall all extensions that you'll want to use and they will be installed into your home directory on the remote system.

You'll probably want at least the [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) extension and maybe [Trailing Spaces](https://marketplace.visualstudio.com/items?itemName=shardulm94.trailing-spaces).

Sadly the zid-gpl system does not have `clang-format`, not even as a module. But there is a pre-built static binary on [github](https://github.com/muttleyxd/clang-format-static-binaries/releases), which you can use. For clang-format integration see my [previous writeup](../tutorium_2020-03-30/clang-format/README.md).

## Example project

I've set up a small example project with a makefile. I've also pre-configured build tasks and a debug config. You can use this to build and debug your program through Visual Studio Code on zid-gpl.
