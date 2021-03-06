//////////////////////////////////////////////////////////////////////////////
// Arduino Uno WIFI WeMOS D1 R2
// Product : Oriphase Water Cycler For Pets
// Version 1.0.0
// Author : Foxman, Vladimir Lin, Feng Yun Lin, FYL
// 寵物飲水機
//////////////////////////////////////////////////////////////////////////////
#include "EEPROM.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
#include "FS.h"
//////////////////////////////////////////////////////////////////////////////
// AITK For Arduino WeMOS
//////////////////////////////////////////////////////////////////////////////
#include <AitkUnoGPIOs.h>
#include <AitkWeMosEEPROM.h>
#include <AitkWeMosWiFi.h>
#include <AitkWeMosPowerController.h>
//////////////////////////////////////////////////////////////////////////////
// 系統參數
//////////////////////////////////////////////////////////////////////////////
bool               Console           = true                                  ;
bool               UseWiFi           = true                                  ;
int                Debug             = 1                                     ;
int                BaudRate          = 115200                                ;
int                EepromSize        = 4096                                  ;
bool               doDelay           = true                                  ;
unsigned int       MicrosecondsDelay = 10000                                 ;
char * VersionMessage = "Oriphase Water Cycler For Pets : Version 2021.03.06.21.31" ;
//////////////////////////////////////////////////////////////////////////////
// 電源控制器掃描控制點
//////////////////////////////////////////////////////////////////////////////
int                PowerProbedMillis = 0                                     ;
int                PowerSwitchMode   = 0                                     ;
int                PumpIO            = 2                                     ;
int                SwitchIO          = 4                                     ;
int                PumpPower         = 0                                     ;
int                PumpSwitch        = 0                                     ;
//////////////////////////////////////////////////////////////////////////////
bool               DetectWater       = true                                  ;
int                WaterLevel        = 6000                                  ;
int                WaterBasement     = 4000                                  ;
int                HighestWaterLevel = 6600                                  ;
int                LowestWaterLevel  = 4400                                  ;
//////////////////////////////////////////////////////////////////////////////
bool               RewriteEEPROM     = false                                 ;
//////////////////////////////////////////////////////////////////////////////
// WIFI設定
//////////////////////////////////////////////////////////////////////////////
int                WifiMode          = 3                                     ;
bool               WifiConnected     = false                                 ;
int                WifiTimestamp     = 0                                     ;
char             * WifiSSID          = "Actions 7f"                          ;
char             * WifiPassword      = "0912388888"                          ;
char             * OriphaseArduino   = "Oriphase"                            ;
char             * ArduinoSite       = "Oriphase Water Cycler"               ;
char             * SapPassword       = ""                                    ;
char             * SiteUsername      = "admin"                               ;
char             * SitePassword      = "123456789"                           ;
bool               AssignIP          = false                                 ;
char               HostIP      [ 4 ]                                         ;
char               HostGateway [ 4 ]                                         ;
char               HostNetmask [ 4 ]                                         ;
int                WifiHttpPort      = 80                                    ;
bool               HttpInitialized   = false                                 ;
//////////////////////////////////////////////////////////////////////////////
PowerController  * PC                                                        ;
bool               SolenoidValve     = false                                 ;
int                ValveIO           = 3                                     ;
int                ValveSwitch       = 6                                     ;
int                ValveStatus       = 0                                     ;
bool               CareValve         = false                                 ;
ESP8266WebServer * HttpServer        = nullptr                               ;
//////////////////////////////////////////////////////////////////////////////
String StatusToJson          ( void                                        ) {
  ////////////////////////////////////////////////////////////////////////////
  String JSON                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  JSON    = String           ( "{ "                                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  // PumpPower
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( "\"Pump\""                                  ) ;
  JSON   += String           ( " : "                                       ) ;
  if                         ( PumpPower                                   ) {
    JSON += String           ( "1"                                         ) ;
  } else                                                                     {
    JSON += String           ( "0"                                         ) ;
  }                                                                          ;
  JSON   += String           ( " , "                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // SolenoidValve
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( "\"Valve\""                                 ) ;
  JSON   += String           ( " : "                                       ) ;
  if                         ( SolenoidValve                               ) {
    JSON += String           ( "1"                                         ) ;
  } else                                                                     {
    JSON += String           ( "0"                                         ) ;
  }                                                                          ;
  JSON   += String           ( " , "                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // DetectWater
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( "\"DetectWater\""                           ) ;
  JSON   += String           ( " : "                                       ) ;
  if                         ( DetectWater                                 ) {
    JSON += String           ( "1"                                         ) ;
  } else                                                                     {
    JSON += String           ( "0"                                         ) ;
  }                                                                          ;
  JSON   += String           ( " , "                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // WaterLevel
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( "\"WaterLevel\""                            ) ;
  JSON   += String           ( " : "                                       ) ;
  JSON   += String           ( WaterLevel                                  ) ;
  JSON   += String           ( " , "                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // Water Basement
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( "\"Basement\""                              ) ;
  JSON   += String           ( " : "                                       ) ;
  JSON   += String           ( WaterBasement                               ) ;
  JSON   += String           ( " , "                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // Millis
  ////////////////////////////////////////////////////////////////////////////
  int dtm = millis           (                                             ) ;
  JSON   += String           ( "\"Millis\""                                ) ;
  JSON   += String           ( " : "                                       ) ;
  JSON   += String           ( dtm                                         ) ;
  ////////////////////////////////////////////////////////////////////////////
  JSON   += String           ( " }"                                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  return JSON                                                                ;
}
//////////////////////////////////////////////////////////////////////////////
// 電磁水閥開關
//////////////////////////////////////////////////////////////////////////////
void TurnSolenoidValve       ( bool onOff                                  ) {
  ////////////////////////////////////////////////////////////////////////////
  if                         ( onOff                                       ) {
    if                       ( ! SolenoidValve                             ) {
      SolenoidValve = true                                                   ;
      WriteValue             (  ValveIO , 0                                ) ;
    }
  } else                                                                     {
    if                       (   SolenoidValve                             ) {
      SolenoidValve = false                                                  ;
      WriteValue             (  ValveIO , 1                                ) ;
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 切換電磁水閥
//////////////////////////////////////////////////////////////////////////////
void ChangeSolenoidValve     ( void                                        ) {
  ////////////////////////////////////////////////////////////////////////////
  if                         ( SolenoidValve                               ) {
    TurnSolenoidValve        ( false                                       ) ;
    UpdateValveStatus        (                                             ) ;
  } else                                                                     {
    TurnSolenoidValve        ( true                                        ) ;
    UpdateValveStatus        (                                             ) ;
  }
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 目前水位
//////////////////////////////////////////////////////////////////////////////
int CurrentWaterLevel       ( void                                         ) {
  ////////////////////////////////////////////////////////////////////////////
  int a0 = ReadAnalog       ( 0                                            ) ;
  WaterLevel =              ( a0 * 4000 / 670 ) + WaterBasement              ;
  return WaterLevel                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 更新水閥及水位狀態
//////////////////////////////////////////////////////////////////////////////
void UpdateValveStatus      ( void                                         ) {
}
//////////////////////////////////////////////////////////////////////////////
// 更新抽水機狀態
//////////////////////////////////////////////////////////////////////////////
void UpdatePumpStatus       ( void                                         ) {
  bool pc = PC -> Power     (                                              ) ;
  bool in = PC -> InputNow  (                                              ) ;
}
//////////////////////////////////////////////////////////////////////////////
// 建立IP Address封包
//////////////////////////////////////////////////////////////////////////////
IPAddress ComposeIP ( char * ip )                                            {
  return IPAddress ( ip [ 0 ] , ip [ 1 ] , ip [ 2 ] , ip [ 3 ] )             ;
}
//////////////////////////////////////////////////////////////////////////////
// 主控制網頁
//////////////////////////////////////////////////////////////////////////////
void WebServerEntry ( )                                                      {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  IPAddress SIP = WiFi . softAPIP ( )                                        ;
  ////////////////////////////////////////////////////////////////////////////
  String WL      ( WaterLevel                                              ) ;
  String CM   = WL . substring ( 0 , 1                                     ) ;
  String TAIL = WL . substring ( 1                                         ) ;
  String WLS  = CM + String ( "." ) + TAIL                                   ;
  ////////////////////////////////////////////////////////////////////////////
  String HTML                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  HTML  = String ( "<!DOCTYPE html>"                                       ) ;
  HTML += String ( "<html>"                                                ) ;
  HTML += String ( "<head>"                                                ) ;
  HTML += String ( "</head>"                                               ) ;
  HTML += String ( "<body>"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( VersionMessage                                          ) ;
  HTML += String ( "</div>\n"                                              ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Soft Access Point : </span><span>"          ) ;
  HTML += String ( SIP [ 0 ] ) + String ( "." )                              +
          String ( SIP [ 1 ] ) + String ( "." )                              +
          String ( SIP [ 2 ] ) + String ( "." )                              +
          String ( SIP [ 3 ] )                                               ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Current Water Level : </span><span>"        ) ;
  HTML += String ( WLS                                                     ) ;
  HTML += String ( "</span>&nbsp;<span>cm</span></div>\n"                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Pump : </span><span>"                       ) ;
  if             ( PumpPower                                               ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Valve : </span><span>"                      ) ;
  if             ( SolenoidValve                                           ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Detect Water Level : </span><span>"         ) ;
  if             ( DetectWater                                             ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += "<br>\n"                                                           ;
  HTML += String ( "<div><label>Menu</label></div>"                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<ul>"                                                  ) ;
  HTML += String ( "<li><a href='/wifi.html'>WiFi</a></li>\n"              ) ;
  HTML += String ( "<li><a href='/control.html'>Control</a></li>\n"        ) ;
  HTML += String ( "<li><a href='/settings.html'>Settings</a></li>\n"      ) ;
  HTML += String ( "</ul>\n"                                               ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  int dtm = millis (                                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>System Millis : </span><span>"              ) ;
  HTML += String ( dtm                                                     ) ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</body>"                                               ) ;
  HTML += String ( "</html>"                                               ) ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send ( 200 , "text/html" , HTML                            ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerWIFI ( )                                                       {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  String HTML                                                                ;
  String SSID                                                                ;
  String LINE                                                                ;
  String CurrentSSID ( WifiSSID )                                            ;
  int    totalNetworks                                                       ;
  bool   encrypt                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  HTML  = String ( "<!DOCTYPE html>"                                       ) ;
  HTML += String ( "<html>"                                                ) ;
  HTML += String ( "<head>"                                                ) ;
  HTML += String ( "</head>"                                               ) ;
  HTML += String ( "<body>"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( VersionMessage                                          ) ;
  HTML += String ( "</div>\n"                                              ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Use WiFi Controller : </span><span>"        ) ;
  if             ( UseWiFi                                                 ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=WIFI&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=WIFI&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Configure WiFi Access Point Site Name and Password</div><br>\n" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='SiteNameForm'>" ) ;
  HTML += String ( "<input type='hidden' id='Method' name='Method' value='Settings'>" ) ;
  HTML += String ( "<input type='hidden' id='Item' name='Item' value='Site'>" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<label for='SiteName'>Site : </label>\n"               ) ;
  HTML += String ( "<input type='text' id='SiteName' name='Name' value='"  ) ;
  HTML += String ( ArduinoSite                                             ) ;
  HTML += String ( "' />"                                                  ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<label for='PasswordSap'>Soft Access Point Password : </label>\n" ) ;
  HTML += String ( "<input type='text' id='PasswordSap' name='SapPassword' value='"  ) ;
  HTML += String ( SapPassword                                             ) ;
  HTML += String ( "' />"                                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='SiteNameForm'>"            ) ;
  HTML += String ( "Setup Wi-Fi Access Point Site Name and Password</button>" ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Configure WiFi Mode</div><br>\n"                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='WiFiModeForm'>" ) ;
  HTML += String ( "<input type='hidden' id='Method' name='Method' value='Settings'>" ) ;
  HTML += String ( "<input type='hidden' id='Item' name='Item' value='WiFiMode'>" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='1'"          ) ;
  if                ( WifiMode == 1                                        ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Soft Access Point</span><br>\n"            ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='2'"          ) ;
  if                ( WifiMode == 2                                        ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Station</span><br>\n"                      ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='3'"          ) ;
  if                ( WifiMode == 3                                        ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Soft Access Point and Station</span><br>\n" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='WiFiModeForm'>"            ) ;
  HTML += String ( "Setup Wi-Fi Mode</button>"                             ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Configure WiFi Connection</div><br>\n"            ) ;
  HTML += String ( "<div><form action='/Controller' method='GET' id='WiFiForm'>" ) ;
  HTML += String ( "<input type='hidden' name='Method' value='WiFi' />\n"  ) ;
  ////////////////////////////////////////////////////////////////////////////
  totalNetworks = GetEsp8266WifiNetworks ( )                                 ;
  for ( int i = 0 ; i < totalNetworks ; i++ )                                {
    encrypt  = ( WiFi . encryptionType ( i ) == ENC_TYPE_NONE )              ;
    SSID     = String ( WiFi . SSID ( i )   )                                ;
    LINE     = SSID                                                          ;
    LINE    += String ( " ("                )                                ;
    LINE    += String ( WiFi . RSSI ( i )   )                                ;
    LINE    += String ( ")"                 )                                ;
    LINE    += String ( encrypt ? "" : " *" )                                ;
    HTML    += String ( "<input type='radio' name='SSID' value='"          ) ;
    HTML    += SSID                                                          ;
    HTML    += String ( "'"                                                ) ;
    if                ( SSID == CurrentSSID                                ) {
      HTML  += String ( " checked"                                         ) ;
    }                                                                        ;
    HTML    += String ( " /><span>"                                        ) ;
    HTML    += LINE                                                          ;
    HTML    += String ( "</span>"                                          ) ;
    HTML    += String ( "<br>\n"                                           ) ;
  }                                                                          ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<label for='SsidPassword'>Password : </label>\n"       ) ;
  HTML += String ( "<input type='password' id='SsidPassword' name='Password'>" ) ;
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='WiFiForm'>"                ) ;
  HTML += String ( "Setup Wi-Fi</button>"                                  ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<a href='/'>Home</a>\n"                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</body>"                                               ) ;
  HTML += String ( "</html>"                                               ) ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send ( 200 , "text/html" , HTML                            ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerControl ( )                                                    {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  String HTML                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  HTML  = String ( "<!DOCTYPE html>"                                       ) ;
  HTML += String ( "<html>"                                                ) ;
  HTML += String ( "<head>"                                                ) ;
  HTML += String ( "</head>"                                               ) ;
  HTML += String ( "<body>"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( VersionMessage                                          ) ;
  HTML += String ( "</div>\n"                                              ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Pump : </span><span>"                       ) ;
  if             ( PumpPower                                               ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Water&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Water&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Water&Turn=Switch'>Switch</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Valve : </span><span>"                      ) ;
  if             ( SolenoidValve                                           ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Valve&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Valve&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Detect Water Level : </span><span>"         ) ;
  if             ( DetectWater                                             ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Level&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Level&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='WaterBasementForm'>" ) ;
  HTML += String ( "<label for='WaterBasement'>Water Basement ( 1/1000 cm ) : </label>\n" ) ;
  HTML += String ( "<input type='number' id='WaterBasement' name='Value' step=100 value='"  ) ;
  HTML += String ( WaterBasement                                           ) ;
  HTML += String ( "' />\n"                                                ) ;
  HTML += String ( "<input type='hidden' name='Method' value='Settings' />\n" ) ;
  HTML += String ( "<input type='hidden' name='Item' value='Water' />\n"   ) ;
  HTML += String ( "<input type='hidden' name='Property' value='Basement' />\n" ) ;
  HTML += String ( "<button type='submit' form='WaterBasementForm'>"       ) ;
  HTML += String ( "Setup</button>"                                        ) ;
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='WaterHighestForm'>" ) ;
  HTML += String ( "<label for='HighestWater'>Highest Water Level ( 1/1000 cm ) : </label>\n" ) ;
  HTML += String ( "<input type='number' id='HighestWater' name='Value' step=100 value='"  ) ;
  HTML += String ( HighestWaterLevel                                       ) ;
  HTML += String ( "' />\n"                                                ) ;
  HTML += String ( "<input type='hidden' name='Method' value='Settings' />\n" ) ;
  HTML += String ( "<input type='hidden' name='Item' value='Water' />\n"   ) ;
  HTML += String ( "<input type='hidden' name='Property' value='Highest' />\n" ) ;
  HTML += String ( "<button type='submit' form='WaterHighestForm'>"        ) ;
  HTML += String ( "Setup</button>"                                        ) ;
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='WaterLowestForm'>" ) ;
  HTML += String ( "<label for='LowestWater'>Lowest Water Level ( 1/1000 cm ) : </label>\n" ) ;
  HTML += String ( "<input type='number' id='LowestWater' name='Value' step=100 value='"  ) ;
  HTML += String ( LowestWaterLevel                                        ) ;
  HTML += String ( "' />\n"                                                ) ;
  HTML += String ( "<input type='hidden' name='Method' value='Settings' />\n" ) ;
  HTML += String ( "<input type='hidden' name='Item' value='Water' />\n"   ) ;
  HTML += String ( "<input type='hidden' name='Property' value='Lowest' />\n" ) ;
  HTML += String ( "<button type='submit' form='WaterLowestForm'>"         ) ;
  HTML += String ( "Setup</button>"                                        ) ;
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<a href='/'>Home</a>\n"                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</body>"                                               ) ;
  HTML += String ( "</html>"                                               ) ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send ( 200 , "text/html" , HTML                            ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerSettings ( )                                                   {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  String HTML                                                                ;
  ////////////////////////////////////////////////////////////////////////////
  HTML  = String ( "<!DOCTYPE html>"                                       ) ;
  HTML += String ( "<html>"                                                ) ;
  HTML += String ( "<head>"                                                ) ;
  HTML += String ( "</head>"                                               ) ;
  HTML += String ( "<body>"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( VersionMessage                                          ) ;
  HTML += String ( "</div>\n"                                              ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Setup Account</div><br>\n"                        ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='AccountForm'>" ) ;
  HTML += String ( "<input type='hidden' id='Method' name='Method' value='Settings'>" ) ;
  HTML += String ( "<input type='hidden' id='Item' name='Item' value='Account'>" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<label for='AccountUsername'>Username : </label>\n"               ) ;
  HTML += String ( "<input type='text' id='AccountUsername' name='Username' value='"  ) ;
  HTML += String ( SiteUsername                                            ) ;
  HTML += String ( "' />"                                                  ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<label for='AccountPassword'>Password : </label>\n" ) ;
  HTML += String ( "<input type='text' id='AccountPassword' name='Password' value='"  ) ;
  HTML += String ( SitePassword                                            ) ;
  HTML += String ( "' />"                                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='AccountForm'>"            ) ;
  HTML += String ( "Setup Account Username and Password</button>" ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Serial Port Report : </span><span>"         ) ;
  if             ( Console                                                 ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Console&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Console&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><span>Slow Down and Power Saving : </span><span>" ) ;
  if             ( doDelay                                                 ) {
    HTML += String ( "On"  )                                                 ;
  } else                                                                     {
    HTML += String ( "Off" )                                                 ;
  }                                                                          ;
  HTML += String ( "</span></div>\n"                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>"                                                 ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Delay&Turn=On'>Turn On</a>"   ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "&nbsp;"                                                ) ;
  HTML += String ( "<span>"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Settings&Item=Delay&Turn=Off'>Turn Off</a>" ) ;
  HTML += String ( "</span>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Configure Delay Interval</div><br>\n"             ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='IntervalForm'>" ) ;
  HTML += String ( "<input type='hidden' id='Method' name='Method' value='Settings'>" ) ;
  HTML += String ( "<input type='hidden' id='Item' name='Item' value='Interval'>" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<label for='DelayInterval'>Site : </label>\n"          ) ;
  HTML += String ( "<input type='number' id='DelayInterval' name='Microseconds' value='" ) ;
  HTML += String ( MicrosecondsDelay                                       ) ;
  HTML += String ( "' />"                                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='IntervalForm'>"            ) ;
  HTML += String ( "Setup Delay Interval</button>"                         ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div>Configure Pump Switch Mode</div><br>\n"                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<div><form action='/Controller' method='GET' id='PumpSwitchModeForm'>" ) ;
  HTML += String ( "<input type='hidden' id='Method' name='Method' value='Settings'>" ) ;
  HTML += String ( "<input type='hidden' id='Item' name='Item' value='Switch'>" ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='Switch'"     ) ;
  if                ( PowerSwitchMode == 0                                 ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Switch Mode</span><br>\n"                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='Sync'"       ) ;
  if                ( PowerSwitchMode == 1                                 ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Sync Mode</span><br>\n"                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML    += String ( "<input type='radio' name='Mode' value='Isolate'"    ) ;
  if                ( PowerSwitchMode == 2                                 ) {
    HTML  += String ( " checked"                                           ) ;
  }                                                                          ;
  HTML    += String ( " /><span>Isolate Mode</span><br>\n"                 ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</form>"                                               ) ;
  HTML += String ( "</div>"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<button type='submit' form='PumpSwitchModeForm'>"      ) ;
  HTML += String ( "Setup Pump Switch Mode</button>"                       ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<a href='/Controller?Method=EEPROM'>Write Settings into EEPROM</a>\n" ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<a href='/Controller?Method=Reset'>Reboot</a>\n"       ) ;
  HTML += String ( "<br>\n"                                                ) ;
  HTML += String ( "<br>\n"                                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "<a href='/'>Home</a>\n"                                ) ;
  ////////////////////////////////////////////////////////////////////////////
  HTML += String ( "</body>"                                               ) ;
  HTML += String ( "</html>"                                               ) ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send ( 200 , "text/html" , HTML                            ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerController (                                                 ) {
  ////////////////////////////////////////////////////////////////////////////
  if                    ( HttpServer == nullptr                            ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int pc                                                                     ;
  if                    ( HttpServer -> args ( ) > 0                       ) {
    if                  ( HttpServer -> hasArg ( "Method" )                ) {
      ////////////////////////////////////////////////////////////////////////
      String Method = HttpServer -> arg ( "Method" )                         ;
      ////////////////////////////////////////////////////////////////////////
      // 重置機器
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Reset"                                ) {
        //////////////////////////////////////////////////////////////////////
        HttpServer -> sendHeader ( "Location" , "/"          , true        ) ;
        HttpServer -> send       ( 302        , "text/plain" , ""          ) ;
        delayMicroseconds ( 1000000                                        ) ;
        resetFunc         (                                                ) ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 寫入EEPROM
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "EEPROM"                               ) {
        //////////////////////////////////////////////////////////////////////
        FlashEEPROM     (                                                  ) ;
        //////////////////////////////////////////////////////////////////////
        HttpServer -> sendHeader ( "Location" , "/settings"  , true        ) ;
        HttpServer -> send       ( 302        , "text/plain" , ""          ) ;
        return                                                               ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 切換抽水機
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Water"                                ) {
        //////////////////////////////////////////////////////////////////////
        String MSG                                                           ;
        String TURN = HttpServer -> arg ( "Turn" )                           ;
        bool   pwm                                                           ;
        //////////////////////////////////////////////////////////////////////
        if              ( UseWiFi                                          ) {
          if            ( TURN == "On"                                     ) {
            PC -> Turn       ( true                                        ) ;
            UpdatePumpStatus (                                             ) ;
          } else
          if            ( TURN == "Off"                                    ) {
            PC -> Turn       ( false                                       ) ;
            UpdatePumpStatus (                                             ) ;
          } else
          if            ( TURN == "Switch"                                 ) {
            PC -> Switch     (                                             ) ;
            UpdatePumpStatus (                                             ) ;
          }                                                                  ;
          pc = PC -> Power   (                                             ) ;
          PumpPower   = pc ? 1 : 0                                           ;
          HttpServer -> sendHeader ( "Location" , "/control.html" , true   ) ;
          HttpServer -> send       ( 302        , "text/plain"    , ""     ) ;
        } else                                                               {
          HttpServer -> send ( 200 , "text/html" , "Can not do it!"        ) ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        return                                                               ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 切換電磁水閥
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Valve"                                ) {
        //////////////////////////////////////////////////////////////////////
        String TURN = HttpServer -> arg ( "Turn" )                           ;
        //////////////////////////////////////////////////////////////////////
        if              ( UseWiFi                                          ) {
          if            ( TURN == "On"                                     ) {
            TurnSolenoidValve ( true                                       ) ;
            UpdateValveStatus (                                            ) ;
          } else
          if              ( TURN == "Off"                                  ) {
            TurnSolenoidValve ( false                                      ) ;
            UpdateValveStatus (                                            ) ;
          }
          HttpServer -> sendHeader ( "Location" , "/control.html" , true   ) ;
          HttpServer -> send       ( 302        , "text/plain"    , ""     ) ;
          return                                                             ;
        } else                                                               {
          HttpServer -> send ( 200 , "text/html" , "Can not do it!"        ) ;
          return                                                             ;
        }
        //////////////////////////////////////////////////////////////////////
      } else
      if                ( Method == "WiFi"                                 ) {
        String SSID   = HttpServer -> arg ( "SSID"     )                     ;
        String PASSWD = HttpServer -> arg ( "Password" )                     ;
        char   buf [ 256 ]                                                   ;
        SSID   . toCharArray    ( buf , 255                                ) ;
        WifiSSID      = strdup  ( buf                                      ) ;
        PASSWD . toCharArray    ( buf , 255                                ) ;
        WifiPassword  = strdup  ( buf                                      ) ;
        FlashEEPROM     (                                                  ) ;
        HttpServer -> sendHeader ( "Location" , "/"          , true        ) ;
        HttpServer -> send       ( 302        , "text/plain" , ""          ) ;
        return                                                             ;
      }
      if                ( Method == "Settings"                             ) {
        //////////////////////////////////////////////////////////////////////
        String ITEM = HttpServer -> arg   ( "Item" )                         ;
        if              ( ITEM   == "Console"                              ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            Console = true                                                   ;
          } else                                                             {
            Console = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/settings.html" , true  ) ;
          HttpServer -> send       ( 302        , "text/plain"     , ""    ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "Delay"                                ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            doDelay = true                                                   ;
          } else                                                             {
            doDelay = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/settings.html" , true  ) ;
          HttpServer -> send       ( 302        , "text/plain"     , ""    ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "Interval"                             ) {
          String MS = HttpServer -> arg   ( "Microseconds" )                 ;
          MicrosecondsDelay = MS . toInt ( )                                 ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/settings.html" , true  ) ;
          HttpServer -> send       ( 302        , "text/plain"     , ""    ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "Switch"                               ) {
          String MODE = HttpServer -> arg   ( "Mode" )                       ;
          if            ( MODE == "Switch"                                 ) {
            PowerSwitchMode = 0                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          } else
          if            ( MODE == "Sync"                                   ) {
            PowerSwitchMode = 1                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          } else
          if            ( MODE == "Isolate"                                ) {
            PowerSwitchMode = 2                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/settings.html" , true  ) ;
          HttpServer -> send       ( 302        , "text/plain"     , ""    ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "Level"                                ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            DetectWater = true                                               ;
          } else                                                             {
            DetectWater = false                                              ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Water"                                ) {
          String PROP  = HttpServer -> arg ( "Property" )                    ;
          String VALUE = HttpServer -> arg ( "Value"    )                    ;
          int    Z     = VALUE . toInt     (            )                    ;
          if            ( PROP == "Basement"                               ) {
            WaterBasement = Z                                                ;
          } else
          if            ( PROP == "Highest"                                ) {
            HighestWaterLevel = Z                                            ;
          } else
          if            ( PROP == "Lowest"                                 ) {
            LowestWaterLevel = Z                                             ;
          }
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "WiFiMode"                             ) {
          String MODE = HttpServer -> arg   ( "Mode" )                       ;
          WifiMode = MODE . toInt ( )                                        ;
          if            ( WifiMode < 1                                     ) {
            WifiMode = 3                                                     ;
          } else
          if            ( WifiMode > 3                                     ) {
            WifiMode = 3                                                     ;
          }
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/wifi.html" , true      ) ;
          HttpServer -> send       ( 302        , "text/plain" , ""        ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "WIFI"                                 ) {
          String MODE = HttpServer -> arg   ( "Use" )                        ;
          if            ( MODE == "On"                                     ) {
            UseWiFi = true                                                   ;
          } else                                                             {
            UseWiFi = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/wifi.html" , true      ) ;
          HttpServer -> send       ( 302        , "text/plain" , ""        ) ;
          return                                                             ;
        } else
/*
bool               AssignIP         = false                                  ;
char               HostIP      [ 4 ]                                         ;
char               HostGateway [ 4 ]                                         ;
char               HostNetmask [ 4 ]                                         ;
*/
        if              ( ITEM   == "Site"                                 ) {
          String NAME   = HttpServer -> arg ( "Name" )                       ;
          String PASSWD = HttpServer -> arg ( "SapPassword" )                ;
          char site [ 256 ]                                                  ;
          NAME   . toCharArray ( site , 23 )                                 ;
          ArduinoSite = strdup ( site )                                      ;
          PASSWD . toCharArray ( site , 31 )                                 ;
          SapPassword = strdup ( site )                                      ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/"          , true      ) ;
          HttpServer -> send       ( 302        , "text/plain" , ""        ) ;
          return                                                             ;
        } else
        if              ( ITEM   == "Account"                              ) {
          String NAME   = HttpServer -> arg ( "Username" )                   ;
          String PASSWD = HttpServer -> arg ( "Password" )                   ;
          char site [ 256 ]                                                  ;
          NAME   . toCharArray ( site , 31 )                                 ;
          SiteUsername = strdup ( site )                                     ;
          PASSWD . toCharArray ( site , 31 )                                 ;
          SitePassword = strdup ( site )                                     ;
          FlashEEPROM   (                                                  ) ;
          HttpServer -> sendHeader ( "Location" , "/settings.html" , true  ) ;
          HttpServer -> send       ( 302        , "text/plain"     , ""    ) ;
          return                                                             ;
        }
        //////////////////////////////////////////////////////////////////////
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if                    ( HttpServer -> headers ( ) > 0                    ) {
    if                  ( Console                                          ) {
      Serial . print    ( "AJAX Headers : "                                ) ;
      Serial . println  ( HttpServer -> headers ( ) , DEC                  ) ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> sendHeader ( "Location" , "/control.html" , true           ) ;
  HttpServer -> send       ( 302        , "text/plain"    , ""             ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerAJAX      (                                                  ) {
  ////////////////////////////////////////////////////////////////////////////
  if                    ( HttpServer == nullptr                            ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if                    ( HttpServer -> args ( ) > 0                       ) {
    if                  ( HttpServer -> hasArg ( "Method" )                ) {
      ////////////////////////////////////////////////////////////////////////
      String Method = HttpServer -> arg ( "Method" )                         ;
      ////////////////////////////////////////////////////////////////////////
      // 重置機器
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Reset"                                ) {
        //////////////////////////////////////////////////////////////////////
        HttpServer -> sendHeader ( "Location" , "/"          , true        ) ;
        HttpServer -> send       ( 302        , "text/plain" , ""          ) ;
        delayMicroseconds ( 1000000                                        ) ;
        resetFunc         (                                                ) ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 寫入EEPROM
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "EEPROM"                               ) {
        //////////////////////////////////////////////////////////////////////
        FlashEEPROM     (                                                  ) ;
        //////////////////////////////////////////////////////////////////////
        HttpServer -> send                                                   (
          200                                                                ,
          "text/html"                                                        ,
          "Flash EEPROM"                                                   ) ;
        return                                                               ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 切換抽水機
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Water"                                ) {
        //////////////////////////////////////////////////////////////////////
        String MSG                                                           ;
        String TURN = HttpServer -> arg ( "Turn" )                           ;
        bool   pwm                                                           ;
        //////////////////////////////////////////////////////////////////////
        if              ( UseWiFi                                          ) {
          if            ( TURN == "On"                                     ) {
            PC -> Turn       ( true                                        ) ;
            UpdatePumpStatus (                                             ) ;
          } else
          if            ( TURN == "Off"                                    ) {
            PC -> Turn       ( false                                       ) ;
            UpdatePumpStatus (                                             ) ;
          } else
          if            ( TURN == "Switch"                                 ) {
            PC -> Switch     (                                             ) ;
            UpdatePumpStatus (                                             ) ;
          }                                                                  ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        pwm   = PC -> Power ( )                                              ;
        //////////////////////////////////////////////////////////////////////
        if              ( pwm                                              ) {
          MSG = "On"                                                         ;
        } else                                                               {
          MSG = "Off"                                                        ;
        }                                                                    ;
        //////////////////////////////////////////////////////////////////////
        HttpServer -> send                                                   (
          200                                                                ,
          "text/html"                                                        ,
          MSG                                                              ) ;
        return                                                               ;
        //////////////////////////////////////////////////////////////////////
      } else
      ////////////////////////////////////////////////////////////////////////
      // 切換電磁水閥
      ////////////////////////////////////////////////////////////////////////
      if                ( Method == "Valve"                                ) {
        //////////////////////////////////////////////////////////////////////
        String TURN = HttpServer -> arg ( "Turn" )                           ;
        //////////////////////////////////////////////////////////////////////
        if              ( UseWiFi                                          ) {
          if            ( TURN == "On"                                     ) {
            TurnSolenoidValve ( true                                       ) ;
            UpdateValveStatus (                                            ) ;
          } else
          if              ( TURN == "Off"                                  ) {
            PC -> Turn    ( false                                          ) ;
            TurnSolenoidValve ( false                                      ) ;
            UpdateValveStatus (                                            ) ;
          }
          HttpServer -> send                                                 (
            200                                                              ,
            "text/html"                                                      ,
            TURN                                                           ) ;
          return                                                             ;
        } else                                                               {
          HttpServer -> send                                                 (
            200                                                              ,
            "text/html"                                                      ,
            "Can not do it!"                                               ) ;
          return                                                             ;
        }
        //////////////////////////////////////////////////////////////////////
      } else
      if                ( Method == "JSON"                                 ) {
        //////////////////////////////////////////////////////////////////////
        String JSON = StatusToJson ( )                                       ;
        //////////////////////////////////////////////////////////////////////
        HttpServer -> send                                                   (
          200                                                                ,
          "text/json"                                                        ,
          JSON                                                             ) ;
        //////////////////////////////////////////////////////////////////////
      } else
      if                ( Method == "Settings"                             ) {
        //////////////////////////////////////////////////////////////////////
        String ITEM = HttpServer -> arg   ( "Item" )                         ;
        if              ( ITEM   == "Console"                              ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            Console = true                                                   ;
          } else                                                             {
            Console = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Delay"                                ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            doDelay = true                                                   ;
          } else                                                             {
            doDelay = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Interval"                             ) {
          String MS = HttpServer -> arg   ( "Microseconds" )                 ;
          MicrosecondsDelay = MS . toInt ( )                                 ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Switch"                               ) {
          String MODE = HttpServer -> arg   ( "Mode" )                       ;
          if            ( MODE == "Switch"                                 ) {
            PowerSwitchMode = 0                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          } else
          if            ( MODE == "Sync"                                   ) {
            PowerSwitchMode = 1                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          } else
          if            ( MODE == "Isolate"                                ) {
            PowerSwitchMode = 2                                              ;
            PC -> setSwitch ( PowerSwitchMode )                              ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Level"                                ) {
          String TURN = HttpServer -> arg   ( "Turn" )                       ;
          if            ( TURN == "On"                                     ) {
            DetectWater = true                                               ;
          } else                                                             {
            DetectWater = false                                              ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Water"                                ) {
          String PROP  = HttpServer -> arg ( "Property" )                    ;
          String VALUE = HttpServer -> arg ( "Value"    )                    ;
          int    Z     = VALUE . toInt     (            )                    ;
          if            ( PROP == "Basement"                               ) {
            WaterBasement = Z                                                ;
          } else
          if            ( PROP == "Highest"                                ) {
            HighestWaterLevel = Z                                            ;
          } else
          if            ( PROP == "Lowest"                                 ) {
            LowestWaterLevel = Z                                             ;
          }
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "WIFI"                                 ) {
          String MODE = HttpServer -> arg   ( "Mode" )                       ;
          WifiMode = MODE . toInt ( )                                        ;
          if            ( WifiMode < 1                                     ) {
            WifiMode = 3                                                     ;
          } else
          if            ( WifiMode > 3                                     ) {
            WifiMode = 3                                                     ;
          }
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "WIFI"                                 ) {
          String MODE = HttpServer -> arg   ( "Use" )                        ;
          if            ( MODE == "On"                                     ) {
            UseWiFi = true                                                   ;
          } else                                                             {
            UseWiFi = false                                                  ;
          }                                                                  ;
          FlashEEPROM   (                                                  ) ;
        } else
        if              ( ITEM   == "Site"                                 ) {
          String NAME = HttpServer -> arg   ( "Name" )                       ;
          char site [ 24 ]                                                   ;
          NAME . toCharArray ( site , 23 )                                   ;
          ArduinoSite = strdup ( site )                                      ;
          FlashEEPROM   (                                                  ) ;
        }
/*
myString.toCharArray(buf, len)
char             * WifiSSID         = "Actions 7f"                           ;
char             * WifiPassword     = "0912388888"                           ;
char             * SapPassword      = ""                                     ;

bool               AssignIP         = false                                  ;
char               HostIP      [ 4 ]                                         ;
char               HostGateway [ 4 ]                                         ;
char               HostNetmask [ 4 ]                                         ;

char             * SiteUsername     = "admin"                                ;
char             * SitePassword     = "123456789"                            ;
*/
        //////////////////////////////////////////////////////////////////////
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if                    ( HttpServer -> headers ( ) > 0                    ) {
    if                  ( Console                                          ) {
      Serial . print    ( "AJAX Headers : "                                ) ;
      Serial . println  ( HttpServer -> headers ( ) , DEC                  ) ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send                                                         (
    200                                                                      ,
    "text/html"                                                              ,
    "OK"                                                                   ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerNotFound ( )                                                   {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send                                                         (
    404                                                                      ,
    "text/plain"                                                             ,
    "File NOT found!"                                                      ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void ReportWIFI              ( void                                        ) {
  ////////////////////////////////////////////////////////////////////////////
  if                         ( ! Console                                   ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  int totalNetworks = GetEsp8266WifiNetworks ( )                             ;
  for                       ( int i = 0 ; i < totalNetworks ; i++          ) {
    PrintEsp8266WifiStation ( i                                            ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void ReportSTA              ( void                                         ) {
  ////////////////////////////////////////////////////////////////////////////
  if                        ( ! Console                                    ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  Serial . println          ( ""                                           ) ;
  Serial . print            ( "IP address: "                               ) ;
  Serial . println          ( WiFi . localIP ( )                           ) ;
  Serial . println          ( "WiFi status:"                               ) ;
  WiFi   . printDiag        ( Serial                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void ReportSAP              ( void                                         ) {
  ////////////////////////////////////////////////////////////////////////////
  if                        ( ! Console                                    ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  Serial . print            ( "Soft Access Point : "                       ) ;
  Serial . println          ( WiFi . softAPIP ( )                          ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
bool isConnectionTimeout     ( void                                        ) {
  int dt = millis            (                                             ) ;
  dt -= WifiTimestamp                                                        ;
  return                     ( dt > 10000                                  ) ;
}
//////////////////////////////////////////////////////////////////////////////
void BuildUpSoftAP           (                                             ) {
  ////////////////////////////////////////////////////////////////////////////
  SetEsp8266WifiAP           (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
//  WiFi   . begin             ( ArduinoSite , SapPassword                   ) ;
  WiFi   . softAP            ( ArduinoSite , SapPassword                   ) ;
//  IPAddress myIP = WiFi.softAPIP();
  if                         ( MDNS . begin ( OriphaseArduino )            ) {
    MDNS . addService        ( "http" , "tcp" , 80                         ) ;
  }                                                                          ;
//  if                         ( AssignIP                                    ) {
//    WiFi . config            ( ComposeIP ( HostIP      )                     , // IP Address
//                               ComposeIP ( HostGateway )                     , // Gateway
//                               ComposeIP ( HostNetmask )                   ) ; // Netmask
//  }                                                                          ;
  WifiConnected = true                                                       ;
  WifiTimestamp = millis     (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
  ReportWIFI                 (                                             ) ;
  ReportSAP                  (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void ConnectToWIFI           (                                             ) {
  ////////////////////////////////////////////////////////////////////////////
  int dt                                                                     ;
  int status                                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  SetEsp8266WifiSTA          (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
  WiFi   . begin             ( WifiSSID , WifiPassword                     ) ;
  if                         ( AssignIP                                    ) {
    WiFi . config            ( ComposeIP ( HostIP      )                     , // IP Address
                               ComposeIP ( HostGateway )                     , // Gateway
                               ComposeIP ( HostNetmask )                   ) ; // Netmask
  }                                                                          ;
  WifiConnected = false                                                      ;
  WifiTimestamp = millis     (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
  do                                                                         {
    delay                    ( 100                                         ) ;
    status = WiFi . status   (                                             ) ;
    dt     = millis          (                                             ) ;
  } while ( ( status != WL_CONNECTED ) && ( ( dt-WifiTimestamp ) < 5000 )  ) ;
  ////////////////////////////////////////////////////////////////////////////
  if                         ( status == WL_CONNECTED                      ) {
    //////////////////////////////////////////////////////////////////////////
    WifiConnected = true                                                     ;
    PrepareHTTP              (                                             ) ;
    ReportWIFI               (                                             ) ;
    ReportSTA                (                                             ) ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void BuildUpWifiBoth         (                                             ) {
  ////////////////////////////////////////////////////////////////////////////
  int dt                                                                     ;
  int status                                                                 ;
  ////////////////////////////////////////////////////////////////////////////
  SetEsp8266WifiBoth         (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
//  WiFi   . begin             ( ArduinoSite , SapPassword                   ) ;
  WiFi   . softAP            ( ArduinoSite , SapPassword                   ) ;
  if                         ( MDNS . begin ( OriphaseArduino )            ) {
    MDNS . addService        ( "http" , "tcp" , 80                         ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  ReportWIFI                 (                                             ) ;
  ReportSAP                  (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
  WiFi   . begin             ( WifiSSID , WifiPassword                     ) ;
  if                         ( AssignIP                                    ) {
    WiFi . config            ( ComposeIP ( HostIP      )                     , // IP Address
                               ComposeIP ( HostGateway )                     , // Gateway
                               ComposeIP ( HostNetmask )                   ) ; // Netmask
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  WifiConnected    = false                                                   ;
  WifiTimestamp    = millis  (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
  do                                                                         {
    delay                    ( 100                                         ) ;
    status = WiFi . status   (                                             ) ;
    dt     = millis          (                                             ) ;
  } while ( ( status != WL_CONNECTED ) && ( ( dt-WifiTimestamp ) < 5000 )  ) ;
  ////////////////////////////////////////////////////////////////////////////
  if                         ( status == WL_CONNECTED                      ) {
    //////////////////////////////////////////////////////////////////////////
    WifiConnected = true                                                     ;
    PrepareHTTP              (                                             ) ;
    ReportSTA                (                                             ) ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void PrepareHTTP           ( void                                          ) {
  ////////////////////////////////////////////////////////////////////////////
  if                       ( HttpInitialized                               ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpInitialized = true                                                     ;
  HttpServer -> on         ( "/"              , WebServerEntry             ) ;
  HttpServer -> on         ( "/index.html"    , WebServerEntry             ) ;
  HttpServer -> on         ( "/wifi.html"     , WebServerWIFI              ) ;
  HttpServer -> on         ( "/control.html"  , WebServerControl           ) ;
  HttpServer -> on         ( "/settings.html" , WebServerSettings          ) ;
  HttpServer -> on         ( "/Controller"    , WebServerController        ) ;
  HttpServer -> on         ( "/AJAX"          , WebServerAJAX              ) ;
  HttpServer -> onNotFound (                    WebServerNotFound          ) ;
  HttpServer -> begin      (                                               ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void SetupWIFI                             (                               ) {
  ////////////////////////////////////////////////////////////////////////////
  switch                                   (  WifiMode                     ) {
    //////////////////////////////////////////////////////////////////////////
    case 1                                                                   :
      ////////////////////////////////////////////////////////////////////////
      BuildUpSoftAP                        (                               ) ;
      HttpServer    = new ESP8266WebServer ( WifiHttpPort                  ) ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
    case 2                                                                   :
      ////////////////////////////////////////////////////////////////////////
      ConnectToWIFI                        (                               ) ;
      HttpServer    = new ESP8266WebServer ( WifiHttpPort                  ) ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
    case 3                                                                   :
      ////////////////////////////////////////////////////////////////////////
      BuildUpWifiBoth                      (                               ) ;
      HttpServer    = new ESP8266WebServer ( WifiHttpPort                  ) ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void DoingWIFI              (                                              ) {
  ////////////////////////////////////////////////////////////////////////////
  switch                    (  WifiMode                                    ) {
    //////////////////////////////////////////////////////////////////////////
    // Soft Access Point
    //////////////////////////////////////////////////////////////////////////
    case 1                                                                   :
      ////////////////////////////////////////////////////////////////////////
      if                    ( HttpInitialized                              ) {
        if                  ( HttpServer != nullptr                        ) {
          HttpServer -> handleClient ( )                                     ;
        }                                                                    ;
      } else                                                                 {
        PrepareHTTP         (                                              ) ;
        ReportWIFI          (                                              ) ;
        ReportSAP           (                                              ) ;
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
    // STA
    //////////////////////////////////////////////////////////////////////////
    case 2                                                                   :
      ////////////////////////////////////////////////////////////////////////
      if                    ( WifiConnected                                ) {
        if                  ( HttpInitialized                              ) {
          if                ( HttpServer != nullptr                        ) {
            HttpServer -> handleClient ( )                                   ;
          }                                                                  ;
        } else                                                               {
          PrepareHTTP       (                                              ) ;
          ReportWIFI        (                                              ) ;
          ReportSTA         (                                              ) ;
        }                                                                    ;
      } else                                                                 {
        if                  ( WiFi . status ( ) == WL_CONNECTED            ) {
          WifiConnected = true                                               ;
          PrepareHTTP       (                                              ) ;
          ReportWIFI        (                                              ) ;
          ReportSTA         (                                              ) ;
        } else                                                               {
          if                ( isConnectionTimeout ( )                      ) {
            ConnectToWIFI   (                                              ) ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
    case 3                                                                   :
      ////////////////////////////////////////////////////////////////////////
      if                     ( HttpInitialized                             ) {
        if                   ( HttpServer != nullptr                       ) {
          HttpServer -> handleClient ( )                                     ;
        }                                                                    ;
      } else                                                                 {
        PrepareHTTP          (                                             ) ;
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
      if                     ( ! WifiConnected                             ) {
        if                   ( WiFi . status ( ) == WL_CONNECTED           ) {
          WifiConnected = true                                               ;
          ReportSTA          (                                             ) ;
        }                                                                    ;
      }                                                                      ;
      ////////////////////////////////////////////////////////////////////////
    break                                                                    ;
    //////////////////////////////////////////////////////////////////////////
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 啟動設定飲水機內定設置
//////////////////////////////////////////////////////////////////////////////
void InitializeWaterCycler   (                                             ) {
  ////////////////////////////////////////////////////////////////////////////
  // 抽水機
  ////////////////////////////////////////////////////////////////////////////
  SetOutput                  ( PumpIO                                      ) ;
  WriteValue                 ( PumpIO  , 0                                 ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 抽水開關
  ////////////////////////////////////////////////////////////////////////////
  SetInput                   (  SwitchIO                                   ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 電磁水閥
  ////////////////////////////////////////////////////////////////////////////
  SetOutput                  ( ValveIO                                     ) ;
  WriteValue                 ( ValveIO , 0                                 ) ;
  SetInput                   ( ValveSwitch                                 ) ;
  ValveStatus = ReadValue    ( ValveSwitch                                 ) ;
  TurnSolenoidValve          ( false                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 暫時沒有用到
  ////////////////////////////////////////////////////////////////////////////
  SetInput                   (  7                                          ) ;
  SetInput                   (  8                                          ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 水位偵測器
  ////////////////////////////////////////////////////////////////////////////
  pinMode                    ( A0 , INPUT                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  PC   = new PowerController ( PumpIO , SwitchIO , PowerSwitchMode , false ) ;
  PC  -> Turn                ( PumpPower > 0                               ) ;
  PC  -> GetInput            (                                             ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void ProbeWaterCycler              (                                       ) {
  ////////////////////////////////////////////////////////////////////////////
  int  dtm       = millis          (                                       ) ;
  int  dt        = dtm - PowerProbedMillis                                   ;
  bool in                                                                    ;
  bool changed   = false                                                     ;
  bool pc                                                                    ;
  bool vsChanged = false                                                     ;
  int  cwl       = 0                                                         ;
  int  vs        = 0                                                         ;
  ////////////////////////////////////////////////////////////////////////////
  if                               ( dt < 10                               ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  // 偵測電磁水閥開關是否有改變
  ////////////////////////////////////////////////////////////////////////////
  vs           = ReadValue         ( ValveSwitch                           ) ;
  if                               ( vs != ValveStatus                     ) {
    vsChanged  = true                                                        ;
    ValveStatus = vs                                                         ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  // 偵測水位
  ////////////////////////////////////////////////////////////////////////////
  cwl          = CurrentWaterLevel (                                       ) ;
  if                               ( DetectWater                           ) {
    if                             ( cwl > HighestWaterLevel               ) {
      if                           ( SolenoidValve                         ) {
        CareValve = false                                                    ;
        TurnSolenoidValve          ( false                                 ) ;
        UpdateValveStatus          (                                       ) ;
      }                                                                      ;
    } else
    if                             ( cwl < LowestWaterLevel                ) {
      if                           ( vsChanged                             ) {
        if                         ( SolenoidValve                         ) {
          CareValve = true                                                   ;
        } else                                                               {
          CareValve = false                                                  ;
        }                                                                    ;
        ChangeSolenoidValve        (                                       ) ;
      } else                                                                 {
        if                         ( ! CareValve                           ) {
          if                       ( ! SolenoidValve                       ) {
            TurnSolenoidValve      ( true                                  ) ;
            UpdateValveStatus      (                                       ) ;
          }                                                                  ;
        }                                                                    ;
      }                                                                      ;
    } else                                                                   {
      if                           ( vsChanged                             ) {
        ChangeSolenoidValve        (                                       ) ;
      }                                                                      ;
    }                                                                        ;
  } else                                                                     {
    if                             ( vsChanged                             ) {
      ChangeSolenoidValve          (                                       ) ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  PowerProbedMillis = dtm                                                    ;
    pc         = PC -> Probe       (                                       ) ;
    if                             ( pc                                    ) {
      changed  = true                                                        ;
    }                                                                        ;
  ////////////////////////////////////////////////////////////////////////////
  if                               ( ! changed                             ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  pc           = PC -> Power       (                                       ) ;
  in           = PC -> InputNow    (                                       ) ;
  PumpPower    = pc ? 1 : 0                                                  ;
  PumpSwitch   = in ? 1 : 0                                                  ;
  UpdatePumpStatus                 (                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 更新現有狀態到EEPROM
  ////////////////////////////////////////////////////////////////////////////
  WriteToEEPROM                    ( 224 , 4 , (char *)&PumpPower          ) ;
  WriteToEEPROM                    ( 228 , 4 , (char *)&PumpSwitch         ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 讀寫設定
//////////////////////////////////////////////////////////////////////////////
typedef struct OriphaseWaterCyclerConfigure                                  {
  char         Oriphase     [  8 ]                                           ; //   0
  char         Site         [ 24 ]                                           ; //   8
  char         SapPassword  [ 32 ]                                           ; //  32
  char         WifiSSID     [ 32 ]                                           ; //  64
  char         WifiPassword [ 32 ]                                           ; //  96
  char         Username     [ 32 ]                                           ; // 128
  char         Password     [ 32 ]                                           ; // 160
  char         IP           [  4 ]                                           ; // 192
  char         Gateway      [  4 ]                                           ; // 196
  char         Netmask      [  4 ]                                           ; // 200
  char         Debug                                                         ; // 204
  char         Console                                                       ; // 205
  char         UseWiFi                                                       ; // 206
  char         WifiMode                                                      ; // 207
  char         doDelay                                                       ; // 208
  char         AssignIP                                                      ; // 209
  char         SwitchMode                                                    ; // 210
  char         DetectWater                                                   ; // 211
  int          BaudRate                                                      ; // 212
  unsigned int MicrosecondsDelay                                             ; // 216
  int          HttpPort                                                      ; // 220
  int          PumpPower                                                     ; // 224
  int          PumpSwitch                                                    ; // 228
  int          WaterBasement                                                 ; // 232
  int          HighestWaterLevel                                             ; // 236
  int          LowestWaterLevel                                              ; // 240
}              OriphaseConfigure                                             ;
//////////////////////////////////////////////////////////////////////////////
void FlashEEPROM       ( void                                              ) {
  ////////////////////////////////////////////////////////////////////////////
  OriphaseWaterCyclerConfigure conf                                          ;
  int                          vs                                            ;
  ////////////////////////////////////////////////////////////////////////////
  vs   = sizeof        ( OriphaseWaterCyclerConfigure                      ) ;
  memset               ( &conf , 0 , vs                                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  memcpy               ( conf . Oriphase     , OriphaseArduino     ,  8    ) ;
  strncpy              ( conf . Site         , ArduinoSite         , 24    ) ;
  strncpy              ( conf . SapPassword  , SapPassword         , 32    ) ;
  strncpy              ( conf . WifiSSID     , WifiSSID            , 32    ) ;
  strncpy              ( conf . WifiPassword , WifiPassword        , 32    ) ;
  strncpy              ( conf . Username     , SiteUsername        , 32    ) ;
  strncpy              ( conf . Password     , SitePassword        , 32    ) ;
  memcpy               ( conf . IP           , HostIP              ,  4    ) ;
  memcpy               ( conf . Gateway      , HostGateway         ,  4    ) ;
  memcpy               ( conf . Netmask      , HostNetmask         ,  4    ) ;
  ////////////////////////////////////////////////////////////////////////////
  conf . Debug             = char ( Debug               )                    ;
  conf . Console           = char ( Console             )                    ;
  conf . SwitchMode        = char ( PowerSwitchMode     )                    ;
  conf . WifiMode          = char ( WifiMode            )                    ;
  conf . UseWiFi           = UseWiFi     ? 1 : 0                             ;
  conf . doDelay           = doDelay     ? 1 : 0                             ;
  conf . AssignIP          = AssignIP    ? 1 : 0                             ;
  conf . DetectWater       = DetectWater ? 1 : 0                             ;
  conf . BaudRate          = BaudRate                                        ;
  conf . MicrosecondsDelay = MicrosecondsDelay                               ;
  conf . HttpPort          = WifiHttpPort                                    ;
  conf . PumpPower         = PumpPower                                       ;
  conf . PumpSwitch        = PumpSwitch                                      ;
  conf . WaterBasement     = WaterBasement                                   ;
  conf . HighestWaterLevel = HighestWaterLevel                               ;
  conf . LowestWaterLevel  = LowestWaterLevel                                ;
  ////////////////////////////////////////////////////////////////////////////
  WriteToEEPROM        ( 0 , vs , (char *) &conf                           ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 從EEPROM讀取現有設定
//////////////////////////////////////////////////////////////////////////////
void ReloadEEPROM      (                                                   ) {
  ////////////////////////////////////////////////////////////////////////////
  bool                         initialized = true                            ;
  char                         OriphaseInitialized [ 9 ]                     ;
  OriphaseWaterCyclerConfigure conf                                          ;
  int                          vs                                            ;
  ////////////////////////////////////////////////////////////////////////////
  vs   = sizeof        ( OriphaseWaterCyclerConfigure                      ) ;
  ////////////////////////////////////////////////////////////////////////////
  memset               ( &conf , 0 , vs                                    ) ;
  ////////////////////////////////////////////////////////////////////////////
  EEPROM . begin       ( EepromSize                                        ) ;
  for                  ( int i = 0 ; i < 9 ; i++                           ) {
    OriphaseInitialized [ i ] = 0                                            ;
  }                                                                          ;
  ReadFromEEPROM       ( 0 , 8 , OriphaseInitialized                       ) ;
  for                  ( int i = 0 ; i < 8 ; i++                           ) {
    if ( OriphaseInitialized [ i ] != OriphaseArduino [ i ] )                {
      initialized  = false                                                   ;
    }                                                                        ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  if                   ( ( ! initialized ) || RewriteEEPROM                ) {
    FlashEEPROM        (                                                   ) ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  memset               ( &conf , 0 , vs                                    ) ;
  ReadFromEEPROM       ( 0 , vs , (char *) &conf                           ) ;
  ////////////////////////////////////////////////////////////////////////////
  ArduinoSite  = strdup ( conf . Site                                      ) ;
  SapPassword  = strdup ( conf . SapPassword                               ) ;
  WifiSSID     = strdup ( conf . WifiSSID                                  ) ;
  WifiPassword = strdup ( conf . WifiPassword                              ) ;
  SiteUsername = strdup ( conf . Username                                  ) ;
  SitePassword = strdup ( conf . Password                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  memcpy               ( HostIP              , conf . IP      , 4          ) ;
  memcpy               ( HostGateway         , conf . Gateway , 4          ) ;
  memcpy               ( HostNetmask         , conf . Netmask , 4          ) ;
  ////////////////////////////////////////////////////////////////////////////
  Debug               = int ( conf . Debug                                 ) ;
  Console             = int ( conf . Console                               ) ;
  WifiMode            = int ( conf . WifiMode                              ) ;
  PowerSwitchMode     = int ( conf . SwitchMode                            ) ;
  doDelay             =     ( conf . doDelay     > 0                       ) ;
  UseWiFi             =     ( conf . UseWiFi     > 0                       ) ;
  AssignIP            =     ( conf . AssignIP    > 0                       ) ;
  DetectWater         =     ( conf . DetectWater > 0                       ) ;
  BaudRate            =       conf . BaudRate                                ;
  MicrosecondsDelay   =       conf . MicrosecondsDelay                       ;
  WifiHttpPort        =       conf . HttpPort                                ;
  PumpPower           =       conf . PumpPower                               ;
  PumpSwitch          =       conf . PumpSwitch                              ;
  WaterBasement       =       conf . WaterBasement                           ;
  HighestWaterLevel   =       conf . HighestWaterLevel                       ;
  LowestWaterLevel    =       conf . LowestWaterLevel                        ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 迴路延遲
//////////////////////////////////////////////////////////////////////////////
void LoopDelay               (                                             ) {
  ////////////////////////////////////////////////////////////////////////////
  if                         ( ! doDelay                                   ) {
    return                                                                   ;
  }                                                                          ;
  delayMicroseconds          ( MicrosecondsDelay                           ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// 顯示系統啟動訊息
//////////////////////////////////////////////////////////////////////////////
void ShowBootupMessages      (                                             ) {
  if                         ( Console                                     ) {
    Serial . begin           ( BaudRate                                    ) ;
    while                    ( ! Serial                                    ) {
      delay                  ( 1                                           ) ;
    }                                                                        ;
    delay                    ( 1000                                        ) ;
    Serial . println         ( VersionMessage                              ) ;
  }                                                                          ;
}
//////////////////////////////////////////////////////////////////////////////
void setup              (                                                  ) {
  ////////////////////////////////////////////////////////////////////////////
  ReloadEEPROM          (                                                  ) ;
  ShowBootupMessages    (                                                  ) ;
  InitializeWaterCycler (                                                  ) ;
  SetupWIFI             (                                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// -| setup |-
//////////////////////////////////////////////////////////////////////////////
// +| loop |+
//////////////////////////////////////////////////////////////////////////////
void loop               (                                                  ) {
  ////////////////////////////////////////////////////////////////////////////
  ProbeWaterCycler      (                                                  ) ;
  DoingWIFI             (                                                  ) ;
  LoopDelay             (                                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
// -| loop |-
//////////////////////////////////////////////////////////////////////////////
