ksyntax-highlighting-wrapper
============================

ksyntax-highlighting-wrapper is a Q_Property / QML wrapper for KDE's [KSyntaxHighlighting](https://api.kde.org/frameworks/syntax-highlighting/html/index.html).

----------------------------------------------
To use this project do

1. Download it - either git clone or release tarball
2. Build it and install it to a location cmake can find it

In your project add:

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
    KSyntaxHighlightingWrapper::registerKshwQml();
    ...
    engine.load(url);
    return app.exec();
}
```
3. In QML:
```QML
...
import KSyntaxHighlighting 1.0
...
    TextArea {
        id: textArea
        KSyntaxHighlighting {
            qmlTextDocument: textArea.textDocument
            definitionName: "QML"
            themeName: "Breeze Light"
        }
    }
...
```

----------------------------------------------
An [example-application](src/example/qml/main.qml)

![Screenshot_2020-08-23_16-01-08](https://user-images.githubusercontent.com/2571823/90980154-fac0a280-e559-11ea-9054-25c9c3678083.png)

is build optionally by adding ```-DBUILD_EXAMPLE``` to cmake's commandline. To see the library in action check also
[qmlarkdown](https://github.com/schnitzeltony/qmlarkdown).

----------------------------------------------
For dependencies check [CMakeLists.txt](CMakeLists.txt) / ```find_package```.

----------------------------------------------
**THE CODE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND and licensed under LGPL-3.0 License.**
