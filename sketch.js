// WiFi variables
let SERVER_ADDRESS = "http://10.10.81.101";
let PATH_GET = "/data";
let PATH_POST = "/post";

let readyForWiFiData;
let sendSuccess;

// project variables
let gridSize = 20;
let player;
let maze;
let currentAngle = 0;

function handleResponse(res) {
  parseData(res.data);
  readyForWiFiData = true;
}

function parseData(data) {
  // get values from data
  let potDelta = data.A0.delta;

  // use data to update project variables
  if (potDelta != 0) {
    currentAngle += potDelta;
  }
}

// game function
function createPlayer() {
  player = new Sprite(width / 2, height / 2, gridSize - 2);
  player.color = color(255, 255, 0);
}

// game function
function createMaze() {
  maze = [];
  let sx = width / 2;
  let sy = height / 2;

  for (let y = 0; y < height; y += gridSize) {
    for (let x = 0; x < width; x += gridSize) {
      if (random() < 0.3333) {
        let sprite = new Sprite(sx, sy, gridSize, gridSize, "k");
        sprite.offset.x = x - width / 2;
        sprite.offset.y = y - height / 2;
        sprite.color = 0;
        maze.push(sprite);
      }
    }
  }
}

function setup() {
  randomSeed(1010);
  new Canvas(windowHeight, windowHeight);
  world.gravity.y = 5;
  createMaze();
  createPlayer();

  // WiFi setup
  readyForWiFiData = true;
  sendSuccess = false;
}

function draw() {
  background(220, 20, 120);

  // update board rotation
  for (let i = 0; i < maze.length; i++) {
    maze[i].rotateTo(currentAngle, 3);
  }

  // if ball goes off the board
  if (player.x > width || player.x < 0 || player.y > height || player.y < 0) {
    for (let i = 0; i < maze.length; i++) {
      maze[i].remove();
    }
    maze = null;

    player.remove();
    player = null;
    createMaze();
    createPlayer();
    currentAngle = 0;
    sendSuccess = true;
  }

  // WiFi update
  if (sendSuccess) {
    sendSuccess = false;
    let data = { success: 1 };
    httpPost(SERVER_ADDRESS + PATH_POST, data);
  } else if (readyForWiFiData) {
    readyForWiFiData = false;
    loadJSON(SERVER_ADDRESS + PATH_GET, handleResponse);
  }
}
