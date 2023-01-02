Name: qmtcc
Version: 0.0.1
Release: alt1

Summary: Qt based graphical frontend to multitran dictionary
License: GPL
Group: System/Internationalization

Source: %name-%{version}alpha1.tar.bz2

# Automatically added by buildreq on Tue Jan 25 2005
BuildRequires: fontconfig freetype2 gcc-c++ libbtree-devel libfacet-devel libmtquery-devel libmtsupport-devel libqt3-devel libstdc++-devel xorg-x11-devel xorg-x11-libs

%description
Qt based graphical frontend to multitran dictionary

%prep
%setup -q -n %name-%{version}alpha1

%build
export PATH=$PATH:/usr/lib/qt3/bin
qmake
%make_build


%install
%makeinstall INSTALL_ROOT=%buildroot


%files
%doc ChangeLog
%_bindir/*


%changelog
* Tue Jan 25 2005 Stanislav Ievlev <inger@altlinux.org> 0.0.1-alt1
- initial build
