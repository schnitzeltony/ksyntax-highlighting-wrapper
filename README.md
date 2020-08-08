ksyntax-highlighting-wrapper
============================

ksyntax-highlighting-wrapper is a Q_Property / QML wrapper for KDE's [KSyntaxHighlighting](https://api.kde.org/frameworks/syntax-highlighting/html/index.html).
Adding syntax highligting to a QML-application is as easy as:

1. In your main.cpp:

```cpp
#include <ksyntaxhighlightingwrapper.h>
...
int main(int argc, char *argv[])
{
    QQmlApplicationEngine engine;
    ...
    KSyntaxHighlightingWrapper::registerQml();
    ...
    engine.load(url);
    return app.exec();
}
```
2. In your QML
```QML
    ...
    TextArea {
        id: textArea
        KSyntaxHighlighting {
            qmlTextDocument: textArea.textDocument
            definitionName: "QML"
            themeName: "Default"
        }
    }
    ...
```

An [example-application](src/example)

![Screenshot_2020-08-08_23-10-36](https://user-images.githubusercontent.com/2571823/89720635-8e2f9a80-d9d4-11ea-89cd-4640c71ed12d.jpeg)

is build optionally by adding ```-DBUILD_EXAMPLE``` to cmake's commandline.

For dependencies check [CMakeLists.txt](CMakeLists.txt) / ```find_package```.

**THE CODE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND and licensed under LGPL-3.0 License.**
