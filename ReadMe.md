# PowerGrid Game Implementation

To use the project:

1. Install Visual Studio 2015
2. Download Qt (32 bit - latest version) --- [This is the link to the RC version](http://download.qt.io/development_releases/qt/5.6/5.6.0-rc/qt-opensource-windows-x86-msvc2015-5.6.0-rc.exe)
3. Install Qt into the default location
4. Open Visual Studio, go to Tools/Extensions and Updates...
5. Click on the 'Online' section
6. Search for 'qt'
7. Install the 'Qt5Package' plugin
8. After Visual Studio has been restarted, open Qt5/Qt Options, then click Add
9. Give **5.6** for version number and the path should be 'C:\Qt\Qt5.6.0\5.6\msvc2015'

NB: not required for this project, but if you plan to create Qt projects in the future, create a new Windows environment variable named **QTDIR**, which the value **C:\Qt\Qt5.6.0\5.6\msvc2015\bin**