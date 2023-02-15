#define GVG_DEBUG

#ifdef GVG_DEBUG
  #define debug(n) Serial.println(n)
#else
  #define debug(n) 
#endif