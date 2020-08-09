ksyntax-highlighting-wrapper
============================

ksyntax-highlighting-wrapper is a Q_Property / QML wrapper for KDE's [KSyntaxHighlighting](https://api.kde.org/frameworks/syntax-highlighting/html/index.html).

----------------------------------------------
Using this libray to have syntax-highligting in your QML-project is just a matter of adding few lines::

1. In CMakeList.txt:
```cmake
...
find_package(KSyntaxHighlightingWrapper REQUIRED)
...
target_link_libraries(<your-target>
...
        KSyntaxHighlightingWrapper::KSyntaxHighlightingWrapper
...
)

```

2. In main.cpp:

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
3. In QML:
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

----------------------------------------------
An [example-application](src/example/main.qml)

![Screenshot_2020-08-08_23-10-36](https://user-images.githubusercontent.com/2571823/89720635-8e2f9a80-d9d4-11ea-89cd-4640c71ed12d.jpeg)

is build optionally by adding ```-DBUILD_EXAMPLE``` to cmake's commandline. To see the library in action check also
[qmlarkdown](https://github.com/schnitzeltony/qmlarkdown).

----------------------------------------------
For dependencies check [CMakeLists.txt](CMakeLists.txt) / ```find_package```.

----------------------------------------------
**THE CODE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND and licensed under LGPL-3.0 License.**
