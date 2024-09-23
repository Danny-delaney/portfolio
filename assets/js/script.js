const canvas = document.getElementById("the_canvas")
const context = canvas.getContext("2d");
context.imageSmoothingEnabled = false;

var start = "false"

// const storedName = localStorage.getItem('userName');
// if (storedName) {
//     document.forms["helloForm"]["nameInput"].value = storedName;
// }

// if(!("true" == localStorage.getItem("formComplete"))){
//     localStorage.setItem("formComplete", "false");
// }

// document.getElementById('showCanvasButton').addEventListener('click', function () {
//     const nameInputValue = document.forms["helloForm"]["name"].value;

//     if (nameInputValue !== '') {
//         // Save the name in localStorage
//         localStorage.setItem('userName', nameInputValue);
//         localStorage.setItem("formComplete", "true");
//         location.reload();
//     }
//     else {
//         alert('Please enter your name before showing the canvas.');
//     }
// });

// document.getElementById('hideCanvasButton').addEventListener('click', function () {
//     localStorage.setItem("formComplete", "false");
//     location.reload();
// });

function validateForm(){
    var x = document.forms["helloForm"]["name"].value;
    if (x == "") {
        alert("I need to know your name so I can say Hello");
        return false;
    }
    else{
        alert("Hello there " + document.forms["helloForm"]["name"].value);
        
        start = true;
    }
    localStorage.setItem("username", x);
}

if (start == true)
{
    document.getElementById('hiddenTable').style.display = 'inline';
    document.getElementById('hiddenButton').style.display = 'inline';
    document.getElementById('the_canvas').style.display = 'inline';
    document.getElementById('hideCanvasButton').style.display = 'inline';
    document.getElementById('showCanvasButton').style.display = 'none';
}


let image = new Image();
image.src = "assets/img/pixilart-sprite.png";
image.onload = gameloop;

function GameObject(spritesheet, x, y, width, height, frame) {
    this.spritesheet = spritesheet;
    this.x = x;
    this.y = y;
    this.width = width;
    this.height = height;
    this.frame = frame;
}

let player = new GameObject(image, 0, 400, 100, 100, 0);

let block1 = new GameObject(image, 105, 205, 90, 90, 4);
let block2 = new GameObject(image, 405, 205, 90, 90, 4);
let block3 = new GameObject(image, 505, 605, 90, 90, 4);

let block1Start = { x: 105, y: 205 }; // Initial position of block1
let block2Start = { x: 405, y: 205 }; // Initial position of block2
let block3Start = { x: 505, y: 605 }; // Initial position of block3

let land1 = new GameObject(image, 0, 0, 200, 900, 7);
let land2 = new GameObject(image, 300, 100, 400, 200, 7);
let land3 = new GameObject(image, 400, 400, 300, 400, 7);
let land4 = new GameObject(image, 800, 600, 300, 300, 7);
let land5 = new GameObject(image, 1000, 400, 300, 100, 7);

let thevoid = new GameObject(image, 0, 0, 1200, 900, 8);

let holder = new GameObject(image, 500, 700, 100, 100, 6);
let end = new GameObject(image, 1100, 400, 100, 100, 5);

let win = false;

function GamerInput(input) {
    this.action = input; // Hold the current input as a string
}

let gamerInput = new GamerInput("None");

function input(event) {
    event.preventDefault();
    if (event.type === "keydown") {
        switch (event.key) {
            case "ArrowLeft": // Left Arrow
                gamerInput = new GamerInput("Left");
                break;
            case "ArrowUp": // Up Arrow
                gamerInput = new GamerInput("Up");
                break;
            case "ArrowRight": // Right Arrow
                gamerInput = new GamerInput("Right");
                break;
            case "ArrowDown": // Down Arrow
                gamerInput = new GamerInput("Down");
                break;
            case " ":
                gamerInput = new GamerInput("Space");
                break;
            default:
                gamerInput = new GamerInput("None"); //No Input
        }
    }
    else {
        gamerInput = new GamerInput("None");
    }
}

function checkBounds() {

    // Check left boundary
    if (player.x < 0) {
        player.x = 0;
        touchedWall = true;
    }

    // Check right boundary
    if (player.x + player.width > canvas.width) {
        player.x = canvas.width - player.width;
        touchedWall = true;
    }

    // Check top boundary
    if (player.y < 0) {
        player.y = 0;
        touchedWall = true;
    }

    // Check bottom boundary
    if (player.y + player.height > canvas.height) {
        player.y = canvas.height - player.height;
        touchedWall = true;
    }

    // Check collision with blocks
    if (checkCollision(player, block1)) {
        if (!isObjOnland(block1, block2, block3)) {
            move(block1);
        }
    }
    if (checkCollision(player, block2)) {
        if (!isObjOnland(block2, block1, block3)) {
            move(block2);
        }
    }
    if (checkCollision(player, block3)) {
        if (!isObjOnland(block3, block1, block2)) {
            move(block3);
        }
    }
    else {
        document.body.style.backgroundColor = "";
    }

    if (checkCollision(player, end)) {
        win = true;
    }

    // Check collision with land
    if (isPlayerOnland(player)) {
        player.x = 0;
        player.y = 400;
        backInTime();
    }
}

function isObjOnland(obj, obj1, obj2) {
    const landObjects = [land1, land2, land3, land4, land5, obj1, obj2];
    return landObjects.every(landObj => !checkCollision(obj, landObj));
}

function isPlayerOnland() {
    const landObjects = [land1, land2, land3, land4, land5, block1, block2, block3];
    return landObjects.every(landObj => !checkCollision(player, landObj));
}

function checkCollision(obj1, obj2) {
    if (
        obj1.x < obj2.x + obj2.width &&
        obj1.x + obj1.width > obj2.x &&
        obj1.y < obj2.y + obj2.height &&
        obj1.y + obj1.height > obj2.y
    ) {
        return true;
    }
    else {
        return false;
    }
}


function move(obj) {
    if (gamerInput.action === "Up") {
        console.log("Move Block Up");
        obj.y -= 100;
    } else if (gamerInput.action === "Down") {
        console.log("Move Block Up");
        obj.y += 100;
    } else if (gamerInput.action === "Left") {
        console.log("Move Block Up");
        obj.x -= 100;
    } else if (gamerInput.action === "Right") {
        console.log("Move Block Up");
        obj.x += 100;
    }
}

function backInTime() {
    if (!(checkCollision(block1, holder))) {
        resetBlockPosition(block1, block1Start);
    }
    if (!(checkCollision(block2, holder))) {
        resetBlockPosition(block2, block2Start);
    }
    if (!(checkCollision(block3, holder))) {
        resetBlockPosition(block3, block3Start);
    }
}


function resetBlockPosition(block, startPos) {
    block.x = startPos.x;
    block.y = startPos.y;
}

function update() {
    if (gamerInput.action === "Up") {
        console.log("Move Up");
        player.y -= 2; // Move Player Up
        player.frame = 3;
    } else if (gamerInput.action === "Down") {
        console.log("Move Down");
        player.frame = 0;
        player.y += 2; // Move Player Down
    } else if (gamerInput.action === "Left") {
        console.log("Move Left");
        player.x -= 2; // Move Player Left
        player.frame = 2;
    } else if (gamerInput.action === "Right") {
        console.log("Move Right");
        player.x += 2; // Move Player Right
        player.frame = 1;
    } else if (gamerInput.action === "Space") {
        console.log("Back in Time");
        backInTime();
    }

    checkBounds();
}

function draw() {
    if (win) {
        context.clearRect(0, 0, canvas.width, canvas.height);
        context.font = "30px Arial";
        context.fillStyle = "green"; // Text color

        // Add text to the canvas
        context.fillText("Winner Winner Chicken Dinner", 600, 450);
    }
    else {
        context.clearRect(0, 0, canvas.width, canvas.height);

        drawSprite(thevoid);

        drawSprite(land1);
        drawSprite(land2);
        drawSprite(land3);
        drawSprite(land4);
        drawSprite(land5);

        drawSprite(holder);
        drawSprite(end);

        drawSprite(block1);
        drawSprite(block2);
        drawSprite(block3);

        drawSprite(player);
    }
}

function drawSprite(sprite) {

    const frameWidth = 10;
    const frameHeight = 10;
    const sourceY = 0;

    context.drawImage(
        sprite.spritesheet,
        sprite.frame * frameWidth, // sourceX
        sourceY,                 // sourceY
        frameWidth,              // frameWidth
        frameHeight,             // frameHeight
        sprite.x,                  // destinationX
        sprite.y,                  // destinationY
        sprite.width,              // destinationWidth
        sprite.height              // destinationHeight
    );
}

function gameloop() {

    update();
    draw();

    window.requestAnimationFrame(gameloop);
}

function clickableDpadReleased() {
    // Reset movement when a button is released
    gamerInput = new GamerInput("");
}

function clickDpadYellow() {
    // Change movement direction when the "Y" button is clicked
    gamerInput = new GamerInput("Up");
}

function clickDpadBlue() {
    // Change movement direction when the "X" button is clicked
    gamerInput = new GamerInput("Left");
}

function clickDpadRed() {
    // Change movement direction when the "B" button is clicked
    gamerInput = new GamerInput("Right");
}

function clickDpadGreen() {
    // Change movement direction when the "A" button is clicked
    gamerInput = new GamerInput("Down");
}
function clickBackInTime() {
    backInTime();
}

window.requestAnimationFrame(gameloop);

window.addEventListener('keydown', input);
window.addEventListener('keyup', input);