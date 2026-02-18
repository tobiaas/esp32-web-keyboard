#include <WiFi.h>
#include <WebServer.h>
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;
WebServer server(80);

const char* ssid = "ESP-Keyboard";
const char* pass = "12345678";

// Modifier-Bitmask:
// 0x01 = LCtrl (0xE0)  0x02 = LShift (0xE1)
// 0x04 = LAlt  (0xE2)  0x08 = LGUI   (0xE3)
// 0x10 = AltGr (0xE6)
void tap(uint8_t key, uint8_t modMask = 0) {
  if (modMask & 0x01) Keyboard.pressRaw(0xE0);
  if (modMask & 0x02) Keyboard.pressRaw(0xE1);
  if (modMask & 0x04) Keyboard.pressRaw(0xE2);
  if (modMask & 0x08) Keyboard.pressRaw(0xE3);
  if (modMask & 0x10) Keyboard.pressRaw(0xE6);
  Keyboard.pressRaw(key);
  delay(5);
  Keyboard.releaseAll();
}

void handleKey() {
  if (!server.hasArg("c")) { server.send(400, "text/plain", "Missing key"); return; }
  uint8_t key = (uint8_t)server.arg("c").toInt();
  uint8_t mod = server.hasArg("m") ? (uint8_t)server.arg("m").toInt() : 0;
  tap(key, mod);
  server.send(200, "text/plain", "OK");
}

void handleRoot() {
  server.send(200, "text/html", R"HTML(
<!doctype html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1">
<title>ESP Keyboard</title>
<style>
* { box-sizing: border-box; -webkit-tap-highlight-color: transparent; }
body {
  font-family: system-ui, sans-serif;
  background: #1a1a2e;
  color: #eee;
  margin: 0;
  padding: 8px;
  user-select: none;
  overflow-x: auto;
}
h3 { margin: 4px 0 4px; text-align: center; font-size: 15px; }
#status { text-align: center; font-size: 10px; color: #888; margin-bottom: 6px; height: 14px; }
.kb { display: inline-flex; flex-direction: column; gap: 3px; padding-bottom: 10px; }
.row { display: flex; gap: 3px; }
.key {
  background: #16213e;
  color: #eee;
  border: 1px solid #2a4080;
  border-bottom: 3px solid #0f3460;
  border-radius: 6px;
  height: 38px;
  min-width: 34px;
  width: 34px;
  font-size: 10px;
  cursor: pointer;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  line-height: 1.2;
  padding: 0 2px;
  text-align: center;
  flex-shrink: 0;
}
.key:active { background: #e94560; border-bottom-width: 1px; transform: translateY(2px); }
.key.active { background: #0f3460; border-color: #e94560; color: #e94560; }
.sh { font-size: 8px; color: #e94560; line-height: 1; }
.ghost { background: transparent !important; border: none !important; }
/* Breiten */
.w15  { width: 54px;  }
.w175 { width: 63px;  }
.w2   { width: 72px;  }
.w225 { width: 81px;  }
.w25  { width: 90px;  }
.w275 { width: 99px;  }
.w6   { width: 213px; }
.gap  { width: 10px; flex-shrink: 0; }
.gap5 { width: 5px;  flex-shrink: 0; }
</style>
</head>
<body>
<h3>ESP-Keyboard (QWERTZ)</h3>
<div id="status">Bereit</div>
<div style="overflow-x:auto; width:100%;">
<div class="kb">

<!-- Funktionstasten -->
<div class="row">
  <button class="key w15" onclick="k(41)">ESC</button>
  <div class="gap"></div>
  <button class="key" onclick="k(58)">F1</button>
  <button class="key" onclick="k(59)">F2</button>
  <button class="key" onclick="k(60)">F3</button>
  <button class="key" onclick="k(61)">F4</button>
  <div class="gap5"></div>
  <button class="key" onclick="k(62)">F5</button>
  <button class="key" onclick="k(63)">F6</button>
  <button class="key" onclick="k(64)">F7</button>
  <button class="key" onclick="k(65)">F8</button>
  <div class="gap5"></div>
  <button class="key" onclick="k(66)">F9</button>
  <button class="key" onclick="k(67)">F10</button>
  <button class="key" onclick="k(68)">F11</button>
  <button class="key" onclick="k(69)">F12</button>
  <div class="gap"></div>
  <button class="key" onclick="k(70)"><span style="font-size:8px">Prt</span>Sc</button>
  <button class="key" onclick="k(71)"><span style="font-size:8px">Scr</span>Lk</button>
  <button class="key" onclick="k(72)">Pau</button>
</div>

<!-- Zahlenreihe -->
<div class="row">
  <button class="key" onclick="k(53)"><span class="sh">&deg;</span>^</button>
  <button class="key" onclick="k(30)"><span class="sh">!</span>1</button>
  <button class="key" onclick="k(31)"><span class="sh">"</span>2</button>
  <button class="key" onclick="k(32)"><span class="sh">&sect;</span>3</button>
  <button class="key" onclick="k(33)"><span class="sh">$</span>4</button>
  <button class="key" onclick="k(34)"><span class="sh">%</span>5</button>
  <button class="key" onclick="k(35)"><span class="sh">&amp;</span>6</button>
  <button class="key" onclick="k(36)"><span class="sh">/</span>7</button>
  <button class="key" onclick="k(37)"><span class="sh">(</span>8</button>
  <button class="key" onclick="k(38)"><span class="sh">)</span>9</button>
  <button class="key" onclick="k(39)"><span class="sh">=</span>0</button>
  <button class="key" onclick="k(45)"><span class="sh">?</span>&szlig;</button>
  <button class="key" onclick="k(46)"><span class="sh">`</span>&acute;</button>
  <button class="key w175" onclick="k(42)">&larr; Back</button>
</div>

<!-- QWERTZ -->
<div class="row">
  <button class="key w15" onclick="k(43)">Tab &rarr;</button>
  <button class="key" onclick="k(20)">Q</button>
  <button class="key" onclick="k(26)">W</button>
  <button class="key" onclick="k(8)">E</button>
  <button class="key" onclick="k(21)">R</button>
  <button class="key" onclick="k(23)">T</button>
  <button class="key" onclick="k(29)">Z</button>
  <button class="key" onclick="k(24)">U</button>
  <button class="key" onclick="k(12)">I</button>
  <button class="key" onclick="k(18)">O</button>
  <button class="key" onclick="k(19)">P</button>
  <button class="key" onclick="k(47)">&Uuml;</button>
  <button class="key" onclick="k(48)"><span class="sh">*</span>+</button>
  <button class="key w15" onclick="k(49)"><span class="sh">'</span>#</button>
</div>

<!-- ASDF -->
<div class="row">
  <button class="key w175" id="caps" onclick="toggleCaps()">Caps &#8681;</button>
  <button class="key" onclick="k(4)">A</button>
  <button class="key" onclick="k(22)">S</button>
  <button class="key" onclick="k(7)">D</button>
  <button class="key" onclick="k(9)">F</button>
  <button class="key" onclick="k(10)">G</button>
  <button class="key" onclick="k(11)">H</button>
  <button class="key" onclick="k(13)">J</button>
  <button class="key" onclick="k(14)">K</button>
  <button class="key" onclick="k(15)">L</button>
  <button class="key" onclick="k(51)">&Ouml;</button>
  <button class="key" onclick="k(52)">&Auml;</button>
  <button class="key w225" onclick="k(40)">Enter &crarr;</button>
</div>

<!-- YXCV -->
<div class="row">
  <button class="key w225" id="lshift" onclick="toggleMod(0x02,'lshift','rshift')">Shift &#8679;</button>
  <button class="key" onclick="k(100)"><span class="sh">&gt;</span>&lt;</button>
  <button class="key" onclick="k(28)">Y</button>
  <button class="key" onclick="k(27)">X</button>
  <button class="key" onclick="k(6)">C</button>
  <button class="key" onclick="k(25)">V</button>
  <button class="key" onclick="k(5)">B</button>
  <button class="key" onclick="k(17)">N</button>
  <button class="key" onclick="k(16)">M</button>
  <button class="key" onclick="k(54)"><span class="sh">;</span>,</button>
  <button class="key" onclick="k(55)"><span class="sh">:</span>.</button>
  <button class="key" onclick="k(56)"><span class="sh">_</span>-</button>
  <button class="key w275" id="rshift" onclick="toggleMod(0x02,'lshift','rshift')">Shift &#8679;</button>
</div>

<!-- Unterste Reihe -->
<div class="row">
  <button class="key w15" id="lctrl" onclick="toggleMod(0x01,'lctrl','rctrl')">Ctrl</button>
  <button class="key w15" id="lgui" onclick="toggleMod(0x08,'lgui')">Win</button>
  <button class="key w15" id="lalt" onclick="toggleMod(0x04,'lalt')">Alt</button>
  <button class="key w6" onclick="k(44)">Space</button>
  <button class="key w15" id="ralt" onclick="toggleMod(0x10,'ralt')">AltGr</button>
  <button class="key w15" onclick="k(101)">Menu</button>
  <button class="key w15" id="rctrl" onclick="toggleMod(0x01,'lctrl','rctrl')">Ctrl</button>
</div>

<!-- Nav-Block + Pfeiltasten in einer Zeile -->
<div class="row" style="align-items:flex-end;">
  <!-- Nav 3x2 -->
  <div style="display:flex;flex-direction:column;gap:3px;">
    <div class="row">
      <button class="key" onclick="k(73)">Ins</button>
      <button class="key" onclick="k(74)">Pos1</button>
      <button class="key" onclick="k(75)">PgUp</button>
    </div>
    <div class="row">
      <button class="key" onclick="k(76)">Del</button>
      <button class="key" onclick="k(77)">End</button>
      <button class="key" onclick="k(78)">PgDn</button>
    </div>
  </div>
  <div class="gap"></div>
  <!-- Pfeiltasten invertiertes T -->
  <div style="display:flex;flex-direction:column;gap:3px;">
    <div class="row">
      <div class="key ghost"></div>
      <button class="key" onclick="k(82)">&uarr;</button>
      <div class="key ghost"></div>
    </div>
    <div class="row">
      <button class="key" onclick="k(80)">&larr;</button>
      <button class="key" onclick="k(81)">&darr;</button>
      <button class="key" onclick="k(79)">&rarr;</button>
    </div>
  </div>
  <div class="gap"></div>
  <!-- Numpad -->
  <div style="display:flex;flex-direction:column;gap:3px;">
    <div class="row">
      <button class="key" onclick="k(83)">NmLk</button>
      <button class="key" onclick="k(84)">/</button>
      <button class="key" onclick="k(85)">*</button>
      <button class="key" onclick="k(86)">-</button>
    </div>
    <div class="row">
      <button class="key" onclick="k(95)">7</button>
      <button class="key" onclick="k(96)">8</button>
      <button class="key" onclick="k(97)">9</button>
      <button class="key" onclick="k(87)" style="height:79px;">+</button>
    </div>
    <div class="row">
      <button class="key" onclick="k(92)">4</button>
      <button class="key" onclick="k(93)">5</button>
      <button class="key" onclick="k(94)">6</button>
    </div>
    <div class="row">
      <button class="key" onclick="k(89)">1</button>
      <button class="key" onclick="k(90)">2</button>
      <button class="key" onclick="k(91)">3</button>
      <button class="key" onclick="k(88)" style="height:79px;">&crarr;</button>
    </div>
    <div class="row">
      <button class="key w2" onclick="k(98)">0</button>
      <button class="key" onclick="k(99)">.</button>
    </div>
  </div>
</div>

</div><!-- .kb -->
</div>

<script>
let mods = 0;

function setActive(id, on) {
  let el = document.getElementById(id);
  if (el) el.classList.toggle('active', on);
}

function toggleMod(bit, ...ids) {
  mods ^= bit;
  let on = !!(mods & bit);
  ids.forEach(id => setActive(id, on));
}

function toggleCaps() {
  k(57);
  document.getElementById('caps').classList.toggle('active');
}

function k(code) {
  let m = mods;
  mods = 0;
  ['lctrl','rctrl','lshift','rshift','lalt','ralt','lgui']
    .forEach(id => setActive(id, false));

  let st = document.getElementById('status');
  fetch('/key?c=' + code + '&m=' + m)
    .then(() => { st.textContent = 'HID ' + code + (m ? ' mod=0x' + m.toString(16) : ''); })
    .catch(() => { st.textContent = 'Fehler!'; });
}
</script>
</body>
</html>
)HTML");
}

void setup() {
  USB.begin();
  Keyboard.begin();
  delay(1000);
  WiFi.softAP(ssid, pass);
  server.on("/", handleRoot);
  server.on("/key", handleKey);
  server.begin();
}

void loop() {
  server.handleClient();
}