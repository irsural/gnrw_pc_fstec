#-------------------------------------------------
#
# Project created by QtCreator 2015-11-05T11:56:02
#
#-------------------------------------------------

DEFINES += IRS_LIB_IRSMBUS_DEBUG_TYPE=IRS_LIB_DEBUG_NONE

QT       += core gui network

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.6
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VERSION_STR=\"$${VERSTR}\"

DEFINES += GNRWPC_FSB

!contains(DEFINES, GNRWPC_FSB) {
  # Обработка кнопок Включить и Выключить (включается уровень самого большего
  # времени наработки) для ФСТЭК для старого варианта прошивки
  #DEFINES += GNRWPC_POWER_ONOFF_FSTEC_OLD
}

QMAKE_TARGET_COMPANY = ООО \"\"РЭС\"\"
contains(DEFINES, GNRWPC_FSB) {
  QMAKE_TARGET_PRODUCT = Покров
} else {
  QMAKE_TARGET_PRODUCT = Покров ПЭМИН
}
QMAKE_TARGET_DESCRIPTION = $${QMAKE_TARGET_PRODUCT}
QMAKE_TARGET_COPYRIGHT = $${QMAKE_TARGET_COMPANY}

PROGNAME_STR_1 = '\\"$${QMAKE_TARGET_PRODUCT}\\"'
DEFINES += PROGNAME_STR=\"$${PROGNAME_STR_1}\"

RC_ICONS = irs.ico

TARGET = gnrw_pc_fstec
TEMPLATE = app

INCLUDEPATH += .
INCLUDEPATH += ../mxsrclib/
INCLUDEPATH += ../mxsrclib/arch/qt/



win32-g++ |linux-g++{
  QMAKE_CXXFLAGS += -finput-charset=CP1251
  QMAKE_CXXFLAGS += -fno-strict-aliasing
  QMAKE_CXXFLAGS += -Wno-deprecated
  QMAKE_CXXFLAGS += -Wno-unused-parameter
  QMAKE_CXXFLAGS += -Wextra
  QMAKE_CXXFLAGS += -Woverloaded-virtual
  QMAKE_CXXFLAGS += -Wno-unused-local-typedefs
  QMAKE_CXXFLAGS += -Wno-unused-function
  QMAKE_CXXFLAGS += -std=c++11
  #QMAKE_CXXFLAGS += -Wno-enum-compare
}

win32:LIBS += -lws2_32


SOURCES += main.cpp\
    detect_devices_dialog.cpp \
        mainwindow.cpp \
    ../mxsrclib/correct_alg.cpp \
    ../mxsrclib/csvwork.cpp \
    ../mxsrclib/dbgprn.cpp \
    ../mxsrclib/hardflowg.cpp \
    ../mxsrclib/irs_chart_data.cpp \
    ../mxsrclib/irsadc.cpp \
    ../mxsrclib/irsalg.cpp \
    ../mxsrclib/irscalc.cpp \
    ../mxsrclib/irscpu.cpp \
    ../mxsrclib/irsdbgutil.cpp \
    ../mxsrclib/irsdcp.cpp \
    ../mxsrclib/irsdev.cpp \
    ../mxsrclib/irsdevices.cpp \
    ../mxsrclib/irsdsp.cpp \
    ../mxsrclib/irserror.cpp \
    ../mxsrclib/irsexcept.cpp \
    ../mxsrclib/irsfilter.cpp \
    ../mxsrclib/irsfunnel.cpp \
    ../mxsrclib/irsgpio.cpp \
    ../mxsrclib/irshfftp.cpp \
    ../mxsrclib/irsinit.cpp \
    ../mxsrclib/irsint.cpp \
    ../mxsrclib/irskbd.cpp \
    ../mxsrclib/irslocale.cpp \
    ../mxsrclib/irslwip.cpp \
    ../mxsrclib/irsmath.cpp \
    ../mxsrclib/irsmatrix.cpp \
    ../mxsrclib/irsmbus.cpp \
    ../mxsrclib/irsmcutil.cpp \
    ../mxsrclib/irsmem.cpp \
    ../mxsrclib/irsmenu.cpp \
    ../mxsrclib/irsprop.cpp \
    ../mxsrclib/irsstd.cpp \
    ../mxsrclib/irsstring.cpp \
    ../mxsrclib/irsstrm.cpp \
    ../mxsrclib/irssysutils.cpp \
    ../mxsrclib/irstcpip.cpp \
    ../mxsrclib/irstime.cpp \
    ../mxsrclib/irsvariant.cpp \
    ../mxsrclib/measmul.cpp \
    ../mxsrclib/meassup.cpp \
    ../mxsrclib/mxdata.cpp \
    ../mxsrclib/mxextbase.cpp \
    ../mxsrclib/mxextchart.cpp \
    ../mxsrclib/mxifar.cpp \
    ../mxsrclib/mxnet.cpp \
    ../mxsrclib/mxnetc.cpp \
    ../mxsrclib/mxnetr.cpp \
    ../mxsrclib/niusbgpib.cpp \
    ../mxsrclib/niusbgpib_hardflow.cpp \
    ../mxsrclib/timer.cpp \
    ../mxsrclib/arch/qt/counter.cpp \
    ../mxsrclib/arch/qt/mxifa.cpp \
    settings.cpp \
    settingsdialog.cpp \
    setdeviceipdialog.cpp \
    gnrw.cpp

HEADERS  += mainwindow.h \
    ../mxsrclib/correct_alg.h \
    ../mxsrclib/csvwork.h \
    ../mxsrclib/dbgprn.h \
    ../mxsrclib/hardflowg.h \
    ../mxsrclib/irs_chart_data.h \
    ../mxsrclib/irs_codecvt.h \
    ../mxsrclib/irsadc.h \
    ../mxsrclib/irsadcabs.h \
    ../mxsrclib/irsalg.h \
    ../mxsrclib/irscalc.h \
    ../mxsrclib/irschartwin.h \
    ../mxsrclib/irsconfig.h \
    ../mxsrclib/irsconfig0.h \
    ../mxsrclib/irsconsole.h \
    ../mxsrclib/irsconsolestd.h \
    ../mxsrclib/irscpp.h \
    ../mxsrclib/irscpu.h \
    ../mxsrclib/irsdacabs.h \
    ../mxsrclib/irsdbgutil.h \
    ../mxsrclib/irsdcp.h \
    ../mxsrclib/irsdefs.h \
    ../mxsrclib/irsdefsbase.h \
    ../mxsrclib/irsdev.h \
    ../mxsrclib/irsdevices.h \
    ../mxsrclib/irsdsp.h \
    ../mxsrclib/irserror.h \
    ../mxsrclib/irsexcept.h \
    ../mxsrclib/irsfilter.h \
    ../mxsrclib/irsfinal.h \
    ../mxsrclib/irsfunnel.h \
    ../mxsrclib/irsgpio.h \
    ../mxsrclib/irshfftp.h \
    ../mxsrclib/irsinit.h \
    ../mxsrclib/irsint.h \
    ../mxsrclib/irskbd.h \
    ../mxsrclib/irslimits.h \
    ../mxsrclib/irslocale.h \
    ../mxsrclib/irslwip.h \
    ../mxsrclib/irsmath.h \
    ../mxsrclib/irsmatrix.h \
    ../mxsrclib/irsmbus.h \
    ../mxsrclib/irsmcutil.h \
    ../mxsrclib/irsmem.h \
    ../mxsrclib/irsmenu.h \
    ../mxsrclib/irsnetdefs.h \
    ../mxsrclib/irsparamabs.h \
    ../mxsrclib/irspch.h \
    ../mxsrclib/irsprop.h \
    ../mxsrclib/irsrect.h \
    ../mxsrclib/irsspi.h \
    ../mxsrclib/irsstd.h \
    ../mxsrclib/irsstrconv.h \
    ../mxsrclib/irsstrconvbase.h \
    ../mxsrclib/irsstrdefs.h \
    ../mxsrclib/irsstring.h \
    ../mxsrclib/irsstrm.h \
    ../mxsrclib/irsstrmstd.h \
    ../mxsrclib/irssysutils.h \
    ../mxsrclib/irstable.h \
    ../mxsrclib/irstcpip.h \
    ../mxsrclib/irstest.h \
    ../mxsrclib/irstime.h \
    ../mxsrclib/irstree.h \
    ../mxsrclib/irsvariant.h \
    ../mxsrclib/lwipopts.h \
    ../mxsrclib/measdef.h \
    ../mxsrclib/measmul.h \
    ../mxsrclib/meassup.h \
    ../mxsrclib/mxdata.h \
    ../mxsrclib/mxdatastd.h \
    ../mxsrclib/mxextbase.h \
    ../mxsrclib/mxextchart.h \
    ../mxsrclib/mxifa.h \
    ../mxsrclib/mxifar.h \
    ../mxsrclib/mxnet.h \
    ../mxsrclib/mxnetc.h \
    ../mxsrclib/mxnetd.h \
    ../mxsrclib/mxnetr.h \
    ../mxsrclib/niusbgpib.h \
    ../mxsrclib/niusbgpib_hardflow.h \
    ../mxsrclib/timer.h \
    ../mxsrclib/tstlan4abs.h \
    ../mxsrclib/arch/qt/counter.h \
    ../mxsrclib/arch/qt/Decl-32.h \
    ../mxsrclib/arch/qt/irsdefsarch.h \
    ../mxsrclib/arch/qt/irsqtstrconv.h \
    ../mxsrclib/arch/qt/mxifal.h \
    ../mxsrclib/arch/qt/mxnetda.h \
    ../mxsrclib/arch/qt/ni488.h \
    detect_devices_dialog.h \
    settings.h \
    settingsdialog.h \
    setdeviceipdialog.h \
    gnrw.h

FORMS    += mainwindow.ui \
    detect_devices_dialog.ui \
    settingsdialog.ui \
    setdeviceipdialog.ui

DISTFILES += \
    ../mxsrclib/arch/qt/qt info.txt

RESOURCES += \
    res.qrc
