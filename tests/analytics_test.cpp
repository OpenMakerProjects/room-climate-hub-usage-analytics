#include "../firmware/room-climate-hub-usage-analytics/analytics.h"
#include <cassert>
int main(){
 Usage u;u.advance(0,true);u.advance(1000,true);u.advance(1000,false);assert(u.dailyMs==2000&&u.events==1);
 u.advance(86400000-2000,false);assert(u.dailyMs==0&&u.weeklyMs==2000);
 u.advance(0,true);u.advance(1000,true);assert(u.events==1&&u.dailyMs==1000);
 u.advance(604800000-86400000-1000,false);assert(u.weeklyMs==0);
 Usage wrap;wrap.advance(0,true);wrap.advance(0xffffffffU,true);wrap.advance(1000,false);assert(wrap.uptimeMs==4294968295ULL);
 assert(wrap.dailyMs==wrap.uptimeMs%86400000ULL&&wrap.weeklyMs==wrap.uptimeMs%604800000ULL);
 Usage boundary;boundary.advance(0,true);boundary.advance(86400500,true);assert(boundary.dailyMs==500&&boundary.weeklyMs==86400500);
}
