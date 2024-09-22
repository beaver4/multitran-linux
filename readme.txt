Альфа-версия программы Мультитран для Linux. Выкладывалась в 2005 году на multitran.ru, содержит патчи от разработчиков дистрибутива Alt Linux.
Программа пойдет на любом 64-битном дистрибутиве Linux, на котором устновлена библиотека libc 2.27 или новее (почти любой современный). Других зависимостей у программы нет.


Установка

Распакуйте архив где вам удобно (например в /opt/multitran)
Запустить можно двойным кликом по файлу AppRun.

Чтобы создать ярлык в главном меню 
1. Отредактируйте файл /opt/multitran/usr/share/applications/qmtcc.desktop так 
Exec=/opt/multitran/usr/bin/qmtcc
Icon=/opt/multitran/usr/share/icons/hicolor/512x512/apps/mt-icon.png
где /opt/multitran - путь, куда вы распаковали программу.
2. Положите этот файл в ~/.local/share/applications


Сборка из исходников (для программистов)

Чтобы собрать Мультитран, сначала соберите из исходников Qt3.

cd libraries/
export libdir=/usr/lib
export includedir=/usr/include
# д. б. настроен qtchooser
export QT_SELECT=qt3
cd libmtsupport/
make
sudo make install
cd ..
cd libbtree/
make
sudo make install
cd ..
cd libfacet/
make
sudo make install
cd ..
cd libmtquery/
make
sudo make install
cd ..
cd ../qmtcc/
export QTDIR=/opt/qt/3.3.8
qmake
cd src/
qmake
cd ..
make
make install INSTALL_ROOT=/path/to/multitran-build/AppDir
cd ../..
./linuxdeploy-x86_64.AppImage --appdir AppDir -d alt/qmtcc.desktop -i mt-icon.png
cd multitran-data/
# д. б. убран DESTDIR
DESTDIR=/path/to/multitran-build/AppDir make install