#pragma once
#include <stdint.h>
struct Usage {
 uint64_t uptimeMs=0,dailyMs=0,weeklyMs=0; uint32_t events=0; bool active=false;
 void advance(uint32_t dt,bool next) {
  const uint64_t day=86400000ULL,week=day*7;
  while(dt) {
   uint64_t toDay=day-uptimeMs%day;
   uint32_t step=dt<toDay?dt:(uint32_t)toDay;
   if(active){dailyMs+=step;weeklyMs+=step;}
   uptimeMs+=step;dt-=step;
   if(uptimeMs%day==0){dailyMs=0;events=0;}
   if(uptimeMs%week==0)weeklyMs=0;
  }
  if(next&&!active)++events;
  active=next;
 }
};
