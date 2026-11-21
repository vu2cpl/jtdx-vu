// JTDX-VU: AutoSeq answers a caller missed in the two decodes before the
// latest one (QsoHistory::missed). Build as tools/qsoseq_test.cpp says.
#include <QCoreApplication>
#include <cstdio>
#include "qsohistory.h"
static int fails=0;
static void check(bool ok,const char*w){std::printf("%s  %s\n",ok?"ok  ":"FAIL",w); if(!ok)++fails;}
struct H{QsoHistory h; H(unsigned span){h.init();h.owndata("AS","VU","MK83",false);h.setAnswerSpan(span);}
 void cq(unsigned t){h.message("VU2CPL",QsoHistory::SCQ,0,"MK83","","","",t,"",812,"FT8");}
 void call(const char*c,unsigned t){h.time(t);h.message(c,QsoHistory::RCALL,0,"IO91","","EU","G",t,"-10",1500,"FT8");}
 void other(unsigned t){h.time(t);h.message("XX1XX",QsoHistory::RCQ,0,"JN00","","EU","XX",t,"-15",900,"FT8");}
 QString pick(){QString c,g,r;int rx=0,tx=0,cnt=0,pr=0;unsigned tm=128|64;QString m;h.autoseq(c,g,r,rx,tx,tm,cnt,pr,m);return c;}};
int main(int a,char**v){QCoreApplication app{a,v};unsigned t=36000;
 {H x(0);x.cq(t);x.call("G4ABC",t+15);x.cq(t+30);x.other(t+45);check(x.pick().isEmpty(),"stock (span 0): caller one decode back not answered");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.cq(t+30);x.other(t+45);check(x.pick()=="G4ABC","span: caller one decode back answered");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.cq(t+30);x.other(t+45);x.cq(t+60);x.other(t+75);check(x.pick()=="G4ABC","span: caller two decodes back answered");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.other(t+45);x.other(t+75);x.other(t+105);check(x.pick().isEmpty(),"span: three decodes back not answered");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.cq(t+30);x.call("F5NEW",t+45);check(x.pick()=="F5NEW","a caller in the latest decode wins");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.h.message("G4ABC",QsoHistory::SREPORT,0,"-10","","","",t+30,"",812,"FT8");x.other(t+45);check(x.pick().isEmpty(),"span: one we already answered is not re-picked");}
 {H x(61);x.cq(t);x.call("RA5BB",t+15);x.cq(t+30);x.h.time(t+45);x.h.message("RA5BB",QsoHistory::RCQ,0,"KO85","","EU","UA",t+45,"-18",2205,"FT8");x.cq(t+60);x.other(t+75);check(x.pick().isEmpty(),"span: caller who called CQ since is not answered (RA5BB)");}
 {H x(61);x.cq(t);x.call("G4ABC",t+15);x.call("G4ABC",t+45);x.cq(t+60);x.other(t+75);check(x.pick()=="G4ABC","span: caller who called twice, then stopped, answered");}
 return fails?1:0;}
