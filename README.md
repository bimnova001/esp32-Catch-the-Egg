<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <style>
    body {
      font-family: "Segoe UI", sans-serif;
      background-color: #f8f9fa;
      color: #212529;
      padding: 2rem;
      line-height: 1.6;
    }
    h1, h2, h3 {
      color: #007acc;
    }
    code {
      background: #eee;
      padding: 2px 5px;
      border-radius: 4px;
    }
    pre {
      background: #f1f1f1;
      padding: 10px;
      border-left: 5px solid #007acc;
      overflow-x: auto;
    }
    table {
      border-collapse: collapse;
      width: 100%;
      margin-top: 1rem;
    }
    th, td {
      padding: 10px;
      border: 1px solid #ccc;
      text-align: center;
    }
    ul {
      padding-left: 1.2rem;
    }
    .badge {
      background: #007acc;
      color: white;
      padding: 4px 8px;
      border-radius: 5px;
      font-size: 0.8rem;
      margin-left: 5px;
    }
  </style>
</head>
<body>

<h1>🎮 ESP32 OLED Game Collection</h1>

<p>This project is a fun collection of <strong>classic games</strong> implemented on the <strong>ESP32 with a 128x64 OLED display</strong> and <strong>3-button input</strong>. It includes a mode selector menu, difficulty levels, and long-press to exit game feature.</p>

<hr>

<h2>🧩 Included Games</h2>

<h3>1. ✊✋✌️ Rock Paper Scissors</h3>
<ul>
  <li>Play against the computer</li>
  <li>Three buttons: Rock, Paper, Scissors</li>
  <li>Displays result on OLED</li>
</ul>

<h3>2. 🧱 Tetris</h3>
<ul>
  <li>Move blocks left/right, rotate, and drop</li>
  <li>Clear lines to score points</li>
  <li>Speed increases over time</li>
</ul>

<h3>3. 🥚 Catch the Egg</h3>
<ul>
  <li>Control a basket to catch falling eggs</li>
  <li>Avoid missing eggs</li>
  <li>Long press <code>SELECT</code> to exit to menu</li>
</ul>

<hr>

<h2>📟 Controls</h2>

<table>
  <thead>
    <tr>
      <th>Button</th>
      <th>In Menu</th>
      <th>In Game</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>LEFT</td>
      <td>Move selection left</td>
      <td>Move / Rotate left</td>
    </tr>
    <tr>
      <td>RIGHT</td>
      <td>Move selection right</td>
      <td>Move / Rotate right</td>
    </tr>
    <tr>
      <td>SELECT</td>
      <td>Start game / Select option</td>
      <td>Hold 3 sec to exit to menu</td>
    </tr>
  </tbody>
</table>

<hr>

<h2>⚙️ Required Hardware</h2>
<ul>
  <li>✅ ESP32 DevKit V1</li>
  <li>✅ 128x64 OLED Display (SH1106)</li>
  <li>✅ 3 Push buttons (or touch sensors)</li>
  <li>✅ Jumper wires</li>
</ul>

<hr>

<h2>🧠 Features</h2>
<ul>
  <li>OLED graphics via <code>U8g2lib</code></li>
  <li>Menu system with difficulty selection</li>
  <li>Random generation using <code>random()</code></li>
  <li>Button long-press detection via <code>millis()</code></li>
</ul>

<hr>

<h2>📁 Project Structure</h2>

<pre>
project/
├── src/
│   └── main.ino         # Main game logic
├── lib/
│   └── (optional)       # Additional libraries
└── README.html          # This documentation
</pre>

<hr>

<h2>📦 Libraries Used</h2>
<ul>
  <li><a href="https://github.com/olikraus/u8g2" target="_blank">U8g2lib</a> - OLED graphics</li>
  <li><code>Wire.h</code> - I2C communication</li>
  <li>Arduino core for ESP32</li>
</ul>

<hr>

<h2>🚀 How to Run</h2>
<ol>
  <li>Open the project in <strong>Arduino IDE</strong> or <strong>PlatformIO</strong></li>
  <li>Install <code>U8g2</code> via Library Manager</li>
  <li>Connect OLED and buttons as per the code</li>
  <li>Upload and start playing!</li>
</ol>

<hr>

<h2>❤️ Credits</h2>
<p>Made with ❤️ for learning and retro game fun!<br>
By: <strong>Your Name</strong> (Feel free to personalize this)</p>

</body>
</html>
