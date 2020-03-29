# Clang-format

I assume that most of you will probably be using Visual Studio Code on Windows, so that is what I will be focusing on here.

## Installing clang-format on Windows

There are two (main) options for getting clang-format on Windows. You can either install the entire LLVM suite of tools, or you can use the Visual Studio extension, if you are using Visual Studio (that is Visual Studio, **not** Visual Studio Code).

### All of LLVM

Clang-format is part of LLVM and can be downloaded [here](https://llvm.org/builds/). There's a Windows installer that you can use that automatically adds all necessary executables to your `PATH`. This is the easier method, but does require you to update LLVM yourself.

### Visual Studio Extension

If you are using Visual Studio anyway, you might as well use the stand-alone clang-format extension and not install the whole LLVM package, especially if you only need clang-format. You can find the extension in the Visual Studio Marketplace [here](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.ClangFormat).

#### PATH

The extension does not add clang-format to your `PATH` automatically, so you'll have to do that yourself if you want to use it with Visual Studio Code.

On my system the path to the clang-format extension is `C:\Users\BlackMark\AppData\Local\Microsoft\VisualStudio\16.0_6d2aebe8\Extensions\rwgzhdwk.kwi`. This will be different for you, as part of the path is generated somewhat randomly. This path also changes sometimes when the extension is updated, so you're gonna have to update your `PATH` when this happens.

If you don't know how to add things to your `PATH`, [here's how](https://helpdeskgeek.com/windows-10/add-windows-path-environment-variable/).

## Clang-format extension for Visual Studio Code

In order to use clang-format with Visual Studio Code you will not only need the clang-format executable, but also the extension that integrates the executable into Visual Studio Code. You can find this in the Visual Studio Code Marketplace [here](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format).

If clang-format is in your `PATH` this will just work out-of-the-box. Read the _Usage_ to know which keyboard shortcut you need to format the code.

## Linux

If you are using Linux your distributions package manager will very likely provide clang-format. Install it and you should be good-to-go. You will of course also need the Visual Studio Code Extension linked above.

It might happen that your clang-format executable has a version appended to the name. You can either create a symlink or manually specify the path in your `settings.json` of Visual Studio Code. Read the documentation of the extension on how to do that.

## Other editors

If you are using some other editor, it is very likely that there is an extension/plugin that provides clang-format integration, or the editor might even have built-in support. Google it and you will find something.

## Using clang-format

So now that you have clang-format installed and integrated into your editor, how do you use it?
You need a `.clang-format` file that specifies how your code should be formatted. The PS github repository provides one for you [here](https://github.com/uibk-dps-teaching/ps_os_2020/blob/master/.clang-format). Copy it into the same folder as your source file and clang-format will use it to format your code.

If you are not happy with the way this clang-format config files formats your code, feel free to change and adapt it to suit your needs and preferences.
