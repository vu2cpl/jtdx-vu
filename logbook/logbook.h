// This source code file was last time modified by Arvo ES1JA on January 5th, 2019
// All changes are shown in the patch file coming together with the full JTDX source code.

/*
 * From an ADIF file and cty.dat, get a call's DXCC entity and its worked before status
 * VK3ACF July 2013
 */

#ifndef LOGBOOK_H
#define LOGBOOK_H


#include <QString>
#include <QFont>

#include "countrydat.h"
#include "adif.h"

class QDir;

class LogBook
{
public:
    void init(const QString mycall,const QString mygrid,const QString mydate,
              // JTDX-VU: Club Log ADIF used as the worked-before source when set
              const QString clublogFile = "", bool clublogConfirmedOnly = false, const QString clublogSince = "",
              // JTDX-VU: LoTW users count when their last upload is newer than this
              int lotwDays = 365);
    void matchCQZ(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    void matchITUZ(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    void matchDXCC(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    // JTDX-VU: DXCC "per mode" - worked this entity on any band in the same
    // DXCC award mode group (CW / PHONE / DATA) as 'mode'
    void matchDXCCModeGroup(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeMode,
               /*in*/ const QString mode);
    void matchGrid(/*in*/ const QString gridsquare,
              /*out*/ bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    void matchPX(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    void matchCall(/*in*/ const QString call,
              /*out*/ QString &countryName,
                      bool &WorkedBefore,
                      bool &WorkedBeforeBandMode,
               /*in*/ double dialFreq=0,
                      const QString mode="");
    void getDXCC(/*in*/ const QString call,
              /*out*/ QString &countryName);
    QString lotwVersion() const {return _countries.lotwVersion();} // JTDX-VU: newest date in the LoTW CSV
    void getLOTW(/*in*/ const QString call,
              /*out*/ QString &lotw);
    void addAsWorked(const QString call, const QString band, const QString mode, const QString date, const QString gridsquare, const QString name);
    bool getData(/*in*/ const QString call,
              /*out*/ QString &gridsquare,
              /*out*/ QString &name);
    int get_qso_count(const QString mod="");
private:
   CountryDat _countries;
   ADIF _log;
};

#endif // LOGBOOK_H

