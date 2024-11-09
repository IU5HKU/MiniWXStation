#ifndef EN_LOCALE_H
#define EN_LOCALE_H
//***********************************************************
//* MINIWX STATION  - MAIN INFO PAGE
//* improved readability on portable devices by EA1CDV Antonio
//* WebPages in FLASH and smart dynamics pages from:
//* http://blog.tldnr.org/2017/10/25/how-to-deliver-larger-web-pages-with-an-esp8266/
//***********************************************************
const char PAGE_Main_EN[] PROGMEM = "<!DOCTYPE html>\
<html>\
<head>\
<meta lang='en' http-equiv='Content-Type' content='text/html' charset='UTF-8'>\
<title>MiniWX Weather Station</title>\
<style>\
  body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #13F70C; font-size: 32px;}\
  table { display: table; border: 1px solid green; border-collapse: separate; border-spacing: 1px; border-color: green;}\
  th, td {border: 1px solid green; border-collapse: separate; text-align: center; font-size: 28px;}\
  th {border-left: 30px solid green; border-right: 30px solid green; background: green; Color: black; font-size: 32px;}\
  fieldset { border:1px solid #999; border-radius:8px; box-shadow:0 0 10px #999;}\
  legend  { background:#000;}\
  .divTable { width: 100%; display:block; padding-top:10px; padding-bottom:10px; padding-right:10px; padding-left:10px;}\
  .divRow { width: 99%; display:block; padding-bottom:5px;}\
  .divColumn { float: left; width: 24%; display:block; padding-left:8px; }\
  .tabheader { border-style: solid; border-radius:8px; border-color: grey; box-shadow:0 0 8px #999; padding-left:10px;}\
  .notabheader { border-style: solid; border-radius:8px; border-color: black; box-shadow:0 0 8px #999; padding-left:10px; }\
  button { padding: 1px 6px 1px 6px;}\
  button > img,button > span { vertical-align: middle;}\
  button > text,button > span { vertical-align: middle;}\
</style>\
<style>\
.butn {\
  -webkit-border-radius: 10;\
  -moz-border-radius: 10;\
  border-radius: 10px;\
  text-shadow: 1px 1px 1px #666666;\
  -webkit-box-shadow: 0px 1px 3px #666666;\
  -moz-box-shadow: 0px 1px 3px #666666;\
  box-shadow: 0px 1px 1px #666666;\
  font-family: Arial;\
  color: #000;\
  font-size: 16px;\
  background: lightgray;\
  padding: 1px 6px 1px 6px;\
  text-decoration: none;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
}\
\
.butn:hover {\
  background: #59fc3c;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
  text-decoration: none;\
}\
.butn:disabled {\
  background: gray;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
  text-decoration: none;\
}\
</style>\
<style>\
.grid-container {\
  display: grid;\
  grid-template-columns: auto;\
  grid-template-rows: auto auto;\
  grid-column-gap: 1px;\
  grid-row-gap: 6px;\
}\
.item1 {\
  grid-row: 1;\
}\
.item2 {\
  grid-row: 2;\
}\
</style>\
<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js'></script>\
<script>\
setInterval(function() { getData();}, 1000);\
function getData() {\
  var xhttp = new XMLHttpRequest();\
  var reply = new Array;\
  xhttp.onreadystatechange = function() {\
     if (this.readyState == 4 && this.status == 200){\
      reply = this.responseText.split(',');\
      document.getElementById('clock').innerHTML = reply[0];\
      document.getElementById('temp').innerHTML = reply[1];\
      document.getElementById('pres').innerHTML = reply[2];\
      document.getElementById('rhum').innerHTML = reply[3];\
      document.getElementById('dewp').innerHTML = reply[4];\
      document.getElementById('hind').innerHTML = reply[5];\
      document.getElementById('nexttx').innerHTML = reply[6];\
      document.getElementById('rssi').innerHTML = reply[7];\
      document.getElementById('uptime').innerHTML = reply[8];\
    }\
  };\
  xhttp.open('GET', 'jquery', true);\
  xhttp.send();\
}\
</script>\
</head>\
  <body>\
    <hr>\
    <h3 style='text-shadow: 2px 1px grey;'>{{callsign}} Weather Station - <a id='clock' class='tabheader' style='padding-right:10px'>{{time}}</a></h3>\
    <h4>\
    <fieldset style='width:{{fieldsize0}}'>\
    <legend style='text-shadow: 2px 1px grey;'> {{ChipModel}} </legend>\
    <div class='divTable'>\
      <div class='divRow'>\
        <div class='divColumn' style='width:{{fieldsize1}}'>\
          <div class='tabheader'>Parameter</div>\
          <div class='notabheader'>Temperature</div>\
          <div class='notabheader'>Pressure</div>\
          <div class='notabheader'>Relative Humidity</div>\
          <div class='notabheader'>Dew point</div>\
          <div class='notabheader'>Heat Index</div>\
        </div>\
        <div class='divColumn' style='width:{{fieldsize2}}'>\
          <div class='tabheader'>Value</div>\
          <div id='temp' class='notabheader' style='color: white;'>{{degC}}</div>\
          <div id='pres' class='notabheader' style='color: white;'>{{mbar}}</div>\
          <div id='rhum' class='notabheader' style='color: white;'>{{rHum}}</div>\
          <div id='dewp' class='notabheader' style='color: white;'>{{DPdegC}}</div>\
          <div id='hind' class='notabheader' style='color: white;'>{{HIdegC}}</div>\
        </div>\
        <div class='divColumn' style='width:{{fieldsize3}}'>\
          <div class='tabheader'>Unit</div>\
          <div class='notabheader'>ºC</div>\
          <div class='notabheader'>mBar</div>\
          <div class='notabheader'>%</div>\
          <div class='notabheader'>ºC</div>\
          <div class='notabheader'>ºC</div>\
        </div>\
      </div>\
    </div>\
    <div></br></br></br></br></br></br></br></div>\
    <form action='/graphs' method='POST'>\
      <button class='butn' name='Trend' type='graphs' value='true'><img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABkAAAAWCAYAAAA1vze2AAAACXBIWXMAAA7DAAAOwwHHb6hkAAAAIGNIUk0AAIcbAACL/wAA/bEAAINJAAB9LgAA7W0AADg+AAAkPqJQ9KgAAATYSURBVHjapJRbTFN3HMd/yV63bA+LwS173esefZoxM0RgWEScF5QCpVwKp1DL6ek55/9v6Y0iK5cKoqAbGGzP6QWdIk4XXZxu04wtm0C5WUAW58NWjZm6LcuWffdQM2XOmOHD5+mffz753b60bt0rVF36dkGnbP2xRWy8631umu56pea7HVK9HhDKKCSWEYll+eSqyPPz9g+gxuahRidXBYtOgkWnoEYmIbs7EKwzIGDZ0dPlrHyR5N355DUVulxdx8ASi+D6zKpg+gxYYhFSexSxQC5uXd2Bk+EihBVLC3XLFmJ7cl2urmGwxAK4Pr0qmJYCSyxB9oTxlVYA3DHh2qkieGt3xenXBw+ow7a7RX1/8CmS1EOeIdGnocbmwUUV/XYDxvq3oNdReP+bS+cL6Jd7P1PYXu55UpIC02fAEwvgiTR4bAZcSz2liimwxAIcYR3DzXZ85GyHrWTD/S/Ojeb98ReIDkh15DXmuVe2KwUemwWLTEJsG4bcroNHp7LS/6xiBvKxb+G31uBq4AhCdSZ4ardlziT1V2e+zxB5jZup1WxYIWH6NFR9Fl6mYDS8Ccf35UJ1tYIll8C0J2fBE4twtOzDmOzDcckNh68bAcWekcw7135y5jRRwLSVWqs2P5JoKfDkIsT9Z9Gv5AHLRvw2txs9Uj7EkAaWXFopiV+H3H8OfU31+NJ7EK6mBsiRCfglIeO1GnN4vZHIV7GFAqbCh5J09tPgFbgs1egRijHzsQHjiQL0W0vRJ1ggBfvA4uns6mrTUGJzYA47Lnt6cLhJgLNvFKo2C78kZJilNMdtqybqEWuoU9jp4R1HoY4sQz06DndjDS57D+BDQYBqrYBTFNFuqcJ46xAie/dCVjnU6BTYiVtwdkVxzO7AeR6C0xUAH7kBNTIBr2jJBOzmNQvz00ShBiPtqytp4d1R8Ngc3E0WnOchnHJ6wFUXVH0eauIGnAdOoc1ajUueXpxWfFBte2H39SNYb8Z46yDarTWQh66Axa9DjUyACZWZq5+eeRPAy+SrKiG/uUh2hQbhURjOqkGMyT64FSV7xbHZbO+Ti2CDn8PXbINuV3BYMuDCkVwMKlvRW1sBFhoCTy5kN234O7QpTfd++uGGCcB6GvCJL7UJ5R85jEUYVfy44OpES7MNXEuBxeYeDVlLgcXTUCITsDdY8HXSANw2YeKkAVKDGYo+m51TbBZqZAJuW/XtsdjgGwBeoF7FPHqiuxhaMB+H6irhE0WwyLWVgsdFyUUonUnEg7m4+VkREsGNUNqGsrmnpf6R+KWGjNO8bc3pZIRoQN70J5aN+D1dhk57Hhz9l8AS6WdGCQsOgNsboPgPQtWyt8X16cckQqbFaszhQgVRr2N7ZvHiDqROFiAg1kKNPCMMtRSYPg2WSEONpcHj6ZXv/5JUvldIdMhjL3TvWr/MrCYoAxfB43OrTuLHJe3Mdrt+T/FrREQUspYTK9ko87AONrL81BD83xLVlqko3rSWiIhaTUUUNBs8vFuDevwmWGLh+UguQdHn4Jesd3yNla8X571DJJVuIJ/5XWdroA2sZwSsO/Zc8P1xqKEhBO1Vd3yNFWvaxDqi6sK3SKnanGPZnu8N1BQFu4QSpXOVdAkliqdqu8dfV9ITVmqLPUI5uev30N8DAPKfisupFjUwAAAAAElFTkSuQmCC' /> Graphics Trends</button>\
    </form>\
    </fieldset>\
    </h4>\
    <h6>\
    <fieldset style='width:{{fieldsize4}}'>\
    <legend style='text-shadow: 2px 1px grey;'> System Infos </legend>\
      <div class='divTable'>\
          <div class='divRow'>\
            <div class='divColumn' style='width:{{fieldsize5}}'>\
              <div class='tabheader'>Uptime</div>\
              <div class='tabheader'>Next TX</div>\
              <div class='tabheader'>SSID</div>\
              <div class='tabheader'>RSSI</div>\
              <div class='tabheader'>BSSID</div>\
              <div class='tabheader'>IP addr</div>\
              <div class='tabheader'>Position</div>\
            </div>\
            <div class='divColumn' style='width:{{fieldsize6}}'>\
              <div id='uptime' class='notabheader' style='color: silver;'>{{uptime}}</div>\
              <div id='nexttx' class='notabheader' style='color: silver;'>{{nexttx}}</div>\
              <div class='notabheader' style='color: silver;'>{{SSID}}</div>\
              <div id='rssi' class='notabheader' style='color: silver;'>{{RSSI}}</div>\
              <div class='notabheader' style='color: silver;'>{{BSSID}}</div>\
              <div class='notabheader' style='color: silver;'>{{myip}}</div>\
              <div class='notabheader' style='color: silver;'>Lat: {{lat}} Long: {{long}} Alt: {{alt}}m asl</div>\
            </div>\
          </div>\
      </div>\
    </fieldset>\
    <br>\
    <form action='/submit' method='POST'>\
      <fieldset style='width:{{fieldsize7}}'>\
      <legend style='text-shadow: 2px 1px grey;'> Control Panel </legend>\
      <div class='grid-container'>\
        <div class='item1'>\
            <button class='butn' name='MiniWXSettings' type='settings' value='true' formaction='/settings' method='POST'><img src='data:image/png;base64,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'/> Settings</button>\
            <button class='butn' name='SendAPRS' type='submit' value='true'><img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAYAAADE6YVjAAAACXBIWXMAAA7DAAAOwwHHb6hkAAAAIGNIUk0AAIcbAACL/wAA/bEAAINJAAB9LgAA7W0AADg+AAAkPqJQ9KgAAAPCSURBVHjarJZfSNRZFMc/jjON2Tr92a0UzXHtnzSpmCVhbFmS9iAUbG4vPkhET01EYpqp9bBPFfTQ+hA0DyH7sI/9wZamf4wRKFiRC9Mqs4iagk1MKGkyOd99uLk/p3XcjbxwmN/53nvO99x7zj13kiSxwCh4+ZKfQyEq+vtxhkIQDkNODmRlwebNTHg8NLnd/LKQk6REJPX1/BSJ8JvfD4ODiR3k50NlJUQi5F+/zh/zLpIUJ6CU5mYpJUWC/y9ut9TSoo7P/clMW0p1tb6vqvoy559Lfb00M5OABJS6f//XEcxKQ4M0L8mpU4tDMCvNzWqfS+JuaJB3MQlAysqSKiuVIQn71BTeYJC6oiJTll1dMDYGDgeUl4PdDoEAjI/DkiVQVgYfP8LAABQWmpLu7DRFZLPBgQPG9sED2LmTv4Bieno04XRKly9LkuT1mkg8Hv0zDh402I4dRu/qkoqLpXfvjF5aauZraoweCkkrVxof4bB+tXV38830NDx+/On2FZjfwkKIxcz39u0WBnDnDvT0QH290RsbweWCs2dhZgaOH4dIBEIhCAb5kaNHTRRr10pjY9KLF0a/ckUaHJSGhqSODikpSWprM5Hu22fWOBzSvXvSzIx0966Zu3YtPjft7RLl5RZw65YUjUrr1hkjv1/y+aS+PoM9eyaNjEhpaZbNtm1SOGwIenulNWviSS5ckGwjI9bt7+gwia6ogOxseP4cHj2C9HQoKjJHGQjAxIRlEwzC6Kj5fv3aFM3cEY2CLSPDAgIB+PABjhyB3Fxz7j09kJYGhw9DcrIJZO44cQK2boWpKRNcbe08vaumxtrasmVSd7f0/r0Ui0nr10tOp/TmjTQ+bvBNm6z1eXlSJCKNjkrV1dLbt9LwsJSZaa25cUOitVV/zj3Dq1fN+Q4PS0uXGuz+fYM9eWJIQUpNNTmTpDNnDDZ7DXw+UyhOp9TZqUnb5CTnXa74vAwMwM2b5ugAHj6EoSG4fRumpw22ezds2QJ+P/h8Brt0yeSxrAzy8iAzE9xu2ohGxd691k4cDtO2XS4LW75cys21djZb8hs2SCtWxFfTqlXSxo1mp+fOSZK+QxJNTbq42L0rPV2qrVVeXBc+dmxxSVpa9Pu/Wn1VlTJ37Vocgrq6BO/Jp90U7NnzdQSnT0ux2ALPryRKSvTtyZNScvKXOV+9WmptVe9/vvFzpbFRdYcOGeOFnOfkmOfB69WeRL6SFvjf5QBKnz7l/KtX/NDXh72/30zY7eaOeDxMZmfTVlLCRSCcyNHfAwBCmavUZ7FsDAAAAABJRU5ErkJggg=='/> Send APRS Packet</button>\
            <button class='butn' {{wunderstate}} name='SendWUNDER' type='submit' value='true'><img src='data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAYAAADE6YVjAAAACXBIWXMAAA7DAAAOwwHHb6hkAAAAIGNIUk0AAIcbAACL/wAA/bEAAINJAAB9LgAA7W0AADg+AAAkPqJQ9KgAAAYySURBVHjafJZbbJxHFcd/Z2a+/fbmtWNvbMeOE2JSN26T0tAQAlTQRC0gqjYIgagE4i0vgARCgpeINOIiFV546gsICQmVi0CFF6S0ldWShyZWiVJXbUOVSxOSeJ2L7V3bu9/uft/M4WHjKCERI52Zc45mzv+M5nL+oqp8+9APHzl38b252bdmKZcrGGO4XxORe3RVvWvsdrs011Y4+Owz/OGPL5VUtSUHDz438f57c/85e+4MD0ztJE27eO+x1v5fkPWmqrcBQghEUQ4ROH/uDE88cYBXXz0m9qknDzSOvfIPtn3kQbIsvWuhiNwdWHudAkEVtKeLKqCIMagqIWQM9A8wN3caEftVu7xcP4pa4jiGOwIaY27LOpgYQTAYEYSez1qLAN3mag/EWFDFCjSbTZxzwybNUkrlIuARBaOCRQgiBMB6MAiIEIyA5BAxGJciBvBKW4S+jx/A9cUYv0aXPEENxVKFpaVlTBRFhJChKKlAZgVvBCOgRug48FbAGAwGtW006hLEoi6QdK9hi1X2futnyNhuaDQoqZKpARFc5DDGGNDeoQWjdKySRoKR3o6yyOBt79CtCGICQWKsHUFDgU6aMLJ7L2F6knj30yQmJqdJL8kQEBGMhoCxlnZkSboZxq/vwpBlQnstw3fp3TanWDawWndcvHSJGwsrRDLK6J5PsFSGycc+hhnqY6FxHZe1QQRVMD4o7bTDBI79ocLGLng1dAWqNvDU4BCb1OI7HUJUZGVllV1bGnzza4M8ubdIuljn3KnT5D9cYmX2FejmGdyyDx9V8GmGEbDF6sjRIC1+sVrhSKPMpVzKTCQY3+H7g45fTg1TzsFMfZkrqyWe3Rnx+yPjfP0rW3jus+OUbZc//fk16idfZ+lfx+mbfprRPT+iI5aVCycYHR7EtK3icjm2pkVMJ+GTvkA5azHu4IulKq3lBp/fDGMDAxjT5dCuPlpvXmfleB1Y4Dvfm+RLn9nEjbOnsdZTmPgoSaFEacMo4gxBwbhIaa21OWEzalHEdCY8nnoez1u2iGOBiGq+zb5yxOdGUna7Fs2LKfP1RXRxEZICD00OYazgo4iujaHYBufJbj1oN5BkXIzgb7k2e7sx40E4VNiElj11s4IrWK6lgW+MFlgrCZQFl40wO7/ExKMxpTQh62RAGZEcJVui0GdoF/KosSiKiTxYG3PKdXg/7ynaNo/lHI+YmGYuQ+I8razEdKXFniGwhRInriXMnleKfcNQqLPcbdBOWri1BjYPhVIJLVYQSRHtYNqRY6DjWOtmHC866MtTqyQYZwgS8+LiTZaiEkkMTe8o5oocu5DywVIHkQQyx9QDU+AmyD/4KfzUXnyuQlRYRNM2FsEJIChl65jRBh+Uxhh2HYYqykyw/OrMh+wc3sSjwwOkcYOza8ucrFnmu3kuXQls3X6dLxx4iLcrh3nbfJpatczO0bOcPzlDt50SpIodHKoeTbue2EUsZMtMlGO+XOwn1y/8tJUwd9kwajzP7KrixiyvnFJ+ff4q3cxRO5dxcN9GxrYlTD3cYHjrVfYN1bj5z7/z+ksvE7VzVEe33wJJUyJjyaV5LhAojwpvJgl//XcbKQ9xZa1GoZznerqR3752jZpL2Fh2vPvuCm/MJZg0T9xcxlx+i3denuEvv3mHuBkTGU/fyBCyfWqHNtdaRFGE0wJLoYOJVrHBUelUKBQddduh02mSmpiCd5RLEcF3EMlx+UYg9UuMbTCkjTJLyU36hmKqboAb9QWmHp7GrX9iAJlJ6DdCGgYwxuDLnrWQEocI4wYoqMflBbIA6gh4Ng+B91WSdootK2P9w4h6EtMmWEfIPC5Ns17BA4TeNYjEg3rwgohBNeAkgABB8YBqr8aFoEBKPl6PEggookLwGWIMZvPEOAu1eYyxgLlv7Vb0tr3u4w5fCOGe2m+MYXWlzo4d05j9+w/8TkNKo9G4TQbuBPLe3yUhhNuybv8va7HWUqvNAzA2PvEDVJXnnz/auUUTtFjov0cKhYoW8pXbdqk4cP95+Yrmc30KqDFGjxw5MquqyHoGhw8f/snVK7Ufz1+tkfnsHu7VIxO9TARBUeQuagTeZ4BhcnIb1dEN333h5y+8CPDfAQA7VA/i4G4IkgAAAABJRU5ErkJggg=='/> Send WUNDER Packet</button>\
            <button class='butn' name='NTPSync' type='submit' value='true'><img src='data:image/png;base64,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'/> NTP Sync</button>\
            <button class='butn' name='WiFiScan' type='submit' value='true'><img src='data:image/png;base64,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'/> WiFi Scan</button>\
        </div>\
        <div class='item2'>\
            <button class='butn' name='Reboot' type='submit' value='true'><img src='data:image/png;base64,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'/> Reboot</button>\
            <input class='butn' type='button' onclick=\"location.href='https://www.aprsdirect.com/details/main/name/{{callsign}}';\" style='padding: 5px 10px 5px 40px; background-image: url(data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAYAAADE6YVjAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAIGNIUk0AAIcbAACL/wAA/bEAAINJAAB9LgAA7W0AADg+AAAkPqJQ9KgAAAntSURBVHgBAN0JIvYA////AP///wD///8A////AP///wD///8A////AP///wD///8A////j8Xixf85nDn0DIUM8UmlSfHR6NH/////tf///wD///8A////AP///wD///8A////AP///wD///8A////AAD///8A////AP///wD///8A////AP///wD///8A////AP///5DA4MD/DIUM/wB3AP8AfgD/AHYA/xSJFP/I5Mj/////sf///wL///8A////AP///wD///8A////AP///wD///8AAf///wAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACQw+LDb02gTQDx9fEAAAcAAAD8AAAABQAAAPcAABQRFAC6YroAMRcxsAAAAFEAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAjL7gvm9Ro1EA8fXxAAACAAAXCBcASS1JAAoACgAABQAA7O/sAE2hTQDK5spQAAAAswAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAD///8A////AP///wD///8A////AP///5C63br/E4YT/wB4AP8AegD/AHkA/5jLmP//////f79//wB1AP8AfAD/AHcA/xmKGf/E4sT/////tv///wD///8A////AP///wD///8AAP///wD///8A////AP///wD///+KweHB/wiACP8AdwD/AHwA/wB9AP+ZzZn/////tv///0X////ffr5+/wB3AP8AfQD/AHcA/w+GD//M5cz/////rv///wL///8A////AP///wAA////AP///wD///8A////hcHiwf8QgRD/AHUA/wB5AP8CfAL/odGh/////7n///8T////AP///y/////sgcCB/wB2AP8AegD/AHQA/xiGGP/R6dH/////r////wD///8A////AAD///8A////AP///4G+377/FocW/wB3AP8AeQD/AHkA/6DQoP/////C////Df///wD///8A////AP///yr////pg8GD/wB2AP8AeQD/AHYA/yWPJf/T6dP/////tf///wD///8AAP///wD///+Ev9+//w2FDf8AdwD/AH0A/wB6AP+QxpD/////0P///xH///8A////AP///wD///8A////AP///yn////qkcmR/wB7AP8AfQD/AHYA/ySQJP/T6dP/////s////wEA////h8bkxv8Pgw//AHYA/wB7AP8AeQD/jseO/////+b///8j////AP///wD///8A////AP///wD///8A////AP///x3////akciR/wB6AP8AewD/AHUA/x6PHv/d7t3/////lwDJ5snxE4QT/wB2AP8AeQD/AHgA/5DJkP/////c////KP///wD///8A////AP///wD///8A////AP///wD///8A////AP///yH////XisaK/wB2AP8AeQD/AHMA/y6ULv/q9ur6A9cn13nZ6dkHAAcAAAMGAwCOS44AcDdw3wAAAEEAAADdAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA8AAAALg7HTtYBwgHC73jvQAABAAA6evpAPH+8QMD4zHjeQAFAAcA+wAATipOAJBIkAAAAABxAAAAwgAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA7wAAAAI9Hz1Fvd+9AOLs4gAABAAA+f/5AAIzGTMAAPgAAAAEAAC63roAmMyYAAAAAG8AAAAaAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAmAAAAdX+/fwDE4MQAAAUAAAD6AAAwFzAAAMHiwfEPgg//AHYA/wB5AP8AeQD/mMyY/////9P///8i////AP///wD///8A////AP///wD///8A////AP///wD///8A////Lf///+iBwYH/AHUA/wB6AP8AcwD/JY4l/9/w3/4A////lL3fvf8LgAv/AHcA/wB7AP8AewD/lsqW/////9v///8g////AP///wD///8A////AP///wD///8A////AP///y7////th8SH/wB4AP8AewD/AHUA/xiKGP/X69f/////rQD///8A////mLjcuP8Hggf/AHgA/wB8AP8AewD/lMmU/////9v///8b////AP///wD///8A////AP///wD///8k////5IjDiP8AdwD/AH0A/wB3AP8ejR7/zebN/////7P///8BAP///wD///8A////lLbctv8RhBH/AHgA/wB5AP8AeAD/l8uX/////9L///8Z////AP///wD///8A////J////+aJxIn/AHUA/wB6AP8AdwD/IIwg/83nzf////+1////AP///wAA////AP///wD///8A////k7nduf8Mfwz/AHYA/wB5AP8AewD/ms2a/////8////8g////AP///yz////oisWK/wB4AP8AegD/AHQA/xeGF//P6c//////r////wD///8A////AAD///8A////AP///wD///8A////mb/fv/8Hfwf/AHcA/wB8AP8AfAD/lcuV/////8z///9H////24bDhv8AeQD/AH0A/wB3AP8NhA3/yOTI/////67///8C////AP///wD///8AAP///wD///8A////AP///wD///8A////lLbctv8OhQ7/AHgA/wB7AP8AeQD/lsuW//////+Iwoj/AHcA/wB8AP8AdwD/FogW/8DfwP////+2////AP///wD///8A////AP///wAB////AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAk7jeuGxYpFgA8fXxAAACAAAWDRYAOCM4AL/YvwDz+fMAAP0AABcQFwCsXKwAPB08tAAAAE0AAAAAAAAAAAAAAAAAAAAAAAAAAAH///8AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAlrreumlRolEA9vf2AAAIAAAA+wAAAAUAAAD3AAARDhEAt2K3ADcaN7AAAABRAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP///wD///8A////AP///wD///8A////AP///wD///8A////m7ncuf8GgQb/AHcA/wCAAP8AdwD/D4YP/8Phw/////+x////Av///wD///8A////AP///wD///8A////AP///wAA////AP///wD///8A////AP///wD///8A////AP///wD///8A////lb7evv80mTT0AYAB8TydPPHL5Mv/////tf///wD///8A////AP///wD///8A////AP///wD///8A////AAEAAP//RkQ4zukroJ8AAAAASUVORK5CYII=);' value='APRS Direct' >\
        </div>\
      </div>\
      </fieldset>\
    </form>\
    <p>MiniWX Station&#8482; ({{SOFT_VER}}) - click <a href='https://github.com/IU5HKU/MiniWXStation'>here</a> and check for latest version</p>\
    </h6>\
  </body>\
</html>";

//***********************************************************
//* MINIWX STATION  - SETTINGS PAGE
//* set main values through webpage too
//***********************************************************
const char PAGE_MiniWXSettings_EN[] PROGMEM = "<!DOCTYPE html>\
<html>\
<head>\
<meta  lang='en' http-equiv='Content-Type' content='text/html; charset=UTF-8'>\
<title>MiniWXSettings</title>\
</head>\
<style>\
  body { background-color: #000000; font-family: Arial, Helvetica, Sans-Serif; Color: #13F70C; font-size: 16px; }\
  .divHeaderTable { width: 100%; padding-bottom:5px;  display:block; }\
  .divHeaderRow { width: 100%; display:block; height:105px; }\
  .divHeaderColumn { float: left; width: 33%; display:block; }\
  .divTable { width: 100%; display:block; padding-top:10px; padding-bottom:10px; padding-right:10px; padding-left:10px; }\
  .divRow { width: 99%; display:block; padding-bottom:5px; }\
  .divColumn { float: left; width: 49%; display:block; padding-left:8px; }\
  input[type=text] { padding-left:2px; border: 2px solid grey; border-radius: 4px; background-color: black; color: white; box-shadow:0 0 8px #999; font-size: 14px;}\
  fieldset {  border:1px solid #999;  border-radius:8px;  box-shadow:0 0 8px #999; }\
  legend { background:#000; text-shadow: 2px 1px grey; }\
  button { padding: 1px 6px 1px 6px;}\
  button > img,button > span { vertical-align: middle;}\
  button > text,button > span { vertical-align: middle;}\
</style>\
<style>\
.butn {\
  -webkit-border-radius: 10;\
  -moz-border-radius: 10;\
  border-radius: 10px;\
  text-shadow: 1px 1px 1px #666666;\
  -webkit-box-shadow: 0px 1px 3px #666666;\
  -moz-box-shadow: 0px 1px 3px #666666;\
  box-shadow: 0px 1px 1px #666666;\
  font-family: Arial;\
  color: #000;\
  font-size: 16px;\
  background: lightgray;\
  padding: 5px 10px 5px 40px;\
  text-decoration: none;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
}\
.butn:hover {\
  background: #59fc3c;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
  text-decoration: none;\
}\
.butn:disabled {\
  background: gray;\
  background-repeat: no-repeat;\
  background-position: 10px center;\
  text-decoration: none;\
}\
</style>\
<body>\
  <div id='form_container'>\
  <hr>\
    <h1><a style='text-shadow: 2px 1px grey;'>MiniWX Station&#8482; Settings</a></h1>\
    <form style='width:58%' action='/submit' method='POST'>\
        <div class='divTable'>\
          <div class='divRow'>\
            <div class='divColumn' style='width:99%'>\
              <fieldset>\
              <legend><h2> Station Callsign and Position </h2></legend>\
                <label>Callsign</label>\
                <div><input id='callsign' name='callsign' type='text' maxlength='9' size='9' value='{{callsign}}'/></div>\
                <label>Longitude (example 01023.16E)</label>\
                <input id='longitude' name='longitude' type='text' maxlength='9' size='9' value='{{lon}}'/>\
                <label> Latitude (example 4303.01N)</label>\
                <input id='latitude' name='latitude' type='text' maxlength='8' size='8' value='{{lat}}'/>\
                <label> Altitude (meters)</label>\
                <input id='altitude' name='altitude' type='text' maxlength='4' size='4' value='{{alt}}'/>\
              </fieldset>\
            </div>\
          </div>\
        </div>\
        <div class='divTable'>\
          <div class='divRow'>\
            <div class='divColumn'>\
              <fieldset>\
              <legend><h2>APRS Settings</h2></legend>\
                <label>Server Address</label>\
                <div><input id='aprsserveraddr' name='aprsserveraddr' type='text' maxlength='255' size='30' value='{{aprsserveraddr}}'/></div>\
                <label>Password</label>\
                <div><input id='aprspassw' name='aprspassw' type='text' maxlength='5' size='5' value='{{aprspassw}}'/></div>\
                <label>Server Port</label>\
                <div><input id='aprsserverport' name='aprsserverport' type='text' maxlength='5' size='5' value='{{aprsserverport}}'/></div>\
                <label>Transmission Delay (min)</label>\
                <div><input id='txdelay' name='txdelay' type='text' maxlength='2' size='2' value='{{txdelay}}'/></div>\
                <label>Station Packets Comment</label>\
                <div><input id='aprscmnt1' name='aprscmnt1' type='text' maxlength='43' size='43' value='{{aprscmnt1}}'/></div>\
                <label>Telemetry Packets Comment</label>\
                <div><input id='aprscmnt2' name='aprscmnt2' type='text' maxlength='23' size='23' value='{{aprscmnt2}}'/></div>\
              </fieldset>\
            </div>\
            <div class='divColumn'>\
              <fieldset>\
              <legend><h2>STATIC IP Definitions</h2></legend>\
                <span>\
                  <input id='usestatic' name='usestatic' type='hidden' value='false'/>\
                  <input id='usestatic' name='usestatic' type='checkbox' value='true' {{usestaticip}} />\
                  <label>USE STATIC IP (reboot needed)</label>\
                </span><p></p>\
                <label>IP</label>\
                <div><input id='staticip' name='staticip' type='text' maxlength='15' size='15' value='{{staticip}}'/></div>\
                <label>Gateway</label>\
                <div><input id='staticgateway' name='staticgateway' type='text' maxlength='15' size='15' value='{{staticgateway}}'/></div>\
                <label>Subnet Mask</label>\
                <div><input id='staticmask' name='staticmask' type='text' maxlength='15' size='15' value='{{staticmask}}'/></div>\
                <label>DNS1</label>\
                <div><input id='staticdns1' name='staticdns1' type='text' maxlength='15' size='15' value='{{staticdns1}}'/></div>\
                <label>DNS2</label>\
                <div><input id='staticdns2' name='staticdns2' type='text' maxlength='15' size='15'  value='{{staticdns2}}'/></div>\
              </fieldset>\
            </div>\
          </div>\
          <div class='divRow'><div class='divColumn'></br></div></div>\
          <div class='divRow'>\
            <div class='divColumn'>\
              <fieldset>\
              <legend><h2>WUNDER Settings</h2></legend>\
                <div>\
                  <span>\
                    <input id='usewunder' name='usewunder' type='hidden' value='false'/>\
                    <input id='usewunder' name='usewunder' type='checkbox' value='true' {{usewunder}} />\
                    <label>USE WUNDER</label>\
                  </span>\
                </div>\
                <p></p>\
                <label>Wunder ID</label>\
                <div><input id='wunderid' name='wunderid' type='text' maxlength='255' value='{{wunderid}}'/></div>\
                <label>Wunder Password</label>\
                <div><input id='wunderpassw' name='wunderpassw' type='text' maxlength='255' value='{{wunderpassw}}'/></div>\
              </fieldset>\
            </div>\
            <div class='divColumn'>\
              <fieldset>\
              <legend><h2>NTP Settings</h2></legend>\
                <label>Server</label>\
                <div><input id='ntpserver' name='ntpserver'  type='text' maxlength='255' value='{{ntpserver}}'/></div>\
                <label>NTPSync Delay (hours)</label>\
                <div><input id='ntpsyncdelay' name='ntpsyncdelay'  type='text' maxlength='2' size='2' value='{{ntpsyncdelay}}'/></div>\
              </fieldset>\
            </div>\
          </div>\
          <div class='divRow'><div class='divColumn'></br></br></br></div></div>\
          <div class='divRow'>\
            <div class='divColumn' style='width:99%;padding-bottom:10px'>\
              <div>\
                <a><button class='butn' style='padding: 1px 6px 1px 6px;' name='save' type='submit' value='true'><img src='data:png;base64,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'/> Save & Exit </button></a>\
                <a><input class='butn' type='button' onclick='javascript:window.history.go(-1);' style='background-image: url(data:image/png;base64,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);' value='Exit' ></a>\
              </div>\
            </div>\
          </div>\
        </div>\
        <div id='footer' style='padding-left:18px'><p>MiniWX Station&#8482; ({{SOFT_VER}}) - click <a href='https://github.com/IU5HKU/MiniWXStation'>here</a> and check for latest version</p></div>\
    </form>\
  </div>\
</body>\
</html>";
#endif
