//////////////////////////////////////////////////////////////////////////////
// Arduino Uno WIFI WeMOS D1 R1
// Product : Oriphase Water Cycler For Pets
// Version 1.0.0
// Author : Foxman, Vladimir Lin, FYL
// 寵物飲水機
//////////////////////////////////////////////////////////////////////////////
#include "EEPROM.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "ESP8266mDNS.h"
//////////////////////////////////////////////////////////////////////////////
// EEPROM函數庫
//////////////////////////////////////////////////////////////////////////////
// +| WriteToEEPROM |+
// 寫入EEPROM
//////////////////////////////////////////////////////////////////////////////
void WriteToEEPROM  ( int    address                                         ,
                      int    len                                             ,
                      char * Buffer                                          ,
                      bool   commit = true                                 ) {
  for               ( int i = 0 ; i < len ; i++                            ) {
    EEPROM . write  ( address + i , Buffer [ i ]                           ) ;
  }                                                                          ;
  if                ( commit                                               ) {
    EEPROM . commit (                                                      ) ;
  }                                                                          ;
}
//////////////////////////////////////////////////////////////////////////////
// -| WriteToEEPROM |-
//////////////////////////////////////////////////////////////////////////////
// +| ReadFromEEPROM |+
// 讀取EEPROM
//////////////////////////////////////////////////////////////////////////////
void ReadFromEEPROM ( int address , int len , char * Buffer                ) {
  for               ( int i = 0 ; i < len ; i++                            ) {
    Buffer [ i ] = EEPROM . read  ( address + i                            ) ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| ReadFromEEPROM |-
//////////////////////////////////////////////////////////////////////////////
// ESP-8266 WIFI函數庫
//////////////////////////////////////////////////////////////////////////////
// +| SetEsp8266WifiSTA |+
// 設定WIFI為STA模式
//////////////////////////////////////////////////////////////////////////////
void SetEsp8266WifiSTA  (                                                  ) {
  WiFi . mode           ( WIFI_STA                                         ) ;
  WiFi . disconnect     (                                                  ) ;
}
//////////////////////////////////////////////////////////////////////////////
// -| SetEsp8266WifiSTA |-
//////////////////////////////////////////////////////////////////////////////
// +| SetEsp8266WifiAP |+
// 設定WIFI為AP模式
//////////////////////////////////////////////////////////////////////////////
void SetEsp8266WifiAP   (                                                  ) {
  WiFi . mode           ( WIFI_AP                                          ) ;
  WiFi . disconnect     (                                                  ) ;
}
//////////////////////////////////////////////////////////////////////////////
// +| SetEsp8266WifiBoth |+
// 設定WIFI為雙模式
//////////////////////////////////////////////////////////////////////////////
void SetEsp8266WifiBoth (                                                  ) {
  WiFi . mode           ( WIFI_AP_STA                                      ) ;
  WiFi . disconnect     (                                                  ) ;
}
//////////////////////////////////////////////////////////////////////////////
// +| SetEsp8266WifiOff |+
// 設定WIFI為關閉
//////////////////////////////////////////////////////////////////////////////
void SetEsp8266WifiOff  (                                                  ) {
  WiFi . mode           ( WIFI_OFF                                         ) ;
  WiFi . disconnect     (                                                  ) ;
}
//////////////////////////////////////////////////////////////////////////////
// -| SetEsp8266WifiSTA |-
//////////////////////////////////////////////////////////////////////////////
// +| GetEsp8266WifiNetworks |+
// 掃描WIFI網路
//////////////////////////////////////////////////////////////////////////////
int GetEsp8266WifiNetworks ( )                                               {
  return WiFi . scanNetworks ( )                                             ;
}
//////////////////////////////////////////////////////////////////////////////
// -| GetEsp8266WifiNetworks |-
//////////////////////////////////////////////////////////////////////////////
// +| PrintEsp8266WifiStation |+
// 列出現有WIFI狀況
//////////////////////////////////////////////////////////////////////////////
int PrintEsp8266WifiStation ( int id )                                       {
  bool encrypt =   ( WiFi . encryptionType ( id ) == ENC_TYPE_NONE )         ;
  Serial . print   ( id + 1             )                                    ;
  Serial . print   ( " : "              )                                    ;
  Serial . print   ( WiFi . SSID ( id ) )                                    ;
  Serial . print   ( " ("               )                                    ;
  Serial . print   ( WiFi . RSSI ( id ) )                                    ;
  Serial . print   ( ")"                )                                    ;
  Serial . println ( encrypt ? "" : "*" )                                    ;
}
//////////////////////////////////////////////////////////////////////////////
// -| PrintEsp8266WifiStation |-
//////////////////////////////////////////////////////////////////////////////
// Arduino Uno Wifi WeMOS D1 R2讀寫函數
//////////////////////////////////////////////////////////////////////////////
// +| SetOutput |+
// 設定輸出腳位
//////////////////////////////////////////////////////////////////////////////
void SetOutput        ( int id       )                                       {
  switch              (     id       )                                       {
    case  2 : pinMode ( D2  , OUTPUT ) ; break                               ;
    case  3 : pinMode ( D3  , OUTPUT ) ; break                               ;
    case  4 : pinMode ( D4  , OUTPUT ) ; break                               ;
    case  5 : pinMode ( D5  , OUTPUT ) ; break                               ;
    case  6 : pinMode ( D6  , OUTPUT ) ; break                               ;
    case  7 : pinMode ( D7  , OUTPUT ) ; break                               ;
    case  8 : pinMode ( D8  , OUTPUT ) ; break                               ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| SetOutput |-
//////////////////////////////////////////////////////////////////////////////
// +| SetInput |+
// 設定輸入腳位
//////////////////////////////////////////////////////////////////////////////
void SetInput         ( int id             )                                 {
  switch              (     id             )                                 {
    case  2 : pinMode ( D2  , INPUT_PULLUP ) ; break                         ;
    case  3 : pinMode ( D3  , INPUT_PULLUP ) ; break                         ;
    case  4 : pinMode ( D4  , INPUT_PULLUP ) ; break                         ;
    case  5 : pinMode ( D5  , INPUT_PULLUP ) ; break                         ;
    case  6 : pinMode ( D6  , INPUT_PULLUP ) ; break                         ;
    case  7 : pinMode ( D7  , INPUT_PULLUP ) ; break                         ;
    case  8 : pinMode ( D8  , INPUT_PULLUP ) ; break                         ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| SetInput |-
//////////////////////////////////////////////////////////////////////////////
// +| SetPin |+
// 指定腳位作用
//////////////////////////////////////////////////////////////////////////////
void SetPin   ( int id , int mode                                          ) {
  if          ( mode == 0                                                  ) {
    SetInput  ( id                                                         ) ;
  } else                                                                     {
    SetOutput ( id                                                         ) ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| SetPin |-
//////////////////////////////////////////////////////////////////////////////
// +| WriteHigh |+
// 寫入高電位
//////////////////////////////////////////////////////////////////////////////
void WriteHigh             ( int id     )                                    {
  switch                   (     id     )                                    {
    case  2 : digitalWrite ( D2  , HIGH ) ; break                            ;
    case  3 : digitalWrite ( D3  , HIGH ) ; break                            ;
    case  4 : digitalWrite ( D4  , HIGH ) ; break                            ;
    case  5 : digitalWrite ( D5  , HIGH ) ; break                            ;
    case  6 : digitalWrite ( D6  , HIGH ) ; break                            ;
    case  7 : digitalWrite ( D7  , HIGH ) ; break                            ;
    case  8 : digitalWrite ( D8  , HIGH ) ; break                            ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| WriteHigh |-
//////////////////////////////////////////////////////////////////////////////
// +| WriteLow |+
// 寫入低電位
//////////////////////////////////////////////////////////////////////////////
void WriteLow              ( int id    )                                     {
  switch                   (     id    )                                     {
    case  2 : digitalWrite ( D2  , LOW ) ; break                             ;
    case  3 : digitalWrite ( D3  , LOW ) ; break                             ;
    case  4 : digitalWrite ( D4  , LOW ) ; break                             ;
    case  5 : digitalWrite ( D5  , LOW ) ; break                             ;
    case  6 : digitalWrite ( D6  , LOW ) ; break                             ;
    case  7 : digitalWrite ( D7  , LOW ) ; break                             ;
    case  8 : digitalWrite ( D8  , LOW ) ; break                             ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| WriteLow |-
//////////////////////////////////////////////////////////////////////////////
// +| WriteValue |+
// 指定腳位特定值
//////////////////////////////////////////////////////////////////////////////
void WriteValue ( int id , int mode                                        ) {
  if            ( mode == 0                                                ) {
    WriteLow    (     id                                                   ) ;
  } else                                                                     {
    WriteHigh   (     id                                                   ) ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| WriteValue |-
//////////////////////////////////////////////////////////////////////////////
// +| ReadValue |+
// 讀取腳位值
//////////////////////////////////////////////////////////////////////////////
int ReadValue                    ( int id                                  ) {
  switch                         ( id                                      ) {
    case  2 : return digitalRead ( D2                                      ) ;
    case  3 : return digitalRead ( D3                                      ) ;
    case  4 : return digitalRead ( D4                                      ) ;
    case  5 : return digitalRead ( D5                                      ) ;
    case  6 : return digitalRead ( D6                                      ) ;
    case  7 : return digitalRead ( D7                                      ) ;
    case  8 : return digitalRead ( D8                                      ) ;
  }                                                                          ;
  return 0                                                                   ;
}
//////////////////////////////////////////////////////////////////////////////
// -| ReadValue |-
//////////////////////////////////////////////////////////////////////////////
// +| WriteAnalog |+
// 寫入類比腳位
//////////////////////////////////////////////////////////////////////////////
void WriteAnalog         ( int id , int value )                              {
  switch                 (     id             )                              {
    case 0 : analogWrite ( 0      ,     value ) ; break                      ;
  }
}
//////////////////////////////////////////////////////////////////////////////
// -| WriteAnalog |-
//////////////////////////////////////////////////////////////////////////////
// +| ReadAnalog |+
// 讀取類比腳位
//////////////////////////////////////////////////////////////////////////////
int ReadAnalog                  ( int id )                                   {
  switch                        (     id )                                   {
    case  0 : return analogRead ( 0      )                                   ;
  }                                                                          ;
  return 0                                                                   ;
}
//////////////////////////////////////////////////////////////////////////////
// -| ReadAnalog |-
//////////////////////////////////////////////////////////////////////////////
// 控制電源類別
//////////////////////////////////////////////////////////////////////////////
class PowerController                                                        {
  ////////////////////////////////////////////////////////////////////////////
  public                                                                     :
    //////////////////////////////////////////////////////////////////////////
    PowerController ( int  output                                            ,
                      int  input                                             ,
                      int  mode    = 0                                       ,
                      bool lowAsOn = true                                  ) {
      OutputPin     = output                                                 ;
      InputPin      = input                                                  ;
      CurrentOutput = false                                                  ;
      CurrentInput  = false                                                  ;
      LowAsOn       = lowAsOn                                                ;
      SwitchMode    = mode                                                   ;
      SetPin ( input  , 0 )                                                  ;
      SetPin ( output , 1 )                                                  ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 設定電源控制模式
    // 0 - Switch Mode  : 切換模式
    // 1 - Sync Mode    : 同步模式
    // 2 - Isolate Mode : 孤立模式
    //////////////////////////////////////////////////////////////////////////
    void setSwitch  ( int mode )                                             {
      SwitchMode = mode                                                      ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 同步開關狀態
    //////////////////////////////////////////////////////////////////////////
    bool GetInput (void)                                                     {
      int  v  = ReadValue ( InputPin )                                       ;
      CurrentInput = ( v <= 0 )                                              ;
      return CurrentInput                                                    ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 自動探測開關與電源
    //////////////////////////////////////////////////////////////////////////
    bool Probe                 ( void                                      ) {
      int  v       = ReadValue ( InputPin                                  ) ;
      bool on      =           ( v <= 0                                    ) ;
      bool changed = false                                                   ;
      switch                   ( SwitchMode                                ) {
        // 0 - Switch Mode
        case 0                                                               :
          if                   ( CurrentInput  != on                       ) {
            CurrentInput = on                                                ;
            Switch             (                                           ) ;
            changed      = true                                              ;
          }
        break                                                                ;
        // 1 - Sync Mode
        case 1                                                               :
          if                   ( CurrentInput  != on                       ) {
            CurrentInput = on                                                ;
            changed      = true                                              ;
          }
          if                   ( CurrentOutput != on                       ) {
            Turn               ( on                                        ) ;
            changed      = true                                              ;
          }
        break                                                                ;
        // 2 - Isolate Mode
        case 2                                                               :
          if                   ( CurrentInput  != on                       ) {
            changed = true                                                   ;
          }
          CurrentInput   = on                                                ;
        break                                                                ;
      }
      return changed                                                         ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 開關
    //////////////////////////////////////////////////////////////////////////
    void Turn ( bool OnOff )                                                 {
      if ( CurrentOutput == OnOff )                                          {
        return                                                               ;
      }
      CurrentOutput = OnOff                                                  ;
      if ( LowAsOn )                                                         {
        WriteValue ( OutputPin , OnOff ? 0 : 1 )                             ;
      } else                                                                 {
        WriteValue ( OutputPin , OnOff ? 1 : 0 )                             ;
      }
    }
    //////////////////////////////////////////////////////////////////////////
    // 切換電源
    //////////////////////////////////////////////////////////////////////////
    void Switch ( void )                                                     {
      Turn ( CurrentOutput ? false : true )                                  ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 現有電源狀態
    //////////////////////////////////////////////////////////////////////////
    bool Power ( void )                                                      {
      return CurrentOutput                                                   ;
    }
    //////////////////////////////////////////////////////////////////////////
    // 現有開關狀態
    //////////////////////////////////////////////////////////////////////////
    bool InputNow ( void )                                                   {
      return CurrentInput                                                    ;
    }
    //////////////////////////////////////////////////////////////////////////
  protected                                                                  :
    //////////////////////////////////////////////////////////////////////////
    int  OutputPin                                                           ;
    int  InputPin                                                            ;
    bool CurrentOutput                                                       ;
    bool CurrentInput                                                        ;
    bool LowAsOn                                                             ;
    int  SwitchMode                                                          ;
    //////////////////////////////////////////////////////////////////////////
  private                                                                    :
    //////////////////////////////////////////////////////////////////////////
}                                                                            ;
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
char * VersionMessage = "Oriphase Water Cycler For Pets : Version 2021.03.02.04.43" ;
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
int                HighestWaterLevel = 7200                                  ;
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
ESP8266WebServer * HttpServer        = nullptr                               ;
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
// 重置函數
//////////////////////////////////////////////////////////////////////////////
void (* resetFunc) (void) = 0                                                ;
//////////////////////////////////////////////////////////////////////////////
// 建立IP Address封包
//////////////////////////////////////////////////////////////////////////////
IPAddress ComposeIP ( char * ip )                                            {
  return IPAddress ( ip [ 0 ] , ip [ 1 ] , ip [ 2 ] , ip [ 3 ] )             ;
}
//////////////////////////////////////////////////////////////////////////////
void WebServerEntry ( )                                                      {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send                                                         (
    200                                                                      ,
    "text/html"                                                              ,
    VersionMessage                                                         ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerWIFI ( )                                                       {
  ////////////////////////////////////////////////////////////////////////////
  String HTML                                                                ;
  String LINE                                                                ;
  int    totalNetworks                                                       ;
  bool   encrypt                                                             ;
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  totalNetworks = GetEsp8266WifiNetworks ( )                                 ;
  for ( int i = 0 ; i < totalNetworks ; i++ )                                {
    encrypt  = ( WiFi . encryptionType ( i ) == ENC_TYPE_NONE )              ;
    LINE     = String ( i + 1 , DEC         )                                ;
    LINE    += String ( " : "               )                                ;
    LINE    += String ( WiFi . SSID ( i )   )                                ;
    LINE    += String ( " ("                )                                ;
    LINE    += String ( WiFi . RSSI ( i )   )                                ;
    LINE    += String ( ")"                 )                                ;
    LINE    += String ( encrypt ? "" : " *" )                                ;
    if ( ( i + 1 ) < totalNetworks )                                         {
      LINE  += String ( "<br>\n"            )                                ;
    }                                                                        ;
    HTML    += LINE                                                          ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send ( 200 , "text/html" , HTML                            ) ;
  ////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////
void WebServerStatus ( )                                                     {
  ////////////////////////////////////////////////////////////////////////////
  if ( HttpServer == nullptr )                                               {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  HttpServer -> send                                                         (
    200                                                                      ,
    "text/html"                                                              ,
    "Oriphase Water Cycler : Status"                                       ) ;
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
        resetFunc       (                                                  ) ;
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
  HttpServer -> on         ( "/"            , WebServerEntry               ) ;
  HttpServer -> on         ( "/index.html"  , WebServerEntry               ) ;
  HttpServer -> on         ( "/status.html" , WebServerStatus              ) ;
  HttpServer -> on         ( "/WIFI"        , WebServerWIFI                ) ;
  HttpServer -> on         ( "/AJAX"        , WebServerAJAX                ) ;
  HttpServer -> onNotFound (                  WebServerNotFound            ) ;
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
  WriteValue                 ( PumpIO , 0                                  ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 電磁水閥
  ////////////////////////////////////////////////////////////////////////////
  SetOutput                  ( ValveIO                                     ) ;
  WriteValue                 ( ValveIO , 1                                 ) ;
  TurnSolenoidValve          ( false                                       ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 抽水開關
  ////////////////////////////////////////////////////////////////////////////
  SetInput                   (  SwitchIO                                   ) ;
  ////////////////////////////////////////////////////////////////////////////
  // 暫時沒有用到
  ////////////////////////////////////////////////////////////////////////////
  SetInput                   (  6                                          ) ;
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
  int  dtm     = millis            (                                       ) ;
  int  dt      = dtm - PowerProbedMillis                                     ;
  bool in                                                                    ;
  bool changed = false                                                       ;
  bool pc                                                                    ;
  int  cwl     = 0                                                           ;
  ////////////////////////////////////////////////////////////////////////////
  if                               ( dt < 10                               ) {
    return                                                                   ;
  }                                                                          ;
  ////////////////////////////////////////////////////////////////////////////
  // 偵測水位
  ////////////////////////////////////////////////////////////////////////////
  cwl          = CurrentWaterLevel (                                       ) ;
  if                               ( DetectWater                           ) {
    if                             ( cwl > HighestWaterLevel               ) {
      TurnSolenoidValve            ( false                                 ) ;
      UpdateValveStatus            (                                       ) ;
    } else
    if                             ( cwl < LowestWaterLevel                ) {
      TurnSolenoidValve            ( true                                  ) ;
      UpdateValveStatus            (                                       ) ;
    }
  }
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
