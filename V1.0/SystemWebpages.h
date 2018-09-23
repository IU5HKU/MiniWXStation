//*************************************************
//* MINIWX STATION  - SYSTEM WEBPAGES HTML        *
//*************************************************

//*************************************************
//* PAGE NOT FOUND                                *
//*************************************************
const char HTTP_404_STYLE[] PROGMEM = "<style>\
  body {font-family: FixedSys, Helvetica, Sans-Serif;}\
  fieldset { border:1px solid #999; border-radius:8px; box-shadow:0 0 10px #999;}\
  legend  { background:#fff;}\
  .divTable { width: 100%; display:block; padding-top:10px; padding-bottom:10px; padding-right:10px; padding-left:10px;}\
  .divRow { width: 99%; display:block; padding-bottom:5px;}\
  .divColumn { float: left; width: 24%; display:block; padding-left:8px; }\
  .tabheader { border-style: solid; border-radius:8px; border-color: grey; box-shadow:0 0 8px #999; padding-left:10px;}\
  th { border-style: solid; border-radius:8px; border-color: grey; box-shadow:0 0 8px #999; padding-left:10px; background:white; Color:#000}\
  .notabheader { border-style: solid; border-radius:8px; border-color: black; box-shadow:0 0 8px #999; padding-left:10px;}\
  td { border-style: solid; border-radius:8px; border-color: black; box-shadow:0 0 8px #999; padding-left:10px;}\
</style>";

const char HTTP_404_BODY[] PROGMEM = "<fieldset style='width:49%;'><legend style='font-weight:bold;'> MiniWX&#8482; Server Message </legend>\
  <div class='divTable'><div class='divRow'><div class='divColumn' style='width:98%'>\
  <div><img src='data:image/png;base64,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' />\
  <a style='font-size: 76px'> 404</a style='font-weight:bold;'> File Not Found</a></div>\
  </div></div></div>\
  <div class='divTable'><div class='divRow'><div class='divColumn' style='width:22%'>\
  <div>FreeHeap</div>\
  <div>ChipID</div>\
  <div>FlashChipId</div>\
  <div>FlashChipSize</div>\
  <div>getCycleCount()</div>\
  <div>UPTime</div>\
  <div>URI: </div>\
  <div>Method</div>\
  <div>Arguments</div>\
  </div>";

//*************************************************
//* HTML ELEMENTS                                 *
//*************************************************
const char HTTP_HEAD[] PROGMEM = "<!DOCTYPE html><html><head>\
                                    <meta lang='{{language}}' http-equiv='Content-Type' content='text/html' charset='UTF-8'>";
const char HTTP_SCRIPT[] PROGMEM = "<script>\
                                     setTimeout(function(){history.go(-1);}, 3000);\
                                    </script></head>";
const char HTTP_SETS_SCRIPT[] PROGMEM = "<script>\
                                     setTimeout(function(){window.location.reload(history.go(-2));}, 3000);\
                                    </script></head>";
const char HTTP_REBOOT_SCRIPT[] PROGMEM = "<script>\
                                     setTimeout(function(){history.go(-1);}, 20000);\
                                    </script></head>";
const char HTTP_NO_SCRIPT[] PROGMEM = "<script></script></head>";
const char HTTP_STYLE[] PROGMEM = "<style>\
  body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #13F70C; font-size: 32px;}\
  fieldset { border:1px solid #999; border-radius:8px; box-shadow:0 0 10px #999;}\
  legend  { background:#000;}\
  .divTable { width: 100%; display:block; padding-top:10px; padding-bottom:10px; padding-right:10px; padding-left:10px;}\
  .divRow { width: 99%; display:block; padding-bottom:5px;}\
  .divColumn { float: left; width: 24%; display:block; padding-left:8px; }\
  .tabheader { border-style: solid; border-radius:8px; border-color: grey; box-shadow:0 0 8px #999; padding-left:10px;}\
  th { border-style: solid; border-radius:8px; border-color: grey; box-shadow:0 0 8px #999; padding-left:10px; background:green; Color:#000}\
  .notabheader { border-style: solid; border-radius:8px; border-color: black; box-shadow:0 0 8px #999; padding-left:10px;}\
  td { border-style: solid; border-radius:8px; border-color: black; box-shadow:0 0 8px #999; padding-left:10px;}\
  button { padding: 1px 6px 1px 6px;}\
  button > img,button > span { vertical-align: middle;}\
</style>";
const char HTTP_BODY[] PROGMEM = "<body>";
const char HTTP_FOOT[] PROGMEM = "</body></html>";

//*************************************************
//* HTML SYMBOLS - future use in graphs           *
//*************************************************
const char up_arrow[] PROGMEM = "&#8593;";
const char down_arrow[] PROGMEM = "&#8595;";
const char black_sun[] PROGMEM = "&#9728;";
const char cloud[] PROGMEM = "&#9729;";
const char umbrella[] PROGMEM = "&#9730;";
const char degreeC[] PROGMEM = "&#8451;";
const char degreeF[] PROGMEM = "&#8457;";
const char unlock[] PROGMEM = "&#128275;";
const char lock[] PROGMEM = "&#128274;";
const char trademark[] PROGMEM = "&#8482;";
