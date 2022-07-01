
let window_names=["kale","ginger","onion","cilantro","cabbage","carrot","broccoli","mushroom","potato","tomato","beet","pepper","cauliflower","celery","avocado","spinach","radish","basil","mint","parsley"]

let rows=4;
let columns=5;
let windows=[];
let d;

function preload(){
  createCanvas(windowWidth, windowHeight);

  let w_count=0;
  for(let i=0;i<rows;i++){
    for(let j=0;j<columns;j++){
      windows.push(new Window(window_names[w_count],j,i));
      w_count++;
    }
  }

}

var socket=io()

function setup() {
  createCanvas(windowWidth, windowHeight);
  let w_count=0;
  // for(let i=0;i<rows;i++){
  //   for(let j=0;j<columns;j++){
  //     windows.push(new Window(window_names[w_count],j,i));
  //     w_count++;
  //   }
  // }
  socket.emit("getSetup");
}


function draw() {
  background(20);
  for(let i=0;i<columns*rows;i++){
    windows[i].display();
  }
}



class Window{
  constructor(name, x,y){
    this.name=name;
    this.w=width/columns;
    this.h=height/rows;
    this.x=this.w*x;
    this.y=this.h*y;
    this.fill=0;
  }
  windowResize(x,y){
    this.w=windowWidth/columns;
    this.h=windowHeight/rows;
    this.x=this.w*x;
    this.y=this.h*y;
  }

  display(){
    noStroke();
    if(this.fill<0){
      fill(24,165,89);
    }else{
      fill(this.fill);
    }
    rect(this.x,this.y,this.w,this.h);
    fill(255);
    text(this.name,this.x+10,this.y+10)
  }

}


function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  for(let i=0;i<rows;i++){
    for(let j=0;j<columns;j++){
      windows[i*columns+j].windowResize(j,i);
    }
  }}


socket.on("photoresistor", function (data) {
  for(let i=0;i<columns*rows;i++){
    if("photo_"+windows[i].name==data.board){
      windows[i].fill=data.photo/4096*255*1.5;
    }
  }
  d=data;
  console.log(data);
});

socket.on("allWindos", function (data) {
  console.log(data);
  d=data;
  for(let i=0;i<columns*rows;i++){
    for (let j=0;j<data.length;j++){
      if(windows[i].name==data[j].name){
        windows[i].fill=data[j].value/4096*255*1.5;
      }
    }
  }
});
