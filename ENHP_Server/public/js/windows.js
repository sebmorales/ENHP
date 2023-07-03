//the socket here is just a relay, whatever it gets it spits out
//this sketch should run on desktop (not phone)
let socket = io();
let boards=[];
let OSCs=[];

let swiper={speed:10, x:0}
let sonify=false;

let min_w;
let max_col=7;

function setup() {
  createCanvas(windowWidth, windowHeight);
  noStroke();
}

function draw() {
  background(0);
  let t_s=map(width,0,1000,4,15);
  textSize(t_s);
  let b_n=boards.length;
  let col_w;
  if(b_n>max_col){
    col_w=width/max_col;
  }else{
    col_w=width/b_n;
  }
 
  let row_n=ceil(b_n/max_col);
  let row_h=height/row_n;

  for(let i=0; i<boards.length;i++){
    let c=i%max_col;
    let r=floor(i/max_col);
    // if (r<0){r=0;}
    let min = color(0, 26, 13);
    let max = color(0, 220, 60);
    noStroke();
    fill(lerpColor(min,max,boards[i].photo))
    rect(c*col_w,r*row_h,col_w,row_h);
    fill(255);
    if(sonify){
      if( c*col_w<swiper.x && swiper.x<c*col_w+col_w ){
        freq=map(boards[i].photo,0,1,120,500);
        OSCs[i].freq(freq,0.1);
        fill(0,0,map(boards[i].photo,0,1,0,255));
        rect(c*col_w,r*row_h,col_w,row_h);
        if(!boards[i].playing ){
          OSCs[i].pan(map(height/width,.2,3,-1,1));
          OSCs[i].amp(.2,0.2);
          OSCs[i].start();
        }
        boards[i].playing=true;

        // }
      }else{
        if(boards[i].playing){
          OSCs[i].amp(0,0.2)
          // OSCs[i].stop(.2)
          boards[i].playing=false;
        }
      }
    }
    fill(255);
    text(boards[i].name, c*col_w+t_s,row_h*r+t_s+5);

  }
  if(sonify){
    stroke(255);
    strokeWeight(3);
    line(swiper.x,0,swiper.x,height);
    swiper.x+=swiper.speed;
    if(swiper.x>width){
      swiper.x=0;
    }
  }
}

// 
socket.on("photo", function (b) {
  // console.log(b);
  for(let j=0;j<b.length;j++){
    let data=b[j];
    let board_exists=false;
    for (let i=0;i<boards.length;i++){
      if(boards[i].name==data.name){
        boards[i].photo=data.photo;
        boards[i].time=data.time;
        board_exists=true;
      }

    }
    // there is a bug where sometimes a mqtt message is sent 
    // if(!board_exists && !data.name.includes("_msg")){
      if(!board_exists){
        let new_board={
        "name":data.name,
        "photo":data.photo,
        "time":data.time,
        "playing":false

      }
      boards.push(new_board);
      OSCs.push(new p5.Oscillator('sine'));
    }  
  }
});


function mousePressed(){
  console.log(boards)
  sonify=!sonify;
  if(!sonify){
    for(i=0;i<OSCs.length;i++){
      OSCs[i].stop();
    }
  }
}

function windowResized() {
  resizeCanvas(windowWidth, windowHeight);
  min_w=width/max_col;

};