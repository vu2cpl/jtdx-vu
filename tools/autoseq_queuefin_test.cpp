// JTDX-VU: a station queued by Halt and queue is called again when he sends
// RR73/73 to someone else (RFIN), with the RR73/73 marker off as well (the
// display now feeds his RFIN for a queued station). Build as tools/qsoseq_test.cpp says.
#include <QCoreApplication>
#include <cstdio>
#include "qsohistory.h"
static int fails=0;
static void check(bool ok,const char*w){std::printf("%s  %s\n",ok?"ok  ":"FAIL",w); if(!ok)++fails;}
struct H{QsoHistory h; H(){h.init();h.owndata("AS","VU","MK83",false);}
 void hiscq(unsigned t){h.time(t);h.message("ZF2OO",QsoHistory::RCQ,0,"EK99","","NA","ZF",t,"-12",2088,"FT8");}
 void rep(unsigned t){h.message("ZF2OO",QsoHistory::SREPORT,0,"-12","","","",t,"",812,"FT8");}
 void fin(unsigned t){h.time(t);h.message("ZF2OO",QsoHistory::RFIN,0,"","","NA","ZF",t,"-13",2088,"FT8");}
 void other(unsigned t){h.time(t);h.message("XX1XX",QsoHistory::RCQ,0,"JN00","","EU","XX",t,"-15",900,"FT8");}
 QString pick(){QString c,g,r;int rx=0,tx=0,cnt=0,pr=0;unsigned tm=128|64;QString m;h.autoseq(c,g,r,rx,tx,tm,cnt,pr,m);return c;}};
int main(int a,char**v){QCoreApplication app{a,v};unsigned t=63500;
 {H x;x.hiscq(t);x.rep(t+15);x.rep(t+45);x.rep(t+75);x.h.setQueue({"ZF2OO"});x.fin(t+120);check(x.pick()=="ZF2OO","queued (halted after our report): his RR73 to another station calls him");}
 {H x;x.hiscq(t);x.rep(t+15);x.rep(t+45);x.h.setQueue({"ZF2OO"});x.fin(t+120);x.other(t+150);check(x.pick().isEmpty(),"his RR73 one decode back: not picked late (queue picks on the latest decode)");}
 {H x;x.hiscq(t);x.rep(t+15);x.rep(t+45);x.fin(t+120);check(x.pick().isEmpty(),"not queued: his RR73 to another station is not answered");}
 {H x;x.h.setQueue({"ZF2OO"});x.fin(t);check(x.pick()=="ZF2OO","queued, never worked: his RR73 calls him");}
 return fails?1:0;}
